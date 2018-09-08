
// omxclient_qtapplication.cpp
// 2017 Aug 06

#include "omxclient_qtapplication.hpp"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#include <Shlobj.h>
#define PATH_DELIML  "\\"
#define PATH_DELIMR  "\\"
#else
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#define PATH_DELIML  "/"
#define PATH_DELIMR  "/"
#endif

#if !defined(WIN32) && !defined(Sleep)
#define Sleep(_x_) usleep(1000*(_x_))
#endif

#define SERVER_NAME_FIELD         "SERVER_NAME"
#define CHANNEL_FILE_NAME_FIELD   "CHANNEL_FILE_NAME"
#define CONF_DIR_LAST       ".raspconfig"
#define CONF_FILE_NAME3     "raspconfig.txt"


static const char* GetConfBaseDir(char* a_buf, int a_bufLen)
{
#ifdef WIN32

    SHGetSpecialFolderPath();

#elif defined(ANDROID)
    strncpy(a_buf,"/sdcard/Download",a_bufLen);
#else

    const char *homedir;
    struct passwd *pw = getpwuid(getuid());

    homedir = pw->pw_dir;
    if(!homedir){homedir="";}
    strncpy(a_buf,homedir,a_bufLen);

#endif

    return a_buf;
}


static const char* GetConfDirName(char* a_buf, int a_bufLen)
{
    GetConfBaseDir(a_buf,a_bufLen);
    strncat(a_buf,PATH_DELIML CONF_DIR_LAST PATH_DELIMR,a_bufLen);
    return a_buf;
}


static void CreateConfigDirIfNeeded(char* a_buf, int a_bufLen)
{
    int nWrite;
#ifdef WIN32
    nWrite=snprintf(a_buf,a_bufLen,"mkdir ");
#else
    nWrite=snprintf(a_buf,a_bufLen,"mkdir -p ");
#endif
    GetConfBaseDir(a_buf+nWrite,a_bufLen-nWrite);
    strncat(a_buf,PATH_DELIML CONF_DIR_LAST PATH_DELIMR,a_bufLen);
    system(a_buf);
}


static const char* GetConfigFileName(char* a_buf, int a_bufLen)
{
    GetConfDirName(a_buf, a_bufLen);
    strncat(a_buf,CONF_FILE_NAME3,a_bufLen);
    return a_buf;
}

static const char* GetOwnHostName(char*,int);
namespace omxclient {
std::vector<omxclient::QtApplication*>   g_vectorOfApplications;
}


omxclient::QtApplication::QtApplication(int& a_argc, char** a_argv)
    :
      QApplication(a_argc,a_argv)
{
    m_pToSet = NULL;
    m_pCentralTab = NULL;


    size_t unFound;
    const wchar_t* cpcTmpWstr;
    const char* cpcTmpStr;
    QFile qfConfFile;
    QTextStream aStream;
    std::string tmpStr;
    std::wstring tmpWstr;
    QString line ;
    char vcConfFileNm[128];

    m_pActiveButton = NULL;
    m_nConnected = 0;
    m_nConfigLoaded = 0;
    connect(this, SIGNAL(NewHostFoundSignal(QString)),this,SLOT(NewHostFoundSlot(QString)));

    CreateConfigDirIfNeeded(vcConfFileNm,127);
    qfConfFile.setFileName(GetConfigFileName(vcConfFileNm,127));
    if (!qfConfFile.open(QIODevice::ReadOnly | QIODevice::Text)){ goto threadStartPoint;}

    aStream.setDevice(&qfConfFile);
    aStream.setCodec("UTF-8");

    while(!aStream.atEnd()){
        line = aStream.readLine();

        if(line.contains(tr(SERVER_NAME_FIELD))){
            tmpStr = line.toStdString();
            unFound = tmpStr.find("=");
            if(unFound != std::string::npos){
                cpcTmpStr = tmpStr.c_str()+unFound+1;
                while(((*cpcTmpStr==' ')||(*cpcTmpStr=='\t'))&&(*cpcTmpStr!=0)) {++cpcTmpStr;}
                if(*cpcTmpStr){m_serverName=cpcTmpStr;}
            }
        }
        else if(line.contains(tr(CHANNEL_FILE_NAME_FIELD))){
            tmpWstr = line.toStdWString();
            unFound = tmpWstr.find(L"=");
            if(unFound != std::wstring::npos){
                cpcTmpWstr = tmpWstr.c_str()+unFound+1;
                while(((*cpcTmpWstr==' ')||(*cpcTmpWstr=='\t'))&&(*cpcTmpWstr!=0)){++cpcTmpWstr;}
                if(*cpcTmpWstr){m_channelFileName=cpcTmpWstr;}
            }
        }
    } // while(!aStream.atEnd()){

threadStartPoint:
    m_threadSearchNewRasp=std::thread(&QtApplication::SearchThreadFunction,this);
    m_threadDev=std::thread(&QtApplication::DeviceThreadFunction,this);
    g_vectorOfApplications.push_back(this);
}


omxclient::QtApplication::~QtApplication()
{
    char vcConfFileNm[128];

    m_nWork=0;
    m_semaForDevThread.post();
    m_threadDev.join();
    m_threadSearchNewRasp.join();

    QFile qfConfFile (GetConfigFileName(vcConfFileNm,127));
    if (!qfConfFile.open(QIODevice::WriteOnly | QIODevice::Text)){ return;}
    QTextStream aStream(&qfConfFile);
    aStream.setCodec("UTF-8");

    if(m_serverName.length()){
        aStream<<SERVER_NAME_FIELD "="<<m_serverName.c_str()<<"\n";
    }

    if(m_channelFileName.length()){
        aStream<<CHANNEL_FILE_NAME_FIELD "="<<QString::fromStdWString(m_channelFileName)<<"\n";
    }
}


const std::string& omxclient::QtApplication::GetServerName()const
{
    return m_serverName;
}


void omxclient::QtApplication::SetServerName(const std::string& a_srvName)
{
    m_serverName = a_srvName;
}


const std::wstring& omxclient::QtApplication::GetChannelsFileName()const
{
    return m_channelFileName;
}


void omxclient::QtApplication::SetChannelsFileName(const std::wstring& a_chnlsFileName)
{
    m_channelFileName = a_chnlsFileName;
}


unsigned int omxclient::QtApplication::IsConnected()const
{
    return m_nConnected;
}


unsigned int omxclient::QtApplication::IsConfigLoaded()const
{
    return m_nConfigLoaded;
}


const std::vector<omxclient::SChannelItem>& omxclient::QtApplication::GetChannels()const
{
    return m_vectChannels;
}


void omxclient::QtApplication::Connect(const std::string& a_serverName)
{
    SDevTaskStruct aDevTask(CONNECT_PRIVATE_NO_BUF);
    m_serverName = a_serverName;
    aDevTask.arg1 = &m_serverName;
    m_fifoDevTask.AddElement(aDevTask);
    m_semaForDevThread.post();
}


void omxclient::QtApplication::Disconnect()
{
    SDevTaskStruct aDevTask(DEVTASK::DISCONNECT);
    m_fifoDevTask.AddElement(aDevTask);
    m_semaForDevThread.post();
}


void omxclient::QtApplication::NewHostFoundSlot(QString a_hostName)
{
    m_raspberriesGui.push_back(a_hostName);
}


void omxclient::QtApplication::StopAllSlot()
{
    if(m_nConnected){
        int nRet = m_clientFts.RunCommandOnRemoteHost2("",0);
        if(m_pActiveButton){
            setStyleSheet("background-color:white;");
        }
	if(nRet<0){Disconnect();}
    }
    if(m_pCentralTab){m_pCentralTab->setCurrentIndex(0);}
}


void omxclient::QtApplication::LoadConfigFileSlot()
{
    QFileDialog aDlg;
    QStringList filters;

    filters << "Stream files (*.m3u8)"
            << "Text files (*.txt)"
            << "Any files (*)";
    aDlg.setNameFilters(filters);
    aDlg.exec();

    QStringList selectedFiles = aDlg.selectedFiles();

    if(selectedFiles.size()){
        QString aFileName = selectedFiles.at(0);
        LoadConfigFileAny(aFileName.toStdWString());
	if(m_pToSet){m_pToSet->setText(aFileName);}
    } // if(selectedFiles.size()){
}


// #EXTINF:
// #EXTGRP:
void omxclient::QtApplication::LoadConfigFileAny(const std::wstring& a_fileName)
{
    if(m_nConfigLoaded){return;}

    const char* EXTINF="#EXTINF:";
    const char* EXTGRP="#EXTGRP:";
    SChannelItem channelItem;
    std::wstring aLine;
    QString line ;
    QString groupName("default"), channelName("default"), url("default");
    QFile qfConfFile (QString::fromStdWString(a_fileName));

    m_channelFileName = a_fileName;
    if (!qfConfFile.open(QIODevice::ReadOnly | QIODevice::Text)){ return;}
    QTextStream aStream(&qfConfFile);
    aStream.setCodec("UTF-8");

    while(!aStream.atEnd()){

        line = aStream.readLine();

        if(line.contains(tr(EXTGRP))){
            aLine = line.toStdWString().c_str()+strlen(EXTGRP);// to be corrected
            groupName = QString::fromStdWString(aLine);
        }
        else if(line.contains(tr(EXTINF))){
            aLine = line.toStdWString().c_str()+strlen(EXTINF)+2; // to be corrected
            channelName = QString::fromStdWString(aLine);
        }
        else if(line.contains(tr("#"))){
            continue;
        }
        else{
            url = line;
            channelItem.grpName = groupName;
            channelItem.chnName = channelName;
            channelItem.url = url;
            m_vectChannels.push_back(channelItem);
        }
    } // while(!aStream.atEnd()){

    emit ConfigLoadedSignal();
    m_nConfigLoaded = 1;
}


void omxclient::QtApplication::EmitAddNewChnlAny(const QString& a_groupName,
                                                 const QString& a_chnName,const QUrl& a_url)
{
    emit AddNewChnlSignal(a_groupName,a_chnName,a_url);
}


void omxclient::QtApplication::EmitSetSelectedTabAny(int a_nIndex)
{
    emit SetSelectedTabSignal(a_nIndex);
}


int omxclient::QtApplication::RunCommandOnRemoteHost(const char* a_command, int a_count)
{
    if(m_nConnected){
        int nReturn = m_clientFts.RunCommandOnRemoteHost2(a_command,a_count);
        if(nReturn<0){Disconnect();}
    }
    return -1;
}


bool omxclient::QtApplication::MakeOneTest(const char* a_hostName)
{
    common::SocketTCP aSearchSocket;
    int nRet;
    char vcForRead[2];
    bool bRet(false);

    nRet= aSearchSocket.connectC(a_hostName,FTS_SINGLE_SHOT_PORT);
    if(!nRet){
        if((aSearchSocket.readC(vcForRead,2,10)==2)&&(vcForRead[0]=='o')&&(vcForRead[1]=='k')){
            QString raspIp;
            printf("found!!!!!!!!!!!!!!!\n");
            m_raspberriesDev.push_back(a_hostName);
            raspIp = tr(a_hostName);
            emit NewHostFoundSignal(raspIp);
            bRet = true;
        }

    }
    aSearchSocket.closeC();
    return bRet;
}


void omxclient::QtApplication::DeviceThreadFunction(void)
{
    SDevReturn strDevRet;
    SDevTaskStruct aDevTask;
    std::string* pServName;
    bool bFree;

    m_nWork = 1;
    m_nConnected = 0;

    while(m_nWork){
        m_semaForDevThread.wait();

        while(m_fifoDevTask.Extract(&aDevTask)){
            if(m_nWork){
                strDevRet.inp = aDevTask;
                bFree = true;
                switch(aDevTask.type)
                {
                case DEVTASK::NO_TASK:
                    break;
                case CONNECT_PRIVATE_NO_BUF:
                    strDevRet.inp.type = DEVTASK::CONNECT;
                    bFree = false;
                case DEVTASK::CONNECT:
                    if(!m_nConnected){
                        pServName = (std::string*)aDevTask.arg1;
                        strDevRet.ret = m_clientFts.ConnectPermanent(pServName->c_str());
                        if(!strDevRet.ret){m_nConnected=1;}
                    }else{strDevRet.ret=-1;}
                    if(bFree){delete pServName;}
                    emit DeviceThreadReturnSignal(strDevRet);
                    break;
                case DEVTASK::DISCONNECT:
                    if(m_nConnected){
                        m_clientFts.DisconnectFromPermanent();strDevRet.ret=0;
                        m_nConnected = 0;
                    }else{strDevRet.ret=-1;}
                    emit DeviceThreadReturnSignal(strDevRet);
                    break;
                default:
                    break;
                }
            } // if(m_nWork){
        } // while(m_fifoDevTask.Extract(&aDevTask)){
    } // while(m_nWork){
}


void omxclient::QtApplication::SearchThreadFunction(void)
{
    int i;
    int nIndex;
    size_t ifnd,unFounded;
    const char* hostIp;
    char* pcLastDotPlus1;
    size_t unLenToWrite;
    char l_host[MAX_HOSTNAME_LENGTH], l_server[MAX_HOSTNAME_LENGTH];
    bool bFound;

    hostIp=GetOwnHostName(l_host,MAX_HOSTNAME_LENGTH);
    strncpy(l_server,hostIp,MAX_HOSTNAME_LENGTH);
    pcLastDotPlus1 = (strrchr(l_server,'.')+1);
    unLenToWrite = MAX_HOSTNAME_LENGTH-((size_t)pcLastDotPlus1-(size_t)l_server);

    printf("l_host=%s, pcLastDotPlus1=%s, unLenToWrite=%d\n",hostIp,pcLastDotPlus1,(int)unLenToWrite);

    m_nWork = 1;

    if(m_serverName.length()){MakeOneTest(m_serverName.c_str());}

    while(m_nWork){
        for(i=0; (i<255)&&m_nWork;++i){
            if(!m_nConnected){if(!m_nWork){goto returnPoint;}Sleep(1000);}
            nIndex = (i+9)%256;
            snprintf(pcLastDotPlus1,unLenToWrite,"%d",nIndex);
            unFounded = m_raspberriesDev.size();
            bFound=false;
            for(ifnd=0;ifnd<unFounded;++ifnd){
                if(m_raspberriesDev[ifnd]==l_server){bFound=true;break;}
            }
            if(bFound){continue;}
            //printf("ServerToCheck=%s\n",l_server);
            MakeOneTest(l_server);

        }

        if(!m_nWork){goto returnPoint;}
        if(m_nConnected){Sleep(20000);}
        else{Sleep(500);}

    }

returnPoint:
    return;
}



/*///////////////////////////////////////////////////////////////////*/

#ifdef WIN32
#include <winsock2.h>
typedef u_long  red_buf_len;
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <memory.h>
#include <sys/ioctl.h>
#define ioctlsocket ioctl
typedef int  red_buf_len;
#endif

//#include <QHostInfo>

#include <iostream>
using namespace std;



static const char* GetOwnHostName(char*,int)
{
#if 0

    printf(
                "hName=%s\n"
                "domName=%s\n",
                QHostInfo::localHostName().toStdString().c_str(),
                QHostInfo::localDomainName().toStdString().c_str());

    QHostAddress addr;
    QHostInfo host = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> ip=host.addresses();

#if 0
    int nSize = ip.size();

    for(int i(0);i<nSize;++i){
        addr = ip.at(i);
        printf("addr[%d]=%s\n",i,addr.toString().toStdString().c_str());
    }
#endif

    foreach (const QHostAddress &address, host.addresses()){
        if (ip[0].toString() == address.toString())
            printf("no reverse DNS for this IP\n");
        else {
            // print domain
            //ui->textEdit->append(address.toString());
            printf("address.toString()=%s\n",address.toString().toStdString().c_str());
        }
    }


    const char* svrName;
    char l_host[MAX_HOSTNAME_LENGTH];
    unsigned int ha;

    if (gethostname(l_host, MAX_HOSTNAME_LENGTH) < 0){return "";}
    svrName = l_host;

    //hostname_to_ip(l_host);
    //return "";

    if ((ha = inet_addr(svrName)) == INADDR_NONE){
        //int i=1;
        struct hostent* hostent_ptr = gethostbyname2(svrName,AF_IPX);

        printf("hostent_ptr->h_name=%s\n",hostent_ptr->h_name);

        for (int i = 0; hostent_ptr->h_addr_list[i] != 0; ++i) {
                struct in_addr addr;
                memcpy(&addr, hostent_ptr->h_addr_list[i], sizeof(struct in_addr));
                cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
            }


#if 0
        svrName = inet_ntoa(*(struct in_addr *)hostent_ptr->h_addr_list[0]);
        printf("svrName=%s\n",svrName);
        while(svrName){
            svrName = inet_ntoa(*(struct in_addr *)hostent_ptr->h_addr_list[i++]);
            printf("svrName=%s\n",svrName);
        }
#endif
    }
#endif

#ifdef DEBUG_APP_NON_RPI
    return "141.34.30.1";
#else
    return "192.168.0.8";
#endif
}

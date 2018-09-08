
// omxclient_qtapplication.hpp
// 2017 Aug 06

#ifndef OMXCLIENT_QTAPPLICATION_HPP
#define OMXCLIENT_QTAPPLICATION_HPP

#include <QApplication>
#include "util_ftsclient.hpp"
#include <thread>
#include <vector>
#include <string>
#include <QPushButton>
#include <QUrl>
#include "common_unnamedsemaphorelite.hpp"
#include "common_fifofast.hpp"
#include "omxclient_commondefs.hpp"
#include <QTableWidget>

namespace omxclient {

struct SChannelItem{QString grpName; QString chnName;QUrl url;};

class QtApplication : public QApplication
{
    Q_OBJECT
public:
    QtApplication(int &argc, char **argv);
    ~QtApplication();

    // members access functions
    const std::string& GetServerName()const;
    void SetServerName(const std::string& srvName);
    const std::wstring& GetChannelsFileName()const;
    void SetChannelsFileName(const std::wstring& chnlsFileName);
    unsigned int IsConnected()const;
    unsigned int IsConfigLoaded()const;
    const std::vector<omxclient::SChannelItem>& GetChannels()const;

    // Other API
    void Connect(const std::string& serverName);
    void Disconnect();

    //void EmitDisconnectedAny(); // any thread
    void EmitAddNewChnlAny(const QString&,const QString&,const QUrl&);
    void EmitSetSelectedTabAny(int);

    int RunCommandOnRemoteHost(const char* command, int count);
    void LoadConfigFileAny(const std::wstring& a_fileName);


public:
signals:
    void NewHostFoundSignal(QString hostName);
    void ConfigLoadedSignal();
    void AddNewChnlSignal(const QString&,const QString&,const QUrl&);
    void SetSelectedTabSignal(int);
    void DeviceThreadReturnSignal(const omxclient::SDevReturn&);

public slots:
    void StopAllSlot();
    void LoadConfigFileSlot();

private slots:
    void NewHostFoundSlot(QString hostName);

private:
    void SearchThreadFunction(void);
    void DeviceThreadFunction(void);
    bool MakeOneTest(const char* hostName);

private:
    enum{CONNECT_PRIVATE_NO_BUF=DEVTASK::CONNECT+1};
private:
    unsigned int                        m_nConnected : 1;
    unsigned int                        m_nWork : 1;
    unsigned int                        m_nConfigLoaded : 1;
    util::FtsClient                     m_clientFts;
    std::thread                         m_threadSearchNewRasp;
    std::thread                         m_threadDev;
    std::vector<QString>                m_raspberriesGui;
    std::vector<std::string>            m_raspberriesDev;
    std::vector<SChannelItem>           m_vectChannels;
    std::string                         m_serverName;
    std::wstring                        m_channelFileName;

    common::UnnamedSemaphoreLite        m_semaForDevThread;
    common::FifoFast<SDevTaskStruct,4>  m_fifoDevTask;

public:
    QPushButton*                        m_pActiveButton;

    // to be deleted
    QPushButton* m_pToSet;
    QTabWidget*	m_pCentralTab;

};

extern std::vector<omxclient::QtApplication*>   g_vectorOfApplications;

}

#define THIS_APP    ((QtApplication*)(QCoreApplication::instance()))
#define m_pActiveButtonMacro    THIS_APP->m_pActiveButton


#endif // OMXCLIENT_QTAPPLICATION_HPP

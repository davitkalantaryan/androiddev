
// omxclient_centralwidget.cpp
// 2017 Aug 03

#include "omxclient_centralwidget.hpp"
#include "omxplayer_centraltab.hpp"
#include <string>
#include "omxclient_qtapplication.hpp"
//#include <QFileDialog>
//#include <QTextStream>

#define m_layoutSrvNmAddBtn m_layoutMain
#define m_layoutMainBtns m_layoutMain
#define m_layoutAnyCommand m_layoutMain
//#define m_layoutAnyCommand m_layoutMain


omxclient::CentralWidget::CentralWidget()
{
    THIS_APP->m_pToSet = &m_btnLoadConfig;

    //
    m_btnLoadConfig.setText("Load config");
    m_btnConnectToServer.setText("Connect To RPI");
    m_btnAddNewChannel.setText("Add new channel");
    m_btnAnyCommand.setText("Run command");
    m_btnStopAll.setText("Stop all");
    m_btnExit.setText("Exit");

    m_layoutSrvNmAddBtn.addWidget(&m_serverName);
    m_layoutSrvNmAddBtn.addWidget(&m_btnConnectToServer);
    m_layoutMain.addLayout(&m_layoutSrvNmAddBtn);
    m_layoutMainBtns.addWidget(&m_btnAddNewChannel);
    m_layoutMainBtns.addWidget(&m_btnLoadConfig);
    m_layoutMain.addLayout(&m_layoutMainBtns);
    m_layoutAnyCommand.addWidget(&m_leAnyCommand);
    m_layoutAnyCommand.addWidget(&m_btnAnyCommand);
    m_layoutMain.addLayout(&m_layoutAnyCommand);
    m_layoutMain.addWidget(&m_btnStopAll);
    m_layoutMain.addWidget(&m_btnExit);

    setLayout(&m_layoutMain);

    connect(&m_btnConnectToServer,SIGNAL(clicked()),this,SLOT(ConnectOrDisconnectSlot()));
    connect(THIS_APP,SIGNAL(DeviceThreadReturnSignal(const omxclient::SDevReturn&)),
            this,SLOT(DeviceReturnSlot(const omxclient::SDevReturn&)));
    connect(&m_btnAddNewChannel,SIGNAL(clicked()),this,SLOT(AddNewChannelSlot()));
    connect(&m_btnLoadConfig,SIGNAL(clicked()),THIS_APP,SLOT(LoadConfigFileSlot()));
    connect(&m_btnAnyCommand,SIGNAL(clicked()),this,SLOT(RunCommandSlot()));
    connect(THIS_APP,SIGNAL(NewHostFoundSignal(QString)),
            this,SLOT(NewServerIsFoundedSlot(QString)));
    connect(&m_btnStopAll,SIGNAL(clicked()),THIS_APP,SLOT(StopAllSlot()));
    connect(&m_btnExit,SIGNAL(clicked()),THIS_APP,SLOT(quit()));

    if(THIS_APP->GetServerName().length()){
        m_serverName.setText(THIS_APP->GetServerName().c_str());
        THIS_APP->Connect(THIS_APP->GetServerName());
    }

    if(THIS_APP->GetChannelsFileName().length()){
        THIS_APP->LoadConfigFileAny(THIS_APP->GetChannelsFileName());
    }

}


omxclient::CentralWidget::~CentralWidget()
{
    m_layoutMain.removeWidget(&m_btnExit);
    //m_layoutMain.removeItem(&m_layoutMainBtns);
    m_layoutMainBtns.removeWidget(&m_btnLoadConfig);
    m_layoutMainBtns.removeWidget(&m_btnAddNewChannel);
    m_layoutMain.removeItem(&m_layoutSrvNmAddBtn);
    m_layoutSrvNmAddBtn.removeWidget(&m_btnConnectToServer);
    m_layoutSrvNmAddBtn.removeWidget(&m_serverName);
    m_layoutMain.removeWidget(&m_btnStopAll);
}


void omxclient::CentralWidget::RunCommandSlot()
{
    printf("!!!! fnc:%s\n",__FUNCTION__);
    if(m_pActiveButtonMacro){
        m_pActiveButtonMacro->setStyleSheet("background-color:white;");
    }

    std::string aCommandToRun = m_leAnyCommand.text().toStdString();
    THIS_APP->RunCommandOnRemoteHost(aCommandToRun.c_str(),1);
}


void omxclient::CentralWidget::NewServerIsFoundedSlot(QString a_serverName)
{
    if(!THIS_APP->IsConnected()){
        m_serverName.setText(a_serverName);
        THIS_APP->Connect(a_serverName.toStdString());
    }
}


void omxclient::CentralWidget::DeviceReturnSlot(const omxclient::SDevReturn& a_devRet)
{
    switch(a_devRet.inp.type)
    {
    case DEVTASK::NO_TASK:
        break;
    case DEVTASK::CONNECT:
        if(a_devRet.ret==0){ // connected succesfuly
            m_btnConnectToServer.setText("Disconnect from RPI");
            m_btnConnectToServer.setEnabled(true);
            m_serverName.setDisabled(true);
        }
        else{
            // QMexxageBox // unable to connect
            m_btnConnectToServer.setText("Connect To RPI");
            m_btnConnectToServer.setEnabled(true);
            m_serverName.setEnabled(true);
        }
        break;
    case DEVTASK::DISCONNECT:
        m_btnConnectToServer.setText("Connect To RPI");
        m_btnConnectToServer.setEnabled(true);
        m_serverName.setEnabled(true);
        break;
    default:
        break;
    }
}


void omxclient::CentralWidget::ConnectOrDisconnectSlot()
{
    if(THIS_APP->IsConnected()){
        m_btnConnectToServer.setDisabled(true);
        THIS_APP->Disconnect();
    }
    else{
        m_btnConnectToServer.setDisabled(true);
        THIS_APP->Connect(m_serverName.text().toStdString());
    }  // else for if(m_nConnected){
}


void omxclient::CentralWidget::AddNewChannelSlot()
{
    if(m_newChannelDlg.ExecNew()){
        QString groupName, channelName,url;
        m_newChannelDlg.GetStrings(&groupName,&channelName,&url);
        THIS_APP->EmitAddNewChnlAny(groupName,channelName,QUrl(url));
    }
}


/*////////////////////////////////////////////////*/
omxclient::NewChannelDlg::NewChannelDlg()
{
    m_ok.setText("ok");
    m_leGroup.setText("default");
    m_lblGroup.setText("Group:");
    m_lblChannelName.setText("Channel:");
    m_lblUrl.setText("URL:");

    m_layoutGroup.addWidget(&m_lblGroup);
    m_layoutGroup.addWidget(&m_leGroup);

    m_layoutChannelName.addWidget(&m_lblChannelName);
    m_layoutChannelName.addWidget(&m_leChannelName);

    m_layoutUrl.addWidget(&m_lblUrl);
    m_layoutUrl.addWidget(&m_leUrl);

    m_layoutMain.addLayout(&m_layoutGroup);
    m_layoutMain.addLayout(&m_layoutChannelName);
    m_layoutMain.addLayout(&m_layoutUrl);
    m_layoutMain.addWidget(&m_ok);

    setLayout(&m_layoutMain);

    connect(&m_ok,SIGNAL(clicked()),this,SLOT(OkPushSlot()));
}


omxclient::NewChannelDlg::~NewChannelDlg()
{
    m_layoutGroup.removeWidget(&m_lblGroup);
    m_layoutGroup.removeWidget(&m_leGroup);

    m_layoutChannelName.removeWidget(&m_lblChannelName);
    m_layoutChannelName.removeWidget(&m_leChannelName);

    m_layoutUrl.removeWidget(&m_lblUrl);
    m_layoutUrl.removeWidget(&m_leUrl);

    m_layoutMain.removeItem(&m_layoutGroup);
    m_layoutMain.removeItem(&m_layoutChannelName);
    m_layoutMain.removeItem(&m_layoutUrl);
    m_layoutMain.removeWidget(&m_ok);

}


void omxclient::NewChannelDlg::OkPushSlot()
{
    m_isOk = true;
    QDialog::close();
}


bool omxclient::NewChannelDlg::ExecNew()
{
    m_isOk = false;
    QDialog::exec();
    return m_isOk;
}


void omxclient::NewChannelDlg::GetStrings(QString* a_groupName, QString* a_channelName, QString* a_url)
{
    *a_groupName = m_leGroup.text();
    *a_channelName = m_leChannelName.text();
    *a_url = m_leUrl.text();
}


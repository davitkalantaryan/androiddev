
// omxclient_centralwidget.hpp
// 2017 Aug 03

#ifndef OMXCLIENT_CENTRALWIDGET_HPP
#define OMXCLIENT_CENTRALWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QLabel>
#include "omxclient_commondefs.hpp"

namespace omxclient{

class NewChannelDlg : private QDialog
{
    Q_OBJECT
public:
    NewChannelDlg();
    ~NewChannelDlg();

    bool ExecNew();
    void GetStrings(QString* a_groupName, QString* a_channelName, QString* a_url);

private slots:
    void OkPushSlot();

private:
    QVBoxLayout m_layoutMain;
    QHBoxLayout m_layoutGroup;
    QHBoxLayout m_layoutChannelName;
    QHBoxLayout m_layoutUrl;

    QLabel      m_lblGroup;
    QLabel      m_lblChannelName;
    QLabel      m_lblUrl;

    QLineEdit   m_leGroup;
    QLineEdit   m_leChannelName;
    QLineEdit   m_leUrl;
    QPushButton m_ok;

    bool        m_isOk;
};

class CentralTab;

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget();
    ~CentralWidget();

public slots:
    void ConnectOrDisconnectSlot();
private slots:
    void AddNewChannelSlot();
    void RunCommandSlot();
    void NewServerIsFoundedSlot(QString serverName);
    void DeviceReturnSlot(const omxclient::SDevReturn&);  // informs -> connect, disconnect,

private:
    QVBoxLayout m_layoutMain;
    //QHBoxLayout m_layoutSrvNmAddBtn;
    //QHBoxLayout m_layoutMainBtns;
    QLineEdit   m_serverName;
    QPushButton m_btnConnectToServer;
    QPushButton m_btnAddNewChannel;
    QPushButton m_btnLoadConfig;
    NewChannelDlg   m_newChannelDlg;
    //QHBoxLayout m_layoutAnyCommand;
    QLineEdit   m_leAnyCommand;
    QPushButton m_btnAnyCommand;
    QPushButton m_btnStopAll;
    QPushButton	m_btnExit;

};

}

#endif // OMXCLIENT_CENTRALWIDGET_HPP

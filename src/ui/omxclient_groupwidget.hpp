
// omxclient_groupwidget.hpp
// 2017 Aug 06

#ifndef OMXCLIENT_GROUPWIDGET_HPP
#define OMXCLIENT_GROUPWIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QUrl>

namespace omxclient {


class NewPushButton : public QPushButton
{
    Q_OBJECT
public:
    NewPushButton(const QUrl& url,NewPushButton* next,int index);
    ~NewPushButton();

    QString getUrl()const;
    void RemoveLinks();
    //void SetIndexInTabAny(int index);

private slots:
    void RunProcessOnRemoteSlot();

public:
    NewPushButton* m_prev;
    NewPushButton* m_next;
private:
    QUrl        m_url2;
    int         m_nIndexInTab;
};


class GroupWidget : public QWidget
{
    Q_OBJECT
public:
    GroupWidget(const QString& groupName);
    ~GroupWidget();

    void AddNewChannelGui(const QString& channelName, const QUrl& url);
    //void SetTabAndIndexInTabAny(CentralTab* tab,int index);
    void SetIndexInTabAny(int index);

private slots:
    void RemoveChannelSlot(void* channelBtnPtr);

private:
    //
    QString         m_groupName;
    NewPushButton*  m_firstBtn;
    QPushButton     m_btnStopAll;
    QVBoxLayout     m_lytMain;

    int             m_nNumberOfButtons;
    int             m_nIndexInTab;
};

}

#endif // OMXCLIENT_GROUPWIDGET_HPP

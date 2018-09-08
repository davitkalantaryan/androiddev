
// omxclient_groupwidget.cpp
// 2017 Aug 06


#include "omxclient_groupwidget.hpp"
#include "omxclient_qtapplication.hpp"

#ifdef ANDROID
#define     _BUTTON_HEIGHT_     128
#define     _BUTTON_WIDTH_     500
#elif defined(IOS_MOBILE)
#define     _BUTTON_HEIGHT_     120
#define     _BUTTON_WIDTH_     500
#else
#define     _BUTTON_HEIGHT_     22
#define     _BUTTON_WIDTH_     250
#endif


omxclient::GroupWidget::GroupWidget(const QString& a_groupName)
    :
      m_groupName(a_groupName)
{
    setFixedWidth(_BUTTON_WIDTH_+20);
    m_nNumberOfButtons = 1;
    m_firstBtn = NULL;
    m_btnStopAll.setFixedSize(_BUTTON_WIDTH_,_BUTTON_HEIGHT_);
    m_btnStopAll.setText("Stop All");

    m_lytMain.addWidget(&m_btnStopAll);

    setLayout(&m_lytMain);
    connect(&m_btnStopAll,SIGNAL(clicked()),THIS_APP,SLOT(StopAllSlot()));
}


omxclient::GroupWidget::~GroupWidget()
{
    NewPushButton* pTmp = m_firstBtn;

    while(m_firstBtn){
        pTmp = m_firstBtn->m_next;
        m_lytMain.removeWidget(m_firstBtn);
        delete m_firstBtn;
        m_firstBtn = pTmp;
    }

    m_lytMain.removeWidget(&m_btnStopAll);
}


void omxclient::GroupWidget::SetIndexInTabAny(int a_index)
{
    //m_pCentralTab = NULL;
    m_nIndexInTab = a_index;
}


void omxclient::GroupWidget::AddNewChannelGui(
        const QString& a_channelName,const QUrl& a_url)
{
    NewPushButton* pTmp = m_firstBtn;
    NewPushButton* pNewBtn = new NewPushButton(a_url,m_firstBtn,m_nIndexInTab);

    if(!pNewBtn){
        // error report
        return;
    }
    m_firstBtn = pNewBtn;
    if(pTmp){pTmp->m_prev = pNewBtn;}
    pNewBtn->setText(a_channelName);
    //m_lytMain.addSpacing(1);
    m_lytMain.addWidget(pNewBtn);
    resize( size().width(), (5+_BUTTON_HEIGHT_) * (++m_nNumberOfButtons) );
}


void omxclient::GroupWidget::RemoveChannelSlot(void* a_chnlBtnPtr)
{
    NewPushButton* pBtnToDelete = (NewPushButton*)a_chnlBtnPtr;

    if(pBtnToDelete==m_firstBtn){m_firstBtn=pBtnToDelete->m_next;}
    pBtnToDelete->RemoveLinks();
    pBtnToDelete->hide();
    m_lytMain.removeWidget(pBtnToDelete);
    pBtnToDelete->deleteLater();
    resize( size().width(), (5+_BUTTON_HEIGHT_) * (--m_nNumberOfButtons) );
}


/*///////////////////////////////////////////////*/
omxclient::NewPushButton::NewPushButton(const QUrl& a_url,NewPushButton* a_next,int a_index)
    :
      m_url2(a_url)
{
    m_nIndexInTab = a_index;
    setFixedSize(_BUTTON_WIDTH_,_BUTTON_HEIGHT_);
    setStyleSheet("background-color:white;");
    m_prev = NULL;
    m_next = a_next;
    connect(this,SIGNAL(clicked()),this,SLOT(RunProcessOnRemoteSlot()));
}


omxclient::NewPushButton::~NewPushButton()
{
    disconnect(this,SIGNAL(clicked()),this,SLOT(RunProcessOnRemoteSlot()));
}


void omxclient::NewPushButton::RemoveLinks()
{
    if(m_prev){m_prev->m_next = m_next;}
    if(m_next){m_next->m_prev = m_prev;}
}


QString omxclient::NewPushButton::getUrl()const
{
    return m_url2.toString();
}


void omxclient::NewPushButton::RunProcessOnRemoteSlot()
{
    printf("!!!! fnc:%s\n",__FUNCTION__);
    if(m_pActiveButtonMacro){
        m_pActiveButtonMacro->setStyleSheet("background-color:white;");
    }

    std::string aCommandToRun =
            "omxplayer.bin --live --timeout 20 -o hdmi "+m_url2.toString().toStdString();
    THIS_APP->RunCommandOnRemoteHost(aCommandToRun.c_str(),-1);
    setStyleSheet("background-color:rgb(0,255,0);");
    m_pActiveButtonMacro = this;
    THIS_APP->EmitSetSelectedTabAny(m_nIndexInTab);
}

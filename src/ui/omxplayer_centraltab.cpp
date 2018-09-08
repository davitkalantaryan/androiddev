

// omxplayer_centraltab.cpp
// 2017 Aug 06
#include "omxplayer_centraltab.hpp"
#include "omxclient_qtapplication.hpp"
#include <QTabBar>


omxclient::CentralTab::CentralTab()
    :
      m_youtubeTab("https://www.youtube.com/",false),
      m_edemTab("https://edem.tv/",false),
      m_anyWebTab("https://www.google.com/",true)
{
    THIS_APP->m_pCentralTab = this;

#ifdef ANDROID
    setStyleSheet("QTabBar::tab { height: 100px; }");
#endif

    m_nIndexPrevious = -1;
    addTab(&m_mainWidget,"Main");
    addTab(&m_youtubeTab,"Youtube");
    addTab(&m_edemTab,"edem");
    addTab(&m_anyWebTab,"web");

    if(THIS_APP->IsConfigLoaded()){ConfigFileLoadedSlot();}

    connect(THIS_APP,SIGNAL(ConfigLoadedSignal()),this,SLOT(ConfigFileLoadedSlot()));
    connect(THIS_APP,SIGNAL(AddNewChnlSignal(const QString&,const QString&,const QUrl&)),
            this,SLOT(AddNewChnlSlot(const QString&,const QString&,const QUrl&)));
    connect(THIS_APP,SIGNAL(SetSelectedTabSignal(int)),this,SLOT(SetSelectedTabSlot(int)));

}


void omxclient::CentralTab::SetSelectedTabSlot(int a_index)
{
    if(m_nIndexPrevious>=0){
        tabBar()->setTabTextColor(m_nIndexPrevious,QColor(Qt::black));
    }
    tabBar()->setTabTextColor(a_index,QColor(Qt::green));
    m_nIndexPrevious = a_index;
}


omxclient::CentralTab::~CentralTab()
{
    //
}


void omxclient::CentralTab::AddNewChnlSlot(
        const QString& a_groupName, const QString& a_channelName, const QUrl& a_url)
{
    SGrp* pGrp;
    std::wstring grpName = a_groupName.toStdWString();

    if(!m_hashTabs.FindEntry(grpName.c_str(),grpName.length()*2,&pGrp)){
        int nIndex;
        if(a_groupName==tr("Main")){
            // error report
            return;
        }
        pGrp = new SGrp;

        pGrp->grpWidget = new GroupWidget(a_groupName);
        if(!pGrp->grpWidget){
            delete pGrp;
            return;
        }

        pGrp->scrArea = new QScrollArea;
        if(!pGrp->scrArea){
            delete pGrp->grpWidget;
            delete pGrp;
            return;
        }
        pGrp->scrArea->setWidget(pGrp->grpWidget);
        m_hashTabs.AddEntry(grpName.c_str(),grpName.length()*2,pGrp);
        nIndex=addTab(pGrp->scrArea,a_groupName);
        pGrp->grpWidget->SetIndexInTabAny(nIndex);
    }

    pGrp->grpWidget->AddNewChannelGui(a_channelName,a_url);
}


void omxclient::CentralTab::ConfigFileLoadedSlot()
{
    const std::vector<SChannelItem>& vectChannels = THIS_APP->GetChannels();
    const size_t cunNumOfChannels = vectChannels.size();

    for(size_t i(0);i<cunNumOfChannels;++i)
        AddNewChnlSlot(vectChannels[i].grpName,vectChannels[i].chnName,vectChannels[i].url);
}

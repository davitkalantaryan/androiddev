
// omxplayer_centraltab.hpp
// 2017 Aug 06

#ifndef OMXPLAYER_CENTRALTAB_HPP
#define OMXPLAYER_CENTRALTAB_HPP

#include <QTabWidget>
#include "omxclient_centralwidget.hpp"
#include "common_hashtbl.hpp"
#include "omxclient_groupwidget.hpp"
#include <QScrollArea>
#include "omxclient_youtube.hpp"
#include <QUrl>

namespace omxclient{

class CentralTab : public QTabWidget
{
    Q_OBJECT
public:
    CentralTab();
    ~ CentralTab();

private slots:
    void SetSelectedTabSlot(int index);
    void ConfigFileLoadedSlot();
    void AddNewChnlSlot(const QString& groupName, const QString& channelName, const QUrl& url);

private:
    struct SGrp{QScrollArea* scrArea; GroupWidget* grpWidget;};

private:
    CentralWidget           m_mainWidget;
    common::HashTbl<SGrp*>  m_hashTabs;
    int                     m_nIndexPrevious;
    YoutubeWidget           m_youtubeTab;
    YoutubeWidget           m_edemTab;
    YoutubeWidget           m_anyWebTab;
};

}

#endif // OMXPLAYER_CENTRALTAB_HPP

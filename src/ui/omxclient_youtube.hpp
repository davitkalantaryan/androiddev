
// omxclient_youtube.hpp
// 2017 Aug 13

#ifndef OMXCLIENT_YOUTUBE_HPP
#define OMXCLIENT_YOUTUBE_HPP

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUrl>
#ifdef webkitwidgets_defined
#include <QWebView>
#include <QWebPage>
#define PARENT_CLASS QWebView
#if (PARENT_CLASS==QWebView)
#define USED_CLASS2(_memb_)
#else  // #if (PARENT_CLASS==QWebView)
#define USED_CLASS2(_memb_) QWebPage _memb_;
#endif  // #ifdef PARENT_CLASS
#else // #ifdef webkitwidgets_defined
#define PARENT_CLASS QWidget
#define USED_CLASS2(_memb_)
#endif  // #ifdef webkitwidgets_defined


namespace omxclient{

class MyWebPage : public PARENT_CLASS
{
    Q_OBJECT
public:
    MyWebPage();
    void load(const QUrl& url);

private slots:
    void renderSlot();

private:
    USED_CLASS2(m_wp)
};

class YoutubeWidget : public QWidget
{
    Q_OBJECT
public:
    YoutubeWidget(const QString& a_initialPage, bool a_naviEnabled);
    ~YoutubeWidget();

private slots:
    void ShowYoutubeVideoSlot();
    void LinkChangedSlot( const QUrl& link);
    void LinkClickedSlot( const QUrl& link);
    void SetNewLinkSlot();

private:
    QVBoxLayout m_lytMain;
    QLineEdit   m_leUrl;
    MyWebPage   m_web;
};

}

#endif // OMXCLIENT_YOUTUBE_HPP

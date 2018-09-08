

// omxclient_youtube.cpp
// 2017 Aug 13

#include "omxclient_youtube.hpp"
#include "omxclient_qtapplication.hpp"
#include <QPainter>
#ifdef webkitwidgets_defined
#include <QWebFrame>
#endif
//#include <QWebEngineView>
//#include <QtWebView>


omxclient::YoutubeWidget::YoutubeWidget(const QString& a_initialPage, bool a_naviEnabled)
{
    m_leUrl.setText(a_initialPage);
    m_web.load(QUrl(m_leUrl.text()));
    //pQWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    m_leUrl.setEnabled(a_naviEnabled);

    m_lytMain.addWidget(&m_leUrl);
    m_lytMain.addWidget(&m_web);

    setLayout(&m_lytMain);

    connect(&m_web,SIGNAL(urlChanged(const QUrl&)),this,SLOT(LinkChangedSlot(const QUrl&)));
    //connect(pQWebView->page(),SIGNAL(linkClicked(const QUrl&)),this,SLOT(LinkClickedSlot(const QUrl&)));

    connect(&m_leUrl,SIGNAL(editingFinished()),this,SLOT(SetNewLinkSlot()));

}


omxclient::YoutubeWidget::~YoutubeWidget()
{
    m_lytMain.removeWidget(&m_leUrl);
    m_lytMain.removeWidget(&m_web);
}


void omxclient::YoutubeWidget::SetNewLinkSlot()
{
    QString aNewLink = m_leUrl.text();
    m_web.load(QUrl(aNewLink));
}



void omxclient::YoutubeWidget::ShowYoutubeVideoSlot()
{
}


void omxclient::YoutubeWidget::LinkChangedSlot(const QUrl& a_link)
{
    printf("1. link=%s\n",a_link.toString().toStdString().c_str());
    m_leUrl.setText(a_link.toString());
}

void omxclient::YoutubeWidget::LinkClickedSlot(const QUrl& a_link)
{
    printf("2. link=%s\n",a_link.toString().toStdString().c_str());
}


/* ////////////// */

omxclient::MyWebPage::MyWebPage()
{
#ifdef webkitwidgets_defined
    //connect(&m_wp, SIGNAL(loadFinished(bool)),this, SLOT(renderSlot()));
#endif
}


void omxclient::MyWebPage::load(const QUrl& a_url)
{
    printf("!!!!!%s, uri=%s\n",__FUNCTION__,a_url.toString().toStdString().c_str());

#ifdef webkitwidgets_defined
#if (PARENT_CLASS==QWebView)
    QWebView::load(a_url);
#else   //#if (PARENT_CLASS==QWebView)
    m_wp.load(a_url);
#endif  //#if (PARENT_CLASS==QWebView)
#endif  // #ifdef webkitwidgets_defined
}


void omxclient::MyWebPage::renderSlot()
{
#ifdef webkitwidgets_defined
#if 0
    m_wp.setViewportSize(m_wp.mainFrame()->contentsSize());

    QImage image(m_wp.viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);

    m_wp.mainFrame()->render(&painter);
    painter.end();

    printf("!!!!!!!!!!!!!!!!!!!!\n");

    //QImage thumbnail = image.scaled(400, 400);
    //thumbnail.save("thumbnail.png");
    //emit finished();
#endif
#endif  // #ifdef webkitwidgets_defined
}

//#include "main_chatsystem.h"
// 2017 Jul 12
// main_chatsystem.cpp

#include <QApplication>
#include "omxplayer_centraltab.hpp"
#include "omxclient_qtapplication.hpp"
#include <stdio.h>
#include <QUrl>
#include <signal.h>
#ifdef ANDROID
#include <QtWebView/QtWebView>
#endif

typedef void(*TYPE_SIG_HANDLER)(int);

static void SignalHandlerSimple(int a_sigNum);

int main(int argc, char* argv[])
{
    freopen( "/dev/null", "w", stderr);

#ifdef WIN32
    printf("nFork=%d\n",nFork);
#else // #ifdef WIN32

    struct sigaction sigAction;
    sigemptyset(&sigAction.sa_mask);
    //sigAction.sa_flags = SA_SIGINFO;
    //sigAction.sa_sigaction = (TYPE_SIG_HANDLER)SignalHandlerSimple;
    sigAction.sa_flags = 0;
    sigAction.sa_handler = (TYPE_SIG_HANDLER)SignalHandlerSimple;
    //sigaction(SIGINT, &sigAction, NULL);
    sigaction(SIGPIPE, &sigAction, NULL);

#endif // #ifdef WIN32

    omxclient::QtApplication myApp(argc, argv);
    qRegisterMetaType<QString>("QString");
    qRegisterMetaType<QUrl>("QUrl");
    qRegisterMetaType<omxclient::SDevReturn>("omxclient::SDevReturn");

    omxclient::CentralTab aMain;

    //printf("!!!!!!!!!!!!!!!!!!!!! \n");

    aMain.show();


    myApp.exec();
    return 0;
}


static void SignalHandlerSimple(int a_sigNum)
{
    omxclient::QtApplication* pApp;
    size_t unSize, unI;

    switch(a_sigNum)
    {
    case SIGINT:
        printf("SIGINT\n"); // will never happen
        //s_ftsServer.StopAllServers();
        break;
    case SIGPIPE:
        printf("SIGPIPE\n");
        unSize = omxclient::g_vectorOfApplications.size();
        for(unI=0;unI<unSize;++unI)
        {
            pApp = omxclient::g_vectorOfApplications[unI];
            pApp->Disconnect();
        }
        break;
    default:
        printf("default\n");
        break;
    }
}


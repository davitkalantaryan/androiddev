
// util_ftsclient.cpp
// 2017 Aug 03

#include "util_ftsclient.hpp"
#include "ftssystem_interface_common.h"
#include <string.h>

util::FtsClient::FtsClient()
{
	m_nPermanentConnected = 0;
}


util::FtsClient::~FtsClient()
{
	DisconnectFromPermanent();
}


bool util::FtsClient::Echo(const char* a_hostName)
{
	common::SocketTCP aSock;
	int nRet = aSock.connectC(a_hostName, FTS_SINGLE_SHOT_PORT);
	char vcBuff[2];

	if (nRet) { aSock.closeC(); return false; }
	nRet = aSock.readC(vcBuff, 2, 1000);
	if((nRet>=2)&&(vcBuff[0]=='o')&&(vcBuff[1]=='k')){aSock.closeC();return true;}
	aSock.closeC(); 
	return false;
}


int util::FtsClient::ConnectPermanent(const char* a_hostName)
{
	char vcBuffer[4];
	int nRet;
	
	if(m_nPermanentConnected){return 1;}
	nRet= m_socketPermanent.connectC(a_hostName, FTS_PERMANENT_CONNECT_PORT);

	if(nRet){return nRet;}
	nRet=m_socketPermanent.readC(vcBuffer, 2, 10000);
	if((nRet>=2)&&(vcBuffer[0]=='o')&&(vcBuffer[1]=='k')){ m_nPermanentConnected=1; return 0;}
	m_socketPermanent.closeC();
	return -1;
}


void util::FtsClient::DisconnectFromPermanent()
{
	if(!m_nPermanentConnected){return;}
	m_socketPermanent.closeC();
	m_nPermanentConnected = 0;
}


int util::FtsClient::RunCommandOnRemoteHost2(const char* a_command, int a_count)
{
    int  nAdditionalToSend;
    char pHeader[FTS_HEADER_SIZE];

    PREPARE_BUFFER1(&nAdditionalToSend,pHeader,a_command,a_count);
    if(m_socketPermanent.writeC(pHeader, FTS_HEADER_SIZE)<0){return -1;}
    return m_socketPermanent.writeC(a_command, nAdditionalToSend);
}

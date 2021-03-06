#include "stdafx.h"
#define SRC_USAGE
#include "common_servertcp.hpp"

#ifdef WIN32
	#include <WinSock2.h>
	#define gettidNew	GetCurrentThreadId
#else
	#include <sys/socket.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <memory.h>
	#include <sys/syscall.h>
	#define gettidNew(...)	syscall(SYS_gettid)
#endif

#include <stdio.h>

namespace WORK_STATUSES {enum{STOPPED=0,TRYING_TO_STOP,RUN};}


common::ServerTCP::ServerTCP()
	:
	m_nWorkStatus(WORK_STATUSES::STOPPED),
	m_nServerThreadId(0)
{
}

common::ServerTCP::~ServerTCP()
{
	StopServer();
}


int common::ServerTCP::StartServer(
	TypeAccept a_fpAddClient, void* a_owner,
	int a_nPort, int* a_pnRetCode, long int a_lnTimeout,
	bool a_bReuse, bool a_bLoopback)
{
	int nError;
	int& nRetCode = a_pnRetCode ? *a_pnRetCode : nError;

	if(m_nWorkStatus != WORK_STATUSES::STOPPED){return -1;}

#ifdef _CD_VERSION__
	nRetCode = CreateServer( a_nPort, a_bReuse,true );
#else
	nRetCode = CreateServer( a_nPort,a_bReuse, a_bLoopback) ;
#endif

	if(nRetCode != 0 ){
		closeC();
		return nRetCode;
	}

	RunServer(a_lnTimeout,a_fpAddClient,a_owner);
	return 0;
}



void common::ServerTCP::RunServer(int a_lnTimeout, TypeAccept a_fpAddClient, void* a_owner)
{
	sockaddr_in remoteAddress;
	SocketTCP aClientSocket;
	int nError, nClientSocket;

	m_nServerThreadId = gettidNew();
	m_nWorkStatus = WORK_STATUSES::RUN;

	while(m_nWorkStatus== WORK_STATUSES::RUN){
		if ((nError=ServerAccept(nClientSocket,a_lnTimeout,&remoteAddress)) == 1){
			aClientSocket.SetSockDescriptor( nClientSocket );
			(*a_fpAddClient)(a_owner,aClientSocket,&remoteAddress);
			aClientSocket.closeC();
		}

#ifndef _CD_VERSION__
#endif
	}

	closeC();
	m_nWorkStatus = WORK_STATUSES::STOPPED;
	m_nServerThreadId = 0;
}



void common::ServerTCP::StopServer(void)
{
	int nCurrentThreadId(gettidNew());
	if(m_nWorkStatus != WORK_STATUSES::RUN){return;}
	
	if(nCurrentThreadId!=m_nServerThreadId){
		m_nWorkStatus = WORK_STATUSES::TRYING_TO_STOP;
		closeC();
		while(m_nWorkStatus== WORK_STATUSES::TRYING_TO_STOP){SWITCH_SCHEDULING(1);}
	}
	else {
		m_nWorkStatus = WORK_STATUSES::STOPPED;
		m_nServerThreadId = 0;
		closeC();
	}
}



/*
 * ServerAccept: server waiting for new connection
 * Parameter:
 *	a_ppClient:	accepted client socket
 * Return:
 *    < 0:	error
 *   	0:	timeout
 *	1:	ok
 */
int common::ServerTCP::ServerAccept(int& a_nClientSocket, int a_lnTimeout, sockaddr_in* a_bufForRemAddress)
{

	fd_set rfds;

	int maxsd = 0;
	int rtn = 0;

	FD_ZERO( &rfds );
	FD_SET( (unsigned int)m_socket, &rfds );
	
	maxsd = m_socket + 1;

	// In not windows cases pselect instead of select can be considered
	// The reason that in pselect m_Timout remains constant
	// But here we choose other solution
	struct timeval		aTimeout2;
	struct timeval*		pTimeout;


	if( a_lnTimeout >= 0 )
	{
#ifdef WIN32
		aTimeout2.tv_sec = a_lnTimeout/1000L;
		aTimeout2.tv_usec = (a_lnTimeout%1000L)*1000L ;
#else
		aTimeout2.tv_sec = (time_t)(a_lnTimeout/1000L);
		aTimeout2.tv_usec = (suseconds_t)((a_lnTimeout%1000L)*1000L) ;
#endif
		pTimeout = &aTimeout2;
	}
	else
	{
		pTimeout = NULL;
	}

	rtn = select(maxsd, &rfds, (fd_set *) 0, (fd_set *) 0, pTimeout);

	switch(rtn)
	{
		case 0:	/* time out */
			return 0;
		case SOCKET_ERROR_NEW:
			if( errno == EINTR ){/*interrupted by signal*/return 2;}
			return(E_SELECT);
		default:
			break;
	}

	if( !FD_ISSET( m_socket, &rfds ) ){return(E_FATAL);}

	struct sockaddr_in addr;

#ifdef	WIN32
	int addr_len = sizeof(addr);
#else
	socklen_t addr_len = sizeof(addr);
#endif
	a_nClientSocket = (int)accept( m_socket, (struct sockaddr *)&addr, &addr_len);


	if(CHECK_FOR_SOCK_INVALID(a_nClientSocket) ){return 0;}

#ifdef	WIN32
	{
		u_long non = 1;
		ioctlsocket( a_nClientSocket, FIONBIO, &non);
	}
#else
	int status;
	if( (status = fcntl( a_nClientSocket, F_GETFL, 0 )) != -1)
	{
		status |= O_NONBLOCK;
		fcntl( a_nClientSocket, F_SETFL, status );
	}
#endif

	if (a_bufForRemAddress){
		//struct sockaddr_in* pIncAddr = (struct sockaddr_in*)a_pIncAddr;
		*a_bufForRemAddress = addr;
	}

	return 1;
}



int common::ServerTCP::CreateServer(int a_nPort, bool a_bReuse,bool a_bLoopback)
{
	struct sockaddr_in addr;
	int rtn = -1,addr_len;
	char l_host[MAX_HOSTNAME_LENGTH];

    m_socket = (int)socket( AF_INET, SOCK_STREAM, 0 );
	if (CHECK_FOR_SOCK_INVALID(m_socket)) { return(E_NO_SOCKET); }
	
	if(a_bReuse){int i(1);setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i));}

	if (gethostname(l_host, MAX_HOSTNAME_LENGTH) < 0){return E_UNKNOWN_HOST;}

	memset( (char *)&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family	= AF_INET;
	addr.sin_port = htons( a_nPort );

	addr.sin_addr.s_addr = htonl( (a_bLoopback ? INADDR_LOOPBACK : INADDR_ANY ) );
	//addr.sin_addr.s_addr = htonl((a_bLoopback ? INADDR_LOOPBACK : INADDR_ANY));

#ifdef	WIN32
	{
		u_long non = 1;
		ioctlsocket( m_socket, FIONBIO, &non);
	}
#else
	int status;
	if( (status = fcntl( m_socket, F_GETFL, 0 )) != -1)
	{
		status |= O_NONBLOCK;
		fcntl( m_socket, F_SETFL, status );
	}
#endif

	addr_len = sizeof(addr);
	rtn = bind( m_socket, (struct sockaddr *) &addr, addr_len );
	if( CHECK_FOR_SOCK_ERROR(rtn) ){return(E_NO_BIND);}

	rtn = ::listen( m_socket, 64);
	if (CHECK_FOR_SOCK_ERROR(rtn)) { return(E_NO_LISTEN); }

	return 0;
}

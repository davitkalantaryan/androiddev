// util_ftsclient.hpp
// 2017 Aug 03

#ifndef __util_ftsclient_hpp__
#define __util_ftsclient_hpp__

#include "common_sockettcp.hpp"
#include "ftssystem_interface_common.h"

namespace util {

class FtsClient
{
public:
	FtsClient();
	virtual ~FtsClient();

	static bool Echo(const char* serverName);
	int ConnectPermanent(const char* hostName);
	void DisconnectFromPermanent();
    int RunCommandOnRemoteHost2(const char* command, int count);

protected:
	common::SocketTCP	m_socketPermanent;
	unsigned int	m_nPermanentConnected : 1;
};

}


#endif  // #ifndef __util_ftsclient_hpp__


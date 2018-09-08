
// main_client_test.cpp
// 2017 Aug 02

#include <stdio.h>
#include "util_ftsclient.hpp"

int main(int argc, char* argv[])
{
	const char* serverName = "";
	int nRet(0);

	if(argc<2){
		fprintf(stderr,"Provide server name!\n");
		return 1;
	}

	serverName = argv[1];

	common::socketN::Initialize();

	printf("echo(%s)=%s\n",serverName,util::FtsClient::Echo(serverName)?"true":"false");

	goto returnPoint;
returnPoint:
	common::socketN::Cleanup();

	return nRet;
}

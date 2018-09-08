#ifndef __createnewprocess1_h__
#define __createnewprocess1_h__



#ifdef _WIN64
	#ifndef WIN32
		#define WIN32
	#endif
#endif



#ifdef _MSC_VER
#if(_MSC_VER >= 1400)
//#define		_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif
#endif


///// Includes
#include <stddef.h>
#include <fcntl.h>

#ifdef WIN32
	#include <io.h>
	#ifndef snprintf
#if _MSC_VER<1900
		#define snprintf _snprintf
#endif
	#endif /*#ifndef snprintf*/
#else
	#include <unistd.h>
	#include <errno.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void* GetResourceForCreateProcess(void);
void FreeCreateProcessResource(void* a_pResource);
void InitCreateProcessResourse(void* resourse); // not necessary to call

void KillProcess2(int signal, void* processHandle);

void* CreateNewProcess1(char* a_argv[], int a_nFD_Inp, int a_nFD_Out, int a_nFD_Err,
					   char* a_pcErrBuf, int a_nErrBufLen, void* a_pReserved );

void* CreateNewProcess3(const char* a_cpcExecute, int a_nFD_Inp, int a_nFD_Out, int a_nFD_Err,
                       char* a_pcErrBuf, int a_nErrBufLen, void* resourse );

int WaitChildProcess1( void* a_Process, void* a_pTime );

void CloseHandles1( void* a_lnProc, void* a_lnThread );


#ifdef __cplusplus
}
#endif



#endif /*#ifndef __createnewprocess1_h__*/

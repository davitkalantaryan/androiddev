/*****************************************************************************
 * File		  alog.h
 * created on 2012-06-26
 *****************************************************************************
 * Author:	D.Kalantaryan, Tel:033762/77552 kalantar
 * Email:	davit.kalantaryan@desy.de
 * Mail:	DESY, Platanenallee 6, 15738 Zeuthen
 *****************************************************************************
 * Description
 *   ...
 ****************************************************************************/
#ifndef __alog_h__
#define __alog_h__



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


#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

#ifdef WIN32

#include <windows.h>
#include <ERRNO.H>

typedef HANDLE	TYPEMUTEX;
#define	COLORTP				WORD

const	COLORTP	DEFAULT		= 0;
const	COLORTP	BLACK		= 0;
const	COLORTP	BLUE		= 1;
const	COLORTP	GUYN2		= 2;
const	COLORTP	GUYN3		= 3;
const	COLORTP RED			= 4;
const	COLORTP MAGENTA		= 5;
const	COLORTP GUYN6		= 6;
const	COLORTP LightGray	= 7;
const	COLORTP GUYN8		= 8;
const	COLORTP GUYN9		= 9;
const	COLORTP LightGreen	= 10;
const	COLORTP LightCyan	= 11;
const	COLORTP GYAN	= 11;
const	COLORTP LightRed	= 12;
const	COLORTP GREEN		= 13;
const	COLORTP YELLOW		= 14;
const	COLORTP WHITE		= 15;

#else /*#ifdef WIN32*/
#include <pthread.h>

typedef pthread_mutex_t	TYPEMUTEX;
#define COLORTP				int

const	COLORTP	DEFAULT		= 9;
const	COLORTP	BLACK		= 0;
const	COLORTP RED			= 1;
const	COLORTP GREEN		= 2;
const	COLORTP YELLOW		= 3;
const	COLORTP BLUE		= 4;
const	COLORTP MAGENTA		= 5;
const	COLORTP CYAN		= 6;
const	COLORTP WHITE		= 7;

#define	RESET				0
#define	BRIGHT				1
#define	DIM					2
#define	UNDERLINE			3
#define	BLINK				4
#define	REVERSE				7
#define	HIDDEN				8


#endif /*#ifdef WIN32*/

#ifdef __cplusplus
extern "C"
{
#endif

extern int ConsTextColor1( FILE* a_fpFile, const COLORTP* a_pcColor, COLORTP* a_pcOldColor );
extern int ConsBgColor1( FILE* a_fpFile, COLORTP a_cColor, COLORTP* a_pcOldColor );

#ifdef __cplusplus
}
#endif


#define		DEFAULT_LOG_MAX_SIZE	16384



class ALog
{
public:
	ALog(size_t a_unMaxSize = DEFAULT_LOG_MAX_SIZE);

        ALog(const char *a_szFileName);

//	virtual ~ALog();
	void	SetOutputPtr( FILE* a_pFile );

	bool	Open( const char *a_szFileName );

	bool	Open( const char *a_szFileName, const char* a_pcFormat );

	void	Close();
	
	int		Write1( const char *a_szFmt, ... );

	/*
	 * Writing log without any time stamp
	 *
	 */
	int		WriteWTS1( const char *a_szFmt, ... );

	int		WriteOnlyTS1();

	int		Write2(	const COLORTP* a_pBgColor, const COLORTP* a_TxtColor, 
					char* a_pTempBuffer, size_t a_unBufSize,
					const char *a_szFmt, ... );

	/*
	 * Writing log without any time stamp
	 *
	 */
	int		WriteWTS2(	const COLORTP* a_pBgColor, const COLORTP* a_TxtColor, 
						char* a_pTempBuffer, size_t a_unBufSize,
						const char *a_szFmt, ... );

	int		WriteOnlyTS2(	const COLORTP* a_pBgColor, const COLORTP* a_TxtColor,
							char* a_pTempBuffer,size_t a_unBufSize);

	int		ClearLog();

	bool	LockLog();

	void	UnLockLog();

	size_t	GetLogSize()const;

	size_t	GetLogBuffer( char* Buffer, const size_t& BufferSize )const;

	FILE*	GetPtrToFile()const{ return m_pFile; }

	int		SetTextColor1( const COLORTP& a_cColor, COLORTP* a_pcOldColor = NULL );

	int		SetBgColor1( const COLORTP& a_cColor, COLORTP* a_pcOldColor = NULL );

	void	SetBold();

	void	UnBold();

	void	SetMaxLogSize(const size_t& a_unMaxLogSize);

protected:
	int		WritePrvt( const char *a_szFmt, va_list& a_args );

	int		WriteWTSPrvt( const char *a_szFmt, va_list& a_args );

	int		WriteOnlyTSPrvt();

	int		ClearLogPrvt();

	void	CopyBufIntoFilePrvt(const char* a_cpcFileName,char* a_pcBuffer,size_t a_unBufSize);


protected:
	FILE*			m_pFile;
	size_t			m_unMaxSize;
	int				m_cBgColor;
	int				m_cTxColor;


	class mut
	{
	public:
		mut();
		~mut();
		int	Lock();
		int	UnLock();
	private:
		TYPEMUTEX	m_MutexLock;
	}m_lock;


};




#endif

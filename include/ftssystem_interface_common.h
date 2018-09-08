
// ftssystem_interface_common.h
// 2017 Aug 03

#ifndef __ftssystem_interface_common_h__
#define __ftssystem_interface_common_h__

#include <stddef.h>
#include <string.h>

#define FTS_SINGLE_SHOT_PORT                    9050
#define FTS_PERMANENT_CONNECT_PORT              9051

#define FTS_HEADER_SIZE                         64
#define FTS_INCOMMING_BUF_LEN(_buff_)           (*((int*)(_buff_)))
#define FTS_CMD_OPTION(_buff_)                  (*(((int*)(_buff_))+1))
#define FTS_CMD_REPEAT_COUNT(_buff_)            (*(((int*)(_buff_))+2))
#define FTS_COMMAND_LEN_PLUS1(_buff_)           (*(((int*)(_buff_))+3))
#define FTS_COMMANDM(_buff_)                    (((char*)(_buff_))+FTS_HEADER_SIZE)
#define OFFSET_TO_BUF(_buff_,_index_)           *(((char*)(_buff_))+(_index_))

#define PREPARE_BUFFER1(_cmdLenPlus1Ptr_,_buffer_for_header_,_command_, _count_)  do{ \
    int _nTotalLen_;\
    \
    (*(_cmdLenPlus1Ptr_)) =(int)strlen((_command_))+1,_nTotalLen_=(*(_cmdLenPlus1Ptr_))+FTS_HEADER_SIZE; \
    FTS_INCOMMING_BUF_LEN((_buffer_for_header_))=_nTotalLen_;\
    FTS_CMD_OPTION((_buffer_for_header_))=FTS_COMMAND::CALL_REPEAT;\
    FTS_CMD_REPEAT_COUNT((_buffer_for_header_))=((int)(_count_));\
    FTS_COMMAND_LEN_PLUS1((_buffer_for_header_))=(*(_cmdLenPlus1Ptr_)); \
    }while(0)

#ifdef __cplusplus
namespace FTS_COMMAND {
	enum {
#else
enum FTS_COMMAND {
#endif

	CALL_REPEAT = 1
	};

#ifdef __cplusplus
} // namespace FTS_COMMAND{
#endif

#ifdef __cplusplus
namespace util{
#endif


struct SMem {
	void* buff; size_t size;
#ifdef __cplusplus
	SMem(); ~SMem(); int resize(size_t newSize);
private:
    SMem(const SMem& ){}
#endif
};

#ifdef __cplusplus
}//namespace util {
#endif


#endif  // #ifndef __ftssystem_interface_common_h__

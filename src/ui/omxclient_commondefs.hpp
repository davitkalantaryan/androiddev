
// omxclient_commondefs.hpp
// 2017.08.31

#ifndef OMXCLIENT_COMMONDEFS_HPP
#define OMXCLIENT_COMMONDEFS_HPP

#include <stdlib.h>
#include <stddef.h>
#include <memory.h>

namespace omxclient{

namespace DEVTASK{enum TYPE{NO_TASK,CONNECT,DISCONNECT=CONNECT+10};}
struct SDevTaskStruct{int type;void* arg1;
                      SDevTaskStruct(int a_type=DEVTASK::NO_TASK):type(a_type),arg1(NULL){}};
struct SDevReturn{SDevTaskStruct inp;int ret;};

}

#endif // OMXCLIENT_COMMONDEFS_HPP

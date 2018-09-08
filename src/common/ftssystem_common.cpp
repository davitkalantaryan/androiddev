
// ftssystem_common.cpp
// 2017 Aug 03

#include "ftssystem_interface_common.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus

/*///////////////////////////////////////////*/
util::SMem::SMem()
{
	buff = NULL; size = 0;
}


util::SMem::~SMem()
{
    //printf("util::SMem::~SMem()\n");
	free(buff);
}


int util::SMem::resize(size_t a_newSize)
{
	if (a_newSize>size) {
        buff = realloc(buff, a_newSize);
		if (!buff) { size = 0; return -1; }
		size = a_newSize;
	}
	return 0;
}

#endif  // #ifdef __cplusplus

#include "operator_new.h"
#include "malloc.h"
#include <new>

#ifdef __cplusplus

void* operator new (size_t size) throw(std::bad_alloc){
	return mymalloc(SRAMIN,(u32)size);
}

void operator delete(void *pointer) throw(){
    myfree(SRAMIN,pointer);
}

#endif // __cplusplusDEBUG





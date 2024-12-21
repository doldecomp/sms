#ifndef JKRHEAP_HPP
#define JKRHEAP_HPP

#include "dolphin.h"

class JKRHeap;

void* operator new(size_t);
void* operator new(size_t, int);
void* operator new(size_t, JKRHeap*, int);
void* operator new[](size_t);
void* operator new[](size_t, int);
void* operator new[](size_t, JKRHeap*, int);

void operator delete(void*);
void operator delete[](void*);

#endif

#include "pch.h"
#include "Allocator.h"
#include "MemoryMgr.h"

/*-------------------
	PoolAllocator
-------------------*/

void* PoolAllocator::Alloc(int size)
{
	return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	return GMemory->Release(ptr);
}
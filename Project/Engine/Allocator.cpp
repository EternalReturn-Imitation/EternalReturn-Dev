#include "pch.h"
#include "Allocator.h"
#include "MemoryMgr.h"

/*-------------------
	PoolAllocator
-------------------*/

void* PoolAllocator::Alloc(int size)
{
	return MemoryMgr::GetInst()->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	MemoryMgr::GetInst()->Release(ptr);
}
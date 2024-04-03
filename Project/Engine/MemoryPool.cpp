#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int allocSize) : _allocSize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (_queue.empty() == false)
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	// Pool에 메모리 반납
	_queue.push(ptr);

	--_allocCount;
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;

	// Pool에 여분이 있는지?
	if (_queue.empty() == false)
	{
		// 있으면 하나 꺼내온다
		header = _queue.front();
		_queue.pop();
	}

	// 없으면 새로 만들다
	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}	

	++_allocCount;

	return header;
}

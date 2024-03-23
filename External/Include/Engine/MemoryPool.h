#pragma once

/*-----------------
	MemoryHeader
------------------*/

struct MemoryHeader
{
	// [MemoryHeader][Data]
	MemoryHeader(int size) : allocSize(size) { }

	static void* AttachHeader(MemoryHeader* header, int size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int allocSize;
	// TODO : 필요한 추가 정보
};

/*-----------------
	MemoryPool
------------------*/

class MemoryPool
{
public:
	MemoryPool(int allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();

private:
	int _allocSize = 0;
	int _allocCount = 0;

	queue<MemoryHeader*> _queue;
};


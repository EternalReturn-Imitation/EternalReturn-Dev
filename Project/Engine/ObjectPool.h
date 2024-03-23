#pragma once
#include "MemoryPool.h"
#include "CEntity.h"

using namespace std;

template<typename Type>
class ObjectPool
{
private:
	static int				s_allocSize;
	static MemoryPool		s_pool;

public:
	template<typename... Args>
	static Type* Pop(Args&&...args)
	{
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
		new(memory)Type(forward<Args>(args)...);
		((CEntity*)memory)->SetManagedByMemory(true);
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->~Type();
		s_pool.Push(MemoryHeader::DetachHeader(obj));
		obj = nullptr;
	}

	static shared_ptr<Type> MakeShared()
	{
		shared_ptr<Type> ptr = { Pop(), Push };
		return ptr;
	}
};

template<typename Type>
int ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };


//사용법
// CGameObject* ex = ObjectPool<CGameObject>::Pop(); //오브젝트 풀에서 가져옴
// ObjectPool<CGameObject>::Push(); //오브젝트 풀에 반납.
// 스마트 포인터로 사용시
// shared_ptr<CGameObject> sptr = ObjectPool<CGameObject>::MakeShared();
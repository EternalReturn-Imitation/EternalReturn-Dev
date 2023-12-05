#include "pch.h"
#include "CRes.h"

CRes::CRes(RES_TYPE _type, bool _bEngine)
	: m_Type(_type)
	, m_iRefCount(0)
	, m_bEngine(_bEngine)
{
}

CRes::CRes(const CRes& _Other)
	: CEntity(_Other)
	, m_Type(_Other.m_Type)
	, m_iRefCount(0)
	, m_strKey(_Other.m_strKey)
	, m_strRelativePath(_Other.m_strRelativePath)
{
}

CRes::~CRes()
{
}

void CRes::Release()
{
	--m_iRefCount;

	if (0 == m_iRefCount)
	{
		delete this;
	}
}

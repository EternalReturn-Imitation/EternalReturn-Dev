#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
{
}

CPathMgr::~CPathMgr()
{
}


void CPathMgr::init()
{
	GetCurrentDirectory(256, m_szContentPath);

	// 상위폴더로 감
	int iLen = (int)wcslen(m_szContentPath);

	int count = 0;
	for (int i = iLen - 1; i >= 0; --i)
	{
		if (L'\\' == m_szContentPath[i])
		{
			++count;
		}
		if (count == 2) {
			m_szContentPath[i] = 0;
			break;
		}
	}

	// + bin\\content
	wcscat_s(m_szContentPath, L"\\OutputFile\\bin\\content\\");
}

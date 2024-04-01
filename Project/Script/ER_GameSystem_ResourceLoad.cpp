#include "pch.h"
#include "ER_GameSystem.h"

#include <Engine\CResMgr.h>
#include <Engine\CPathMgr.h>
#include <Engine\CEventMgr.h>

bool ER_GameSystem::AllResourceLoad()
{
	// Content 폴더에 있는 파일 이름들을 확인
	m_vecResPath.clear();
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	FindFileName(strContentPath);

	wstring strFileKey;
	wstring stdDirPath;
	wstring strExt;
	// 파일명만 분리해서 key 값으로 사용하기.

	// 가능하면 같은 폴더 확인

	// 파일명으로 리소스 로딩
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		RES_TYPE type = GetResTypeByExt(m_vecResPath[i], strFileKey, stdDirPath);

		if (type == RES_TYPE::END)
			continue;

		switch (type)
		{
		case RES_TYPE::MESHDATA:
			CResMgr::GetInst()->Load<CMeshData>(strFileKey, m_vecResPath[i], stdDirPath);
			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(strFileKey, m_vecResPath[i], stdDirPath);
			break;
		case RES_TYPE::PREFAB:

			break;
		case RES_TYPE::MESH:
			CResMgr::GetInst()->Load<CMesh>(strFileKey, m_vecResPath[i], stdDirPath);
			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(strFileKey, m_vecResPath[i], stdDirPath);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(strFileKey, m_vecResPath[i], stdDirPath);
			break;
		case RES_TYPE::BONE:
			CResMgr::GetInst()->Load<CBone>(strFileKey, m_vecResPath[i], stdDirPath);
		}
	}


	// 리소스의 원본파일 체크
	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			wstring strFilePath = strContentPath + pair.second->GetDirectoryPath() + pair.first;
			if (!filesystem::exists(strFilePath))
			{
				tEvent evn = {};
				evn.Type = EVENT_TYPE::DELETE_RESOURCE;
				evn.wParam = (DWORD_PTR)i;
				evn.lParam = (DWORD_PTR)pair.second.Get();
				CEventMgr::GetInst()->AddEvent(evn);
			}
		}
	}

	return true;
}

void ER_GameSystem::FindFileName(const wstring& _FolderPath)
{
	WIN32_FIND_DATA FindData = {};

	wstring FolderPath = _FolderPath + L"*.*";

	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

	while (FindNextFile(hFindHandle, &FindData))
	{
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!wcscmp(FindData.cFileName, L".."))
			{
				continue;
			}

			FindFileName(_FolderPath + FindData.cFileName + L"\\");
			continue;
		}

		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		wstring RelativePath = _FolderPath + FindData.cFileName;
		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

		m_vecResPath.push_back(RelativePath);
	}

	FindClose(hFindHandle);
}

RES_TYPE ER_GameSystem::GetResTypeByExt(const wstring& _relativepath, wstring& _FileKey, wstring& _Dir)
{
	wchar_t szExt[50] = {};
	wchar_t szFileName[50] = {};
	wchar_t szdrive[50] = {};
	wchar_t szdir[50] = {};
	_wsplitpath_s(_relativepath.c_str(), szdrive, 50, szdir, 50, szFileName, 50, szExt, 50);
	wstring strExt = szExt;

	_FileKey = szFileName + strExt;
	_Dir = szdir;

	if (L".mdat" == strExt)
		return RES_TYPE::MESHDATA;
	else if (L".pref" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpg" == strExt
		|| L".jpeg" == strExt || L".bmp" == strExt
		|| L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
		return RES_TYPE::SOUND;
	else if (L".bone" == strExt)
		return RES_TYPE::BONE;
	else
		return RES_TYPE::END;
}
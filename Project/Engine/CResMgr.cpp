#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

CResMgr::CResMgr()
	: m_Changed(false)
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();
}



void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}


void CResMgr::tick()
{
	m_Changed = false;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CMeshData> CResMgr::LoadFBX(const wstring& _strPath, int singleMeshData)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	wstring strKey = strFileName + L".mdat";
	strName += strKey;

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strKey);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath, singleMeshData);
	pMeshData->SetKey(strKey);
	pMeshData->SetRelativePath(strName);

	m_arrRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strKey, pMeshData.Get()));

	// meshdata 를 실제파일로 저장
	pMeshData->Save(strKey);

	m_Changed = true;

	return pMeshData;
}

Ptr<CBone> CResMgr::LoadFBXBone(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"Bone\\";
	wstring strKey = strFileName + L".bone";
	strName += strKey;

	Ptr<CBone> pBone = FindRes<CBone>(strKey);

	if (nullptr != pBone)
		return pBone;

	pBone = CBone::LoadFbx(_strPath);

	// ResMgr 에 Bone 등록
	if (nullptr != pBone)
	{
		pBone->SetKey(strKey);
		pBone->SetRelativePath(strName);
		
		m_arrRes[(UINT)RES_TYPE::BONE].insert(make_pair(strKey, pBone.Get()));

		// meshdata 를 실제파일로 저장
		pBone->Save(strKey);
	}

	m_Changed = true;

	return pBone;
}


void CResMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	assert(!(iter == m_arrRes[(UINT)_type].end()));

	m_arrRes[(UINT)_type].erase(iter);

	m_Changed = true;
}
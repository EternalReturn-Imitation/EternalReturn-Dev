#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CResMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

#include "CFBXLoader.h"


CMeshData::CMeshData(bool _bEngine)
	: CRes(RES_TYPE::MESHDATA, _bEngine)
{
}

CMeshData::~CMeshData()
{
	Safe_Del_Vec(m_vecMeshData);
}


CGameObject* CMeshData::Instantiate()
{
	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);

	int iMeshDataCnt = (int)m_vecMeshData.size();
	
	// 단일메시
	if (1 == iMeshDataCnt)
	{
		pNewObj->AddComponent(new CMeshRender);
		pNewObj->MeshRender()->SetMesh(m_vecMeshData[0]->m_pMesh);

		for (UINT i = 0; i < m_vecMeshData[0]->m_vecMtrl.size(); ++i)
		{
			pNewObj->MeshRender()->SetMaterial(m_vecMeshData[0]->m_vecMtrl[i], i);
		}
	}

	// 복수메시
	else if (1 < iMeshDataCnt)
	{
		for (size_t i = 0; i < iMeshDataCnt; ++i)
		{
			CGameObject* pNewChildObj = new CGameObject;

			pNewChildObj->AddComponent(new CTransform);
			pNewChildObj->AddComponent(new CMeshRender);

			pNewChildObj->MeshRender()->SetMesh(m_vecMeshData[i]->m_pMesh);

			for (UINT j = 0; j < m_vecMeshData[i]->m_vecMtrl.size(); ++j)
			{
				pNewChildObj->MeshRender()->SetMaterial(m_vecMeshData[i]->m_vecMtrl[j], j);
			}

			pNewObj->AddChild(pNewChildObj);
		}
	}

	// Animation 파트 추가
	// if (false == m_pMesh->IsAnimMesh())
	// 	return pNewObj;

	// CAnimator3D* pAnimator = new CAnimator3D;
	// pNewObj->AddComponent(pAnimator);
	// 
	// pAnimator->SetBones(m_pMesh->GetBones());
	// pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	return pNewObj;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath, int singleMeshData)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	int iContainerCnt = loader.GetContainerCount();	// 컨테이너 갯수 확인

	CMeshData* pMeshData = new CMeshData(true);

	if (singleMeshData)
		iContainerCnt = 1;

	for (int i = 0; i < iContainerCnt; ++i)
	{
		// 메쉬 가져오기
		CMesh* pMesh = nullptr;
		pMesh = CMesh::CreateFromContainer(loader, i);

		// ResMgr 에 메쉬 등록
		if (nullptr != pMesh)
		{
			wstring strMeshKey;
			// strMeshKey += path(strFullPath).stem();
			strMeshKey = pMesh->GetName();
			strMeshKey += L".mesh";
			CResMgr::GetInst()->AddRes<CMesh>(strMeshKey, pMesh);

			// 메시를 실제 파일로 저장
			pMesh->Save(strMeshKey);
		}

		// 컨테이너 받아오기
		const tContainer* container = &loader.GetContainer(i);

		vector<Ptr<CMaterial>> vecMtrl;

		// 메테리얼 가져오기
		for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
		{
			// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(i).vecMtrl[j].strMtrlName);
			assert(pMtrl.Get());

			vecMtrl.push_back(pMtrl);
		}

		tMeshData* MeshData = new tMeshData;
		MeshData->m_pMesh = pMesh;
		MeshData->m_vecMtrl = vecMtrl;
		MeshData->m_ParentIdx = container->iParentIdx;
		pMeshData->m_vecMeshData.emplace_back(MeshData);
	}

	return pMeshData;
}

int CMeshData::Save(const wstring& _strRelativePath)
{
	// 상대경로 저장
	wstring RelativePath = L"meshdata\\";
	RelativePath += _strRelativePath;
	SetRelativePath(RelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_meshdata = path_content.wstring() + L"meshdata\\";

	if (false == exists(path_meshdata))
	{
		create_directory(path_meshdata);
	}

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 메시데이터 갯수 저장
	UINT iMeshDataCnt = (UINT)m_vecMeshData.size();
	fwrite(&iMeshDataCnt, sizeof(UINT), 1, pFile);

	// Mesh Key 저장	
	// Mesh Data 저장
	for (UINT i = 0; i < iMeshDataCnt; ++i)
	{
		SaveResRef(m_vecMeshData[i]->m_pMesh.Get(), pFile);

		// material 정보 저장
		UINT iMtrlCount = (UINT)m_vecMeshData[i]->m_vecMtrl.size();
		fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

		UINT iIdx = 0;
		wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();
		strMtrlPath += L"material\\";

		for (; iIdx < iMtrlCount; ++iIdx)
		{
			if (nullptr == m_vecMeshData[i]->m_vecMtrl[iIdx])
				continue;

			// Material 인덱스, Key, Path 저장
			fwrite(&iIdx, sizeof(UINT), 1, pFile);
			SaveResRef(m_vecMeshData[i]->m_vecMtrl[iIdx].Get(), pFile);
		}

		// iIdx = -1; // 마감 값
		// fwrite(&iIdx, sizeof(UINT), 1, pFile);
	}

	fclose(pFile);

	return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);

	// MeshData 갯수 읽기
	UINT iMeshDataCnt = 0;
	fread(&iMeshDataCnt, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < iMeshDataCnt; ++i)
	{
		tMeshData* MeshData = new tMeshData;

		// Mesh Load
		Ptr<CMesh> pMesh = nullptr;
		LoadResRef<CMesh>(pMesh, pFile);
		assert(pMesh.Get());
		MeshData->m_pMesh = pMesh;


		// material 정보 읽기
		vector<Ptr<CMaterial>> vecMtrl;

		// 재질 갯수
		UINT iMtrlCount = 0;
		fread(&iMtrlCount, sizeof(UINT), 1, pFile);


		vecMtrl.resize(iMtrlCount);

		for (UINT i = 0; i < iMtrlCount; ++i)
		{
			UINT idx = -1;
			fread(&idx, 4, 1, pFile);
			if (idx == -1)
				break;

			wstring strKey, strPath;

			Ptr<CMaterial> pMtrl;
			LoadResRef<CMaterial>(pMtrl, pFile);

			vecMtrl[i] = pMtrl;
		}
		
		MeshData->m_vecMtrl = vecMtrl;

		m_vecMeshData.emplace_back(MeshData);
	}

	fclose(pFile);

	return S_OK;
}
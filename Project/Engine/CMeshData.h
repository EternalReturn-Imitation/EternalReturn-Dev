#pragma once
#include "CRes.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

struct tMeshData
{
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;
	int						m_ParentIdx = -1;
};

class CMeshData :
	public CRes
{
	// single Mesh
	vector<tMeshData*>		m_vecMeshData;

public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath, int singleMeshData = 0);
	
	virtual int Save(const wstring& _strFilePath) override;
	virtual int Load(const wstring& _strFilePath) override;

	CGameObject* Instantiate();

	const Ptr<CMesh>& GetMesh(int i = 0) { return m_vecMeshData[i]->m_pMesh; }

	CLONE_DISABLE(CMeshData)
public:
	CMeshData(bool _bEngine = false);
	virtual ~CMeshData();
};


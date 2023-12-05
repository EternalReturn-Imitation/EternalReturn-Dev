#pragma once
#include "CRes.h"

class CMesh
	: public CRes
{
private:
	ComPtr<ID3D11Buffer>	m_VB;
	D3D11_BUFFER_DESC		m_tVBDesc;
	UINT					m_VtxCount;

	ComPtr<ID3D11Buffer>	m_IB;
	D3D11_BUFFER_DESC		m_tIBDesc;
	UINT					m_IdxCount;

	void*					m_pVtxSys;
	void*					m_pIdxSys;

public:
	void Create(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount);

private:
	virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
	virtual int Save(const wstring& _strRelativePath) { return S_OK; }


	void render();
	void render_particle(UINT _iParticleCount);



private:
	virtual void UpdateData() override;
	

public:
	CMesh(bool _bEngine = false);
	~CMesh();
};
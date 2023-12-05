#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
	CGameObject*	m_ProtoObj;

public:
	CGameObject* Instantiate();

private:
	virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
	virtual int Save(const wstring& _strRelativePath) { return S_OK; }

private:
	virtual void UpdateData() override {}

public:
	void RegisterProtoObject(CGameObject* _Proto);

public:
	CPrefab();
	~CPrefab();
};


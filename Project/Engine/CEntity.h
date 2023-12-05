#pragma once

class CEntity
{
private:
	static UINT g_iNextID;

private:
	wstring		m_strName;
	const UINT  m_iID;

public:
	void SetName(const wstring& _strName) {  m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();
};


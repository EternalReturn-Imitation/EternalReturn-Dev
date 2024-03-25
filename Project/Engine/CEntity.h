#pragma once

class CEntity
{
private:
	static UINT g_iNextID;

private:
	wstring		m_strName;
	const UINT  m_iID;

	//메모리풀 관련
private:
	bool	m_bManagedByMemory;

public:
	void SetName(const wstring& _strName) {  m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

	void SetManagedByMemory(bool _b) { m_bManagedByMemory = _b; }
	bool GetManagedByMemory() { return m_bManagedByMemory; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();
};


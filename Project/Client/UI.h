#pragma once


#include "ImGuiMgr.h"

class UI
{
private:
	string			m_strName;		// 타이틀 이름, 
	string			m_strID;		// ID 이름

	ImVec2			m_vSize;		// UI 크기
	ImVec2			m_vPopupPos;	// UI 위치

	UI*				m_ParentUI;		// 부모 UI
	vector<UI*>		m_vecChildUI;	// 자식 UI 목록

	bool			m_Modal;		// 모달 or 모달리스
	bool			m_Active;		// UI 활성화 체크

public:
	virtual void init() {}
	virtual void tick() {}
	virtual void finaltick();
	virtual int render_update() = 0;


public:	
	void SetActive(bool _Active) { m_Active = _Active; }
	bool IsActive() { return m_Active; }

	void SetModal(bool _Modal) { m_Modal = _Modal; }
	bool IsModal() { return m_Modal; }

	ImVec2 GetPopupPos() { return m_vPopupPos; }
	void SetPopupPos(ImVec2 _vPos) { m_vPopupPos = _vPos; }

	const string& GetName() { return m_strName; }
	void SetName(const string& _Name) { m_strName = _Name; }

	const string& GetID() { return m_strID; }

	void SetSize(float _width, float _height) { m_vSize = ImVec2(_width, _height); }

	void AddChildUI(UI* _UI)
	{
		_UI->m_ParentUI = this;
		m_vecChildUI.push_back(_UI);
	}


public:
	UI(const string& _Name);
	virtual ~UI();
};


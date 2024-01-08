#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"
#include "CMeshRender.h"

#include "ptr.h"
#include "CMaterial.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
		}

		m_pCurAnim->finaltick();
	}	
}

void CAnimator2D::UpdateData()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();
	Vec2 vBackSize = m_pCurAnim->GetBackSize();

	int iAnimUse = 1;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);
	pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
	pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
	pMtrl->SetScalarParam(VEC2_2, &frm.Offset);
	pMtrl->SetScalarParam(VEC2_3, &vBackSize);

	pMtrl->SetTexParam(TEX_0, m_pCurAnim->GetAtlasTex());
}

void CAnimator2D::Clear()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	int iAnimUse = 0;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);

	Ptr<CTexture> pTex = nullptr;
	pMtrl->SetTexParam(TEX_0, pTex);
}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnim2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::CreateAnimation(const wstring& _strAnimName
	, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize
	, int _FrameCount, int _FPS)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
	
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{		
		pair.second->SaveToLevelFile(_File);
	}

	wstring strCurAnimName;	
	if(nullptr != m_pCurAnim)	
	{
		strCurAnimName = m_pCurAnim->GetName();
	}
	SaveWString(strCurAnimName, _File);
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_bRepeat, sizeof(bool), 1, _File);

	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim2D* pNewAnim = new CAnim2D;
		pNewAnim->LoadFromLevelFile(_File);
		
		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
		pNewAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	LoadWString(strCurAnimName, _File);

	m_pCurAnim = FindAnim(strCurAnimName);		
}

void CAnimator2D::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	size_t AnimCount = m_mapAnim.size();
	wstring Animation2D;

	for (size_t i = 0; i < AnimCount; ++i) {
		CAnim2D* pNewAnim = new CAnim2D;
		Animation2D += pNewAnim->SaveToDB();

		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
		pNewAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
	{
		strCurAnimName = m_pCurAnim->GetName();
	}

	wstring query = L"INSERT INTO ANIMATOR2D(GameObject_ID, Repeat, AnimCount, Animation2D, CurAnimName) VALUES("
		+ std::to_wstring(_gameObjectID) + L","
		+ std::to_wstring(m_bRepeat) + L","
		+ std::to_wstring(AnimCount) + L","
		+ L"'" + Animation2D + L"',"
		+ L"'" + strCurAnimName + L"');";

	CONVERTQUERY(query, Query);

	char* errMsg;
	EXECQUERY(Query, errMsg);
}

void CAnimator2D::LoadFromDB(int _gameObjectID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	sqlite3_stmt* stmt;
	const char* sql = "SELECT Repeat, AnimCount, Animation2D, CurAnimName FROM ANIMATOR2D WHERE GameObject_ID = ?";

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
		// 오류 처리
		return;
	}

	sqlite3_bind_int(stmt, 1, _gameObjectID);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		m_bRepeat = sqlite3_column_int(stmt, 0) != 0;
		size_t AnimCount = sqlite3_column_int(stmt, 1);
		std::string sAnimation2D = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		std::string sStrCurAnimName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

		std::wstring Animation2D;
		Animation2D.assign(sAnimation2D.begin(), sAnimation2D.end());

		std::wstring strCurAnimName;
		strCurAnimName.assign(sStrCurAnimName.begin(), sStrCurAnimName.end());

		// Animation2D 데이터를 파싱하여 각 애니메이션 로드
		std::wstringstream wss(Animation2D);
		std::wstring animData;
		for (size_t i = 0; i < AnimCount; ++i) {
			std::getline(wss, animData, L'\0'); // 각 애니메이션 데이터 분리
			CAnim2D* pNewAnim = new CAnim2D;
			pNewAnim->LoadFromDB(animData);

			m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
			pNewAnim->m_pOwner = this;
		}

		m_pCurAnim = FindAnim(strCurAnimName);
	}

	sqlite3_finalize(stmt);
}

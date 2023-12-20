#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"


CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_fTime(0.f)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{	
	if (m_bFinish)
		return;

	m_fTime += DT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

	m_AtlasTex = _AtlasTex;

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_vBackSize = _vBackSize / vResolution;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution;
		frm.SliceUV = _vSlice / vResolution;

		m_vecFrm.push_back(frm);
	}
}

void CAnim2D::SaveToLevelFile(FILE* _File)
{
	SaveWString(GetName(), _File);
	
	size_t FrameCount = m_vecFrm.size();
	fwrite(&FrameCount, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, _File);
	fwrite(&m_vBackSize, sizeof(Vec2), 1, _File);
	
	SaveResRef(m_AtlasTex.Get(), _File);
}

void CAnim2D::LoadFromLevelFile(FILE* _File)
{
	wstring name;
	LoadWString(name, _File);
	SetName(name);

	size_t FrameCount = 0;
	fread(&FrameCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FrameCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, _File);
		m_vecFrm.push_back(frm);
	}
	
	fread(&m_vBackSize, sizeof(Vec2), 1, _File);
		
	LoadResRef(m_AtlasTex, _File);
}

wstring CAnim2D::SaveToDB()
{
	std::wstringstream oss;

	oss << GetName() << L"\n";

	// 프레임 카운트와 프레임 데이터 저장
	size_t FrameCount = m_vecFrm.size();
	oss << FrameCount << L"\n";
	for (const auto& frame : m_vecFrm) {
		oss.write(reinterpret_cast<const wchar_t*>(&frame), sizeof(tAnim2DFrm));
	}

	// 배경 크기 저장
	oss.write(reinterpret_cast<const wchar_t*>(&m_vBackSize), sizeof(Vec2));

	wstring texRes = SaveResRefToDB(m_AtlasTex.Get());

	oss << texRes;

	return oss.str();
}

void CAnim2D::LoadFromDB(const std::wstring& serializedData) {
	std::wstringstream wss(serializedData);
	std::wstring line;

	// 이름 로드
	std::getline(wss, line);
	SetName(line);

	// 프레임 카운트 로드
	std::getline(wss, line);
	size_t FrameCount = std::stoull(line);
	m_vecFrm.resize(FrameCount);

	for (size_t i = 0; i < FrameCount; ++i) {
		tAnim2DFrm frm;
		wss.read(reinterpret_cast<wchar_t*>(&frm), sizeof(tAnim2DFrm));
		m_vecFrm.push_back(frm);
	}

	// 배경 크기 로드
	Vec2 backSize;
	wss.read(reinterpret_cast<wchar_t*>(&backSize), sizeof(Vec2));
	m_vBackSize = backSize;

	// 리소스 참조 로드
	Ptr<CTexture> res;
	LoadResRefFromDB(res, wss);
	m_AtlasTex = res;
}

#include "pch.h"

#include "CEventMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderMgr.h"
#include "ptr.h"
#include "CResMgr.h"

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = _LayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_LayerName)->GetLayerIndex();

	CEventMgr::GetInst()->AddEvent(evn);
}

void DestroyObject(CGameObject* _DeletObject)
{
	if (_DeletObject->IsDead())
		return;

	tEvent evn = {};

	evn.Type = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)_DeletObject;
	
	CEventMgr::GetInst()->AddEvent(evn);
}



void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;	
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;	
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}


bool IsValidObj(CGameObject*& _Target)
{
	if (nullptr == _Target)
		return false;

	if (_Target->IsDead())
	{
		_Target = nullptr;
		return false;
	}

	return true;
}

const char* ToString(RES_TYPE type)
{
	return RES_TYPE_STR[(UINT)type];
}

const wchar_t* ToWString(RES_TYPE type)
{
	return RES_TYPE_WSTR[(UINT)type];
}


const char* ToString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_STR[(UINT)type];
}

const char* ToString(COLLIDER2D_TYPE type)
{
	return COLLIDER2D_TYPE_STR[(UINT)type];
}

const wchar_t* ToWString(COLLIDER2D_TYPE type)
{
	return COLLIDER2D_TYPE_WSTR[(UINT)type];
}

COLLIDER2D_TYPE ToCollider2DType(wstring _wstring) {
	for (int i = 0; i < (UINT)COLLIDER2D_TYPE::END; i++) {
		if (COLLIDER2D_TYPE_WSTR[i] == _wstring)
			return (COLLIDER2D_TYPE)i;
	}
}

string ToString(wstring _wstring)
{
	string sString;
	sString.assign(_wstring.begin(), _wstring.end());

	return sString;
}

wstring ToWString(string _string)
{
	wstring wString;
	wString.assign(_string.begin(), _string.end());

	return wString;
}

void SaveWString(const wstring& _str, FILE* _File)
{	
	UINT iLen = (UINT)_str.length();
	fwrite(&iLen, sizeof(UINT), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[256] = {};

	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _File);
	fread(szBuffer, sizeof(wchar_t), iLen, _File);	

	_str = szBuffer;
}

void SaveResRef(Ptr<CRes> _Res, FILE* _File)
{
	int i = 0;
	if (nullptr == _Res)
	{
		fwrite(&i, sizeof(i), 1, _File);
	}
	else
	{
		i = 1;
		fwrite(&i, sizeof(i), 1, _File);
		SaveWString(_Res->GetKey(), _File);
		SaveWString(_Res->GetRelativePath(), _File);
	}
}

wstring SaveResRefToDB(Ptr<CRes> _Res)
{
	std::wstringstream wss;

	int i = 0;
	if (nullptr == _Res)
	{
		wss << L"0\n";
	}
	else
	{
		wss << L"1\n";
		wss << _Res->GetKey() << L"\n";
		wss << _Res->GetRelativePath() << L"\n";
	}
	
	return wss.str();
}

template<typename T>
void LoadResRefFromDB(Ptr<T>& _Res, std::wstringstream& wss) {
	int exists;
	wss >> exists;
	std::wstring line;
	std::getline(wss, line); // 숫자 뒤의 개행 문자를 소비

	if (exists) {
		std::wstring strKey, strRelativePath;

		std::getline(wss, strKey);
		std::getline(wss, strRelativePath);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}

const wchar_t* ToWString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_WSTR[(UINT)type];
}

std::wstring Vec3ToWString(const Vec3& vec) {
	wstring result = std::to_wstring(vec.x) + L"," + std::to_wstring(vec.y) + L"," + std::to_wstring(vec.z);
	return result;
}

Vec3 WStringToVec3(const std::wstring& wstr)
{
	Vec3 vec;
	std::wistringstream wss(wstr);
	wchar_t comma; // 쉼표를 저장하기 위한 임시 변수

	wss >> vec.x >> comma >> vec.y >> comma >> vec.z;
	return vec;
}

#pragma once

template <typename T, UINT _Size>
void DeleteArray(T* (&Arr)[_Size])
{
	for (UINT i = 0; i < _Size; ++i)
	{
		if (nullptr != Arr[i])
			delete Arr[i];
	}
}

// ������Ʈ ����
class CGameObject;
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName);

// ������Ʈ ����
void DestroyObject(CGameObject* _DeletObject);

// DrawDebugShape
void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCube(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugFrustumCube(const Matrix& _matWorld, bool DepthTest);


// GameObject ��ȿ�� üũ
bool IsValidObj(CGameObject*& _Target);


const char* ToString(RES_TYPE);
const wchar_t* ToWSTring(RES_TYPE);

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWSTring(COMPONENT_TYPE);

const char* ToString(COLLIDER2D_TYPE);
const wchar_t* ToWString(COLLIDER2D_TYPE);

COLLIDER2D_TYPE ToCollider2DType(wstring _wstring);

string ToString(wstring _wstring);
wstring ToWString(string _string);


// Save / Load
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

class CRes;
template<typename T>
class Ptr;

void SaveResRef(Ptr<CRes> _Res, FILE* _File);

class CResMgr;
template<typename T>
void LoadResRef(Ptr<T>& _Res, FILE* _File)
{
	int i = 0;	
	fread(&i, sizeof(i), 1, _File);
	
	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}


wstring SaveResRefToDB(Ptr<CRes> _Res);
void SaveResRefToDB(Ptr<CRes> _Res, wstring& _Key, wstring& _RelativePath);

void SaveGameObjectPtr(CGameObject* _Obj, FILE* _File);
void LoadGameObjectPtr(wstring& _ObjName, FILE* _File);

template<typename T>
void LoadResRefFromDB(Ptr<T>& _Res, std::wstringstream& wss) {
	int exists;
	wss >> exists;
	std::wstring line;
	std::getline(wss, line); // ���� ���� ���� ���ڸ� �Һ�

	if (exists) {
		std::wstring strKey, strRelativePath;

		std::getline(wss, strKey);
		std::getline(wss, strRelativePath);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}

template<typename T>
void LoadResRefFromDB(Ptr<T>& _Res, const wstring& _Key, const wstring& _RelativePath) {
	if (_Key != L"0") {
		_Res = CResMgr::GetInst()->Load<T>(_Key, _RelativePath);
	}
}

const wchar_t* ToWString(COMPONENT_TYPE type);

std::wstring Vec3ToWString(const Vec3& vec);
Vec3 WStringToVec3(const std::wstring& wstr);

std::wstring IntArrayToWString(const std::vector<int>& intArray);
std::vector<int> WStringToIntArray(const std::wstring& str);





template<typename T, UINT Size>
void Safe_Del_Array(T* (&arr)[Size])
{
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
			delete arr[i];
	}	
}

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
		}
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}


// math
float Rad2Deg(float _radian); 
float Deg2Rad(float _Degree); 
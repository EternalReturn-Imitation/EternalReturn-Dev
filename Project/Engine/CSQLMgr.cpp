#include "pch.h"
#include "CSQLMgr.h"

//매니저 관련
#include "CPathMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "components.h"

CSQLMgr::CSQLMgr()
	:db(nullptr)
{
}

CSQLMgr::~CSQLMgr()
{
	sqlite3_close(db);
}

void CSQLMgr::init()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	wstring upperPath = strPath + L"SQLLight\\EldenRing_DB.db";
	
	string stringResult;
	stringResult.assign(upperPath.begin(), upperPath.end());

	const char* cstr = stringResult.c_str();

	int rc = sqlite3_open(cstr, &db);
	if (rc) {
		assert(false);
	}
}

int CSQLMgr::InsertToLevel(wstring _levelName)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// 쿼리 문자열 준비
	const char* szQuery = "INSERT INTO LEVEL(Level_Name) VALUES (?)";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_text16(stmt, 1, _levelName.c_str(), -1, SQLITE_TRANSIENT);

		// 쿼리 실행
		if (sqlite3_step(stmt) != SQLITE_DONE) {
			// 에러 처리: 쿼리 실행에 실패했을 경우
			assert(false);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}

	long long insertedId = sqlite3_last_insert_rowid(db);

	return (int)insertedId;
}

int CSQLMgr::InsertToLayer(int _levelID, wstring _layerName, int _layerIdx)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// 쿼리 문자열 준비
	const char* szQuery = "INSERT INTO LAYER(Level_ID, Layer_Name, Layer_Idx) VALUES (?, ?, ?)";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _levelID);
		sqlite3_bind_text16(stmt, 2, _layerName.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 3, _layerIdx);

		// 쿼리 실행
		if (sqlite3_step(stmt) != SQLITE_DONE) {
			// 에러 처리: 쿼리 실행에 실패했을 경우
			assert(false);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}

	long long insertedId = sqlite3_last_insert_rowid(db);

	return (int)insertedId;
}

int CSQLMgr::InsertToGameObject(int _layerID, wstring _gameObjectName, int _parentID)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	// 쿼리 문자열 준비
	const char* szQuery = "INSERT INTO GAMEOBJECT(Layer_ID, GameObject_Name, Parent_ID) VALUES (?, ?, ?)";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, _layerID);
		sqlite3_bind_text16(stmt, 2, _gameObjectName.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt, 3, _parentID);

		// 쿼리 실행
		if (sqlite3_step(stmt) != SQLITE_DONE) {
			// 에러 처리: 쿼리 실행에 실패했을 경우
			assert(false);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}

	long long insertedId = sqlite3_last_insert_rowid(db);

	return (int)insertedId;
}

void CSQLMgr::SelectFromLevel(int _levelID, CLevel*& _level)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	long long levelId = _levelID;

	// 쿼리 문자열 준비
	const char* szQuery = "SELECT Level_Name FROM LEVEL WHERE ID = ?";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// ID 바인딩
		sqlite3_bind_int64(stmt, 1, levelId);

		// 쿼리 실행
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			// 결과를 얻습니다.
			const wchar_t* levelName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 0));
			_level->SetName(levelName);
		}
		else {
			assert(false);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}
}

void CSQLMgr::SelectFromLayer(int _levelID, CLevel*& _level)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	int levelID = _levelID;

	// 쿼리 문자열 준비
	const char* szQuery = "SELECT ID, Layer_Name, Layer_Idx FROM Layer WHERE Level_ID = ?";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// Level_ID 바인딩
		sqlite3_bind_int(stmt, 1, levelID);

		// 쿼리 실행 및 결과 처리
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			// 결과를 얻습니다.
			int layer_ID = (sqlite3_column_int(stmt, 0));
			const wchar_t* layerName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			int layer_Idx = (sqlite3_column_int(stmt, 2));

			CLayer* pLayer = _level->GetLayer(layer_Idx);
			pLayer->SetName(layerName);

			//여기서 게임오브젝트를 생성하고 레이어에 넣어줌
			SelectFromGameObject(layer_ID, layer_Idx, _level);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}
}

void CSQLMgr::SelectFromGameObject(int _layerID, int layer_Idx, CLevel*& _level)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	int layerID = _layerID;

	// 쿼리 문자열 준비
	const char* szQuery = "SELECT ID, GameObject_Name, Parent_ID, Component_Type_Array FROM GAMEOBJECT WHERE Layer_ID = ?";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// Level_ID 바인딩
		sqlite3_bind_int(stmt, 1, layerID);

		// 쿼리 실행 및 결과 처리
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			// 결과를 얻습니다.
			int gameObject_ID = (sqlite3_column_int(stmt, 0));
			const wchar_t* gameObjectName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			int parentID = (sqlite3_column_int(stmt, 2));
			const wchar_t* comTypeArr = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 3));

			vector<int> vComTypeArr = WStringToIntArray(comTypeArr);

			CGameObject* pNewObject = CreateGameObject(gameObject_ID, gameObjectName, parentID, vComTypeArr);
			_level->AddGameObject(pNewObject, _layerID, false);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}
}

void CSQLMgr::SelectFromComponent(int _GameObjectID, CGameObject*& _gameObject)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	int gameObjectID = _GameObjectID;
	for (int i = 0; i < (int)COMPONENT_TYPE::END; i++) {
		wstring componentType = ToWSTring((COMPONENT_TYPE)i);
		
		
	}

}

void CSQLMgr::UpdateToGameObject(int _gameObjectID, wstring _comType)
{
	const char* szQuery = "UPDATE GAMEOBJECT SET Component_Type_Array = ? WHERE ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// 새로운 텍스트 바인딩
		sqlite3_bind_text16(stmt, 1, _comType.c_str(), -1, SQLITE_TRANSIENT);

		// GameObject_ID 바인딩
		sqlite3_bind_int(stmt, 2, _gameObjectID);

		// 쿼리 실행
		if (sqlite3_step(stmt) != SQLITE_DONE) {
			// 에러 처리
			assert(false);
		}
		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}
}

void CSQLMgr::DeleteAllRecordToAllTable()
{
	// 테이블 이름을 가져오기 위한 콜백 함수
	auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
		std::vector<std::string>* tables = static_cast<std::vector<std::string>*>(data);
		for (int i = 0; i < argc; i++) {
			tables->push_back(argv[i]);
		}
		return 0;
	};

	char* errMsg;
	std::vector<std::string> tables;
	int rc = sqlite3_exec(db, "SELECT name FROM sqlite_master WHERE type='table'", callback, &tables, &errMsg);

	if (rc != SQLITE_OK) {
		assert(false);
	}

	// 각 테이블에 대해 모든 레코드 삭제
	for (const auto& table : tables) {
		std::string sql = "DELETE FROM " + table;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

		if (rc != SQLITE_OK) {
			assert(false);
		}
	}
}

CGameObject* CSQLMgr::CreateGameObject(int _gameObjectID, const wstring _gameObjectName, int _parentID, vector<int> _comTypeArr)
{
	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(_gameObjectName);

	for(int i=0;i<_comTypeArr.size();i++){

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)_comTypeArr[i])
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			Component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			Component = nullptr;
			break;
		case COMPONENT_TYPE::SKYBOX:
			Component = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			Component = new CDecal;
			break;
		}

		Component->LoadFromDB(_gameObjectID);
		pNewGameObject->AddComponent(Component);
	}

	//스크립트 로드 코드 짜기


	//자식오브젝트
	int parentID = _gameObjectID;

	// 쿼리 문자열 준비
	const char* szQuery = "SELECT ID, GameObject_Name, Component_Type_Array FROM GAMEOBJECT WHERE Parent_ID = ?";
	sqlite3_stmt* stmt;

	// 쿼리 준비
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// Level_ID 바인딩
		sqlite3_bind_int(stmt, 1, parentID);

		// 쿼리 실행 및 결과 처리
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			// 결과를 얻습니다.
			int gameObject_ID = (sqlite3_column_int(stmt, 0));
			const wchar_t* gameObjectName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* comTypeArr = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));

			vector<int> vComTypeArr = WStringToIntArray(comTypeArr);

			CGameObject* pChildObject = CreateGameObject(gameObject_ID, gameObjectName, parentID, vComTypeArr);
			pNewGameObject->AddChild(pChildObject);
		}

		// 스테이트먼트 종료
		sqlite3_finalize(stmt);
	}
	else {
		// 쿼리 준비에 실패했을 경우의 처리
		assert(false);
	}


	return pNewGameObject;
}

string CSQLMgr::CreateInsertQuery(const std::vector<std::wstring>& scriptNames, int gameObjectID)
{
	std::stringstream query;
	query << "INSERT INTO SCRIPT(Script_Name, GameObject_ID) VALUES ";

	for (size_t i = 0; i < scriptNames.size(); ++i) {
		query << "('" << ToString(scriptNames[i]) << "', " << gameObjectID << ")";
		if (i < scriptNames.size() - 1) {
			query << ", ";
		}
	}
	
	return query.str();
}

#include "pch.h"
#include "CSQLMgr.h"

//매니저 관련
#include "CPathMgr.h"

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

	//초기화 할 때 RESOURCES에 있는 데이터들 삭제.
	string sQuery = "DELETE FROM RESOURCES;";
	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);
}

int CSQLMgr::InsertToLevel(wstring _levelName)
{
	wstring query = L"INSERT INTO LEVEL(Level_Name) VALUES('" + _levelName + L"');";

	CONVERTQUERY(query, Query);

	char* errMsg;
	EXECQUERY(Query, errMsg);

	long long insertedId = sqlite3_last_insert_rowid(db);

	return insertedId;
}

int CSQLMgr::InsertToLayer(int _levelID, wstring _layerName)
{
	wstring query = L"INSERT INTO LAYER(Layer_Name, Level_ID) VALUES('" + _layerName + L" '," + std::to_wstring(_levelID)+ L");";

	CONVERTQUERY(query, Query);

	char* errMsg;
	EXECQUERY(Query, errMsg);

	long long insertedId = sqlite3_last_insert_rowid(db);

	return insertedId;
}

int CSQLMgr::InsertToGameObject(int _layerID, wstring _gameObjectName)
{
	wstring query = L"INSERT INTO GAMEOBJECT(GameObject_Name, Layer_ID) VALUES('" + _gameObjectName + L" '," + std::to_wstring(_layerID) + L");";

	CONVERTQUERY(query, Query);

	char* errMsg;
	EXECQUERY(Query, errMsg);

	long long insertedId = sqlite3_last_insert_rowid(db);

	return insertedId;
}

void CSQLMgr::InsertToComponent(int _gameObjectID, int _type, wstring _attribute)
{
}

void CSQLMgr::DeleteAllRecordToAllTable()
{
	/*string sQuery = "DELETE FROM Level;";
	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);

	sQuery = "DELETE FROM Layer;";
	cQuery = sQuery.c_str();

	EXECQUERY(cQuery, errMsg);

	sQuery = "DELETE FROM GameObject;";
	cQuery = sQuery.c_str();

	EXECQUERY(cQuery, errMsg);

	sQuery = "DELETE FROM Component;";
	cQuery = sQuery.c_str();

	EXECQUERY(cQuery, errMsg);*/

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

#include "pch.h"
#include "CSQLMgr.h"

//매니저 관련
#include "CPathMgr.h"

//assert 매크로
#define EXECQUERY(Query, ErrMsg)		\
	if (sqlite3_exec(db, Query, 0, 0, &ErrMsg) != SQLITE_OK)	\
		{ \
			assert(false);	\
		}

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

void CSQLMgr::InsertToLevel(wstring _levelName)
{
	wstring query = L"INSERT INTO Level(Level_Name) VALUES('" + _levelName + L"');";

	string sQuery;
	sQuery.assign(query.begin(), query.end());

	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);


}

void CSQLMgr::InsertToLayer(int _levelID, wstring _layerName)
{
}

void CSQLMgr::InsertToGameObject(int _layerID, wstring _levelName)
{
}

void CSQLMgr::InsertToComponent(int _gameObjectID, int _type, wstring _attribute)
{
}

void CSQLMgr::DeleteAllRecordToAllTable()
{
	string sQuery = "DELETE FROM Level;";
	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);
}

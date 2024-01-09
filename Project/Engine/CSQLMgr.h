#pragma once
#include "CSingleton.h"

#include <SQLLight/sqlite3.h>

    //쿼리 실행 매크로
    #define EXECQUERY(Query, ErrMsg)		\
	    if (sqlite3_exec(db, Query, 0, 0, &ErrMsg) != SQLITE_OK)	\
		    { \
			    assert(false);	\
		    }

    //wstring to query 매크로
    #define CONVERTQUERY(wstring, Query)	\
	    string sQuery;	\
	    sQuery.assign(query.begin(), query.end());		\
	    \
	    const char* Query = sQuery.c_str();	

class CLevel;
class CSQLMgr :
    public CSingleton<CSQLMgr>
{
    SINGLE(CSQLMgr)

public:
    void init();

public:
    int InsertToLevel(wstring _levelName);
    int InsertToLayer(int _levelID, wstring _layerName, int _layerIdx);
    int InsertToGameObject(int _layerID, wstring _gameObjectName, int _parentID);

    void SelectFromLevel(int _levelID, CLevel*& _level);
    void SelectFromLayer(int _levelID, CLevel*& _level);
    void SelectFromGameObject(int _layerID, int layer_Idx, CLevel*& _level);
    void SelectFromComponent(int _GameObjectID, CGameObject*& _gameObject);

    void UpdateToGameObject(int _gameObjectID, wstring _comType);

    void DeleteAllRecordToAllTable();

public:
    CGameObject* CreateGameObject(int _gameObjectID, const wstring _gameObjectName, int _parentID, vector<int> _comTypeArr);
    string CreateInsertQuery(const std::vector<std::wstring>& scriptNames, int gameObjectID);

public:
    sqlite3* GetDB() { return db; }

private:
    sqlite3* db;
};
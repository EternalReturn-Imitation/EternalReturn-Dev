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

class CSQLMgr :
    public CSingleton<CSQLMgr>
{
    SINGLE(CSQLMgr)

public:
    void init();

public:
    int InsertToLevel(wstring _levelName);
    int InsertToLayer(int _levelID, wstring _layerName);
    int InsertToGameObject(int _layerID, wstring _gameObjectName);
    void InsertToComponent(int _gameObjectID, int _type, wstring _attribute);

    void SelectFromLevel();
    void SelectFromLayer();
    void SelectFromGameObject();
    void SelectToComponent();

    void DeleteAllRecordToAllTable();

public:
    sqlite3* GetDB() { return db; }

private:
    sqlite3* db;
};
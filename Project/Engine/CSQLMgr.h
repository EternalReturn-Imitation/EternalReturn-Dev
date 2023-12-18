#pragma once
#include "CSingleton.h"

#include <SQLLight/sqlite3.h>

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

private:
    sqlite3* db;
};
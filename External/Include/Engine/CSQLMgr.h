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
    void InsertToLevel(wstring _levelName);
    void InsertToLayer(int _levelID, wstring _layerName);
    void InsertToGameObject(int _layerID, wstring _levelName);
    void InsertToComponent(int _gameObjectID, int _type, wstring _attribute);

    void SelectFromLevel();
    void SelectFromLayer();
    void SelectFromGameObject();
    void SelectToComponent();

    void DeleteAllRecordToAllTable();

private:
    sqlite3* db;
};
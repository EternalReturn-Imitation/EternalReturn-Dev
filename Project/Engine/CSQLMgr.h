#pragma once
#include "CSingleton.h"

#include <SQLLight/sqlite3.h>

class CSQLMgr :
    public CSingleton<CSQLMgr>
{
    SINGLE(CSQLMgr)

public:
    void Init();

private:
    sqlite3* db;
};
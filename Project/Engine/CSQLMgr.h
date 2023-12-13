#pragma once
#include "CSingleton.h"

#include <SQLLight/sqlite3.h>

class CSQLMgr :
    public CSingleton<CSQLMgr>
{
    SINGLE(CSQLMgr)

public:
    void init();

private:
    sqlite3* db;
};
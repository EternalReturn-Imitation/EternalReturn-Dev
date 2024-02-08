#pragma once
#include "CComponent.h"
class CFindPath :
    public CComponent
{
public:
    CFindPath();
    ~CFindPath();

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType);
    virtual void LoadFromDB(int _gameObjectID);

    CLONE(CFindPath);
};


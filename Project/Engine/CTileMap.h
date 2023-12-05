#pragma once

#include "CRenderComponent.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    UINT                m_iTileCountX;  // 타일 가로
    UINT                m_iTileCountY;  // 타일 세로
    Vec2                m_vSliceSize;   // 타일 하나의 크기(UV 단위)
    vector<tTile>       m_vecTile;
    CStructuredBuffer*  m_Buffer;

public:
    virtual void finaltick() override;
    virtual void render() override;

    void UpdateData();

    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize)  { m_vSliceSize = _vSliceSize; }


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;


    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};


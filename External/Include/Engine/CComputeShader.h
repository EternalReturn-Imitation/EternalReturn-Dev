#pragma once
#include "CShader.h"

class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3D11ComputeShader>     m_CS;
    ComPtr<ID3DBlob>                m_CSBlob;   

protected:
    // 쉐이더에 전달할 상수 데이터
    tMtrlConst                      m_Const;

    // 그룹 개수
    UINT                            m_iGroupX; 
    UINT                            m_iGroupY;
    UINT                            m_iGroupZ;

    // 그룹 1개당 스레드 개수
    UINT                            m_iGroupPerThreadX;
    UINT                            m_iGroupPerThreadY;
    UINT                            m_iGroupPerThreadZ;

public:
    void CreateComputeShader(const wstring& _strFileName, const string& _strFuncName);
    void Execute();

private:
    virtual void UpdateData() = 0;
    virtual void Clear() = 0;


    CLONE_DISABLE(CComputeShader);
public:
    CComputeShader();
    ~CComputeShader();
};


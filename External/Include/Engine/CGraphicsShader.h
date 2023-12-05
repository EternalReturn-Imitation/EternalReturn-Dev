#pragma once
#include "CShader.h"


struct tScalarParam
{
    SCALAR_PARAM    eParam;
    string          strDesc;
};

struct tTexParam
{
    TEX_PARAM       eParam;
    string          strDesc;
};


class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;    

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;

    RS_TYPE                         m_RSType;
    DS_TYPE                         m_DSType;
    BS_TYPE                         m_BSType;

    SHADER_DOMAIN                   m_Domain;

    vector<tScalarParam>            m_vecScalarParam;
    vector<tTexParam>               m_vecTexParam;

public:
    void CreateVertexShader(const wstring& _strFileName, const string& _strFuncName);
    void CreateGeometryShader(const wstring& _strFileName, const string& _strFuncName);
    void CreatePixelShader(const wstring& _strFileName, const string& _strFuncName);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }
    void SetDomain(SHADER_DOMAIN _domain) { m_Domain = _domain; }

    SHADER_DOMAIN GetDomain() { return m_Domain; }
    virtual void UpdateData() override;

    void AddScalarParam(SCALAR_PARAM _Param, const string& _desc) { m_vecScalarParam.push_back(tScalarParam{ _Param, _desc });}
    void AddTexParam(TEX_PARAM _Param, const string& _desc) { m_vecTexParam.push_back(tTexParam{_Param, _desc}); }

    const vector<tScalarParam>& GetScalarParam() { return m_vecScalarParam; }
    const vector<tTexParam>& GetTexParam() { return m_vecTexParam; }

public:
    CGraphicsShader();
    ~CGraphicsShader();
};


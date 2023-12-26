#pragma once
#include "CRes.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

class CTexture :
    public CRes
{
private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11RenderTargetView>	    m_RTV;
    ComPtr<ID3D11DepthStencilView>	    m_DSV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;
    ScratchImage                        m_Image;

    UINT                                m_iRecentNum;

public:
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
        , UINT _BindFlag, D3D11_USAGE _Usage);

    int Create(ComPtr<ID3D11Texture2D> _tex2D);


public:
    float Width() { return (float)m_Desc.Width; }
	float Height() { return (float)m_Desc.Height; }
	ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
	ComPtr<ID3D11RenderTargetView>	    GetRTV() { return m_RTV; }
	ComPtr<ID3D11DepthStencilView>	    GetDSV() { return m_DSV; }
	ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }

private:
    virtual int Load(const wstring& _strFilePath) override;
public:
    virtual int Save(const wstring& _strRelativePath) override;

public:
    // _PipelineStage : PIPELINE_STAGE
    void UpdateData(int _iRegisterNum, int _PipelineStage);
    void UpdateData_CS(int _iRegisterNum, bool _bShaderRes);
    void Clear();
    static void Clear(int _iRegisterNum);
    void Clear_CS(bool _bShaderRes);

    bool IsCube() { return m_Desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE ? true : false; }

private:
    virtual void UpdateData() override;

public:
    CTexture(bool _bEngine = false);
    ~CTexture();
};


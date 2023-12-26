#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CSound.h"

#include "CPathMgr.h"


class CResMgr :
    public CSingleton<CResMgr>
{
    SINGLE(CResMgr)
private:
    map<wstring, Ptr<CRes>> m_arrRes[(UINT)RES_TYPE::END];
    bool                    m_Changed;

public:
    void init();
    void tick();

private:
    void InitSound();
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultComputeShader();
    void CreateDefaultMaterial();  



public:
    const map<wstring, Ptr<CRes>>& GetResources(RES_TYPE _Type) { return m_arrRes[(UINT)_Type]; }

    // _BindFlag = D3D11_BIND_FLAG
    Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
        , DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);

    Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D);

    bool IsResourceChanged() { return m_Changed; }

    template<typename T>
    Ptr<T> FindRes(const wstring& _strKey);

    template<typename T>
    void AddRes(const wstring& _strKey, Ptr<T>& _Res);

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

private:
    void DeleteRes(RES_TYPE _type, const wstring& _strKey);

    friend class CEventMgr;
};

template<typename T>
RES_TYPE GetResType()
{
    const type_info& mesh = typeid(CMesh);
    //const type_info& meshdata = typeid(CMeshData);
    const type_info& material = typeid(CMaterial);
    const type_info& texture = typeid(CTexture);
    const type_info& sound = typeid(CSound);
    const type_info& prefab = typeid(CPrefab);
    const type_info& gs = typeid(CGraphicsShader);
    const type_info& cs = typeid(CComputeShader);

    if (typeid(T).hash_code() == mesh.hash_code())
        return RES_TYPE::MESH;
    if (typeid(T).hash_code() == gs.hash_code())
        return RES_TYPE::GRAPHICS_SHADER;
    if (typeid(T).hash_code() == cs.hash_code())
        return RES_TYPE::COMPUTE_SHADER;
    if (typeid(T).hash_code() == texture.hash_code())
        return RES_TYPE::TEXTURE;
    if (typeid(T).hash_code() == material.hash_code())
        return RES_TYPE::MATERIAL;
    if (typeid(T).hash_code() == prefab.hash_code())
        return RES_TYPE::PREFAB;
    if (typeid(T).hash_code() == sound.hash_code())
        return RES_TYPE::SOUND;

    return RES_TYPE::END;
}


template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
    RES_TYPE type = GetResType<T>();
      
    map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)type].find(_strKey);
    if (iter == m_arrRes[(UINT)type].end())
        return nullptr;

    return (T*)iter->second.Get();    
}


template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, Ptr<T>& _Res)
{
    // 중복키로 리소스 추가하려는 경우
    assert( ! FindRes<T>(_strKey).Get() );

    RES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
    _Res->SetKey(_strKey);

    m_Changed = true;
}


template<typename T>
inline Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<CRes> pRes = FindRes<T>(_strKey).Get();
    
    // 이미 해당 키로 리소스가 있다면, 반환
    if (nullptr != pRes)
        return (T*)pRes.Get();
            
    pRes = new T;
    pRes->SetKey(_strKey);
    pRes->SetRelativePath(_strRelativePath);

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    if (FAILED(pRes->Load(strFilePath)))
    {
        assert(nullptr);
    }

    RES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, pRes));


    m_Changed = true;

    return (T*)pRes.Get();
}

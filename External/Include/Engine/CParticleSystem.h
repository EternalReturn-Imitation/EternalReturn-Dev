#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_RWBuffer;
    CStructuredBuffer*          m_ModuleDataBuffer;
    
    tParticleModule             m_ModuleData;
    Ptr<CParticleUpdateShader>  m_UpdateCS;

    bool                        m_bPosCustom;

    Vec3                        m_PointPos;
   
    float                       m_AccTime;

    Ptr<CTexture>               m_pParticleTexture; // 파티클 텍스쳐

    bool                        m_bTickToggle;
    float                       m_aSpawnNum[2];
    bool                        m_bDestoryTrigger;
    float                       m_fTime;

public:
    const int* GetModuleCheckList() { return m_ModuleData.ModuleCheck; }
    void ActiveModule(PARTICLE_MODULE _ModuleType) { m_ModuleData.ModuleCheck[(UINT)_ModuleType] = true; }
    void DeactivateModule(PARTICLE_MODULE _ModuleType) { m_ModuleData.ModuleCheck[(UINT)_ModuleType] = false; }

    tParticleModule& GetParticleInfo() { return m_ModuleData; }
    void SetParticleInfo(tParticleModule& _ParticleModule) { m_ModuleData = _ParticleModule; }

    // ===============
    // 파티클 정보 설정
    // ===============
    
    void SetSpawnInitialColor(Vec3 _vInitialColor) { m_ModuleData.vSpawnColor = _vInitialColor; }   // 생성 시 초기 색상 설정
    void SetSpawnRate(int _SpawnRate) { m_ModuleData.SpawnRate = _SpawnRate; }                      // 초당 생성 개수 설정
    
    void SetMaxParticleCount(int _SpawnCnt) { m_ModuleData.iMaxParticleCount = _SpawnCnt; }         // 최대 파티클 최대 생성 개수

    void SetParticleTexture(Ptr<CTexture> _tex) { m_pParticleTexture = _tex; }

    void SetParticleSpawnNum(int _i) { m_aSpawnNum[1] = (float)_i * m_ModuleData.SpawnRate; }

    void SetDestoryTrigger(bool _b) { m_bDestoryTrigger = _b; }

    void SetParticleRot(Vec3 _rot) { m_ModuleData.vRot.x = _rot.x, m_ModuleData.vRot.y = _rot.y, m_ModuleData.vRot.z = _rot.z; }

    // 파티클 데이터 얻기
    const tParticleModule& GetParticleData() { return m_ModuleData; }

public:
    void SetPosCustom(Vec3 _relativePos) { m_PointPos = _relativePos; }

    void GetRayPos();
    Vec3 GetWorldPos(Vec3 _relativePos, Vec3 _relrativeRot);

    Ptr<CParticleUpdateShader>  GetCS() { return m_UpdateCS; }

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    ~CParticleSystem();
};


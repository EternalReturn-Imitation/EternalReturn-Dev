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
   
    float                       m_AccTime;

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
    

    void SetMaxParticleCound(int _SpawnCnt) { m_ModuleData.iMaxParticleCount = _SpawnCnt; }         // 최대 파티클 최대 생성 개수

    // 파티클 데이터 얻기
    const tParticleModule& GetParticleData() { return m_ModuleData; }


public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    ~CParticleSystem();
};


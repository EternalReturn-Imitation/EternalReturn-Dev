#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CPathFindMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleData{}
	, m_AccTime(0.f)
	, m_PointPos{}
	, m_pParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_TX_RioShootFire.png"))
{
	m_ModuleData.iMaxParticleCount = 3000;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	m_ModuleData.SpawnRate = 50;
	m_ModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);
	m_ModuleData.vSpawnScaleMin = Vec3(1.f, 1.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(0.5, 0.5, 0.5);

	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = Vec3(1.f, 1.f, 1.f);
	m_ModuleData.Space = 0; // 시뮬레이션 좌표계

	m_ModuleData.MinLifeTime = 3.f;
	m_ModuleData.MaxLifeTime = 5.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = 0.5f;
	m_ModuleData.EndScale = 1.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	m_ModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	m_ModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_ModuleData.AddVelocityType = 0; // From Center
	m_ModuleData.Speed = 1.f;
	m_ModuleData.vVelocityDir = Vec3(0.f, 1.f, 0.f);
	m_ModuleData.OffsetAngle;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_ModuleData.StartDrag = 1.f;
	m_ModuleData.EndDrag = -1.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_ModuleData.fNoiseTerm = 1.f;
	m_ModuleData.fNoiseForce = 1.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityAlignment = true;
	m_ModuleData.VelocityScale = true;
	m_ModuleData.vMaxVelocityScale = Vec3(1.f, 1.f, 1.f);
	m_ModuleData.vMaxSpeed = 1.f;




	// 입자 메쉬
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// 파티클 전용 재질
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);

	// 파티클 업데이트 컴퓨트 쉐이더	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	// 파티클 버퍼
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	// 파티클 스폰 개수 전달용 버퍼
	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	// 모듈 활성화 및 모듈 설정 정보 버퍼
	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_RWBuffer)
		delete m_RWBuffer;

	if (nullptr != m_ModuleDataBuffer)
		delete m_ModuleDataBuffer;
}


void CParticleSystem::GetRayPos()
{
	// 비교용 Map Collider 받기
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// 비교용 Main Camera의 Ray 받기
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// 교차점 위치 얻기
	IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, Map);
	m_PointPos = result.vCrossPoint;
}

Vec3 CParticleSystem::GetWorldPos(Vec3 _relativePos, Vec3 _relativeRot)
{
	Matrix matWorldScale = XMMatrixIdentity();
	matWorldScale = XMMatrixScaling(1.f, 1.f, 1.f);
	Matrix matWorld = XMMatrixIdentity();

	Matrix matWorldRot = XMMatrixIdentity();
	matWorldRot = XMMatrixRotationX(_relativeRot.x);
	matWorldRot *= XMMatrixRotationY(_relativeRot.y);
	matWorldRot *= XMMatrixRotationZ(_relativeRot.z);

	Matrix m_matWorldPos = XMMatrixTranslation(_relativePos.x, _relativePos.y, _relativePos.z);
	
	matWorld = matWorldScale * matWorldRot * m_matWorldPos;
	
	return matWorld.Translation();
}

void CParticleSystem::finaltick()
{
	

	// 스폰 레이트 계산
	// 1개 스폰 시간
	float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	m_AccTime += DT;

	// 누적시간이 개당 생성시간을 넘어서면
	if (fTimePerCount < m_AccTime)
	{
		// 초과 배율 ==> 생성 개수
		float fData = m_AccTime / fTimePerCount;

		// 나머지는 남은 시간
		m_AccTime = fTimePerCount * (fData - floor(fData));

		// 버퍼에 스폰 카운트 전달
		tRWParticleBuffer rwbuffer = { (int)fData, };
		m_RWBuffer->SetData(&rwbuffer);
	}


	// 파티클 업데이트 컴퓨트 쉐이더
	m_ModuleDataBuffer->SetData(&m_ModuleData);

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
	m_UpdateCS->SetModuleData(m_ModuleDataBuffer);
	m_UpdateCS->SetNoiseTexture(CResMgr::GetInst()->FindRes<CTexture>(L"noise_01.png"));
	
	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());
	
	// 마우스클릭지점 : MouseUIScript로 이동 예정
	
	//if (KEY_TAP(KEY::RBTN))
	//{
	//	GetRayPos();
	//	m_ModuleData.SpawnRate = 2;
	//}
	//else
	//	m_ModuleData.SpawnRate = 0;
	//
	//Vec3 worldpos = GetWorldPos(m_PointPos, Transform()->GetRelativeRot());
	//worldpos.y += 1.f;
	//m_UpdateCS->SetParticleObjectPos(worldpos);
	

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	// 파티클버퍼 t20 에 바인딩
	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	// 모듈 데이터 t21 에 바인딩
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);

	// Particle Render
	GetMaterial(0)->SetTexParam(TEX_0, m_pParticleTexture);

	GetMaterial(0)->UpdateData();
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);

	// 파티클 버퍼 바인딩 해제
	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
}

void CParticleSystem::render(UINT _iSubset)
{
	render();
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	SaveResRef(m_UpdateCS.Get(), _File);
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);

	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();
	}
}
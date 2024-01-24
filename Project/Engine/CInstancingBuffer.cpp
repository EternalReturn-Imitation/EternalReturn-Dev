#include "pch.h"
#include "CInstancingBuffer.h"

#include "CDevice.h"
#include "CMaterial.h"
#include "CResMgr.h"

CInstancingBuffer::CInstancingBuffer()
	: m_iMaxCount(10)
	, m_iAnimInstCount(0)
	, m_pBoneBuffer(nullptr)
{
	m_pBoneBuffer = new CStructuredBuffer;
}

CInstancingBuffer::~CInstancingBuffer()
{
	if (nullptr != m_pBoneBuffer)
		delete m_pBoneBuffer;
}

void CInstancingBuffer::init()
{
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_iMaxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pInstancingBuffer)))
		assert(NULL);

	m_pCopyShader = (CCopyBoneShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"CopyBoneCS").Get();
}

void CInstancingBuffer::SetData()
{
	if (m_vecData.size() > m_iMaxCount)
	{
		Resize((UINT)m_vecData.size());
	}

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(m_pInstancingBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy(tMap.pData, &m_vecData[0], sizeof(tInstancingData) * m_vecData.size());
	CONTEXT->Unmap(m_pInstancingBuffer.Get(), 0);

	// 본 행렬정보 메모리 복사
	if (m_vecBoneMat.empty())
		return;

	UINT iBufferSize = (UINT)m_vecBoneMat.size() * m_vecBoneMat[0]->GetBufferSize();
	if (m_pBoneBuffer->GetBufferSize() < iBufferSize)
	{
		m_pBoneBuffer->Create(m_vecBoneMat[0]->GetElementSize()
			, m_vecBoneMat[0]->GetElementCount() * (UINT)m_vecBoneMat.size(), SB_TYPE::READ_WRITE, false, nullptr);
	}

	// 복사용 컴퓨트 쉐이더 실행
	UINT iBoneCount = m_vecBoneMat[0]->GetElementCount();
	m_pCopyShader->SetBoneCount(iBoneCount);

	for (UINT i = 0; i < (UINT)m_vecBoneMat.size(); ++i)
	{
		m_pCopyShader->SetRowIndex(i);
		m_pCopyShader->SetSourceBuffer(m_vecBoneMat[i]);
		m_pCopyShader->SetDestBuffer(m_pBoneBuffer);
		m_pCopyShader->Execute();
	}

	// Bone 정보 전달 레지스터
	m_pBoneBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}


void CInstancingBuffer::AddInstancingBoneMat(CStructuredBuffer* _pBuffer)
{
	++m_iAnimInstCount;
	m_vecBoneMat.push_back(_pBuffer);
}

void CInstancingBuffer::Resize(UINT _iCount)
{
	m_pInstancingBuffer = nullptr;

	m_iMaxCount = _iCount;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_iMaxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pInstancingBuffer)))
		assert(NULL);
}
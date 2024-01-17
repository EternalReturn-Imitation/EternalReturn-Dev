#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"

#include "CSetColorShader.h"
#include "CParticleUpdateShader.h"
#include "CAnimation3DShader.h"

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;


	Ptr<CMesh> pMesh = nullptr;

	// ==============
	// PointMesh »ý¼º
	// ==============
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	UINT idx = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddRes(L"PointMesh", pMesh);


	// =============
	// RectMesh »ý¼º
	// =============
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh", pMesh);

	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();


	// ===========
	// Circle Mesh
	// ===========
	//      3 
	//   4     2
	//  5 --0-- 1	
	// ¹ÝÁö¸§
	float fRadius = 0.5f;

	// °¢µµ
	UINT Slice = 40;
	float fTheta = XM_2PI / (float)Slice;

	// Áß½ÉÁ¡
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	vecVtx.push_back(v);

	// Á¤Á¡ À§Ä¡ ÁöÁ¤
	for (UINT i = 0; i < Slice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		vecVtx.push_back(v);
	}

	// ÀÎµ¦½º ¼³Á¤
	for (UINT i = 0; i < Slice - 1; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// ¸¶Áö¸· »ï°¢Çü
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(Slice);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CircleMesh", pMesh);

	vecIdx.clear();
	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CircleMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();


	// ========
	// RingMesh
	// ========

	fRadius = 0.7f;
	UINT iRingSliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö
	float fRingSliceAngle = XM_2PI / iRingSliceCount;
	float fRingUVXStep = 1.f / (float)iRingSliceCount;

	// out
	for (UINT j = 0; j <= iRingSliceCount; ++j)
	{
		float theta = j * fRingSliceAngle;

		v.vPos = Vec3(fRadius * cosf(j * fRingSliceAngle)
			, 0.f
			, fRadius * sinf(j * fRingSliceAngle));

		v.vUV = Vec2(fRingUVXStep * j, 1.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.Normalize();

		v.vTangent.x = -fRadius * sinf(theta);
		v.vTangent.y = 0.f;
		v.vTangent.z = fRadius * cosf(theta);
		v.vTangent.Normalize();

		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		vecVtx.push_back(v);
	}
	
	fRadius = 0.3f;
	// in
	for (UINT j = 0; j <= iRingSliceCount; ++j)
	{
		float theta = j * fRingSliceAngle;

		v.vPos = Vec3(fRadius * cosf(j * fRingSliceAngle)
			, 0.f
			, fRadius * sinf(j * fRingSliceAngle));

		v.vUV = Vec2(fRingUVXStep * j, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.y = -1.f;
		v.vNormal.Normalize();

		v.vTangent.x = -fRadius * sinf(theta);
		v.vTangent.y = 0.f;
		v.vTangent.z = fRadius * cosf(theta);
		v.vTangent.Normalize();

		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iRingSliceCount; ++i)
	{
		vecIdx.push_back(i);
		vecIdx.push_back(i + (iRingSliceCount + 1));
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i + (iRingSliceCount + 1));
		vecIdx.push_back(i + (iRingSliceCount + 2));
		vecIdx.push_back(i + 1);
	}

	vecIdx.push_back(iRingSliceCount);
	vecIdx.push_back(iRingSliceCount * 2 + 1);
	vecIdx.push_back(0);
	vecIdx.push_back(iRingSliceCount * 2 + 1);
	vecIdx.push_back(iRingSliceCount + 1);
	vecIdx.push_back(0);


	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RingMesh", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// ========
	// CubeMesh
	// ========

	Vtx arrCube[24] = {};
	// À­¸é
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// ¾Æ·§ ¸é	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// ¿ÞÂÊ ¸é
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// ¿À¸¥ÂÊ ¸é
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// µÞ ¸é
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// ¾Õ ¸é
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// ÀÎµ¦½º
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CubeMesh", pMesh);
	vecIdx.clear();

	pMesh = new CMesh(true);
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(0);
	vecIdx.push_back(7); vecIdx.push_back(6); vecIdx.push_back(1); vecIdx.push_back(2); vecIdx.push_back(5);
	vecIdx.push_back(6); vecIdx.push_back(7); vecIdx.push_back(4); vecIdx.push_back(3); vecIdx.push_back(2);
	vecIdx.push_back(5); vecIdx.push_back(4);

	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CubeMesh_Debug", pMesh);
	

	// À­¸é
	arrCube[0].vPos = Vec3(-1.f, 1.f, 1.f);
	arrCube[1].vPos = Vec3(1.f, 1.f, 1.f);
	arrCube[2].vPos = Vec3(1.f, 1.f, -1.f);
	arrCube[3].vPos = Vec3(-1.f, 1.f, -1.f);

	// ¾Æ·§ ¸é	
	arrCube[4].vPos = Vec3(-1.f, -1.f, -1.f);
	arrCube[5].vPos = Vec3(1.f, -1.f, -1.f);
	arrCube[6].vPos = Vec3(1.f, -1.f, 1.f);
	arrCube[7].vPos = Vec3(-1.f, -1.f, 1.f);
	
	// ¿ÞÂÊ ¸é
	arrCube[8].vPos = Vec3(-1.f, 1.f, 1.f);
	arrCube[9].vPos = Vec3(-1.f, 1.f, -1.f);
	arrCube[10].vPos = Vec3(-1.f, -1.f, -1.f);
	arrCube[11].vPos = Vec3(-1.f, -1.f, 1.f);

	// ¿À¸¥ÂÊ ¸é
	arrCube[12].vPos = Vec3(1.f, 1.f, -1.f);
	arrCube[13].vPos = Vec3(1.f, 1.f, 1.f);
	arrCube[14].vPos = Vec3(1.f, -1.f, 1.f);
	arrCube[15].vPos = Vec3(1.f, -1.f, -1.f);
	
	// µÞ ¸é
	arrCube[16].vPos = Vec3(1.f, 1.f, 1.f);
	arrCube[17].vPos = Vec3(-1.f, 1.f, 1.f);
	arrCube[18].vPos = Vec3(-1.f, -1.f, 1.f);
	arrCube[19].vPos = Vec3(1.f, -1.f, 1.f);

	// ¾Õ ¸é
	arrCube[20].vPos = Vec3(-1.f, 1.f, -1.f);;
	arrCube[21].vPos = Vec3(1.f, 1.f, -1.f);
	arrCube[22].vPos = Vec3(1.f, -1.f, -1.f);
	arrCube[23].vPos = Vec3(-1.f, -1.f, -1.f);

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"FrustumMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Sphere Mesh
	// ===========
	fRadius = 0.5f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // °¡·Î ºÐÇÒ °³¼ö
	UINT iSliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vNormal.Cross(v.vTangent, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// ÀÎµ¦½º
	// ºÏ±ØÁ¡
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// ¸öÅë
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ³²±ØÁ¡
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"SphereMesh", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Cone Mesh
	// ===========
	
	// ºÏ±ØÁ¡
	v.vPos = Vec3(0.f, 0.5, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// Body
	iSliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö

	fSliceAngle = XM_2PI / iSliceCount;

	fUVXStep = 1.f / (float)iSliceCount;

	for (UINT j = 0; j <= iSliceCount; ++j)
	{
		float theta = j * fSliceAngle;

		v.vPos = Vec3(fRadius * cosf(j * fSliceAngle)
			, 0.f
			, fRadius * sinf(j * fSliceAngle));

		v.vUV = Vec2(fUVXStep * j, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.Normalize();

		v.vTangent.x = -fRadius * sinf(theta);
		v.vTangent.y = 0.f;
		v.vTangent.z = fRadius * cosf(theta);
		v.vTangent.Normalize();

		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		vecVtx.push_back(v);
	}

	// bottom
	for (UINT j = 0; j <= iSliceCount; ++j)
	{
		float theta = j * fSliceAngle;

		v.vPos = Vec3(fRadius * cosf(j * fSliceAngle)
			, 0.f
			, fRadius * sinf(j * fSliceAngle));

		v.vUV = Vec2(fUVXStep * j, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal = v.vPos;
		v.vNormal.y = -1.f;
		v.vNormal.Normalize();

		v.vTangent.x = -fRadius * sinf(theta);
		v.vTangent.y = 0.f;
		v.vTangent.z = fRadius * cosf(theta);
		v.vTangent.Normalize();

		v.vNormal.Cross(v.vTangent, v.vBinormal);
		v.vBinormal.Normalize();

		vecVtx.push_back(v);
	}

	// ³²±ØÁ¡
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vec3(0.f, -1.f, 0.f);
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);
	vecVtx.push_back(v);

	// ÀÎµ¦½º
	// ºÏ±ØÁ¡
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// ³²±ØÁ¡
	iBottomIdx = (UINT)vecVtx.size() - 1;
	
	for (UINT i = 0; i <= iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (iSliceCount + i + 2));
		vecIdx.push_back(iBottomIdx - (iSliceCount + i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"ConeMesh", pMesh);

	
}

void CResMgr::CreateDefaultGraphicsShader()
{
	Ptr<CGraphicsShader> pShader = nullptr;

	// ============================
	// Std2DShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);



	// ======================================
	// Std2DLightShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// g_tex_1              : Nomal Texture
	// ======================================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DLightShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2DLight");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DLight");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddScalarParam(INT_1, "Test Int");
	pShader->AddScalarParam(FLOAT_0, "Test Float");
	pShader->AddScalarParam(VEC2_0, "Test Vec2");
	pShader->AddScalarParam(VEC4_0, "Test Vec4");

	pShader->AddTexParam(TEX_0, "Output Texture 1");
	pShader->AddTexParam(TEX_1, "Output Texture 2");
	pShader->AddTexParam(TEX_2, "Output Texture 3");

	AddRes(pShader->GetKey(), pShader);

	// =================
	// DebugShape Shader
	// Topology : LineStrip
	// RS_TYPE  : CULL_NONE
	// DS_TYPE  : NO_TEST_NO_WRITE
	// BS_TYPE  : Default
	// g_vec4_0 : OutColor
	// ==================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DebugShapeShader");
	pShader->CreateVertexShader(L"shader\\debugshape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debugshape.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);

	// =================
	// DebugShape Shader OutLiner
	// Topology : LineStrip
	// RS_TYPE  : CULL_NONE
	// DS_TYPE  : NO_TEST_NO_WRITE
	// BS_TYPE  : AlphaBlend
	// g_vec4_0 : OutColor
	// ==================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DebugShape_OutLineShader");
	pShader->CreateVertexShader(L"shader\\debugshape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debugshape.fx", "PS_DebugShape_OutLine");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);

	// =================
	// DebugShape Shader Frustum
	// Topology : LineStrip
	// RS_TYPE  : CULL_NONE
	// DS_TYPE  : NO_TEST_NO_WRITE
	// BS_TYPE  : AlphaBlend
	// g_vec4_0 : OutColor
	// ==================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DebugShape_FrustumShader");
	pShader->CreateVertexShader(L"shader\\debugshape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debugshape.fx", "PS_DebugShape_Frustum");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// TileMap Shader
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : LESS
	// BS_TYPE : MASK

	// Parameter
	// g_tex_0 : Tile Atlas Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TileMapShader");
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// ParticleRender
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_WRITE
	// BS_TYPE : ALPHA_BLEND

	// Parameter
	// g_int_0 : Particle Index
	// 
	// Domain : TRANSPARENT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"ParticleRenderShader");
	pShader->CreateVertexShader(L"shader\\particle_render.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particle_render.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particle_render.fx", "PS_ParticleRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	// pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes(pShader->GetKey(), pShader);



	// ============================
	// GrayShader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : POSTPROCESS
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"GrayShader");
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayShader");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Distortion Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : POSTPROCESS
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DistortionShader");
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	// Parameter
	pShader->AddTexParam(TEX_1, "Noise Texture");

	AddRes(pShader->GetKey(), pShader);



	// ============================
	// TestShader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT	 
	// Domain : MASK
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TestShader");
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_TestShader");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_TestShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Parameter
	pShader->AddScalarParam(INT_0, "Color Type");
	pShader->AddTexParam(TEX_0, "Output Texture");



	// ============================
	// Std3DShader
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT
	// Domain : MASK
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std3DShader");

	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	// Parameter	
	pShader->AddScalarParam(FLOAT_0, "Spec Coeff");
	pShader->AddTexParam(TEX_0, "Output Texture");
	pShader->AddTexParam(TEX_1, "Normal Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// StdRingShader
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT
	// Domain : MASK
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"StdRingShader");

	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	// Parameter	
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// SkyBoxShader
	// RS_TYPE : CULL_FRONT
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT
	// Domain : MASK
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"SkyBoxShader");

	pShader->CreateVertexShader(L"shader\\skybox.fx", "VS_SkyBoxShader");
	pShader->CreatePixelShader(L"shader\\skybox.fx", "PS_SkyBoxShader");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Parameter	
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Decal Shader
	// RS_TYPE : CULL_FRONT
	// DS_TYPE : NoTest_NoWirte
	// BS_TYPE : ALPHA
	// Domain  : Decal
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DecalShader");

	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

	// Parameter	
	pShader->AddTexParam(TEX_1, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Deferred Decal Shader
	// RS_TYPE : CULL_FRONT
	// DS_TYPE : NoTest_NoWirte
	// BS_TYPE : DEFEREED_DECAL_BLEND
	// Domain  : Decal
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DeferredDecalShader");

	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_DeferredDecal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_DeferredDecal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFEREED_DECAL_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL);

	// Parameter	
	pShader->AddTexParam(TEX_1, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Std3D_Deferred
	// RS_TYPE : CULL_FRONT
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT
	// Domain : Deferred
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std3D_DeferredShader");

	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	// Parameter	
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// DirLightShader
	// RS_TYPE : CULL_BACK
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : ONE_ONE
	// Domain : LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DirLightShader");

	pShader->CreateVertexShader(L"shader\\light.fx", "VS_DirLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_DirLightShader");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// PointLightShader
	// RS_TYPE : CULL_FRONT
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : ONE_ONE
	// Domain : LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"PointLightShader");

	pShader->CreateVertexShader(L"shader\\light.fx", "VS_PointLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_PointLightShader");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);

	AddRes(pShader->GetKey(), pShader);

	// =====================================
	// MergeShader
	// MRT              : SwapChain
	// Domain           : DOMAIN_LIGHT
	// Mesh             : RectMesh
	// Rasterizer       : CULL_BACK
	// DepthStencil     : NO_TEST_NO_WRITE
	// Blend            : Default
	// =====================================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"MergeShader");

	pShader->CreateVertexShader(L"shader\\light.fx", "VS_MergeShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_MergeShader");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

	AddRes(pShader->GetKey(), pShader);

	// =====================================
	// ShadowMap Shader
	// MRT              : SHADOWMAP
	// Domain           : DOMAIN_LIGHT	
	// Rasterizer       : CULL_BACK
	// DepthStencil     : LESS
	// Blend            : Default
	// =====================================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"ShadowMapShader");

	pShader->CreateVertexShader(L"shader\\light.fx", "VS_ShadowMap");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_ShadowMap");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

	AddRes(pShader->GetKey(), pShader);

	// =====================================
	// Tess Shader
	// MRT              : SwapChain
	// Domain           : DOMAIN_OPAQUE	
	// Rasterizer       : CULL_NONE
	// DepthStencil     : LESS
	// Blend            : Default
	// =====================================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TessShader");

	pShader->CreateVertexShader(L"shader\\tess.fx", "VS_Tess");
	pShader->CreateHullShader(L"shader\\tess.fx", "HS_Tess");
	pShader->CreateDomainShader(L"shader\\tess.fx", "DS_Tess");
	pShader->CreatePixelShader(L"shader\\tess.fx", "PS_Tess");

	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// LandScapeShader	
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT
	// 
	// Parameter
	// g_tex_0 : Output Texture
	// g_tex_1 : Normal Texture
	// Domain : Opaque
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"LandScapeShader");
	pShader->CreateVertexShader(L"shader\\landscape.fx", "VS_LandScape");
	pShader->CreateHullShader(L"shader\\landscape.fx", "HS_LandScape");
	pShader->CreateDomainShader(L"shader\\landscape.fx", "DS_LandScape");
	pShader->CreatePixelShader(L"shader\\landscape.fx", "PS_LandScape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	AddRes(pShader->GetKey(), pShader);
}

void CResMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pCS = nullptr;

	// Texture »ö»ó º¯°æ ½¦ÀÌ´õ
	pCS = new CSetColorShader(32, 32, 1);
	pCS->SetKey(L"SetColorCS");
	pCS->CreateComputeShader(L"shader\\setcolor.fx", "CS_SetColor");
	AddRes(pCS->GetKey(), pCS);

	// Particle Update ½¦ÀÌ´õ
	pCS = new CParticleUpdateShader(128, 1, 1);
	pCS->SetKey(L"ParticleUpdateCS");
	pCS->CreateComputeShader(L"shader\\particle_update.fx", "CS_ParticleUpdate");
	AddRes(pCS->GetKey(), pCS);

	// Animation Matrix Update ½¦ÀÌ´õ
	pCS = new CAnimation3DShader(256, 1, 1);
	pCS->SetKey(L"Animation3DUpdateCS");
	pCS->CreateComputeShader(L"shader\\animation3d.fx", "CS_Animation3D");
	AddRes(pCS->GetKey(), pCS);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// ===========
	// ==  2 D  ==
	// ===========
	 
	// Std2D Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);

	// Std2DAnim Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DAnimMtrl", pMtrl);

	// Std2DLight Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"Std2DLightMtrl", pMtrl);

	// Std2DLight Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"Std2DAnimLightMtrl", pMtrl);

	// ===========
	// ==  3 D  ==
	// ===========

	// Std3DMtrl	
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DShader"));
	AddRes(L"Std3DMtrl", pMtrl);

	// SkyBoxMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"SkyBoxShader"));
	AddRes(L"SkyBoxMtrl", pMtrl);

	// Std3D_DeferredShader
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));
	AddRes(L"Std3D_DeferredMtrl", pMtrl);


	// ===========
	// == Decal ==
	// ===========

	// DecalMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DecalShader"));
	AddRes(L"DecalMtrl", pMtrl);
	
	// DeferredDecal
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DeferredDecalShader"));
	AddRes(L"DeferredDecalMtrl", pMtrl);


	// ===========
	// == Light ==
	// ===========

	// DirLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DirLightShader"));
	AddRes(L"DirLightMtrl", pMtrl);

	// PointLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PointLightShader"));
	AddRes(L"PointLightMtrl", pMtrl);

	// ===========
	// == Debug ==
	// ===========
	
	// DebugShape Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugShapeShader"));
	AddRes(L"DebugShapeMtrl", pMtrl);

	// DebugShape Sphere Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugShape_OutLineShader"));
	AddRes(L"DebugShapeSphereMtrl", pMtrl);

	// DebugShape Frustum Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugShape_FrustumShader"));
	AddRes(L"DebugShapeFrustumMtrl", pMtrl);

	// =============
	// == Graphic ==
	// =============

	// TileMap Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes(L"TileMapMtrl", pMtrl);

	// Particle Render Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderMtrl", pMtrl);

	// MergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"MergeShader"));
	AddRes(L"MergeMtrl", pMtrl);

	// ShadowMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ShadowMapShader"));
	AddRes(L"ShadowMapMtrl", pMtrl);

	// LandScapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"LandScapeShader"));
	AddRes(L"LandScapeMtrl", pMtrl);

	// =================
	// == PostProcess ==
	// =================

	// GrayShader(PostProcess)
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GrayShader"));
	AddRes(L"GrayMtrl", pMtrl);

	// DistortionShader(PostProcess)
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DistortionShader"));
	AddRes(L"DistortionMtrl", pMtrl);

	// TessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TessShader"));
	AddRes(L"TessMtrl", pMtrl);
}
#pragma once
#include "global.h"

//===============
// Struct of FBX 
//===============
struct tFbxMaterial
{
	tMtrlData	tMtrl;
	wstring     strMtrlName;
	wstring     strDiff;
	wstring		strNormal;
	wstring		strSpec;
	wstring		strEmis;
};

struct tWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
};

struct tContainer
{
	wstring								strName;
	vector<Vec3>						vecPos;
	vector<Vec3>						vecTangent;
	vector<Vec3>						vecBinormal;
	vector<Vec3>						vecNormal;
	vector<Vec2>						vecUV;

	vector<Vec4>						vecIndices;
	vector<Vec4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Transform 관련 정보
	bool								bGroupNode;	// 그룹노드 여부
	tTransformInfo						tLocalTransform;


	// Animation 관련 정보
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}

	int									iParentIdx = -1;
};

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring				strBoneName;
	int					iDepth;			// 계층구조 깊이
	int					iParentIndx;	// 부모 Bone 의 인덱스
	FbxAMatrix			matOffset;		// Offset 행렬( -> 뿌리 -> Local)
	FbxAMatrix			matBone;
	vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		strName;
	FbxTime		tStartTime;
	FbxTime		tEndTime;
	FbxLongLong	llTimeLength;
	FbxTime::EMode eMode;
};



class CMesh;

class CFBXLoader
{
private:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	FbxImporter* m_pImporter;

	vector<tContainer>				m_vecContainer;
	int								m_iContainerCnt;

	// Animation
	vector<tBone*>					m_vecBone;
	FbxArray<FbxString*>			m_arrAnimName;
	vector<tAnimClip*>				m_vecAnimClip;


public:
	void init();
	void LoadFbx(const wstring& _strPath);

public:
	int GetContainerCount() { return (int)m_vecContainer.size(); }
	const tContainer& GetContainer(int _iIdx) { return m_vecContainer[_iIdx]; }
	vector<tBone*>& GetBones() { return m_vecBone; }
	vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }

private:
	void LoadMeshDataFromNode(FbxNode* _pRoot, int _iParentIdx = -1);
	void LoadMesh(FbxMesh* _pFbxMesh);
	void LoadMaterial(FbxSurfaceMaterial* _pMtrlSur);
	void LoadTransfrom(FbxNode* _pNode);

	Vec3 GetTangent(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	Vec3 GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	Vec3 GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	Vec2 GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iVtxId, int _iVtxCnt, int _iIdx);

	Vec4 GetMtrlData(FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
	wstring GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

	void LoadTexture();
	void CreateMaterial();

	// Animation
	void LoadSkeleton(FbxNode* _pNode);
	void LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
	void LoadAnimationClip();
	void Triangulate(FbxNode* _pNode);

	void LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer);
	void LoadWeightsAndIndices(FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
	void LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform
		, int _iBoneIdx, tContainer* _pContainer);

	int FindBoneIndex(string _strBoneName);
	FbxAMatrix GetTransform(FbxNode* _pNode);

	void CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer);

	FbxQuaternion EulerToQuaternion(const FbxVector4& euler);
public:
	CFBXLoader();
	~CFBXLoader();
};


#include "pch.h"
#include "global.h"

tTransform g_transform = {};
tGlobal GlobalData = {};



extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END] =
{
	"MESHDATA",
	"MATERIAL",
	"PREFAB",
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
	"COMPUTE_SHADER"
};

extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END] =
{
	L"MESHDATA",
	L"MATERIAL",
	L"PREFAB",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"GRAPHICS_SHADER",
	L"COMPUTE_SHADER"
};



extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END] =
{

	"TRANSFORM",		// 위치, 크기, 회전
	"COLLIDER2D",		// 2차원 충돌
	"COLLIDER3D",		// 3차원 충돌
	"ANIMATOR2D",		// Sprite Animation
	"ANIMATOR3D",		// Bone Sknning Animation
	"LIGHT2D",		// 2차원 광원
	"LIGHT3D",		// 3차원 광원
	"CAMERA",			// Camera
	"BEHAVIORTREE",	// 행동트리
	"MESHRENDER",		// 기본적인 렌더링
	"PARTICLESYSTEM", // 입자 렌더링
	"TILEMAP",		// 2차원 타일
	"LANDSCAPE",		// 3차원 지형
	"SKYBOX",			// SkyBox
	"DECAL",			// 내부 렌더링
};

extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END] =
{
	L"TRANSFORM",
	L"COLLIDER2D",
	L"COLLIDER3D",
	L"ANIMATOR2D",
	L"ANIMATOR3D",
	L"LIGHT2D",
	L"LIGHT3D",
	L"CAMERA",
	L"BEHAVIORTREE",
	L"MESHRENDER",
	L"PARTICLESYSTEM",
	L"TILEMAP",
	L"LANDSCAPE",
	L"SKYBOX",
	L"DECAL",
};

extern const char* COLLIDER2D_TYPE_STR[(UINT)COLLIDER2D_TYPE::END] =
{
	"RECT",
	"CIRCLE",
};

extern const wchar_t* COLLIDER2D_TYPE_WSTR[(UINT)COLLIDER2D_TYPE::END] =
{
	L"RECT",
	L"CIRCLE",
};
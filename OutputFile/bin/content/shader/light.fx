#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

struct PS_OUT
{
    float4 vDiffuse : SV_Target0;
    float4 vSpecular : SV_Target1;
    float4 vShadow : SV_Target2;
};

// =====================================
// DirLightShader
// MRT              : LIGHT
// Mesh             : RectMesh
// Rasterizer       : CULL_BACK
// DepthStencil     : NO_TEST_NO_WRITE
// Blend            : ONE_ONE

// Parameter
#define NormalTargetTex     g_tex_0
#define PoisitionTargetTex  g_tex_1
#define ShadowMapTargetTex  g_tex_2
#define LightIdx            g_int_0
#define LightVP             g_mat_0
// =====================================

VS_OUT VS_DirLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 사용하는 메쉬가 RectMesh(로컬 스페이스에서 반지름 0.5 짜리 정사각형)
    // 따라서 2배로 키워서 화면 전체가 픽셀쉐이더가 호출될 수 있게 한다.
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    
    return output;
}


PS_OUT PS_DirLightShader(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vScreenUV = _in.vPosition.xy / g_Resolution.xy;
    float3 vViewPos = PoisitionTargetTex.Sample(g_sam_0, vScreenUV).xyz;
    float3 vViewNormal = NormalTargetTex.Sample(g_sam_0, vScreenUV).xyz;
    
    // 픽셀 위치랑 동일한 UV 위치에서 Position 값을 가져왔는데, 해당 지점에 기록된 물체가 없다.
    if (vViewPos.x == 0.f && vViewPos.y == 0.f && vViewPos.z == 0.f)
    {
        discard;
    }
    
    // 광원 계산 결과를 받을 변수 선언
    tLightColor LightColor = (tLightColor) 0.f;
    float fSpecPow = 0.f;
    
    // 광원 본인의 정보가 LightBuffer 내에서 어디에 있는지 인덱스를 입력
    // 빛을 넣어줄 물체의 위치값과 노말값을 타겟 텍스쳐에서 가져와서 입력
    CalcLight3D(vViewPos, vViewNormal, LightIdx, LightColor, fSpecPow);
        
    // 그림자 판정
    float fShadowPow = 0.f;
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float4 vLightProj = mul(float4(vWorldPos, 1.f), LightVP);
    float2 vShadowMapUV = vLightProj.xy / vLightProj.w;
    vShadowMapUV.x = vShadowMapUV.x / 2.f + 0.5f;
    vShadowMapUV.y = (1.f - vShadowMapUV.y / 2.f) - 0.5f;
    
    if (vShadowMapUV.x < 0.f || 1.f < vShadowMapUV.x ||
        vShadowMapUV.y < 0.f || 1.f < vShadowMapUV.y)
    {
        fShadowPow = 0.f;
    }
    else
    {
        float fDepth = vLightProj.z / vLightProj.w;
        float fLightDepth = ShadowMapTargetTex.Sample(g_sam_1, vShadowMapUV);
    
        if (fLightDepth + 0.002f <= fDepth)
        {
            // 그림자
            fShadowPow = 0.5f;
        }
    }
    
    // 계산된 최종 광원의 세기를 각 타겟(Diffuse, Specular) 에 출력
    output.vDiffuse = (LightColor.vDiffuse + LightColor.vAmbient) * (1.f - fShadowPow);
    output.vSpecular = g_Light3DBuffer[LightIdx].Color.vDiffuse * (1.f - fShadowPow);
    output.vShadow = fShadowPow;
    
    // output.vDiffuse.a = 1.f;
    // output.vSpecular.a = 1.f;
    
    return output;
}


// =====================================
// PointLightShader
// MRT              : LIGHT
// Mesh             : SphereMesh
// Rasterizer       : CULL_FRONT
// DepthStencil     : NO_TEST_NO_WRITE
// Blend            : ONE_ONE

// Parameter
//#define NormalTargetTex     g_tex_0
//#define PoisitionTargetTex  g_tex_1
//#define LightIdx            g_int_0
// =====================================
VS_OUT VS_PointLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 사용하는 메쉬가 RectMesh(로컬 스페이스에서 반지름 0.5 짜리 정사각형)
    // 따라서 2배로 키워서 화면 전체가 픽셀쉐이더가 호출될 수 있게 한다.
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        
    return output;
}


PS_OUT PS_PointLightShader(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vScreenUV = _in.vPosition.xy / g_Resolution.xy;
    
    float3 vViewPos = PoisitionTargetTex.Sample(g_sam_0, vScreenUV).xyz;
    float3 vViewNormal = NormalTargetTex.Sample(g_sam_0, vScreenUV).xyz;
    
    // 픽셀 위치랑 동일한 UV 위치에서 Position 값을 가져왔는데, 해당 지점에 기록된 물체가 없다.
    if (vViewPos.x == 0.f && vViewPos.y == 0.f && vViewPos.z == 0.f)
    {
        //output.vDiffuse = float4(1.f, 0.f, 0.f, 1.f);
        //output.vSpecular = float4(1.f, 0.f, 0.f, 1.f);
        //return output;
        discard;
    }
    
    // 볼륨메쉬 내부 영역인지 확인하기
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocal = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    float fDist = length(vLocal.xyz);
    
    if (0.5f < fDist)
    {
        //output.vDiffuse = float4(1.f, 0.f, 0.f, 1.f);
        //output.vSpecular = float4(1.f, 0.f, 0.f, 1.f);
        //return output;
        discard;
    }
        
    // 광원 계산 결과를 받을 변수 선언
    tLightColor LightColor = (tLightColor) 0.f;
    float fSpecPow = 0.f;
    
    // 광원 본인의 정보가 LightBuffer 내에서 어디에 있는지 인덱스를 입력
    // 빛을 넣어줄 물체의 위치값과 노말값을 타겟 텍스쳐에서 가져와서 입력
    CalcLight3D(vViewPos, vViewNormal, LightIdx, LightColor, fSpecPow);

    // 계산된 최종 광원의 세기를 각 타겟(Diffuse, Specular) 에 출력
    output.vDiffuse = LightColor.vDiffuse + LightColor.vAmbient;
    output.vSpecular = g_Light3DBuffer[LightIdx].Color.vDiffuse * fSpecPow;
        
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}






// =====================================
// MergeShader
// MRT              : SwapChain
// Domain           : DOMAIN_LIGHT
// Mesh             : RectMesh
// Rasterizer       : CULL_BACK
// DepthStencil     : NO_TEST_NO_WRITE
// Blend            : Default

// Parameter
#define ColorTargetTex    g_tex_0
#define DiffuseTargetTex  g_tex_1
#define SpecularTargetTex g_tex_2
#define EmissiveTargetTex g_tex_3
#define ShadowTargetTex   g_tex_4
// =====================================
VS_OUT VS_MergeShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 사용하는 메쉬가 RectMesh(로컬 스페이스에서 반지름 0.5 짜리 정사각형)
    // 따라서 2배로 키워서 화면 전체가 픽셀쉐이더가 호출될 수 있게 한다.
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    
    return output;
}


float4 PS_MergeShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    
    float2 vScreenUV = _in.vPosition.xy / g_Resolution.xy;
        
    float4 vColor = ColorTargetTex.Sample(g_sam_0, vScreenUV);
    float4 vDiffuse = DiffuseTargetTex.Sample(g_sam_0, vScreenUV);
    float4 vSpecular = SpecularTargetTex.Sample(g_sam_0, vScreenUV);
    float4 vEmissive = EmissiveTargetTex.Sample(g_sam_0, vScreenUV);
    float fShadowPow = ShadowTargetTex.Sample(g_sam_0, vScreenUV).r;
    
    vOutColor.xyz = vColor.xyz * vDiffuse.xyz * (1.f - fShadowPow)
                    + (vSpecular.xyz * vColor.a) * (1.f - fShadowPow)
                    + vEmissive.xyz;
    
    vOutColor.xyz = vColor.xyz * 1.f * (1.f - fShadowPow)
                    + (1.f * vColor.a) * (1.f - fShadowPow)
                    + vEmissive.xyz;
    
    vOutColor.a = 1.f;
    
    return vOutColor;
}


// ===============
// DepthMap Shader
// MRT : ShadowMap MRT
// RS : CULL_BACK
// BS : Default
// DS : Less
// ===============
struct VS_SHADOW_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_SHADOW_OUT
{
    float4 vPosition : SV_Position;
    float4 vProjPos : POSITION;
};

VS_SHADOW_OUT VS_ShadowMap(VS_SHADOW_IN _in)
{
    VS_SHADOW_OUT output = (VS_SHADOW_OUT) 0.f;
    
    float3 backupVPos = _in.vPos;
    
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }
    
    // 사용하는 메쉬가 RectMesh(로컬 스페이스에서 반지름 0.5 짜리 정사각형)
    // 따라서 2배로 키워서 화면 전체가 픽셀쉐이더가 호출될 수 있게 한다.
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    output.vProjPos = output.vPosition;
    output.vProjPos.xyz /= output.vProjPos.w;    
    
            
    return output;
}

float4 PS_ShadowMap(VS_SHADOW_OUT _in) : SV_Target
{
    return float4(_in.vProjPos.z, 0.f, 0.f, 0.f);
}




#endif
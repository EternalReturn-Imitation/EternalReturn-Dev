#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"

static float3 g_vLightPos = float3(0.f, 0.f, 0.f);
static float3 g_vLightDir = float3(1.f, -1.f, 1.f);

static float3 g_vLightColor = float3(1.f, 1.f, 1.f);
static float g_fLightSpecCoeff = 1.0f;
static float3 g_vLightAmb = float3(0.15f, 0.15f, 0.15f); // 환경광

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;   
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal: BINORMAL;
    
};


VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    // 로컬에서의 Normal 방향을 월드로 이동    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
    
    
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);    
    output.vUV = _in.vUV;
      
    return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.5f, 0.5f, 0.5f, 1.f);
        
    float3 vViewNormal = _in.vViewNormal;
    
    if(g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    if (g_btex_1)
    {
        float3 vNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;        
        
        // 0 ~ 1 범위의 값을 -1 ~ 1 로 확장        
        vNormal = vNormal * 2.f - 1.f;
        
        float3x3 vRotateMat =
        {
            _in.vViewTangent,
            -_in.vViewBinormal,
            _in.vViewNormal        
        };
        
        vViewNormal = mul(vNormal, vRotateMat);        
    }
    
        
    // ViewSpace 에서의 광원의 방향
    float3 vViewLightDir = normalize(mul(float4(normalize(g_vLightDir), 0.f), g_matView)).xyz;
    
    // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
    float fLightPow = saturate(dot(vViewNormal, -vViewLightDir));
    
    // 반사광
    float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(-vViewLightDir, vViewNormal)) * vViewNormal);
    float3 vEye = normalize(_in.vViewPos);
   
    // 반사광의 세기 구하기
    float fSpecPow = saturate(dot(vViewReflect, -vEye));
    fSpecPow = pow(fSpecPow, 20);
           
    vOutColor.xyz = (vOutColor.xyz * g_vLightColor * fLightPow) // 물체 기본색상 출력
                    + (vOutColor.xyz * g_vLightColor * g_vLightAmb) // 환경광
                    + (g_vLightColor * g_fLightSpecCoeff * fSpecPow * fLightPow); //반사광
    
    return vOutColor;
}



#endif
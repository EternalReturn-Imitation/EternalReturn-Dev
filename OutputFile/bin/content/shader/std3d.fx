#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"

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


//
// Std3DShader
//
// Param
#define SPEC_COEFF      saturate(g_float_0) // 반사 계수

#define IS_SKYBOX_ENV   g_btexcube_0
#define SKYBOX_ENV_TEX  g_cube_0

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
        
        vViewNormal = normalize(mul(vNormal, vRotateMat));
    }
    
    tLightColor lightcolor = (tLightColor) 0.f;
    float fSpecPow = 0.f;
    
    for (uint i = 0; i < g_Light3DCount; ++i)
    {
        CalcLight3D(_in.vViewPos, vViewNormal, i, lightcolor, fSpecPow);
    }
                 
    vOutColor.xyz = vOutColor.xyz * lightcolor.vDiffuse.xyz
                    + vOutColor.xyz * lightcolor.vAmbient.xyz
                    + saturate(g_Light3DBuffer[0].Color.vDiffuse.xyz) * 0.3f * fSpecPow * SPEC_COEFF;
    
    if(IS_SKYBOX_ENV)
    {        
        float3 vEye = normalize(_in.vViewPos);
        float3 vEyeReflect = normalize(reflect(vEye, vViewNormal));
        
        vEyeReflect = normalize(mul(float4(vEyeReflect, 0.f), g_matViewInv));
        
        vOutColor *= SKYBOX_ENV_TEX.Sample(g_sam_0, vEyeReflect);
    }
    
    return vOutColor;
}



#endif
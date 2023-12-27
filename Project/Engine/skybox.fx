#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#include "func.fx"


struct VS_SKY_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;    
};

struct VS_SKY_OUT
{
    float4 vPosition : SV_Position; 
    float2 vUV : TEXCOORD;
    
    float3 vPos : POSITION;
};

#define SKYBOX_TYPE g_int_0


VS_SKY_OUT VS_SkyBoxShader(VS_SKY_IN _in)
{
    VS_SKY_OUT output = (VS_SKY_OUT) 0.f;
            
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
    vProjPos.z = vProjPos.w;
        
    output.vPosition = vProjPos;    
    output.vUV = _in.vUV;
    output.vPos = _in.vPos;
    
    return output;
}

float4 PS_SkyBoxShader(VS_SKY_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // 0 : Sphere Type
    if (0 == SKYBOX_TYPE)
    {
        if (g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    
    // 1 : Cube Type
    else if (1 == SKYBOX_TYPE)
    {
        if (g_btexcube_0)
        {
            float3 vUV = normalize(_in.vPos);
            vOutColor = g_cube_0.Sample(g_sam_0, vUV);
        }
    }
    
    return vOutColor;
}

#endif
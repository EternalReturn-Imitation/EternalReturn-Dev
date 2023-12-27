#ifndef _DECAL
#define _DECAL

#include "value.fx"
#include "func.fx"


// ================================
// DecalShader
// MRT                  : SwapChain
// DOMAIN               : DOMAIN_DECAL
// Mesh                 : CubeMesh
// Rasterizer           : CULL_FRONT
// DepthStencil State   : No_Test_No_Write
// Blend State          : Default

// Parameter
#define PositionTargetTex  g_tex_0
#define OutputTex          g_tex_1
#define IsOutputTex        g_btex_1
// ================================
struct VS_DECAL_IN
{
    float3 vPos : POSITION;    
    float2 vUV  : TEXCOORD;
};

struct VS_DECAL_OUT
{
    float4 vPosition : SV_Position;
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_DECAL_OUT VS_Decal(VS_DECAL_IN _in)
{
    VS_DECAL_OUT output = (VS_DECAL_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Decal(VS_DECAL_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // 데칼오브젝트 영역에 해당하는 Position 값을 가져온다.
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float3 vViewPos = PositionTargetTex.Sample(g_sam_0, vScreenUV).xyz;    
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;    
    float3 vLocal = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    if (abs(vLocal.x) < 0.5f && abs(vLocal.y) < 0.5f && abs(vLocal.z) < 0.5f)
    {
        // Decal 내부에 있는 좌표        
        //vOutColor = float4(1.f, 0.f, 0.f, 1.f);        
        float2 vUV = vLocal.xz;
        vUV.x = vUV.x + 0.5f;
        vUV.y = (1.f - vUV.y) - 0.5f; 
        
        if (IsOutputTex)
        {
            vOutColor = OutputTex.Sample(g_sam_0, vUV);
        }
        else
        {
            discard;
        }         
    }
    else
    {
        // Deacl 외부에 있는 좌표        
        discard;        
    }
    
    return vOutColor;
}



// ================================
// Deferred DecalShader
// MRT                  : Deferred Decal (ColorTarget, EmissiveTarget)
// DOMAIN               : DOMAIN_DECAL
// Mesh                 : CubeMesh
// Rasterizer           : CULL_FRONT
// DepthStencil State   : No_Test_No_Write
// Blend State          : ALPHA_BLEND

// Parameter
#define PositionTargetTex  g_tex_0
#define OutputTex          g_tex_1
#define IsOutputTex        g_btex_1
#define IsEmissive         g_int_0
// ================================
VS_DECAL_OUT VS_DeferredDecal(VS_DECAL_IN _in)
{
    VS_DECAL_OUT output = (VS_DECAL_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;
    
    return output;
}

struct PS_DECAL_OUT
{
    float4 vColor : SV_Target0;
    float4 vEmissive : SV_Target1;
};

PS_DECAL_OUT PS_DeferredDecal(VS_DECAL_OUT _in) : SV_Target
{
    PS_DECAL_OUT output = (PS_DECAL_OUT) 0.f;
    
    // 데칼오브젝트 영역에 해당하는 Position 값을 가져온다.
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float3 vViewPos = PositionTargetTex.Sample(g_sam_0, vScreenUV).xyz;
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocal = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    if (abs(vLocal.x) < 0.5f && abs(vLocal.y) < 0.5f && abs(vLocal.z) < 0.5f)
    {
        // Decal 내부에 있는 좌표       
        float2 vUV = vLocal.xz;
        vUV.x = vUV.x + 0.5f;
        vUV.y = (1.f - vUV.y) - 0.5f;
        
        if (IsOutputTex)
        {
            output.vColor = OutputTex.Sample(g_sam_0, vUV);
            
            if (IsEmissive)
            {
                output.vEmissive = output.vColor * output.vColor.a;
            }
        }
        else
        {
            discard;
        }
    }
    else
    {
        // Deacl 외부에 있는 좌표        
        discard;
    }
    
    return output;
}




#endif
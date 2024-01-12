#ifndef _DEBUGSHAPE
#define _DEBUGSHAPE

#include "value.fx"

// =================
// DebugShape Shader
// Topology : LineStrip
// RS_TYPE  : CULL_NONE
// DS_TYPE  : NO_TEST_NO_WRITE
// BS_TYPE  : Default

// Parameter
// g_vec4_0 : OutColor
// ==================
struct VS_DEBUG_IN
{
    float3 vPos : POSITION;    
    float3 vNormal : NORMAL;
};

struct VS_DEBUG_OUT
{
    float4 vPosition : SV_Position;
    
    float3 vViewPos : Position;
    float3 vViewNormal : NORMAL;
};

VS_DEBUG_OUT VS_DebugShape(VS_DEBUG_IN _in)
{
    VS_DEBUG_OUT output = (VS_DEBUG_OUT) 0.f;    
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    
    return output;    
}

float4 PS_DebugShape(VS_DEBUG_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    
    return vOutColor;
}

float4 PS_DebugShape_OutLine(VS_DEBUG_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float3 vEye = -normalize(_in.vViewPos);
    float fOutLine = 1.f - saturate(abs(dot(vEye, _in.vViewNormal)));
    fOutLine = pow(fOutLine, 2.5);
    
    vOutColor = g_vec4_0;
    vOutColor.a = fOutLine;
    
    return vOutColor;
}

float4 PS_DebugShape_Frustum(VS_DEBUG_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    
    return vOutColor;
}

#endif

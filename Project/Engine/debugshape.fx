#ifndef _DEBUGSHAPE
#define _DEBUGSHAPE

#include "value.fx"

// =================
// DebugShape Shader
// Topology : LineStrip
// RS_TYPE  : CULL_NONE
// DS_TYPE  : NO_TEST_NO_WRITE
// BS_TYPE  : Default
// g_vec4_0 : OutColor
// ==================
struct VS_DEBUG_IN
{
    float3 vPos : POSITION;    
};

struct VS_DEBUG_OUT
{
    float4 vPosition : SV_Position;
};

VS_DEBUG_OUT VS_DebugShape(VS_DEBUG_IN _in)
{
    VS_DEBUG_OUT output = (VS_DEBUG_OUT) 0.f;    
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    return output;    
}

float4 PS_DebugShape(VS_DEBUG_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    
    return vOutColor;
}







#endif

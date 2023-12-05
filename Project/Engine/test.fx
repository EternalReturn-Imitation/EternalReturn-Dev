#ifndef _TEST
#define _TEST

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_TestShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TestShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    if(g_int_0 == 0)
    {
        vOutColor = float4(0.f, 1.f, 0.f, 1.f);
    }
        
    else if(g_int_0 == 1)
    {
        vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    }
     
    else if (g_int_0 == 2)
    {
        vOutColor = float4(0.f, 0.f, 1.f, 1.f);
    }
        
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }    
            
    return vOutColor;
}








#endif
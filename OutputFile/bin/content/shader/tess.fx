#ifndef _TESS
#define _TESS

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


VS_IN VS_Tess(VS_IN _in)
{
    VS_IN output = (VS_IN) 0.f;
    
    output = _in;
    
    return output;
}


struct PatchInfo
{
    float arrEdgeLevel[3] : SV_TessFactor;
    float insideLevel : SV_InsideTessFactor;
};

// 패치 상수 함수(Patch Constant Function)
PatchInfo PatchFunc(InputPatch<VS_IN, 3> _patch, int _patchIdx : SV_PrimitiveID)
{
    PatchInfo output = (PatchInfo) 0.f;
 
    output.arrEdgeLevel[0] = 16.f;
    output.arrEdgeLevel[1] = 16.f;
    output.arrEdgeLevel[2] = 16.f;
    output.insideLevel = 16.f;
    
    return output;
}


// Hull Shader
struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
[outputtopology("triangle_cw")]
[patchconstantfunc("PatchFunc")]
[partitioning("integer")]
[outputcontrolpoints(3)]
[maxtessfactor(64)]
HS_OUT HS_Tess(InputPatch<VS_IN, 3> _patch
                , int _idx : SV_OutputControlPointID
                , int _patchIdx : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.f;
        
    output.vPos = _patch[_idx].vPos;
    output.vUV = _patch[_idx].vUV;
    
    return output;
}



// Domain Shader
struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


[domain("tri")]
DS_OUT DS_Tess(PatchInfo patchlevel
             , OutputPatch<HS_OUT, 3> _patch, float3 _location : SV_DomainLocation)
{
    DS_OUT output = (DS_OUT) 0.f;
      
    
    float3 vLocalPos = _patch[0].vPos * _location[0]
                        + _patch[1].vPos * _location[1]
                        + _patch[2].vPos * _location[2];
    
    
    float2 vUV = _patch[0].vUV * _location[0]
                        + _patch[1].vUV * _location[1]
                        + _patch[2].vUV * _location[2];
    
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    
    
    return output;
}


float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    return vColor;
}







#endif
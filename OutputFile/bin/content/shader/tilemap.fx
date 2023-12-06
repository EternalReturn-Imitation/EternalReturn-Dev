#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "func.fx"


struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

// ============================
// TileMap Shader
// 
// RS_TYPE : CULL_NONE
// DS_TYPE : LESS
// BS_TYPE : MASK

// Parameter
// g_int_0 : Tile X Count
// g_int_1 : Tile Y Count
// g_tex_0 : Tile Atlas Texture
StructuredBuffer<tTile> TileBuffer : register(t20);
// ============================
struct VTX_TILEMAP_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_TILEMAP_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_TILEMAP_OUT VS_TileMap(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV * float2(g_int_0, g_int_1); // 전체 UV 를 타일 개수만큼 확장 
    
    return output;
}

float4 PS_TileMap(VTX_TILEMAP_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // 소수파트, frac(_in.vUV) : 타일 한칸 내에서 픽셀의 상대적인 위치 (0 ~ 1)
    // 정수파트, floor(_in.vUV): 전체 타일 중에서 인덱스(행, 열)    
    int2 TileIdx = floor(_in.vUV);
    int BufferIdx = g_int_0 * TileIdx.y + TileIdx.x;
    float2 vUV = TileBuffer[BufferIdx].vLeftTop + (TileBuffer[BufferIdx].vSlice * frac(_in.vUV));    
    
    if(g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    }
    
    return vOutColor;
}

#endif


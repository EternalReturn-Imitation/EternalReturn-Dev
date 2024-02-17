#ifndef _RAYCAST
#define _RAYCAST

#include "value.fx"
#include "func.fx"

RWStructuredBuffer<tRaycastOut> OUTPUT : register(u0);

// Laycast Vertex Pos Buffer
StructuredBuffer<float4> g_arrVtx : register(t25);

Texture2D g_naviTex : register(t1);

#define CAM_POS         g_vec4_0
#define CAM_DIR         g_vec4_1

#define FACE_X          g_int_0
#define FACE_Z          g_int_1

#define MAX_X_UV        g_float_0
#define MAX_Z_UV        g_float_1

[numthreads(32, 32, 1)]
void CS_Raycast(int3 _iThreadID : SV_DispatchThreadID)
{
    int2 id = _iThreadID.xy;

    float3 vPos[3] = { (float3) 0.f, (float3) 0.f, (float3) 0.f };

    if (0 == id.x % 2)
    {
        // ¾Æ·¡ÂÊ »ï°¢Çü        
        // 2
        // | \
        // 0--1        
        vPos[0].x = id.x / 2;
        vPos[0].z = id.y;

        vPos[1].x = vPos[0].x + 1;
        vPos[1].z = id.y;

        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y + 1;
    }
    else
    {
        // À­ÂÊ »ï°¢Çü
        // 1--0
        //  \ |
        //    2  
        vPos[0].x = (id.x / 2) + 1;
        vPos[0].z = id.y + 1;

        vPos[1].x = vPos[0].x - 1;
        vPos[1].z = id.y + 1;

        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y;
    }
    
    vPos[0].xyz *= 10;
    vPos[1].xyz *= 10;
    vPos[2].xyz *= 10;

    float3 vCrossPoint = (float3) 0.f;
    float fDist = 0.f;

    if (IntersectsLay(vPos, CAM_POS.xyz, CAM_DIR.xyz, vCrossPoint, fDist))
    {
        //OUTPUT[0].vUV = float2(saturate(vCrossPoint.x / (float) FACE_X), saturate(vCrossPoint.z / (float) FACE_Z));
        if ((vCrossPoint.x / (float) FACE_X) < 0.f)
            vCrossPoint.x = 0.f;
        if ((vCrossPoint.z / (float) FACE_Z) < 0.f)
            vCrossPoint.z = 0.f;       
        if ((vCrossPoint.x / (float) FACE_X) > MAX_X_UV)
            vCrossPoint.x = MAX_X_UV * (float) FACE_X;
        if ((vCrossPoint.z / (float) FACE_Z) > MAX_Z_UV)
            vCrossPoint.z = MAX_Z_UV * (float) FACE_Z;        

        OUTPUT[0].vUV = float2(vCrossPoint.x / (float) FACE_X, vCrossPoint.z / (float) FACE_Z);
        OUTPUT[0].fDist = fDist;
        OUTPUT[0].success = 1;
    }
    
    if (OUTPUT[0].success)
    {
        float2 vUV = float2(vCrossPoint.x / (float) FACE_X, MAX_Z_UV - vCrossPoint.z / (float) FACE_Z);
        vUV.y = vUV.y / (float) MAX_Z_UV;
        
        float4 color = g_naviTex.SampleLevel(g_sam_0, vUV,0);
        OUTPUT[0].vRGB = color;
        if (color.a == 0.f)
            OUTPUT[0].success = 0;
    }
        
}

#endif
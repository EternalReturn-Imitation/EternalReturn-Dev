#ifndef _RAYCAST
#define _RAYCAST

#include "value.fx"
#include "func.fx"

RWStructuredBuffer<tRaycastOut> OUTPUT : register(u0);

// Laycast Vertex Pos Buffer
StructuredBuffer<float4> g_arrVtx : register(t25);

#define HEIGHT_MAP      g_tex_0

#define CAM_POS         g_vec4_0
#define CAM_DIR         g_vec4_1

#define FACE_X          g_int_0
#define FACE_Z          g_int_1
#define BUFFER_LEN      g_int_2

#define MIN_X            g_float_0
#define MAX_X            g_float_1
#define MIN_Z            g_float_2
#define MAX_Z            g_float_3

[numthreads(1024, 1, 1)]
void CS_Raycast(int3 _iThreadID : SV_DispatchThreadID)
{
    int2 id = _iThreadID.xy;
    
    int whileNum1 = BUFFER_LEN / 1024;
    whileNum1 += 3;
    if (whileNum1 % 3 == 0)
    {
        int whileNum2 = BUFFER_LEN % whileNum1;
    }
    else
    {
        //3단위로 안나뉘어질경우 예외처리 해주기.
        //지금은 3단위로 나뉘어 떨어져서 내버려둠
    }
    
    //쓰레드가 담당하는 범위를 넘어가면 그냥 return;
    if (id.x * whileNum1 > BUFFER_LEN)
    {
        return;
    }
    
    for (int i = (id.x * whileNum1); i < (whileNum1 * (id.x + 1)); i += 3)
    {
        float3 vPos[3] = { (float3) 0.f, (float3) 0.f, (float3) 0.f };
    
        vPos[0].x = g_arrVtx[i].x;
        vPos[0].z = g_arrVtx[i].y;
    
        vPos[1].x = g_arrVtx[i + 1].x;
        vPos[1].z = g_arrVtx[i + 1].y;
    
        vPos[2].x = g_arrVtx[i + 2].x;
        vPos[2].z = g_arrVtx[i + 2].y;

        float3 vCrossPoint = (float3) 0.f;
        float fDist = 0.f;
    
        if (IntersectsLay(vPos, CAM_POS.xyz, CAM_DIR.xyz, vCrossPoint, fDist))
        {
            //OUTPUT[0].vUV = float2(saturate(vCrossPoint.x / (float) FACE_X), saturate(1.f - vCrossPoint.z / (float) FACE_Z));
            OUTPUT[0].vUV = float2(vCrossPoint.x, vCrossPoint.z);
            OUTPUT[0].vUV.x = vPos[0].x;
            OUTPUT[0].vUV.y = vPos[0].z;
            OUTPUT[0].fDist = fDist;
            OUTPUT[0].success = 1;
        }
    }
}

#endif
#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"



// ComputeShader 입력 Sematic
// SV_GroupThreadID      : 그룹 내에서의 인덱스(3차원)
// SV_GroupID            : 그룹 인덱스
// SV_GroupIndex         : 그룹 내에서의 인덱스(1차원)
// SV_DispatchThreadID   : 전체 그룹 기준, 스레드의 인덱스(3차원)

RWTexture2D<float4> g_Output : register(u0);
#define Color       g_vec4_0 
#define Resolution  g_vec2_0

// 스레드 그룹 개수(HLSL 5.0 기준 1024 제한)
[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{     
    if (Resolution.x <= _ID.x || Resolution.y <= _ID.y)
    {
        return;
    }    
    
    g_Output[_ID.xy] = float4(Color.xyz, 1.f);
}



#endif
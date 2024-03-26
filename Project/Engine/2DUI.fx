#ifndef _2DUI
#define _2DUI

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

VS_OUT VS_2DUI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}

#define BasicTex    g_tex_0

float4 PS_2DUI(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    }

    return vOutColor;
}


// [ CoolDown ]
#define Texture1        g_tex_0
#define Texture2        g_tex_1
#define Texture3        g_tex_2
#define Texture4        g_tex_3
#define CoolDownTex     g_tex_4
#define TexIdx          g_int_0
#define CoolDownRatio   g_float_0

float4 PS_2DUI_CoolDown(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    
    if (g_int_0 == 0 && g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 1 && g_btex_1)
    {
        vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 2 && g_btex_2)
    {
        vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 3 && g_btex_3)
    {
        vOutColor = g_tex_3.Sample(g_sam_0, _in.vUV);
    }
    
    
    if (g_btex_4)
    {
        // ratio 알파값
        float4 CooldownColor = g_tex_4.Sample(g_sam_0, _in.vUV);
        CooldownColor.w = g_float_0;
        
        vOutColor = lerp(CooldownColor, vOutColor, CooldownColor.a);
    }
    
    return vOutColor;
}

// [ GUAGEUI ]
#define MainTex     g_tex_0
#define ReturnTex   g_tex_1
#define MainTexEfc  g_tex_2

#define MainRatio   g_float_0
#define ReturnRatio g_float_1


float4 PS_2DUI_GUAGE(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // 리턴값 먼저 검사
    
    // return bar
    // 리턴값 비율이 UV.x 보다 좌측이면 색상 갱신
    if (g_btex_1 && ReturnRatio >= _in.vUV.x)
        vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    // main Gauge
    // 메인값 비율이 UV.x 보다 좌측이면 색상 갱신
    if (g_btex_0 && MainRatio >= _in.vUV.x)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    
    // MainTexEtc
    if (g_btex_2)
    {
        // time to Move X->
        float speed = 0.3f;
        float offsetX = g_AccTime * speed;
        float2 efcUV = float2(_in.vUV.x * 4.0 + offsetX, _in.vUV.y);
            
        float4 gauge = vOutColor;
        float4 efcColor = g_tex_2.Sample(g_sam_0, efcUV);
        
        // 블렌딩 계산
        float4 blendedColor = (1 - efcColor.w) * gauge + efcColor * efcColor.w;
        
        vOutColor = blendedColor;
    }
    
    return vOutColor;
}

// [ Indicator ]
#define Texture0    g_tex_0
#define Texture1    g_tex_1
#define Texture2    g_tex_2
#define Texture3    g_tex_3
#define Texture4    g_tex_4
#define Texture5    g_tex_5
#define Texture6    g_tex_6
#define Texture7    g_tex_7

#define TextureType     g_int_0


float4 PS_2DUI_Indicator(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_int_0 == 0 && g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 1 && g_btex_1)
    {
        vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 2 && g_btex_2)
    {
        vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 3 && g_btex_3)
    {
        vOutColor = g_tex_3.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 4 && g_btex_4)
    {
        vOutColor = g_tex_4.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 5 && g_btex_5)
    {
        vOutColor = g_tex_5.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 6 && g_btex_6)
    {
        vOutColor = g_tex_6.Sample(g_sam_0, _in.vUV);
    }
    else if (g_int_0 == 7 && g_btex_7)
    {
        vOutColor = g_tex_7.Sample(g_sam_0, _in.vUV);
    }
        
    return vOutColor;
}

// [ Item Slot ]
#define ItemTexture    g_tex_0
#define NORMAL         g_tex_1
#define UNCOMMON       g_tex_2
#define RARE           g_tex_3
#define EPIC           g_tex_4
#define Empty          g_tex_5
#define EquipType      g_tex_6
#define IsNotEmpty     g_int_0
#define ItemGrade      g_int_1


float4 PS_2DUI_ItemSlot(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    
    
    if (IsNotEmpty && g_btex_0)
    {
        if (ItemGrade == 0 && g_btex_1)
        {
            vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV);
        }
        else if (ItemGrade == 1 && g_btex_2)
        {
            vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV);
        }
        else if (ItemGrade == 2 && g_btex_3)
        {
            vOutColor = g_tex_3.Sample(g_sam_0, _in.vUV);
        }
        else if (ItemGrade == 3 && g_btex_4)
        {
            vOutColor = g_tex_4.Sample(g_sam_0, _in.vUV);
        }
        
        float2 ItemUV = _in.vUV;
        ItemUV.x *= (3.f / 2.f);
        ItemUV.x -= 0.25f;
        ItemUV.x = saturate(ItemUV.x);
        
        float4 ItemColor = g_tex_0.Sample(g_sam_0, ItemUV);
        
        // 블렌딩 계산
        vOutColor = (1 - ItemColor.w) * vOutColor + ItemColor * ItemColor.w;
    }
    else if (g_btex_5)
    {
        vOutColor = g_tex_5.Sample(g_sam_0, _in.vUV);
        
        if(g_btex_6)
        {
            float2 ItemUV = _in.vUV;
            ItemUV.x *= (2.f / 1.f);
            ItemUV.x -= 0.5f;
            ItemUV.x = saturate(ItemUV.x);
            
            ItemUV.y *= (4.f / 3.f);
            ItemUV.y -= 0.15f;
            ItemUV.y = saturate(ItemUV.y);
            
            float4 EquipTypeColor = g_tex_6.Sample(g_sam_0, ItemUV);
            vOutColor = (1 - EquipTypeColor.w) * vOutColor + EquipTypeColor * EquipTypeColor * EquipTypeColor.w;
        }
    }
    
    
    return vOutColor;
}

#endif
#ifndef _SOCKETCOLLISION
#define _SOCKETCOLLISION

#include "value.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_SocketCollision(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    //output.vUV = _in.vUV;

    return output;
}


float4 PS_SocketCollision(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;


    vOutColor = float4 (0.f, 0.f, 1.f, 1.f);

    return vOutColor;
}







#endif

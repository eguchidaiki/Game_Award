#include "Sprite.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv) * input.color;
	
    //float bright = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
	
    //float3 sepia;
	
    //sepia.r = input.freedata.r;
    //sepia.g = input.freedata.g;
    //sepia.b = input.freedata.b;
    
    //color.rgb *= sepia;
    
    return color;
}
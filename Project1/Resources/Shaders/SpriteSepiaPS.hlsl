#include "Sprite.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv) * input.color;
	
    //float bright = (color.r + color.g + color.b) * 0.33f;
	
    //float3 sepia;
	
    //sepia.r = bright * input.freedata.r;
    //sepia.g = bright * input.freedata.g;
    //sepia.b = bright * input.freedata.b;
    
    //color.rgb = sepia;
    
    return color;
}
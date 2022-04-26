#include "Sprite.hlsli"

//スプライトの入力は1頂点のみ
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD,matrix instanceMatrix : INSTANCE_WORLD_MAT,float2 size : INSTANCE_DRAWSIZE,float4 uvOffset : INSTANCE_UVOFFSET)
{
	VSOutput output;
    output.svpos = pos;
	output.uv = uv;
    output.ins_matrix = instanceMatrix;
    output.size = size;
    output.uvOffset = uvOffset;
	return output;
}
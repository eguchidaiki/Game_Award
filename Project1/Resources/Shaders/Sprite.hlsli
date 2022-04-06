Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;	//頂点座標
	float2 uv    : TEXCOORD;	//UV座標
	
    matrix ins_matrix : INSTANCE_WORLD_MAT;	//インスタンシング用変換行列
};

struct GSOutput
{
    float4 pos : SV_POSITION;
	
};
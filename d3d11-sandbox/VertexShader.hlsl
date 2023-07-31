struct VS_OUT
{
	float3 color: Color;
	float4 pos: SV_Position;
};

cbuffer CBuf
{
	matrix transform;
};

VS_OUT main( float3 pos : Position, float4 color: Color )
{
	VS_OUT vso;
	vso.pos = mul(float4(pos, 1.0f), transform);
	vso.color = color;
	return vso;
}
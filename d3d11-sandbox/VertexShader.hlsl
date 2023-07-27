struct VS_OUT
{
	float3 color: Color;
	float4 pos: SV_Position;
};

VS_OUT main( float2 pos : Position, float4 color: Color )
{
	VS_OUT vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.color = color;
	return vso;
}
cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VS_OUT
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VS_OUT main( float3 pos : Position, float3 n : Normal )
{
    VS_OUT vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.normal = mul(n, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;

}
Texture2D tex;
Texture2D spec;
Texture2D nmap;
SamplerState samplr;

cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCbuf
{
    bool normalMapEnabled;
    float padding[3];
};


float4 main(float3 worldPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 texCoord : Texcoord) : SV_TARGET
{
    // sample from normal map if enabled
    if (normalMapEnabled)
    {
        // build matrix to transform (rotate) from tangent space
        const float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),\
            normalize(n)
        );
        
        // unpack normal from normal map into tangent space
        const float3 normalSample = nmap.Sample(samplr, texCoord).xyz;
        n.x = normalSample.x * 2.f - 1.0f;
        n.y = -normalSample.y * 2.f + 1.0f;
        n.z = normalSample.z;
        
        // bring normal into view space using matrix
        n = mul(n, tanToView);
    }
    
    
    // fragment to light vector data 
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    // diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    
    // diffuse intensity 
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
    
    // reflected light color
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
    
    // calculate specular intensity based on angle between viewing vector and normal
    float4 specularSample = spec.Sample(samplr, texCoord);
    const float3 specularReflectionColor = specularSample.rgb;
    const float specularPower = pow(2.0f, specularSample.a * 13.f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
        
    // final color 
    return float4(saturate((diffuse + ambient) * tex.Sample(samplr, texCoord).rgb + specular * specularReflectionColor), 1.0f);
}
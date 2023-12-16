float4x4 gWorldViewProjection : WorldViewProjection;

Texture2D gDiffuseTexture : DiffuseTexture;

SamplerState gSamplerState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_Input
{
    float3
    position : POSITION,
    color : COLOR;
    
    float2 UV : TEXCOORD;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 color : COLOR;
    
    float2 UV : TEXCOORD;
};

VS_Output VS(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.position, 1.0f), gWorldViewProjection);
    output.color = input.color;
    
    output.UV = input.UV;
   
    return output;
}

float4 PS(VS_Output input) : SV_Target
{
    return gDiffuseTexture.Sample(gSamplerState, input.UV);
}

technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
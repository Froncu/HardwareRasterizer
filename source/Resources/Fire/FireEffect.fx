float4x4 g_WorldMatrix;
float4x4 g_ViewProjectionMatrix;

float3 g_LightDirection = float3(0.577f, -0.577f, 0.577f);
 
SamplerState g_SamplerState
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState g_RasterizerState
{
    CullMode = none;
    FrontCounterClockwise = false;
};

Texture2D g_DiffuseTexture;



struct VertexShaderInput
{
    float3 position : SV_POSITION;
    
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};



PixelShaderInput DefaultVertexShader(VertexShaderInput input)
{
    PixelShaderInput output;
    
    output.position = mul(float4(input.position, 1.0f), mul(g_WorldMatrix, g_ViewProjectionMatrix));
    
    output.UV = input.UV;
    output.normal = normalize(mul(input.normal, (float3x3) g_WorldMatrix));
    output.tangent = normalize(mul(input.tangent, (float3x3) g_WorldMatrix));
    
    return output;
}

float4 DefaultPixelShader(PixelShaderInput input) : SV_TARGET
{   
    return g_DiffuseTexture.Sample(g_SamplerState, input.UV);
}



technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetVertexShader(CompileShader(vs_5_0, DefaultVertexShader()));
        SetRasterizerState(g_RasterizerState);
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, DefaultPixelShader()));
    }
}
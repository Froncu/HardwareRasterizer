float4x4 g_WorldMatrix;
float4x4 g_ViewProjectionMatrix;

float3 g_LightDirection = float3(0.577f, -0.577f, 0.577f);
 
Texture2D g_DiffuseTexture;

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

BlendState g_BlendState
{
    BlendEnable[0] = true;
    SrcBlend = src_alpha;
    DestBlend = inv_src_alpha;
    BlendOp = add;
    SrcBlendAlpha = zero;
    DestBlendAlpha = zero;
    BlendOpAlpha = add;
    RenderTargetWriteMask[0] = 0x0F;
};



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
        SetRasterizerState(g_RasterizerState);
        SetBlendState(g_BlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, DefaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, DefaultPixelShader()));
    }
}
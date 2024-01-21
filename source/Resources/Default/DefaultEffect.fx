float4x4 g_WorldMatrix;
float4x4 g_ViewProjectionMatrix;

float3 g_LightDirection = float3(0.577f, -0.577f, 0.577f);
 
Texture2D g_DiffuseTexture;

SamplerState g_SamplerState
{

};

RasterizerState g_RasterizerState
{

};

DepthStencilState g_DepthStencilState
{

};

BlendState g_BlendState
{

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

float3 Lambert(float diffuseReflectance, float3 diffuseColor)
{
    return diffuseReflectance * diffuseColor / 3.14159265358979323846f;
}

float3 DefaultPixelShader(PixelShaderInput input) : SV_TARGET
{   
    float3 diffuseColor = g_DiffuseTexture.Sample(g_SamplerState, input.UV).xyz;
    float3 diffuse = Lambert(1.0f, diffuseColor);
    float negatedDotLightDirectionNormal = -dot(g_LightDirection, input.normal);
    
    return negatedDotLightDirectionNormal * diffuse;
}



technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(g_RasterizerState);
        SetDepthStencilState(g_DepthStencilState, 0);
        SetBlendState(g_BlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, DefaultVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, DefaultPixelShader()));
    }
}
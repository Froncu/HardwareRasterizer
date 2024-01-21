float4x4 g_WorldMatrix;
float4x4 g_ViewProjectionMatrix;

float3 g_CameraOrigin;
float3 g_LightDirection = float3(0.577f, -0.577f, 0.577f);

float g_DiffuseReflectance = 7.0f;
float g_Shininess = 25.0f;

Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_SpecularTexture;
Texture2D g_GlossTexture;
 
SamplerState g_PointSamplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState g_LinearSamplerState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState g_AnisotropicSamplerState
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState g_ActiveSamplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState g_RasterizerState
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
    float4 worldPosition : POSITION;
    
    float4 position : SV_POSITION;
    
    float2 UV : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};



PixelShaderInput MyVertexShader(VertexShaderInput input)
{
    PixelShaderInput output;
    
    output.worldPosition = mul(float4(input.position, 1.0f), g_WorldMatrix);
    
    output.position = mul(output.worldPosition, g_ViewProjectionMatrix);
    
    output.UV = input.UV;
    output.normal = normalize(mul(input.normal, (float3x3) g_WorldMatrix));
    output.tangent = normalize(mul(input.tangent, (float3x3) g_WorldMatrix));
    
    return output;
}

float3 GetSampledNormal(float2 UV, float3 normal, float3 tangent, Texture2D normalTexture)
{
    float3 sampledNormal = normalTexture.Sample(g_ActiveSamplerState, UV).xyz * 2.0f - 1.0f;
    float3 binormal = normalize(cross(normal, tangent));
    
    return normalize(mul(sampledNormal, float3x3(tangent, binormal, normal)));
}

float3 Lambert(float diffuseReflectance, float3 diffuseColor)
{
    return diffuseReflectance * diffuseColor / 3.14159265358979323846f;
}

float3 Phong(float3 specularReflectance, float phongExponent, float3 lightDirection, float3 viewDirection, float3 normal)
{
    float3 reflectedLightDirection = reflect(lightDirection, normal);
    float negatedDot = -dot(reflectedLightDirection, viewDirection);
    
    if (negatedDot <= 0.0f)
        return float3(0.0f, 0.0f, 0.0f);

    return specularReflectance * pow(negatedDot, phongExponent);
}

float3 MyPixelShader(PixelShaderInput input) : SV_TARGET
{
    float2 UV = input.UV;
    
    float3 specularReflectance = g_SpecularTexture.Sample(g_ActiveSamplerState, UV).xyz;
    float phongExponent = g_Shininess * g_GlossTexture.Sample(g_ActiveSamplerState, UV);
    float3 viewDirection = normalize(input.worldPosition.xyz - g_CameraOrigin);
    float3 normal = GetSampledNormal(UV, input.normal, input.tangent, g_NormalTexture);
    
    float3 specular = Phong(specularReflectance, phongExponent, g_LightDirection, viewDirection, normal);

    float3 diffuse = Lambert(g_DiffuseReflectance, g_DiffuseTexture.Sample(g_ActiveSamplerState, UV).xyz);
    
    float dotLightDirectionNormal = max(dot(-g_LightDirection, normal), 0.0f);
    
    return dotLightDirectionNormal * (specular + diffuse);
}



technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(g_RasterizerState);
        SetBlendState(g_BlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, MyVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, MyPixelShader()));
    }
}
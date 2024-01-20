float4x4
    g_WorldMatrix,
    g_ViewProjectionMatrix;

float3
    g_CameraPosition,
    g_LightDirection = float3(0.577f, -0.577f, 0.577f);

float
    g_DiffuseReflectance = 7.0f,
    g_Shininess = 25.0f;

Texture2D
    g_DiffuseTexture,
    g_NormalTexture,
    g_SpecularTexture,
    g_GlossinessTexture;

SamplerState 
    g_PointSamplerState{ Filter = MIN_MAG_MIP_POINT; AddressU = Wrap; AddressV = Wrap; },  
    g_LinearSamplerState{ Filter = MIN_MAG_MIP_LINEAR; AddressU = Wrap; AddressV = Wrap; },
    g_AnisotropicSamplerState{ Filter = ANISOTROPIC; AddressU = Wrap; AddressV = Wrap; },
    g_ActiveSamplerState{ Filter = MIN_MAG_MIP_POINT; AddressU = Wrap; AddressV = Wrap; };



struct VS_Input
{
    float3
        position : SV_Position,
        color : COLOR;
    
    float2 UV : TEXCOORD;
    float3
        normal : NORMAL,
        tangent : TANGENT;
};

struct VS_Output
{
    float4
        worldPosition : POSITION,
        position : SV_Position;
    
    float3 color : COLOR;
    
    float2 UV : TEXCOORD;
    float3
        normal : NORMAL,
        tangent : TANGENT;
};



VS_Output VS(VS_Input input)
{
    VS_Output output;
    
    output.worldPosition = mul(float4(input.position, 1.0f), g_WorldMatrix);
    output.position = mul(output.worldPosition, g_ViewProjectionMatrix);
    
    output.normal = normalize(mul(input.normal, (float3x3) g_WorldMatrix));
    output.tangent = normalize(mul(input.tangent, (float3x3) g_WorldMatrix));
    
    output.color = input.color;
    output.UV = input.UV;
    
    return output;
}

float3 GetSampledNormal(float2 UV, float3 normal, float3 tangent, Texture2D normalTexture)
{
    float3
        sampledNormal = normalTexture.Sample(g_ActiveSamplerState, UV).xyz * 2.0f - float3(1.0f, 1.0f, 1.0f),
        binormal = normalize(cross(normal, tangent));
    
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

float3 PS(VS_Output input) : SV_Target
{
    float2 UV = input.UV;
    
    float phongExponent = g_Shininess * g_GlossinessTexture.Sample(g_ActiveSamplerState, UV);
    float3
        specularReflectance = g_SpecularTexture.Sample(g_ActiveSamplerState, UV).xyz,
        normal = GetSampledNormal(UV, input.normal, input.tangent, g_NormalTexture),
        viewDirection = normalize(input.worldPosition.xyz - g_CameraPosition),
    
        specular = Phong(specularReflectance, phongExponent, g_LightDirection, viewDirection, normal),
        diffuse = Lambert(g_DiffuseReflectance, g_DiffuseTexture.Sample(g_ActiveSamplerState, UV).xyz);
    
    float dotLightDirectionNormal = max(dot(-g_LightDirection, normal), 0.0f);
    
    return dotLightDirectionNormal * (specular + diffuse);
}



technique11
{
    pass
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
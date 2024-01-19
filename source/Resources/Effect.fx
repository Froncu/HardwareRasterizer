float4x4 g_WorldViewProjection : WorldViewProjection;

Texture2D g_DiffuseTexture : DiffuseTexture;

SamplerState 
    g_PointSamplerState{ Filter = MIN_MAG_MIP_POINT; AddressU = Wrap; AddressV = Wrap; },  
    g_LinearSamplerState{ Filter = MIN_MAG_MIP_LINEAR; AddressU = Wrap; AddressV = Wrap; },
    g_AnisotropicSamplerState{ Filter = ANISOTROPIC; AddressU = Wrap; AddressV = Wrap; },

    g_ActiveSamplerState{ Filter = MIN_MAG_MIP_POINT; AddressU = Wrap; AddressV = Wrap;  };

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
    float4 position : SV_Position;
    float3 color : COLOR;
    
    float2 UV : TEXCOORD;
    float3
        normal : NORMAL,
        tangent : TANGENT;
};

VS_Output VS(VS_Input input)
{
    VS_Output output;
    
    output.position = mul(float4(input.position, 1.0f), g_WorldViewProjection);
    output.color = input.color;
    
    output.UV = input.UV;
    output.normal = input.normal;
    output.tangent = input.tangent;
    
    return output;
}

float4 PS(VS_Output input) : SV_Target
{
    return g_DiffuseTexture.Sample(g_ActiveSamplerState, input.UV);
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
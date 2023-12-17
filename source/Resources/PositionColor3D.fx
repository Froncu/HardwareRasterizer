float4x4 gWorldViewProjection : WorldViewProjection;

Texture2D gDiffuseTexture : DiffuseTexture;

SamplerState 
    gPointSamplerState{ Filter = MIN_MAG_MIP_POINT; AddressU = Wrap; AddressV = Wrap; },  
    gLinearSamplerState{ Filter = MIN_MAG_MIP_LINEAR; AddressU = Wrap; AddressV = Wrap; },
    gAnisotropicSamplerState{ Filter = ANISOTROPIC; AddressU = Wrap; AddressV = Wrap; };

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

float4 PS_Point(VS_Output input) : SV_Target
{
    return gDiffuseTexture.Sample(gPointSamplerState, input.UV);
}

float4 PS_Linear(VS_Output input) : SV_Target
{
    return gDiffuseTexture.Sample(gLinearSamplerState, input.UV);
}

float4 PS_Anisotropic(VS_Output input) : SV_Target
{
    return gDiffuseTexture.Sample(gAnisotropicSamplerState, input.UV);
}

technique11 PointTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Point()));
    }
}

technique11 LinearTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
    }
}

technique11 AnisotropicTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Anisotropic()));
    }
}
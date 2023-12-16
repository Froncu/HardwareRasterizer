float4x4 gWorldViewProjection : WorldViewProjection;

struct VS_Input
{
    float3
    position : POSITION,
    color : COLOR;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 color : COLOR;
};

VS_Output VS(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.position, 1.0f), gWorldViewProjection);
    output.color = input.color;
    
    return output;
}

float4 PS(VS_Output input) : SV_Target
{
    return float4(input.color, 1.0f);
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
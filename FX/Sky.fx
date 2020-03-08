cbuffer cbPerFrame
{
	float4x4 gWorldViewProj;
};
 
// Danych nienumerycznych nie mo?na zapisywa? w obiekcie cbuffer.
TextureCube gCubeMap;

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};
 
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Ustaw z = w, aby z/w = 1 (czyli kopu?a nieba znajduje si? zawsze w dalszej p?aszczy?nie).
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;
	
	// U?yj lokalnej pozycji wierzcho?ka jako wektora wyszukiwania w teksturze sze?ciennej.
	vout.PosL = vin.PosL;
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return gCubeMap.Sample(samTriLinearSam, pin.PosL);
}

RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// W funkcji g??boko?ci nale?y u?y? nier?wno?ci s?abej (LESS_EQUAL) zamiast ostrej (LESS)  
	// W przeciwnym razie znormalizowane warto?ci g??boko?ci przy z = 1 (NDC) 
	// nie przejd? testu g??boko?ci, je?li bufor zostanie wype?niony jedynkami.

DepthFunc = LESS_EQUAL;
};

technique11 SkyTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
        
        SetRasterizerState(NoCull);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}
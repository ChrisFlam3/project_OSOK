struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{ 
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

//---------------------------------------------------------------------------------------
// Oblicza sk³adniki œwiat³a otoczenia, rozproszonego i odbitego we wzorze na oœwietlenie
// na podstawie œwiat³a kierunkowego. Sk³adniki zwracamy oddzielnie, poniewa¿ póŸniej
// bêd¹ pojedynczo modyfikowane.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L, 
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
	// Inicjalizuj argumenty wyjœciowe.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Wektor œwiat³a jest zwrócony w kierunku przeciwnym do kierunku padania promieni.
	float3 lightVec = -L.Direction;

	// Dodaj sk³adnik œwiat³a otoczenia.
	ambient = mat.Ambient * L.Ambient;	

	// Dodaj sk³adniki œwiat³a rozproszonego i odbitego pod warunkiem, 
	// ¿e pada ono na powierzchniê.
	
	float diffuseFactor = dot(lightVec, normal);

	// Sp³aszcz, aby unikn¹æ dynamicznego rozga³êziania.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// Oblicza sk³adniki œwiat³a otoczenia, rozproszonego i odbitego we wzorze na oœwietlenie
// na podstawie œwiat³a punktowego. Sk³adniki zwracamy oddzielnie, poniewa¿ póŸniej
// bêd¹ pojedynczo modyfikowane.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Inicjalizuj argumenty wyjœciowe.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Wektor z powierzchni do Ÿród³a.
	float3 lightVec = L.Position - pos;
		
	// Odleg³oœæ miêdzy powierzchni¹ a Ÿród³em.
	float d = length(lightVec);
	
	// Test zasiêgu.
	if( d > L.Range )
		return;
		
	// Normalizuj wektor œwiat³a.
	lightVec /= d; 
	
	// Sk³adnik œwiat³a otoczenia.
	ambient = mat.Ambient * L.Ambient;	

	// Dodaj sk³adniki œwiat³a rozproszonego i odbitego pod warunkiem, 
	// ¿e pada ono na powierzchniê.

	float diffuseFactor = dot(lightVec, normal);

	// Sp³aszcz, aby unikn¹æ dynamicznego rozga³êziania.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}

	// Wygaszanie
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec    *= att;
}

//---------------------------------------------------------------------------------------
// Oblicza sk³adniki œwiat³a otoczenia, rozproszonego i odbitego we wzorze na oœwietlenie
// na podstawie œwiat³a reflektorowego. Sk³adniki zwracamy oddzielnie, poniewa¿ póŸniej
// bêd¹ pojedynczo modyfikowane.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Inicjalizuj argumenty wyjœciowe.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Wektor z powierzchni do Ÿród³a.
	float3 lightVec = L.Position - pos;
		
	// Odleg³oœæ miêdzy powierzchni¹ a Ÿród³em.
	float d = length(lightVec);
	
	// Test zasiêgu.
	if( d > L.Range )
		return;
		
	// Normalizuj wektor œwiat³a.
	lightVec /= d; 
	
	// Sk³adnik œwiat³a otoczenia.
	ambient = mat.Ambient * L.Ambient;	

	// Dodaj sk³adniki œwiat³a rozproszonego i odbitego pod warunkiem, 
	// ¿e pada ono na powierzchniê.

	float diffuseFactor = dot(lightVec, normal);

	// Sp³aszcz, aby unikn¹æ dynamicznego rozga³êziania.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
	
	// Skalowanie za pomoc¹ wspó³czynnika œwiat³a reflektorowego i wygaszanie
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Skalowanie za pomoc¹ wspó³czynnika œwiat³a reflektorowego i wygaszanie
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec    *= att;
}

 float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Dekompresuj ka¿d¹ sk³adow¹ z [0,1] do [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	// Buduj ortonormaln¹ bazê.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Przekszta³æ z przestrzeni stycznej do przestrzeni œwiata.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}
 
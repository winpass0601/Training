cbuffer cbTessParam : register( b0 )
{
	float4	g_TessFactor;
	float2	g_InsideFactor;
	uint	g_Mode;
};
// VertexShaderInput(ì¸Åj
struct VSIN{
	float3 pos     : POSITION;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
// VertexShaderOutput(èo)
struct VSOUT{
	float4 pos     : SV_POSITION;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};

struct TriConstant
{
	float	tessFactor[3]	: SV_TessFactor;
	float	insideFactor	: SV_InsideTessFactor;

	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
// Hull Constant Shader
TriConstant RenderTriConstantHS( InputPatch<VSOUT, 3> iPatch)
{
	TriConstant outConst;

	float3 retf;
	float  ritf, uitf;
	switch (g_Mode)
	{
	case 0:
	default:
		ProcessTriTessFactorsAvg( g_TessFactor.xyz, g_InsideFactor.x, retf, ritf, uitf );
		break;
	case 1:
		ProcessTriTessFactorsMax( g_TessFactor.xyz, g_InsideFactor.x, retf, ritf, uitf );
		break;
	case 2:
		ProcessTriTessFactorsMin( g_TessFactor.xyz, g_InsideFactor.x, retf, ritf, uitf );
		break;
	}

	outConst.tessFactor[0] = retf.x;
	outConst.tessFactor[1] = retf.y;
	outConst.tessFactor[2] = retf.z;
	outConst.insideFactor = ritf;


	outConst.normal = iPatch[0].normal;
	outConst.uv = iPatch[0].uv;
	return outConst;
}
// Hull Shader
[domain("tri")]					// tri, quad, isoline
[partitioning("integer")]		// integer, fractional_even, fractional_odd, pow2
[outputtopology("triangle_cw")]	// point, line, triangle_cw, triangle_ccw
[outputcontrolpoints(3)]
[patchconstantfunc("RenderTriConstantHS")]
VSOUT RenderTriIntegerHS( InputPatch<VSOUT, 3> iPatch,  uint pointID : SV_OutputControlPointID )
{
	return iPatch[pointID];
}

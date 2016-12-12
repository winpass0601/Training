cbuffer _Camera : register(b0){
	matrix View;
	matrix Projection;
};
cbuffer _World : register(b1){
	matrix World;
};
cbuffer _Height : register(b2){
	float4 Meta;
};
cbuffer _Shadow :register(b3) {
	matrix ShadowView;
	matrix ShadowProjection;
};
// VertexShaderInput(入）
struct VSIN{
	float3 pos     : POSITION;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
// VertexShaderOutput(出)
struct VSOUT{
	float4 pos     : SV_POSITION;
	float3 normal  : NORMAL;
	float4 SdwCoord: POSITION0;
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
SamplerState samLinear : register(s0); // どういうふうに大きくするか　(S)サンプラーが入ってるレジスター
Texture2D<float4> texAlbed     : register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D<float4> texNormal    : register(t1);
Texture2D<float> texHeight     : register(t2);
// Domain Shader
[domain("tri")]
VSOUT RenderTriDS( TriConstant inConst, float3 domLoc : SV_DomainLocation, const OutputPatch<VSIN, 3> oPatch )
{
	VSOUT outp;

	outp.pos.xyz = oPatch[0].pos * domLoc.z + oPatch[1].pos * domLoc.x + oPatch[2].pos * domLoc.y;
	outp.pos.w = 1.0;
	//outp.normal = oPatch[0].normal * domLoc.z + oPatch[1].normal * domLoc.x + oPatch[2].normal * domLoc.y;

	outp.uv = oPatch[0].uv * domLoc.z + oPatch[1].uv * domLoc.x + oPatch[2].uv * domLoc.y;
	uint2 tex = { (float2)(outp.uv*Meta.xy) };
	outp.normal.xyz = texNormal[tex];
	float h= texHeight[tex]*Meta.z;

	outp.pos.y += h-Meta.w;
	outp.pos = mul(outp.pos, (float4x4)World);
	float4 shadow = outp.pos;
	outp.pos = mul(outp.pos, (float4x4)View);
	outp.pos = mul(outp.pos, (float4x4)Projection);

	//ライトから見たビュー変換
	outp.SdwCoord = mul(shadow, (float4x4)ShadowView);
	outp.SdwCoord = mul(outp.SdwCoord, (float4x4)ShadowProjection);


	outp.normal = mul(outp.normal,(float4x4)World);
	outp.normal = mul(outp.normal, (float4x4)View);
	outp.normal = normalize(outp.normal);




	return outp;
}

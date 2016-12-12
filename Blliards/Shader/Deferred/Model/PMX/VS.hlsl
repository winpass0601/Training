cbuffer _Camera : register(b0){
	matrix View;
	matrix Projection;
};
cbuffer _World : register(b1){
	matrix World;
};
// VertexShaderInput(入）
struct VSIN{
	float3 pos     : SV_POSITION;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
// VertexShaderOutput(出)
struct VSOUT{
	float4 pos		: SV_POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD0;
	float4 uva0		: UVA0;
	float4 uva1		: UVA1;
	float4 uva2		: UVA2;
	float4 uva3		: UVA3;
};
Texture2D texAlbed   : register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D texSphere    : register(t1);
Texture2D texToon      : register(t2);

VSOUT main(VSIN inp){

	VSOUT outp = (VSOUT)0;
	outp.normal.xyz = inp.normal.xyz;
	outp.pos = float4(inp.pos,1.0f);
	outp.pos = mul(outp.pos, (float4x4)World);
	outp.pos = mul(outp.pos, (float4x4)View);
	outp.pos = mul(outp.pos, (float4x4)Projection);


	outp.normal = mul(outp.normal,(float4x4)World);
	outp.normal = mul(outp.normal, (float4x4)View);
	outp.normal = normalize(outp.normal);

	outp.uv   = inp.uv;
	outp.uva0 = inp.uva0;
	outp.uva1 = inp.uva1;
	outp.uva2 = inp.uva2;
	outp.uva3 = inp.uva3;
	return outp;
}
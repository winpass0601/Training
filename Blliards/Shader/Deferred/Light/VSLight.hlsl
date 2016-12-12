cbuffer _Camera : register(b0){
	matrix View;
	matrix Projection;
};
cbuffer _World : register(b1){
	matrix World;
};
// VertexShaderInput(ì¸Åj
struct VSIN{
	float3 pos     : POSITION;
	float3 normal  : NORMAL;
};
// VertexShaderOutput(èo)
struct VSOUT{
	float4 pos     : SV_POSITION;
	float3 normal  : NORMAL;
};

VSOUT main(VSIN inp){

	VSOUT outp = (VSOUT)0;

	outp.pos = float4(inp.pos,1.0f);
	outp.pos = mul(outp.pos, (float4x4)World);
	outp.pos = mul(outp.pos, (float4x4)View);
	outp.pos = mul(outp.pos, (float4x4)Projection);

	outp.normal = float4(inp.normal.xyz,1);
	outp.normal = mul(outp.normal,(float4x4)World);
	outp.normal = mul(outp.normal, (float4x4)View);
	outp.normal = normalize(outp.normal);

	return outp;
}
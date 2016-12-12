// VertexShaderInput(ì¸Åj
struct VSIN{
	float3 pos     : POSITION;
	float2 uv      : TEXCOORD0;
};
// VertexShaderOutput(èo)
struct VSOUT{
	float4 pos     : SV_POSITION;
	float2 uv      : TEXCOORD0;
};

VSOUT main(VSIN inp){

	VSOUT outp = (VSOUT)0;
	outp.pos.xyz = inp.pos;
	outp.pos.w = 1.0f;

	outp.uv = inp.uv;


	return outp;
}
cbuffer _Camera : register(b0){
	matrix View;
	matrix Projection;
};

// VertexShaderInput(ì¸Åj
struct VSIN{
	float3 pos     : POSITION;
	float3 color    :COLOR;
};
// VertexShaderOutput(èo)
struct VSOUT{
	float3 pos     : POSITION;
	float3 color    :COLOR;
};
VSOUT main(VSIN inp){

	VSOUT outp = (VSOUT)0;
	outp.pos = mul(inp.pos, (float4x4)View);
	outp.pos = mul(outp.pos, (float4x4)Projection);

	outp.color = inp.color;
	return outp;
}
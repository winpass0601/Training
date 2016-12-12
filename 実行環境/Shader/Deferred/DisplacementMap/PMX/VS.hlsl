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
// VertexShaderInput(���j
struct VSIN{
	float3 pos     : POSITION;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
// VertexShaderOutput(�o)
struct VSOUT{
	float4 pos     : SV_POSITION;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};

SamplerState samLinear : register(s0); // �ǂ������ӂ��ɑ傫�����邩�@(S)�T���v���[�������Ă郌�W�X�^�[
Texture2D<float4> texAlbed     : register(t0);  // (T)�e�N�X�`���������Ă郌�W�X�^�[
Texture2D<float4> texNormal    : register(t1);
Texture2D<float> texHeight     : register(t2);

VSOUT main(VSIN inp){

	VSOUT outp = (VSOUT)0;
	outp.pos.xyz = inp.pos;
	outp.pos.w = 1.0f;

	outp.uv   = inp.uv;
	outp.uva0 = inp.uva0;
	outp.uva1 = inp.uva1;
	outp.uva2 = inp.uva2;
	outp.uva3 = inp.uva3;


	return outp;
}
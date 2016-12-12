// VertexShaderOutput(出)
struct VSOUT {
	float4 pos     : SV_POSITION;
	float2 uv      : TEXCOORD0;
};

SamplerState samLinear : register(s0); // どういうふうに大きくするか　(S)サンプラーが入ってるレジスター

Texture2D texAlbed			: register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D texNormal			: register(t1);
Texture2D texLighitmap		: register(t2);
//Texture2D texNormal  :register(t1);

float4 main(VSOUT inp) : SV_Target0{
	float4 albed = texAlbed.Sample(samLinear,inp.uv);
	float4 normal = texNormal.Sample(samLinear,inp.uv);
	float4 lighitmap = texLighitmap.Sample(samLinear,inp.uv);
	float4 retcolor = float4(((albed.rgb * lighitmap.rgb)*lighitmap.a).xyz,1.0f);
	return retcolor;
}
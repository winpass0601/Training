// 円周率です.
#define PI          3.1415926535f
cbuffer _Buffer : register(b0) {
	float2 window;//uv変換に使う
	float3 pos;
	float3 color;//
	float lightlen;
	float3 dummy;
};
// VertexShaderOutput(出)
struct VSOUT {
	float4 pos	: SV_POSITION;
	float3 normal  : NORMAL;
};

SamplerState samLinear : register(s0); // どういうふうに大きくするか　(S)サンプラーが入ってるレジスター
Texture2D texNormal    : register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D texWorld     : register(t1);  // (T)テクスチャが入ってるレジスター

float VecLength(float3 vec)
{
	return  sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
float pow(float num)
{
	return num * num;
}

float4 main(VSOUT inp) : SV_Target0{
	float4 res = 1.0f;
	float2 uv = inp.pos.xy/window;
	float4 wpos = texWorld.Sample(samLinear,uv);
	float4 normal = texNormal.Sample(samLinear,uv);
	//float4 lightVec = float4(wpos -  pos,1.0f) ;
	//
	//////lightVec = normalize(lightVec);
	////float4 lightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
	//float3 L = normalize(-lightVec);
	//
	//float3 N = inp.normal;
	//
	//float D = max(0.0f, dot(N, L));
	//D = D * 0.5f + 0.5f;
	//D = D * D;
	//
	////距離減衰を計算
	//float att;
	//att = 1.0f / (0.3f + 0.01f * lightlen + 0.03f * pow(lightlen));
	//
	//D = D * att*5.0f;
	//
	//float3 litcol = color * D;

	res = normal*50;

	//res.b = 0.0f;
	res.a = 1.0f;
	return res;
}
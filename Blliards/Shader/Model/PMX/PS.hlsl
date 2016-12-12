// 円周率です.
#define PI          3.1415926535f
cbuffer MaterialBuffer : register(b0) {
	/// 減衰色
	float4 diffuse;
	/// 光沢色
	float3 specular;
	/// 光沢度
	float specularlity;
	/// 環境色
	float3 ambient;
	/// エッジ色
	float4 edge_color;
	/// エッジサイズ
	float edge_size;
};
// VertexShaderOutput(出)
struct VSOUT {
	float4 pos     : SV_POSITION;
	float3 worldPos :POSITION0;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
SamplerState samLinear : register(s0); // どういうふうに大きくするか　(S)サンプラーが入ってるレジスター
SamplerComparisonState  ShadowSmp     : register(s1);     //!< シャドウマップ用サンプラー比較ステートです.

Texture2D texAlbed   : register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D texSphere    : register(t1);
Texture2D texToon      : register(t2);
//Texture2D texNormal  :register(t1);


float VecLength(float3 vec)
{
	return  sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
float pow(float num)
{
	return num * num;
}

float4 main(VSOUT inp) : SV_Target0{

	float3 pointLight[4];
	pointLight[0] = float3(1.5f, 1.0f, 2.0f);
	pointLight[1] = float3(-1.5f, 1.0f, 2.0f);
	pointLight[2] = float3(1.5f, 1.0f, -2.0f);
	pointLight[3] = float3(-1.5f, 1.0f, -2.0f);
	//float3 L = float3(0.1f,-1,0);
	//L = normalize(-L);
	float3 N = normalize(inp.normal);

	float D = 0.0f;
	for (int i = 0; i < 4; ++i) {
		float3 L = pointLight[i] - inp.worldPos;
		L = normalize(-L);
		float d = max(0.0f, dot(L, N));
		d = d * 0.5f + 0.5f;
		d = d * d;
		float length = VecLength(inp.worldPos- pointLight[i]);
		float att = 1.0f / (0.3f + 0.01f * length + 0.3f * pow(length));
		D += d * att;
	}

	//モデルの色
	float4 albed = texAlbed.Sample(samLinear,inp.uv);
	float4 retcolor = albed * saturate(D);// +float4(ambient, 1.0f);
	retcolor.a = 1.0f;
	return retcolor;
}
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
	float4 SdwCoord: POSITION0;
	float3 normal  : NORMAL;
	float2 uv      : TEXCOORD0;
	float4 uva0    : UVA0;
	float4 uva1    : UVA1;
	float4 uva2    : UVA2;
	float4 uva3    : UVA3;
};
struct PSOUT {
	float4 albed : SV_Target0;
	float4 normal : SV_Target1;
	float4 col2 : SV_Target2;
};
SamplerState samLinear : register(s0); // どういうふうに大きくするか　(S)サンプラーが入ってるレジスター
SamplerComparisonState  ShadowSmp     : register(s1);     //!< シャドウマップ用サンプラー比較ステートです.

Texture2D texAlbed   : register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D texSphere    : register(t1);
Texture2D texToon      : register(t2);
Texture2D ShadowMap     : register(t3);     //!< シャドウマップです.
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

	float z = inp.pos.z / inp.pos.w;
	return float4(z,z,z,1.0f);

}
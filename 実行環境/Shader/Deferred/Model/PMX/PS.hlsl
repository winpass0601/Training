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
struct VSOUT{
	float4 pos		: SV_POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD0;
	float4 uva0		: UVA0;
	float4 uva1		: UVA1;
	float4 uva2		: UVA2;
	float4 uva3		: UVA3;
};
struct PSOUT {
	float4 albed : SV_Target0;
	float4 normal : SV_Target1;
	float4 lighitmap : SV_Target2;
	float4 world : SV_Target3;
};
SamplerState samLinear : register(s0); // どういうふうに大きくするか　(S)サンプラーが入ってるレジスター

Texture2D texAlbed   : register(t0);  // (T)テクスチャが入ってるレジスター
Texture2D texSphere    : register(t1);
Texture2D texToon      : register(t2);


float VecLength(float3 vec)
{
	return  sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
float pow(float num)
{
	return num * num;
}

PSOUT main(VSOUT inp){
	PSOUT outp = (PSOUT)0;
	//モデルの色
	outp.albed = texAlbed.Sample(samLinear,inp.uv);
	outp.normal = float4(inp.normal,inp.pos.z/inp.pos.w);
	outp.lighitmap = 0;
	outp.world = normalize(inp.pos);
	//outp.world.w = outp.normal.w;
	//float4 lightVec = inp.SdwCoord - normalize(inp.pos);
	//float length = VecLength(lightVec);
	////lightVec = normalize(lightVec);
	//float4 lightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
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
	//att = 1.0f / (0.3f + 0.01f * length + 0.03f * pow(length));
	//
	//D = D * att;
	//
	//float3 litcol = lightColor.rgb * D;


	//float3 shadowCoord = inp.SdwCoord.xyz / inp.SdwCoord.w;
	//shadowCoord.x = shadowCoord.x*0.5f + 0.5f;
	//shadowCoord.y = shadowCoord.y*-0.5f + 0.5f;
	//if (!(shadowCoord.x < 0 || shadowCoord.x > 1 || shadowCoord.y < 0 || shadowCoord.y > 1)) {
	//	// 最大深度傾斜を求める.
	//	float  maxDepthSlope = max(abs(ddx(shadowCoord.z)), abs(ddy(shadowCoord.z)));
	//	int  shadowThreshold = 1.;      // シャドウにするかどうかの閾値です.
	//	float  bias = 0.1f;     // 固定バイアスです.
	//	float  slopeScaledBias = 0.01f;     // 深度傾斜.
	//	float  depthBiasClamp = 1.0f;      // バイアスクランプ値.
	//	float  shadowBias = bias + slopeScaledBias * maxDepthSlope;
	//	shadowBias = min(shadowBias, depthBiasClamp);
	//
	//	float3 shadowColor;
	//	shadowThreshold = ShadowMap.SampleCmpLevelZero(ShadowSmp, shadowCoord.xy, shadowCoord.z - shadowBias);
	//	shadowColor = 1.0f - shadowThreshold;
	//
	//	outp.shadow = float4(shadowColor, 1.0f);
	//}


	return outp;
}
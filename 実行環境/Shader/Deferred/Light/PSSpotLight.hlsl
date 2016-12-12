// ‰~ü—¦‚Å‚·.
#define PI          3.1415926535f
cbuffer _Buffer : register(b0) {
	float2 window;//uv•ÏŠ·‚Ég‚¤
	float lightlen;

};
// VertexShaderOutput(o)
struct VSOUT {
	float4 pos     : SV_POSITION;
	float3 normal  : NORMAL;
};

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
	////‹——£Œ¸Š‚ğŒvZ
	//float att;
	//att = 1.0f / (0.3f + 0.01f * length + 0.03f * pow(length));
	//
	//D = D * att;
	//
	//float3 litcol = lightColor.rgb * D;




	return res;
}
// VertexShaderOutput(èo)
struct VSOUT {
	float4 pos     : SV_POSITION;
	float3 color	:COLOR;
};

float4 main(VSOUT inp) : SV_Target0{
	float4 ret = float4(inp.color,1.0f);
	return ret;
}
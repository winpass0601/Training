// �~�����ł�.
#define PI          3.1415926535f
cbuffer MaterialBuffer : register(b0) {
	/// �����F
	float4 diffuse;
	/// ����F
	float3 specular;
	/// ����x
	float specularlity;
	/// ���F
	float3 ambient;
	/// �G�b�W�F
	float4 edge_color;
	/// �G�b�W�T�C�Y
	float edge_size;
};
// VertexShaderOutput(�o)
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
SamplerState samLinear : register(s0); // �ǂ������ӂ��ɑ傫�����邩�@(S)�T���v���[�������Ă郌�W�X�^�[
SamplerComparisonState  ShadowSmp     : register(s1);     //!< �V���h�E�}�b�v�p�T���v���[��r�X�e�[�g�ł�.

Texture2D texAlbed   : register(t0);  // (T)�e�N�X�`���������Ă郌�W�X�^�[
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

	//���f���̐F
	float4 albed = texAlbed.Sample(samLinear,inp.uv);
	float4 retcolor = albed * saturate(D);// +float4(ambient, 1.0f);
	retcolor.a = 1.0f;
	return retcolor;
}
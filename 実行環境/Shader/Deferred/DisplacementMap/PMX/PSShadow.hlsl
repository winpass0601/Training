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
SamplerState samLinear : register(s0); // �ǂ������ӂ��ɑ傫�����邩�@(S)�T���v���[�������Ă郌�W�X�^�[
SamplerComparisonState  ShadowSmp     : register(s1);     //!< �V���h�E�}�b�v�p�T���v���[��r�X�e�[�g�ł�.

Texture2D texAlbed   : register(t0);  // (T)�e�N�X�`���������Ă郌�W�X�^�[
Texture2D texSphere    : register(t1);
Texture2D texToon      : register(t2);
Texture2D ShadowMap     : register(t3);     //!< �V���h�E�}�b�v�ł�.
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
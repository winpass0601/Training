#include <./GPL/Line/GPLLine.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

namespace rinfw {
namespace graphics {

//GPLS�e�X�g�p�R�[�h
GPLLine::GPLLine() {
	lines.reserve(LINE_MAX);
	lines.resize(LINE_MAX);
}

void GPLLine::Release() {
	this->ia.Release();
	this->om.Release();
	this->rs.Release();
	this->vs.Release();
	this->ps.Release();

}
bool GPLLine::createPipeLine(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	if (!this->createIA(device)) {
		return false;
	}
	if (!this->createOM(device)) {
		return false;
	}
	if (!this->createRS(device)) {
		return false;
	}
	if (!this->createVS(device)) {
		return false;
	}
	if (!this->createPS(device)) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC element [] = { // ���̓A�Z���u���[ �X�e�[�W�̒P��̗v�f( HLSL �Z�}���e�B�N�X,�v�f�̃Z�}���e�B�N�X �C���f�b�N�X,�v�f�f�[�^�̃f�[�^�^,���̓A�Z���u���[�����ʂ��鐮���l,�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��),�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������,�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// POS���
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },// COLOR���
	};

	ID3D11VertexShader *vs_buf;
	ID3D10Blob *vsblob; // �C�Ӓ��̃f�[�^��Ԃ��ۂɎg�p
	// �t�@�C�������ɃG�t�F�N�g���R���p�C��
	D3DX11CompileFromFile(TEXT("./Shader/Utility/vs_line.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
	// blob��_buf�Ɋi�[
	device->CreateVertexShader(vsblob->GetBufferPointer(), vsblob->GetBufferSize(), nullptr, &vs_buf); // �R���p�C���ς݃V�F�[�_�[����A���_�V�F�[�_�[ �I�u�W�F�N�g���쐬


	//���_���C�A�E�g�쐬
	ID3D11InputLayout* hpInputLayout = NULL;
	device->CreateInputLayout(element, ARRAYSIZE(element), vsblob->GetBufferPointer(), vsblob->GetBufferSize(), &hpInputLayout);
	vsblob->Release();
	//
	//IA
	{
		this->ia.setGPLSInputLayout(hpInputLayout);
		COMPTR(ID3D11InputLayout) input;
	}

	//VS
	{
		this->vs.setGPLSShader(vs_buf);
	}

	return true;
}
bool GPLLine::createIA(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	this->ia.setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
bool GPLLine::createOM(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	ID3D11BlendState* hpBlendState = NULL;
	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++) {

		//MMD�u�����h���@
		BlendStateDesc.RenderTarget[i].BlendEnable = FALSE;
		BlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;

		BlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;



		BlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	}
	device->CreateBlendState(&BlendStateDesc, &hpBlendState);

	//���̃u�����f�B���O���R���e�L�X�g�ɐݒ�
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->om.setGPLSBlendState(hpBlendState, blendFactor, 0xffffffff);
	this->om.setGPLSRenderTargetView(0, DX11Device::getInstance().getCOMRTV());
	this->om.setGPLSDepthStencilView(DX11Device::getInstance().getCOMDSV());

	return true;
}
bool GPLLine::createRS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float) WindowDevice::getInstance().getWidth();
	vp.Height = (float) WindowDevice::getInstance().getHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	this->rs.setGPLSViewPort(0, vp);

	ID3D11RasterizerState* hpRasterizerState = NULL;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
			D3D11_FILL_WIREFRAME,
			D3D11_CULL_NONE,	//�|���S���̗��\�𖳂���
			FALSE,
			0,
			0.0f,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			FALSE
	};
	device->CreateRasterizerState(&hRasterizerDesc, &hpRasterizerState);
	this->rs.setGPLSRasterizerState(hpRasterizerState);

	return true;
}
bool GPLLine::createVS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}


	ID3D11Buffer *vertexBuffer = nullptr;
	D3D11_BUFFER_DESC bd; // �������@(�o�b�t�@�[ ���\�[�X)
	ZeroMemory(&bd, sizeof(bd)); // ���g���[���N���A
	// Buffer�̐������@�̊i�[
	bd.Usage = D3D11_USAGE_DEFAULT; // �o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	bd.CPUAccessFlags = 0; // CPU����͏������݂̂ݍs���܂�
	bd.ByteWidth = sizeof(Line) * LINE_MAX*2;  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �Ȃ�̃o�b�t�@�ł����H
	bd.StructureByteStride = sizeof(float);


	device->CreateBuffer(&bd, nullptr, &vertexBuffer);
	this->ia.setGPLSVertex(0, vertexBuffer, sizeof(Line), 0);

	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ID3D11Buffer *constantbuffer = nullptr;
	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44) * 2;
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->vs.setGPLSBuffer(0, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44);
	device->CreateBuffer(&bd,nullptr, &constantbuffer);
	this->vs.setGPLSBuffer(1, constantbuffer);
	return true;
}
bool GPLLine::createPS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	ID3D11PixelShader  *ps_buf = nullptr;
	ID3D10Blob *psblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/Utility/ps_line.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	this->ps.setGPLSShader(ps_buf);
	psblob->Release();
	return true;
}

bool GPLLine::Draw(rinfw::core::math::Matrix44 & world) {
	rinfw::core::math::Matrix44 view;
	rinfw::core::math::Matrix44 projection;
	auto context = DX11Device::getInstance().getContext();
	COMPTR(ID3D11Buffer) vertex;

	vertex = ia.getGPLSVertex(0);
	context->UpdateSubresource(vertex.GetInterfacePtr(), 0, NULL, &this->lines[0], 0, 0);
	context->ClearState();
	ia.setStatus();
	om.setStatus();
	rs.setStatus();
	vs.setStatus();
	ps.setStatus();

	static bool wire = false;
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -7.0f);
	static rinfw::core::math::Float3 at(0.0f, 0.0f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static float fov = 45.0f;

	view = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float) window.getWidth();		// �r���[�|�[�g�̕�
	viewport.Height = (float) window.getHeight();		// �r���[�|�[�g�̍���
	viewport.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	viewport.MaxDepth = 1.0f;				// �r���[�|�[�g�̍ő�[�x
	viewport.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	viewport.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu

	projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 10.0f);
	view = rinfw::core::math::matrix44Transpose(view);
	projection = rinfw::core::math::matrix44Transpose(projection);

	COMPTR(ID3D11Buffer) cameraconstantbuffer, worldconstantbuffer;
	cameraconstantbuffer = this->vs.getGPLSBuffer(0);
	worldconstantbuffer = this->vs.getGPLSBuffer(1);
	struct _Camera {
		rinfw::core::math::Matrix44 View;
		rinfw::core::math::Matrix44 Projection;
	};
	_Camera cameramtx;
	cameramtx.View = view;
	cameramtx.Projection = projection;

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(world),0,0);
	this->vs.setBuffertoVS(context);

	context->Draw(this->lines.size(), 0);

	return true;
}
}
}
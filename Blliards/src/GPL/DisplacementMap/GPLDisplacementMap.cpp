#include <./GPL/GPL.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

struct TessBuffer
{
	float tessFactor[4];
	float	insideFactor[2];
	unsigned int			mode;
	unsigned int			pad;
};
namespace rinfw {
namespace graphics {

//GPLS�e�X�g�p�R�[�h
GPLDisplacementMap::GPLDisplacementMap() {

}

void GPLDisplacementMap::Release() {
	this->ia.Release();
	this->om.Release();
	this->rs.Release();
	this->rs_cullnone.Release();
	this->rs_cullwire.Release();
	this->vs.Release();
	this->hs.Release();
	this->ds.Release();
	this->ps.Release();

	this->shadowtex.Release();
	this->depth.Release();
}
bool GPLDisplacementMap::createPipeLine(ID3D11Device *device) {
	UINT width = 1024,height = 1024;
	this->shadowtex.Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	this->depth.Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);


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
	if (!this->createHS(device)) {
		return false;
	}
	if (!this->createDS(device)) {
		return false;
	}
	if (!this->createPS(device)) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC element[] = { // ���̓A�Z���u���[ �X�e�[�W�̒P��̗v�f( HLSL �Z�}���e�B�N�X,�v�f�̃Z�}���e�B�N�X �C���f�b�N�X,�v�f�f�[�^�̃f�[�^�^,���̓A�Z���u���[�����ʂ��鐮���l,�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��),�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������,�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// POS���
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },// NORMAL���
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,		12 + 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV���
		{ "UVA"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV���
		{ "UVA"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV���
		{ "UVA"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16 + 16 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV���
		{ "UVA"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16 + 16 + 16 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV���
	};

	ID3D11VertexShader *vs_buf;
	ID3D10Blob *vsblob; // �C�Ӓ��̃f�[�^��Ԃ��ۂɎg�p
	// �t�@�C�������ɃG�t�F�N�g���R���p�C��
	D3DX11CompileFromFile(TEXT("./Shader/DisplacementMap/Pmx/VS.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
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
	}

	//VS
	{
		this->vs.setGPLSVertexShader(vs_buf);
	}

	return true;
}
bool GPLDisplacementMap::createIA(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	this->ia.setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	return true;
}
bool GPLDisplacementMap::createOM(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	ID3D11BlendState* hpBlendState = NULL;
	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++) {

		//MMD�u�����h���@
		BlendStateDesc.RenderTarget[i].BlendEnable = TRUE;
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

	device->CreateBlendState(&BlendStateDesc, &hpBlendState);
	this->shadow_om.setGPLSBlendState(hpBlendState, blendFactor, 0xffffffff);
	this->shadow_om.setGPLSRenderTargetView(0,this->shadowtex.getCOMRTV());
	this->shadow_om.setGPLSDepthStencilView(this->depth.getCOMDSV());

	return true;
}
bool GPLDisplacementMap::createRS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)WindowDevice::getInstance().getWidth();
	vp.Height = (float)WindowDevice::getInstance().getHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	this->rs.setGPLSViewPort(0, vp);
	this->rs_cullnone.setGPLSViewPort(0, vp);

	ID3D11RasterizerState* hpRasterizerState = NULL;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
			D3D11_FILL_SOLID,
			D3D11_CULL_BACK,	//�|���S���̗��\�𖳂���
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

	hRasterizerDesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&hRasterizerDesc, &hpRasterizerState);
	this->rs_cullnone.setGPLSRasterizerState(hpRasterizerState);

	hRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&hRasterizerDesc, &hpRasterizerState);
	this->rs_cullwire.setGPLSRasterizerState(hpRasterizerState);

	return true;
}
bool GPLDisplacementMap::createVS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	return true;
}
bool GPLDisplacementMap::createHS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	ID3D11HullShader  *hs_buf = nullptr;
	ID3D10Blob *hsblob = nullptr;


	D3DX11CompileFromFile(TEXT("./Shader/DisplacementMap/Pmx/HS.hlsl"), 0, 0, "RenderTriIntegerHS", "hs_5_0", 0, 0, 0, &hsblob, 0, 0);
	device->CreateHullShader(hsblob->GetBufferPointer(), hsblob->GetBufferSize(), nullptr, &hs_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	this->hs.setGPLSHullShader(hs_buf);
	hsblob->Release();





	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // �������@(�o�b�t�@�[ ���\�[�X)
	ZeroMemory(&bd, sizeof(bd)); // ���g���[���N���A
	// Buffer�̐������@�̊i�[
	bd.Usage = D3D11_USAGE_DEFAULT; // �o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	bd.CPUAccessFlags = 0; // CPU����͏������݂̂ݍs���܂�
	bd.ByteWidth = sizeof(TessBuffer);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �Ȃ�̃o�b�t�@�ł����H
	device->CreateBuffer(&bd, nullptr, &constantbuffer);

	this->hs.setGPLSConstantBuffer(0, constantbuffer);


	return true;
}
bool GPLDisplacementMap::createDS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	ID3D11DomainShader  *ds_buf = nullptr;
	ID3D10Blob *dsblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/DisplacementMap/Pmx/DS.hlsl"), 0, 0, "RenderTriDS", "ds_5_0", 0, 0, 0, &dsblob, 0, 0);
	device->CreateDomainShader(dsblob->GetBufferPointer(), dsblob->GetBufferSize(), nullptr, &ds_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	this->ds.setGPLSDomainShader(ds_buf);
	dsblob->Release();


	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // �������@(�o�b�t�@�[ ���\�[�X)
	ZeroMemory(&bd, sizeof(bd)); // ���g���[���N���A
	// Buffer�̐������@�̊i�[
	bd.Usage = D3D11_USAGE_DEFAULT; // �o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	bd.CPUAccessFlags = 0; // CPU����͏������݂̂ݍs���܂�
	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44) * 2;  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �Ȃ�̃o�b�t�@�ł����H
	bd.StructureByteStride = sizeof(float);

	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ds.setGPLSConstantBuffer(0, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ds.setGPLSConstantBuffer(1, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Float4);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ds.setGPLSConstantBuffer(2, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44) * 2;  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ds.setGPLSConstantBuffer(3, constantbuffer);


	return true;
}
bool GPLDisplacementMap::createPS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	ID3D11PixelShader  *ps_buf = nullptr;
	ID3D10Blob *psblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/DisplacementMap/Pmx/PS.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	this->ps.setGPLSPixelShader(ps_buf);
	psblob->Release();

	D3DX11CompileFromFile(TEXT("./Shader/DisplacementMap/Pmx/PSShadow.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	this->shadow_ps.setGPLSPixelShader(ps_buf);
	psblob->Release();

	//�}�e���A���������邽�߂̃R���e�L�X�g�̐���

	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // �������@(�o�b�t�@�[ ���\�[�X)
	ZeroMemory(&bd, sizeof(bd)); // ���g���[���N���A
	// Buffer�̐������@�̊i�[
	bd.Usage = D3D11_USAGE_DYNAMIC; // �o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU����͏������݂̂ݍs���܂�
	bd.ByteWidth = sizeof(rinfw::core::ConstantMaterial);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �Ȃ�̃o�b�t�@�ł����H
	device->CreateBuffer(&bd, nullptr, &constantbuffer);

	this->ps.setGPLSConstantBuffer(0, constantbuffer);


	D3D11_SAMPLER_DESC sampDesc;
	//�T���v���[�̐ݒ�
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.BorderColor[0] = 0.0f;
	sampDesc.BorderColor[1] = 0.0f;
	sampDesc.BorderColor[2] = 0.0f;
	sampDesc.BorderColor[3] = 0.0f;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 2;
	sampDesc.MinLOD = FLT_MAX * -1;
	sampDesc.MaxLOD = FLT_MAX;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	ID3D11SamplerState *samp;
	device->CreateSamplerState(&sampDesc, &samp);
	this->ps.setGPLSSamplerState(0, samp);
	device->CreateSamplerState(&sampDesc, &samp);
	this->ps.setGPLSSamplerState(1, samp);
	device->CreateSamplerState(&sampDesc, &samp);
	this->ps.setGPLSSamplerState(2, samp);
	device->CreateSamplerState(&sampDesc, &samp);
	this->vs.setGPLSSamplerState(0, samp);

	return true;
}
bool GPLDisplacementMap::Clear(ID3D11DeviceContext *context) {
	static float clear[4] = { 0.0f,0.0f,0.0f,1.0f };
	//DEBUG(
	ImGui::Begin("ShadowMap");
	ImGui::SliderFloat4("Clear Shadowmap Color", clear, 0.0f, 1.0f);
	ImGui::End();
	context->ClearRenderTargetView(this->shadowtex.getRTV(), clear);
	context->ClearDepthStencilView(this->depth.getDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return true;
}
bool GPLDisplacementMap::Draw(core::PmxModel &model) {
	return this->Draw(model, DX11Device::getInstance().getContext());
}
bool GPLDisplacementMap::Draw(core::PmxModel & model, rinfw::core::math::Matrix44 &world) {
	return this->Draw(model, world, DX11Device::getInstance().getContext());
}
bool GPLDisplacementMap::Draw(core::PmxModel &model, ID3D11DeviceContext *context) {

	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));
	//this->BlendDebug();
	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer[] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // ���_�̃T�C�Y
	UINT offset = 0;			   // ����̒���
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


	this->vs.setVertexShader(context);


	//+ Camera����

#ifdef _Debug

	static bool wire = false;
	COMPTR(ID3D11Buffer) cameraconstantbuffer, worldconstantbuffer;
	cameraconstantbuffer = this->vs.getGPLSConstantBuffer(0);
	worldconstantbuffer = this->vs.getGPLSConstantBuffer(1);
	struct _Camera {
		rinfw::core::math::Matrix44 View;
		rinfw::core::math::Matrix44 Projection;
	};

	_Camera cameramtx;
	rinfw::core::math::Matrix44 worldmtx;


	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -0.1f);
	static rinfw::core::math::Float3 at(0.0f, 0.15f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static rinfw::core::math::Float3 pos;
	static rinfw::core::math::Float3 scale(1.0f, 1.0f, 1.0f);
	static float fov = 45.0f;

	ImGui::Begin("Model Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("Wire")) {
		wire = !wire;
	}
	ImGui::DragFloat3("eye", (float*)&eye, 0.025f);
	ImGui::DragFloat3("at", (float*)&at, 0.025f);
	ImGui::DragFloat3("up", (float*)&up, 0.025f);

	ImGui::DragFloat3("pos", (float*)&pos, 0.025f);
	ImGui::DragFloat3("scale", (float*)&scale, 0.025f);

	ImGui::End();

	cameramtx.View = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float)window.getWidth();		// �r���[�|�[�g�̕�
	viewport.Height = (float)window.getHeight();		// �r���[�|�[�g�̍���
	viewport.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	viewport.MaxDepth = 1.0f;				// �r���[�|�[�g�̍ő�[�x
	viewport.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	viewport.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu

	cameramtx.Projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 1000.0f);
	cameramtx.View = rinfw::core::math::matrix44Transpose(cameramtx.View);
	cameramtx.Projection = rinfw::core::math::matrix44Transpose(cameramtx.Projection);

	rinfw::core::math::matrix44Translation(worldmtx, pos.x, pos.y, pos.z);
	worldmtx = worldmtx * rinfw::core::math::matrix44Scaling(scale.x, scale.y, scale.z);
	worldmtx = rinfw::core::math::matrix44Transpose(worldmtx);

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &worldmtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

	this->vs.setConstantBuffer(context);
	this->ps.setSamplerState(context);
#endif

	this->om.setStatus(context);
	unsigned long count = 0;
	for (UINT i = 0; i < (UINT)model.mesh.materialCount; i++) {
		if (model.mesh.materials[i].flag == 0x0) {
			count += model.mesh.materials[i].index_count;
			continue;
		}
#ifdef _Debug
		// �`��t���O(8bit) - �ebit 0:OFF 1:ON 0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`��, 0x20:���_�J���[(��2.1�g��), 0x40:Point�`��(��2.1�g��), 0x80:Line�`��(��2.1�g��)
		if (!wire) {
			if (model.mesh.materials[i].flag & 0x1) {
				this->rs_cullnone.setStatus(context);
			}
			else {
				this->rs.setStatus(context);
			}
		}
		else {
			this->rs_cullwire.setStatus(context);
		}
#endif

		ID3D11ShaderResourceView *srv[4] = { nullptr };
		if ((unsigned)model.mesh.materials[i].diffuse_texture_index < model.mesh.texture.size()) {
			srv[0] = model.mesh.texture[model.mesh.materials[i].diffuse_texture_index].getSRV();
		}
		else {
			srv[0] = nullptr;
		}
		if ((unsigned)model.mesh.materials[i].sphere_texture_index < model.mesh.texture.size()) {
			srv[1] = model.mesh.texture[model.mesh.materials[i].sphere_texture_index].getSRV();
		}
		else {
			srv[1] = nullptr;
		}

		if ((unsigned)model.mesh.materials[i].toon_texture_index < model.mesh.texture.size()) {
			srv[2] = model.mesh.texture[model.mesh.materials[i].toon_texture_index].getSRV();
		}
		else {
			srv[2] = nullptr;
		}
		context->VSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->PSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);


		COMPTR(ID3D11Buffer) cons;
		cons = this->ps.getGPLSConstantBuffer(0);
		D3D11_MAPPED_SUBRESOURCE ms; // Buffer���i�[����ׂɂƂ肠�������b�N�������Ȃ��Ƃ����Ȃ��B�ǂ��܂Ń��b�N��������H�T�u���\�[�X �f�[�^�ɃA�N�Z�X�ł���悤�ɂ���
		context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // �A�N�Z�X��ms
		rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		context->Unmap(cons.GetInterfacePtr(), NULL); // ���b�N����
		this->ps.setPixelShader(context);
		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}

	DEBUG(device.getAnotation()->EndEvent());


	return true;
}
bool GPLDisplacementMap::Draw(core::PmxModel &model, rinfw::core::math::Matrix44 &world, ID3D11DeviceContext *context) {

	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));

	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer[] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // ���_�̃T�C�Y
	UINT offset = 0;			   // ����̒���
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	this->om.setStatus(context);

	this->vs.setVertexShader(context);


	//+ Camera����

#ifdef _Debug

	static bool wire = false;
	COMPTR(ID3D11Buffer) cameraconstantbuffer, worldconstantbuffer, heightconstantbuffer,hullconstantbuffer,shadowconstantbuffer;
	cameraconstantbuffer = this->ds.getGPLSConstantBuffer(0);
	worldconstantbuffer  = this->ds.getGPLSConstantBuffer(1);
	heightconstantbuffer = this->ds.getGPLSConstantBuffer(2);
	shadowconstantbuffer = this->ds.getGPLSConstantBuffer(3);
	struct _Camera {
		rinfw::core::math::Matrix44 View;
		rinfw::core::math::Matrix44 Projection;
	};

	_Camera cameramtx;
	rinfw::core::math::Matrix44 worldmtx;

	static TessBuffer tess = {
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,0.0f},
		0,0
	};
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -0.5f);
	static rinfw::core::math::Float3 at(0.0f, 0.15f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static float fov = 45.0f;

	ImGui::Begin("Model Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("Wire")) {
		wire = !wire;
	}
	ImGui::DragFloat3("eye", (float*)&eye, 0.025f);
	ImGui::DragFloat3("at", (float*)&at, 0.025f);
	ImGui::DragFloat3("up", (float*)&up, 0.025f);

	ImGui::DragFloat4("tessFactor", (float*)&tess.tessFactor, 0.025f);
	ImGui::DragFloat2("insideFactor", (float*)&tess.insideFactor, 0.025f);

	ImGui::End();

	cameramtx.View = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float)window.getWidth();		// �r���[�|�[�g�̕�
	viewport.Height = (float)window.getHeight();		// �r���[�|�[�g�̍���
	viewport.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	viewport.MaxDepth = 1.0f;				// �r���[�|�[�g�̍ő�[�x
	viewport.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	viewport.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu

	cameramtx.Projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 1000.0f);
	cameramtx.View = rinfw::core::math::matrix44Transpose(cameramtx.View);
	cameramtx.Projection = rinfw::core::math::matrix44Transpose(cameramtx.Projection);

	worldmtx = world;
	worldmtx = rinfw::core::math::matrix44Transpose(worldmtx);

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &worldmtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(heightconstantbuffer.GetInterfacePtr(), 0, NULL, &this->height, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(shadowconstantbuffer.GetInterfacePtr(), 0, NULL, &this->shadow, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

	this->vs.setConstantBuffer(context);
	this->ps.setSamplerState(context);
#endif

	unsigned long count = 0;
	for (UINT i = 0; i < (UINT)model.mesh.materialCount; i++) {
		//if (model.mesh.materials[i].flag == 0x0) {
		//	count += model.mesh.materials[i].index_count;
		//	continue;
		//}
#ifdef _Debug
		// �`��t���O(8bit) - �ebit 0:OFF 1:ON 0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`��, 0x20:���_�J���[(��2.1�g��), 0x40:Point�`��(��2.1�g��), 0x80:Line�`��(��2.1�g��)
		if (!wire) {
			if (model.mesh.materials[i].flag & 0x1) {
				this->rs_cullnone.setStatus(context);
			}
			else {
				this->rs.setStatus(context);
			}
		}
		else {
			this->rs_cullwire.setStatus(context);
		}
#endif

		ID3D11ShaderResourceView *srv[4] = { nullptr };
		if ((unsigned)model.mesh.materials[i].diffuse_texture_index < model.mesh.texture.size()) {
			srv[0] = model.mesh.texture[model.mesh.materials[i].diffuse_texture_index].getSRV();
		}
		else {
			srv[0] = nullptr;
		}
		if ((unsigned)model.mesh.materials[i].sphere_texture_index < model.mesh.texture.size()) {
			srv[1] = model.mesh.texture[model.mesh.materials[i].sphere_texture_index].getSRV();
		}
		else {
			srv[1] = nullptr;
		}
		if ((unsigned)model.mesh.materials[i].toon_texture_index < model.mesh.texture.size()) {
			srv[2] = model.mesh.texture[model.mesh.materials[i].toon_texture_index].getSRV();
		}
		else {
			srv[2] = nullptr;
		}
		srv[3] = this->shadowtex.getSRV();

		context->VSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->DSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->PSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);

		hullconstantbuffer = this->hs.getGPLSConstantBuffer(0);
		context->UpdateSubresource(hullconstantbuffer.GetInterfacePtr(), 0, NULL, &tess, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

		this->hs.setConstantBuffer(context);
		this->hs.setHullShader(context);
		this->ds.setConstantBuffer(context);
		this->ds.setDomainShader(context);

		context->GSSetShader( NULL, NULL, 0 );


		COMPTR(ID3D11Buffer) cons;
		cons = this->ps.getGPLSConstantBuffer(0);
		D3D11_MAPPED_SUBRESOURCE ms; // Buffer���i�[����ׂɂƂ肠�������b�N�������Ȃ��Ƃ����Ȃ��B�ǂ��܂Ń��b�N��������H�T�u���\�[�X �f�[�^�ɃA�N�Z�X�ł���悤�ɂ���
		context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // �A�N�Z�X��ms
		rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		context->Unmap(cons.GetInterfacePtr(), NULL); // ���b�N����
		this->ps.setConstantBuffer(context);
		this->ps.setPixelShader(context);
		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}



	DEBUG(device.getAnotation()->EndEvent());


	context->ClearState();
	return true;
}

bool GPLDisplacementMap::DrawShadow(core::PmxModel &model, rinfw::core::math::Matrix44 &world) {
	return this->DrawShadow(model, world, DX11Device::getInstance().getContext());
}
bool GPLDisplacementMap::DrawShadow(core::PmxModel &model, rinfw::core::math::Matrix44 &world, ID3D11DeviceContext *context) {

	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));

	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer[] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // ���_�̃T�C�Y
	UINT offset = 0;			   // ����̒���
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


	this->shadow_om.setStatus(context);

	this->vs.setVertexShader(context);


	//+ Camera����

#ifdef _Debug

	static bool wire = false;
	COMPTR(ID3D11Buffer) cameraconstantbuffer, worldconstantbuffer, heightconstantbuffer,hullconstantbuffer;
	cameraconstantbuffer = this->ds.getGPLSConstantBuffer(0);
	worldconstantbuffer  = this->ds.getGPLSConstantBuffer(1);
	heightconstantbuffer = this->ds.getGPLSConstantBuffer(2);

	rinfw::core::math::Matrix44 worldmtx;

	static TessBuffer tess = {
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,0.0f},
		0,0
	};
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -0.5f);
	static rinfw::core::math::Float3 at(0.0f, 0.15f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static float fov = 45.0f;

	ImGui::Begin("Shadow Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("Wire")) {
		wire = !wire;
	}
	ImGui::DragFloat3("eye", (float*)&eye, 0.025f);
	ImGui::DragFloat3("at", (float*)&at, 0.025f);
	ImGui::DragFloat3("up", (float*)&up, 0.025f);

	ImGui::DragFloat4("tessFactor", (float*)&tess.tessFactor, 0.025f);
	ImGui::DragFloat2("insideFactor", (float*)&tess.insideFactor, 0.025f);
	ImGui::Image(this->shadowtex.getSRV(), ImVec2(256, 256));
	ImGui::End();

	shadow.View = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float)window.getWidth();		// �r���[�|�[�g�̕�
	viewport.Height = (float)window.getHeight();		// �r���[�|�[�g�̍���
	viewport.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	viewport.MaxDepth = 1000.0f;				// �r���[�|�[�g�̍ő�[�x
	viewport.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	viewport.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu

	shadow.Projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 1000.0f);
	shadow.View = rinfw::core::math::matrix44Transpose(shadow.View);
	shadow.Projection = rinfw::core::math::matrix44Transpose(shadow.Projection);

	worldmtx = world;
	worldmtx = rinfw::core::math::matrix44Transpose(worldmtx);

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &shadow, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &worldmtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(heightconstantbuffer.GetInterfacePtr(), 0, NULL, &this->height, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

	this->vs.setConstantBuffer(context);
	this->ps.setSamplerState(context);
#endif

	unsigned long count = 0;
	for (UINT i = 0; i < (UINT)model.mesh.materialCount; i++) {
		//if (model.mesh.materials[i].flag == 0x0) {
		//	count += model.mesh.materials[i].index_count;
		//	continue;
		//}
#ifdef _Debug
		// �`��t���O(8bit) - �ebit 0:OFF 1:ON 0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`��, 0x20:���_�J���[(��2.1�g��), 0x40:Point�`��(��2.1�g��), 0x80:Line�`��(��2.1�g��)
		if (!wire) {
			if (model.mesh.materials[i].flag & 0x1) {
				this->rs_cullnone.setStatus(context);
			}
			else {
				this->rs.setStatus(context);
			}
		}
		else {
			this->rs_cullwire.setStatus(context);
		}
#endif

		ID3D11ShaderResourceView *srv[4] = { nullptr };
		if ((unsigned)model.mesh.materials[i].diffuse_texture_index < model.mesh.texture.size()) {
			srv[0] = model.mesh.texture[model.mesh.materials[i].diffuse_texture_index].getSRV();
		}
		else {
			srv[0] = nullptr;
		}
		if ((unsigned)model.mesh.materials[i].sphere_texture_index < model.mesh.texture.size()) {
			srv[1] = model.mesh.texture[model.mesh.materials[i].sphere_texture_index].getSRV();
		}
		else {
			srv[1] = nullptr;
		}
		if ((unsigned)model.mesh.materials[i].toon_texture_index < model.mesh.texture.size()) {
			srv[2] = model.mesh.texture[model.mesh.materials[i].toon_texture_index].getSRV();
		}
		else {
			srv[2] = nullptr;
		}
		srv[3] = this->shadowtex.getSRV();

		context->VSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->DSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->PSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);

		hullconstantbuffer = this->hs.getGPLSConstantBuffer(0);
		context->UpdateSubresource(hullconstantbuffer.GetInterfacePtr(), 0, NULL, &tess, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

		this->hs.setConstantBuffer(context);
		this->hs.setHullShader(context);
		this->ds.setConstantBuffer(context);
		this->ds.setDomainShader(context);

		context->GSSetShader( NULL, NULL, 0 );


		COMPTR(ID3D11Buffer) cons;
		cons = this->ps.getGPLSConstantBuffer(0);
		D3D11_MAPPED_SUBRESOURCE ms; // Buffer���i�[����ׂɂƂ肠�������b�N�������Ȃ��Ƃ����Ȃ��B�ǂ��܂Ń��b�N��������H�T�u���\�[�X �f�[�^�ɃA�N�Z�X�ł���悤�ɂ���
		context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // �A�N�Z�X��ms
		rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		context->Unmap(cons.GetInterfacePtr(), NULL); // ���b�N����
		this->ps.setConstantBuffer(context);
		this->shadow_ps.setPixelShader(context);
		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}



	DEBUG(device.getAnotation()->EndEvent());


	context->ClearState();
	return true;
}

bool GPLDisplacementMap::BlendDebug() {
	auto device = DX11Device::getInstance().getDevice();
	const char* BLEND_items[] =
	{
		"D3D11_BLEND_ZERO",//1
		"D3D11_BLEND_ONE",
		"D3D11_BLEND_SRC_COLOR",
		"D3D11_BLEND_INV_SRC_COLOR",
		"D3D11_BLEND_SRC_ALPHA",
		"D3D11_BLEND_INV_SRC_ALPHA",
		"D3D11_BLEND_DEST_ALPHA",
		"D3D11_BLEND_INV_DEST_ALPHA",
		"D3D11_BLEND_DEST_COLOR",
		"D3D11_BLEND_INV_DEST_COLOR",
		"D3D11_BLEND_SRC_ALPHA_SAT",
		"D3D11_BLEND_BLEND_FACTOR",
		"D3D11_BLEND_INV_BLEND_FACTOR",
		"D3D11_BLEND_SRC1_COLOR",
		"D3D11_BLEND_INV_SRC1_COLOR",
		"D3D11_BLEND_SRC1_ALPHA",
		"D3D11_BLEND_INV_SRC1_ALPHA",
	};
	const char* BLEND_OP_items[] =
	{
		"D3D11_BLEND_OP_ADD",//1
		"D3D11_BLEND_OP_SUBTRACT",
		"D3D11_BLEND_OP_REV_SUBTRACT",
		"D3D11_BLEND_OP_MIN",
		"D3D11_BLEND_OP_MAX",
	};

	const char* COLOR_WRITE_ENABLE_items[] =
	{
		"D3D11_COLOR_WRITE_ENABLE_RED",
		"D3D11_COLOR_WRITE_ENABLE_GREEN",
		"D3D11_COLOR_WRITE_ENABLE_BLUE",
		"D3D11_COLOR_WRITE_ENABLE_ALPHA",
		"D3D11_COLOR_WRITE_ENABLE_ALL",
	};

	if (ImGui::Begin("BlendTest")) {
		static bool AlphaToCoverageEnable = FALSE;
		static bool IndependentBlendEnable = FALSE;
		static bool BlendEnable = TRUE;
		static float BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static UINT SampleMask = 0xffffffff;
		static int BlendOp = 0;//BLEND_OP
		static int BlendOpAlpha = 0;//BLEND_OP

		static int SrcBlend = 4;//BLEND
		static int SrcBlendAlpha = 1;//BLEND

		static int DestBlend = 8;//BLEND_OP
		static int DestBlendAlpha = 0;//BLEND_OP
		static bool  COLOR_WRITE_ENABLE_RED = false;
		static bool  COLOR_WRITE_ENABLE_GREEN = false;
		static bool  COLOR_WRITE_ENABLE_BLUE = false;
		static bool  COLOR_WRITE_ENABLE_ALPHA = false;
		static bool  COLOR_WRITE_ENABLE_ALL = true;



		ImGui::Checkbox("AlphaToCoverageEnable", &AlphaToCoverageEnable);
		ImGui::Checkbox("IndependentBlendEnable", &IndependentBlendEnable);
		ImGui::Checkbox("BlendEnable", &BlendEnable);

		ImGui::DragFloat4("BlendFactor", BlendFactor, 0.01f, 0.0f, 1.0f);
		int v = SampleMask;
		ImGui::InputInt("SampleMask", &v);
		SampleMask = v;

		ImGui::Combo("BlendOp", &BlendOp, BLEND_OP_items, IM_ARRAYSIZE(BLEND_OP_items));
		ImGui::Combo("BlendOpAlpha", &BlendOpAlpha, BLEND_OP_items, IM_ARRAYSIZE(BLEND_OP_items));
		ImGui::Combo("SrcBlend", &SrcBlend, BLEND_items, IM_ARRAYSIZE(BLEND_items));
		ImGui::Combo("SrcBlendAlpha", &SrcBlendAlpha, BLEND_items, IM_ARRAYSIZE(BLEND_items));
		ImGui::Combo("DestBlend", &DestBlend, BLEND_items, IM_ARRAYSIZE(BLEND_items));
		ImGui::Combo("DestBlendAlpha", &DestBlendAlpha, BLEND_items, IM_ARRAYSIZE(BLEND_items));
		ImGui::Checkbox("COLOR_WRITE_ENABLE_RED", &COLOR_WRITE_ENABLE_RED);
		ImGui::Checkbox("COLOR_WRITE_ENABLE_GREEN", &COLOR_WRITE_ENABLE_GREEN);
		ImGui::Checkbox("COLOR_WRITE_ENABLE_BLUE", &COLOR_WRITE_ENABLE_BLUE);
		ImGui::Checkbox("COLOR_WRITE_ENABLE_ALPHA", &COLOR_WRITE_ENABLE_ALPHA);
		ImGui::Checkbox("COLOR_WRITE_ENABLE_ALL", &COLOR_WRITE_ENABLE_ALL);

		if (ImGui::Button("Update")) {
			ID3D11BlendState* hpBlendState = NULL;
			D3D11_BLEND_DESC BlendStateDesc;
			BlendStateDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
			BlendStateDesc.IndependentBlendEnable = IndependentBlendEnable;
			BlendStateDesc.RenderTarget[0].BlendEnable = BlendEnable;
			BlendStateDesc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)(BlendOp + 1);
			BlendStateDesc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)(BlendOpAlpha + 1);
			BlendStateDesc.RenderTarget[0].SrcBlend = (D3D11_BLEND)(SrcBlend + 1);
			BlendStateDesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)(SrcBlendAlpha + 1);
			BlendStateDesc.RenderTarget[0].DestBlend = (D3D11_BLEND)(DestBlend + 1);
			BlendStateDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)(DestBlendAlpha + 1);
			BlendStateDesc.RenderTarget[0].RenderTargetWriteMask =
				(COLOR_WRITE_ENABLE_RED) ? D3D11_COLOR_WRITE_ENABLE_RED : 0
				| (COLOR_WRITE_ENABLE_GREEN) ? COLOR_WRITE_ENABLE_GREEN : 0
				| (COLOR_WRITE_ENABLE_BLUE) ? COLOR_WRITE_ENABLE_BLUE : 0
				| (COLOR_WRITE_ENABLE_ALPHA) ? COLOR_WRITE_ENABLE_ALPHA : 0
				| (D3D11_COLOR_WRITE_ENABLE_ALL) ? D3D11_COLOR_WRITE_ENABLE_ALL : 0;
			device->CreateBlendState(&BlendStateDesc, &hpBlendState);

			this->om.setGPLSBlendState(hpBlendState, BlendFactor, SampleMask);
		}

		ImGui::End();
	}
	return true;
}
void GPLDisplacementMap::setHeight(core::math::Float4 height) {
	this->height = height;
}

}
}
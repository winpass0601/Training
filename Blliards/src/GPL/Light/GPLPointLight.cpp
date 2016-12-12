#include <./GPL/GPL.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

namespace rinfw {
namespace graphics {

//GPLS�e�X�g�p�R�[�h
GPLPointLight::GPLPointLight() {
}

void GPLPointLight::Release() {
	this->ia.Release();
	this->om.Release();
	this->rs.Release();
	this->vs.Release();
	this->ps.Release();

}
bool GPLPointLight::createPipeLine(ID3D11Device *device) {
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

	D3D11_INPUT_ELEMENT_DESC element[] = { // ���̓A�Z���u���[ �X�e�[�W�̒P��̗v�f( HLSL �Z�}���e�B�N�X,�v�f�̃Z�}���e�B�N�X �C���f�b�N�X,�v�f�f�[�^�̃f�[�^�^,���̓A�Z���u���[�����ʂ��鐮���l,�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��),�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������,�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// POS���
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },// NORMAL���
	};

	ID3D11VertexShader *vs_buf = nullptr;
	ID3D10Blob *vsblob = nullptr; // �C�Ӓ��̃f�[�^��Ԃ��ۂɎg�p
	// �t�@�C�������ɃG�t�F�N�g���R���p�C��
	D3DX11CompileFromFile(TEXT("./Shader/Deferred/Light/VSLight.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
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
		this->vs.setGPLSVertexShader(vs_buf);
	}

	return true;
}
bool GPLPointLight::createIA(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	this->ia.setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
bool GPLPointLight::createOM(ID3D11Device *device) {
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
	BlendStateDesc.RenderTarget[0].BlendEnable = TRUE;

	device->CreateBlendState(&BlendStateDesc, &hpBlendState);

	//���̃u�����f�B���O���R���e�L�X�g�ɐݒ�
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->om.setGPLSBlendState(hpBlendState, blendFactor, 0xffffffff);
	this->om.setGPLSRenderTargetView(0, GPLDeferred::getInstance().getRTVLightBuffer());
	this->om.setGPLSDepthStencilView(DX11Device::getInstance().getCOMDSV());

	return true;
}
bool GPLPointLight::createRS(ID3D11Device *device) {
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
	this->rs_wire.setGPLSViewPort(0, vp);

	ID3D11RasterizerState* hpRasterizerState = NULL;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
			D3D11_FILL_SOLID,
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
	hRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&hRasterizerDesc, &hpRasterizerState);
	this->rs_wire.setGPLSRasterizerState(hpRasterizerState);
	return true;
}
bool GPLPointLight::createVS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}


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
	this->vs.setGPLSConstantBuffer(0, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->vs.setGPLSConstantBuffer(1, constantbuffer);


	return true;
}
bool GPLPointLight::createPS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	ID3D11PixelShader  *ps_buf = nullptr;
	ID3D10Blob *psblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/Deferred/Light/PSPointLight.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // �s�N�Z�� �V�F�[�_�[���쐬
	this->ps.setGPLSPixelShader(ps_buf);
	psblob->Release();

	//�}�e���A���������邽�߂̃R���e�L�X�g�̐���

	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // �������@(�o�b�t�@�[ ���\�[�X)
	ZeroMemory(&bd, sizeof(bd)); // ���g���[���N���A
	// Buffer�̐������@�̊i�[
	bd.Usage = D3D11_USAGE_DEFAULT; // �o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	bd.CPUAccessFlags = 0; // CPU����͏������݂̂ݍs���܂�
	bd.ByteWidth = sizeof(PointStruct);  // �o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
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
	COMPTR(ID3D11SamplerState) comsamp;
	device->CreateSamplerState(&sampDesc, &samp);
	comsamp.Attach(samp);
	this->ps.setGPLSSamplerState(0, comsamp);

	this->ps.setGPLSShaderResourceView(0, GPLDeferred::getInstance().getSRVNormalBuffer());
	this->ps.setGPLSShaderResourceView(1, GPLDeferred::getInstance().getSRVWorldBuffer());
	return true;
}
bool GPLPointLight::Draw(core::PmxModel &model) {
	this->Draw(model, DX11Device::getInstance().getContext());
	return true;
}
bool GPLPointLight::Draw(core::PmxModel & model, rinfw::core::math::Matrix44 &world) {
	this->Draw(model, world, DX11Device::getInstance().getContext());
	return true;
}
bool GPLPointLight::Draw(core::PmxModel & model, rinfw::core::math::Matrix44 &world,ID3D11DeviceContext *context) {

	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();

	rinfw::core::math::Matrix44 view;
	rinfw::core::math::Matrix44 projection;

	DEBUG(static bool wire = false);
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -0.1f);
	static rinfw::core::math::Float3 at(0.0f, 0.15f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static float fov = 45.0f;

	ImGui::Begin("Model Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
	DEBUG(if (ImGui::Button("Wire")) { wire = !wire; });
	ImGui::DragFloat3("eye", (float*)&eye, 0.025f);
	ImGui::DragFloat3("at", (float*)&at, 0.025f);
	ImGui::DragFloat3("up", (float*)&up, 0.025f);
	ImGui::End();


	view = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float)window.getWidth();		// �r���[�|�[�g�̕�
	viewport.Height = (float)window.getHeight();		// �r���[�|�[�g�̍���
	viewport.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	viewport.MaxDepth = 1.0f;				// �r���[�|�[�g�̍ő�[�x
	viewport.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	viewport.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu

	projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 1.0f);
	view = rinfw::core::math::matrix44Transpose(view);
	projection = rinfw::core::math::matrix44Transpose(projection);


	this->Draw(model,view,projection,world, context ,wire);
	return true;
}
bool GPLPointLight::Draw(core::PmxModel &model, ID3D11DeviceContext *context) {

	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();

	rinfw::core::math::Matrix44 view;
	rinfw::core::math::Matrix44 projection;
	rinfw::core::math::Matrix44 worldmtx;

	DEBUG(static bool wire = false);
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -0.1f);
	static rinfw::core::math::Float3 at(0.0f, 0.15f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static rinfw::core::math::Float3 pos;
	static rinfw::core::math::Float3 scale(1.0f, 1.0f, 1.0f);
	static float fov = 45.0f;

	ImGui::Begin("Model Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
	DEBUG(if (ImGui::Button("Wire")) { wire = !wire; });
	ImGui::DragFloat3("eye", (float*)&eye, 0.025f);
	ImGui::DragFloat3("at", (float*)&at, 0.025f);
	ImGui::DragFloat3("up", (float*)&up, 0.025f);

	ImGui::DragFloat3("pos", (float*)&pos, 0.025f);
	ImGui::DragFloat3("scale", (float*)&scale, 0.025f);

	ImGui::End();


	view = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float)window.getWidth();		// �r���[�|�[�g�̕�
	viewport.Height = (float)window.getHeight();		// �r���[�|�[�g�̍���
	viewport.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	viewport.MaxDepth = 1.0f;				// �r���[�|�[�g�̍ő�[�x
	viewport.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	viewport.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu

	projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 1000.0f);
	view = rinfw::core::math::matrix44Transpose(view);
	projection = rinfw::core::math::matrix44Transpose(projection);

	rinfw::core::math::matrix44Translation(worldmtx, pos.x, pos.y, pos.z);
	worldmtx = worldmtx * rinfw::core::math::matrix44Scaling(scale.x, scale.y, scale.z);
	worldmtx = worldmtx;

	this->Draw(model,view,projection,worldmtx, context ,wire);
	return true;
}
bool GPLPointLight::Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &viewmtx,rinfw::core::math::Matrix44 &projectionmtx,rinfw::core::math::Matrix44 &worldmtx,ID3D11DeviceContext *context ,bool wireframe) {
	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));

	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer[] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // ���_�̃T�C�Y
	UINT offset = 0;									// ����̒���
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	this->vs.setVertexShader(context);

	COMPTR(ID3D11Buffer) cameraconstantbuffer, worldconstantbuffer;
	cameraconstantbuffer = this->vs.getGPLSConstantBuffer(0);
	worldconstantbuffer = this->vs.getGPLSConstantBuffer(1);
	struct _Camera {
		rinfw::core::math::Matrix44 View;
		rinfw::core::math::Matrix44 Projection;
	};
	_Camera cameramtx;
	cameramtx.View = viewmtx;
	cameramtx.Projection = projectionmtx;

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(worldmtx), 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

	this->vs.setConstantBuffer(context);
	this->om.setStatus(context);
	this->ps.setSamplerState(context);
	unsigned long count = 0;
	for (UINT i = 0; i < (UINT)model.mesh.materialCount; i++) {
		if (model.mesh.materials[i].flag == 0x0) {
			count += model.mesh.materials[i].index_count;
			continue;
		}
		this->rs.setStatus(context);

		//ID3D11ShaderResourceView *srv[4] = { nullptr };
		//if ((unsigned)model.mesh.materials[i].diffuse_texture_index < model.mesh.texture.size()) {
		//	srv[0] = model.mesh.texture[model.mesh.materials[i].diffuse_texture_index].getSRV();
		//}
		//else {
		//	srv[0] = nullptr;
		//}
		//if ((unsigned)model.mesh.materials[i].sphere_texture_index < model.mesh.texture.size()) {
		//	srv[1] = model.mesh.texture[model.mesh.materials[i].sphere_texture_index].getSRV();
		//}
		//else {
		//	srv[1] = nullptr;
		//}
		//
		//if ((unsigned)model.mesh.materials[i].toon_texture_index < model.mesh.texture.size()) {
		//	srv[2] = model.mesh.texture[model.mesh.materials[i].toon_texture_index].getSRV();
		//}
		//else {
		//	srv[2] = nullptr;
		//}
		//context->VSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		//context->PSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);


		//COMPTR(ID3D11Buffer) cons;
		//cons = this->ps.getGPLSConstantBuffer(0);

		//D3D11_MAPPED_SUBRESOURCE ms; // Buffer���i�[����ׂɂƂ肠�������b�N�������Ȃ��Ƃ����Ȃ��B�ǂ��܂Ń��b�N��������H�T�u���\�[�X �f�[�^�ɃA�N�Z�X�ł���悤�ɂ���
		//context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // �A�N�Z�X��ms
		//rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		//memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		//memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		//memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		//memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		//memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		//memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		//context->Unmap(cons.GetInterfacePtr(), NULL); // ���b�N����

		//this->ps.setPixelShader(context);

		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}

	DEBUG(device.getAnotation()->EndEvent());
	return true;
}

bool GPLPointLight::Draw(core::PmxModel &model, rinfw::core::math::Matrix44 &world, rinfw::graphics::Camera &camera) {
	return this->Draw(model,world,camera,DX11Device::getInstance().getContext());
}

bool GPLPointLight::Draw(core::PmxModel &model, rinfw::core::math::Matrix44 &worldmtx, rinfw::graphics::Camera &camera, ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));

	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer[] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // ���_�̃T�C�Y
	UINT offset = 0;									// ����̒���
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	this->vs.setVertexShader(context);

	COMPTR(ID3D11Buffer) worldconstantbuffer,pointconstantbuffer;
	pointconstantbuffer = this->ps.getGPLSConstantBuffer(0);
	worldconstantbuffer = this->vs.getGPLSConstantBuffer(1);
	PointStruct point;
	point.window[0] = 1.0f / (float)WindowDevice::getInstance().getWidth();
	point.window[1] = 1.0f / (float)WindowDevice::getInstance().getHeight();
	point.lightlen = 10.0f;
	point.pos = rinfw::core::math::Float3(worldmtx._41, worldmtx._42, worldmtx._43);
	point.color = rinfw::core::math::Float3(1.0f,1.0f,1.0f);
	context->UpdateSubresource(pointconstantbuffer.GetInterfacePtr(), 0, NULL, &point, 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(worldmtx), 0, 0); // CPU �ɂ���āA�}�b�s���O�s�\�ȃ������[���ɍ쐬���ꂽ�T�u���\�[�X�Ƀ������[����f�[�^���R�s�[�����

	ID3D11Buffer *constantbuffer[] = {
		camera.getBuffer().GetInterfacePtr(),
		worldconstantbuffer,
	};

	context->VSSetConstantBuffers(0, 2, constantbuffer);


	this->om.setStatus(context);

	this->ps.setStatus(context);
	if (camera.getWireframe())
		this->rs_wire.setStatus(context);
	else
		this->rs.setStatus(context);
	unsigned long count = 0;
	for (UINT i = 0; i < (UINT)model.mesh.materialCount; i++) {
		if (model.mesh.materials[i].flag == 0x0) {
			count += model.mesh.materials[i].index_count;
			continue;
		}

		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}

	DEBUG(device.getAnotation()->EndEvent());
	return true;
}

}
}
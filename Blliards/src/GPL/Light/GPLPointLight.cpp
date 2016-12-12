#include <./GPL/GPL.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

namespace rinfw {
namespace graphics {

//GPLSテスト用コード
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

	D3D11_INPUT_ELEMENT_DESC element[] = { // 入力アセンブラー ステージの単一の要素( HLSL セマンティクス,要素のセマンティクス インデックス,要素データのデータ型,入力アセンブラーを識別する整数値,各要素間のオフセット (バイト単位),単一の入力スロットの入力データ クラスを識別,インスタンス単位の同じデータを使用して描画するインスタンスの数)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// POS情報
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },// NORMAL情報
	};

	ID3D11VertexShader *vs_buf = nullptr;
	ID3D10Blob *vsblob = nullptr; // 任意長のデータを返す際に使用
	// ファイルを元にエフェクトをコンパイル
	D3DX11CompileFromFile(TEXT("./Shader/Deferred/Light/VSLight.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
	// blobを_bufに格納
	device->CreateVertexShader(vsblob->GetBufferPointer(), vsblob->GetBufferSize(), nullptr, &vs_buf); // コンパイル済みシェーダーから、頂点シェーダー オブジェクトを作成


	//頂点レイアウト作成
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
		//MMDブレンド方法
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

	//そのブレンディングをコンテキストに設定
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
			D3D11_CULL_NONE,	//ポリゴンの裏表を無くす
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
	D3D11_BUFFER_DESC bd; // 生成方法(バッファー リソース)
	ZeroMemory(&bd, sizeof(bd)); // 中身をゼロクリア
	// Bufferの生成方法の格納
	bd.Usage = D3D11_USAGE_DEFAULT; // バッファーで想定されている読み込みおよび書き込みの方法を識別
	bd.CPUAccessFlags = 0; // CPUからは書き込みのみ行います
	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44) * 2;  // バッファーのサイズ(バイト単位)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // なんのバッファですか？
	bd.StructureByteStride = sizeof(float);

	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->vs.setGPLSConstantBuffer(0, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44);  // バッファーのサイズ(バイト単位)
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
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // ピクセル シェーダーを作成
	this->ps.setGPLSPixelShader(ps_buf);
	psblob->Release();

	//マテリアル情報を入れるためのコンテキストの生成

	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // 生成方法(バッファー リソース)
	ZeroMemory(&bd, sizeof(bd)); // 中身をゼロクリア
	// Bufferの生成方法の格納
	bd.Usage = D3D11_USAGE_DEFAULT; // バッファーで想定されている読み込みおよび書き込みの方法を識別
	bd.CPUAccessFlags = 0; // CPUからは書き込みのみ行います
	bd.ByteWidth = sizeof(PointStruct);  // バッファーのサイズ(バイト単位)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // なんのバッファですか？
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ps.setGPLSConstantBuffer(0, constantbuffer);


	D3D11_SAMPLER_DESC sampDesc;
	//サンプラーの設定
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
	viewport.Width = (float)window.getWidth();		// ビューポートの幅
	viewport.Height = (float)window.getHeight();		// ビューポートの高さ
	viewport.MinDepth = 0.0f;				// ビューポートの最小深度
	viewport.MaxDepth = 1.0f;				// ビューポートの最大深度
	viewport.TopLeftX = 0;				// ビューポートの左側の X 位置
	viewport.TopLeftY = 0;				// ビューポートの上部の Y 位置

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
	viewport.Width = (float)window.getWidth();		// ビューポートの幅
	viewport.Height = (float)window.getHeight();		// ビューポートの高さ
	viewport.MinDepth = 0.0f;				// ビューポートの最小深度
	viewport.MaxDepth = 1.0f;				// ビューポートの最大深度
	viewport.TopLeftX = 0;				// ビューポートの左側の X 位置
	viewport.TopLeftY = 0;				// ビューポートの上部の Y 位置

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
	UINT stride = sizeof(rinfw::core::ConstantVertex); // 頂点のサイズ
	UINT offset = 0;									// ずれの調整
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

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(worldmtx), 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる

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

		//D3D11_MAPPED_SUBRESOURCE ms; // Bufferを格納する為にとりあえずロックをかけないといけない。どこまでロックをかける？サブリソース データにアクセスできるようにする
		//context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // アクセス先ms
		//rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		//memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		//memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		//memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		//memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		//memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		//memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		//context->Unmap(cons.GetInterfacePtr(), NULL); // ロック解除

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
	UINT stride = sizeof(rinfw::core::ConstantVertex); // 頂点のサイズ
	UINT offset = 0;									// ずれの調整
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
	context->UpdateSubresource(pointconstantbuffer.GetInterfacePtr(), 0, NULL, &point, 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(worldmtx), 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる

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
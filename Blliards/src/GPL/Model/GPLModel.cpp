#include <./GPL/GPL.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

namespace rinfw {
namespace graphics {

//GPLSテスト用コード
GPLModel::GPLModel() {
	this->point.num[0] = 0;
	this->point.num[1] = 0;

	for (int i = 0; i < 30; i++) {
		this->point.pos[i] = rinfw::core::math::Float4(0.0, 0.0, 0.0, 0.3f);
		this->point.color[i] = rinfw::core::math::Float4(0.3, 0.3, 0.3, 1.0f);

	}
	for (int i = 0; i < 4; i++) {
		this->spot.pos[i] = rinfw::core::math::Float4(0.0, 0.0, 0.0, 0.3f);
		this->spot.color[i] = rinfw::core::math::Float4(0.3, 0.3, 0.3, 1.0f);
		this->spot.dir[i] = rinfw::core::math::Float4(0.0, 1.0, 0.0, 0.8f);

	}


	this->spot.pos[0] = rinfw::core::math::Float4(-1.0, -0.2, 0.0, 100.0f);
	this->spot.dir[0] = rinfw::core::math::Float4(-1.0, -0.5, 0.0, 0.8f);
	this->spot.color[0] = rinfw::core::math::Float4(0.0, 0.3, 0.0, 1.0f);

	this->spot.pos[1] = rinfw::core::math::Float4(1.0, -0.2, 0.0, 100.0f);
	this->spot.dir[1] = rinfw::core::math::Float4(1.0, -0.5, 0.0, 0.8f);
	this->spot.color[1] = rinfw::core::math::Float4(0.3, 0.0, 0.0, 1.0f);

	this->spot.pos[2] = rinfw::core::math::Float4(0.0, -0.2, 1.0, 100.0f);
	this->spot.dir[2] = rinfw::core::math::Float4(0.0, -0.5, 0.8, 0.8f);
	this->spot.color[2] = rinfw::core::math::Float4(0.0, 0.0, 0.3, 1.0f);

}

void GPLModel::Release() {
	this->ia.Release();
	this->om.Release();
	this->rs.Release();
	this->vs.Release();
	this->ps.Release();
	this->ps_deferred.Release();

}
bool GPLModel::createPipeLine(ID3D11Device *device) {
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

	D3D11_INPUT_ELEMENT_DESC element [] = { // 入力アセンブラー ステージの単一の要素( HLSL セマンティクス,要素のセマンティクス インデックス,要素データのデータ型,入力アセンブラーを識別する整数値,各要素間のオフセット (バイト単位),単一の入力スロットの入力データ クラスを識別,インスタンス単位の同じデータを使用して描画するインスタンスの数)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// POS情報
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },// NORMAL情報
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,		12 + 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV情報
		{ "UVA"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV情報
		{ "UVA"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV情報
		{ "UVA"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16 + 16 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV情報
		{ "UVA"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12 + 12 + 8 + 16 + 16 + 16 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // UV情報
	};

	ID3D11VertexShader *vs_buf = nullptr;
	ID3D10Blob *vsblob = nullptr; // 任意長のデータを返す際に使用
	// ファイルを元にエフェクトをコンパイル
	D3DX11CompileFromFile(TEXT("./Shader/Model/Pmx/VS.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
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
		this->vs.setGPLSShader(vs_buf);
	}

	return true;
}
bool GPLModel::createIA(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	this->ia.setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
bool GPLModel::createOM(ID3D11Device *device) {
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
	this->om.setGPLSRenderTargetView(0, DX11Device::getInstance().getCOMRTV());
	this->om.setGPLSDepthStencilView(DX11Device::getInstance().getCOMDSV());

	return true;
}
bool GPLModel::createRS(ID3D11Device *device) {
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
	this->rs_cullnone.setGPLSViewPort(0, vp);

	ID3D11RasterizerState* hpRasterizerState = NULL;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
			D3D11_FILL_SOLID,
			D3D11_CULL_BACK,	//ポリゴンの裏表を無くす
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
bool GPLModel::createVS(ID3D11Device *device) {
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
	this->vs.setGPLSBuffer(0, constantbuffer);

	bd.ByteWidth = sizeof(rinfw::core::math::Matrix44);  // バッファーのサイズ(バイト単位)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->vs.setGPLSBuffer(1, constantbuffer);


	return true;
}
bool GPLModel::createPS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	ID3D11PixelShader  *ps_buf = nullptr, *ps_buf_deferred = nullptr;
	ID3D10Blob *psblob = nullptr, *psblob_deferred = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/Model/Pmx/PS.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	D3DX11CompileFromFile(TEXT("./Shader/Deferred/Model/Pmx/PS.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob_deferred, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // ピクセル シェーダーを作成
	device->CreatePixelShader(psblob_deferred->GetBufferPointer(), psblob_deferred->GetBufferSize(), nullptr, &ps_buf_deferred);  // ピクセル シェーダーを作成
	this->ps.setGPLSShader(ps_buf);
	this->ps_deferred.setGPLSShader(ps_buf_deferred);
	psblob->Release();
	psblob_deferred->Release();

	//マテリアル情報を入れるためのコンテキストの生成

	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // 生成方法(バッファー リソース)
	ZeroMemory(&bd, sizeof(bd)); // 中身をゼロクリア
	// Bufferの生成方法の格納
	bd.Usage = D3D11_USAGE_DYNAMIC; // バッファーで想定されている読み込みおよび書き込みの方法を識別
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPUからは書き込みのみ行います
	bd.ByteWidth = sizeof(rinfw::core::ConstantMaterial);  // バッファーのサイズ(バイト単位)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // なんのバッファですか？
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ps.setGPLSBuffer(0, constantbuffer);

	bd.Usage = D3D11_USAGE_DEFAULT; // バッファーで想定されている読み込みおよび書き込みの方法を識別
	bd.CPUAccessFlags = 0; // CPUからは書き込みのみ行います

	bd.ByteWidth = sizeof(PointLight);  // バッファーのサイズ(バイト単位)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ps.setGPLSBuffer(1, constantbuffer);

	bd.ByteWidth = sizeof(SpotLight);  // バッファーのサイズ(バイト単位)
	device->CreateBuffer(&bd, nullptr, &constantbuffer);
	this->ps.setGPLSBuffer(2, constantbuffer);



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
	this->ps.setGPLSSamplerState(1, comsamp);
	this->ps.setGPLSSamplerState(2, comsamp);

	this->ps_deferred.setGPLSSamplerState(0, comsamp);
	this->ps_deferred.setGPLSSamplerState(1, comsamp);
	this->ps_deferred.setGPLSSamplerState(2, comsamp);

	return true;
}
bool GPLModel::Draw(core::PmxModel &model) {
	this->Draw(model, DX11Device::getInstance().getContext());
	return true;
}
bool GPLModel::Draw(core::PmxModel & model, rinfw::core::math::Matrix44 &world) {
	this->Draw(model, world, DX11Device::getInstance().getContext());
	return true;
}
bool GPLModel::Draw(core::PmxModel & model, rinfw::core::math::Matrix44 &world, rinfw::graphics::Camera &camera) {
	this->Draw(model, world, camera, DX11Device::getInstance().getContext());
	return true;
}
bool GPLModel::Draw(core::PmxModel & model, rinfw::core::math::Matrix44 &world, ID3D11DeviceContext *context) {

	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();

	rinfw::core::math::Matrix44 view;
	rinfw::core::math::Matrix44 projection;

	static bool wire = false;
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -7.0f);
	static rinfw::core::math::Float3 at(0.0f, 0.0f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static float fov = 45.0f;

	if (model.model_name == L"白球") {
		ImGui::Begin("Model Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
		DEBUG(if (ImGui::Button("Wire")) { wire = !wire; });
		ImGui::DragFloat3("eye", (float*) &eye, 0.025f);
		ImGui::DragFloat3("at", (float*) &at, 0.025f);
		ImGui::DragFloat3("up", (float*) &up, 0.025f);
		ImGui::End();
	}

	view = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float) window.getWidth();		// ビューポートの幅
	viewport.Height = (float) window.getHeight();		// ビューポートの高さ
	viewport.MinDepth = 0.0f;				// ビューポートの最小深度
	viewport.MaxDepth = 1.0f;				// ビューポートの最大深度
	viewport.TopLeftX = 0;				// ビューポートの左側の X 位置
	viewport.TopLeftY = 0;				// ビューポートの上部の Y 位置

	projection = rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(fov), viewport.Width / viewport.Height, 0.1f, 10.0f);
	view = rinfw::core::math::matrix44Transpose(view);
	projection = rinfw::core::math::matrix44Transpose(projection);


	this->Draw(model, view, projection, world, context, wire);
	return true;
}
bool GPLModel::Draw(core::PmxModel &model, ID3D11DeviceContext *context) {

	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();

	rinfw::core::math::Matrix44 view;
	rinfw::core::math::Matrix44 projection;
	rinfw::core::math::Matrix44 worldmtx;

	static bool wire = false;
	static rinfw::core::math::Float3 eye(0.0f, 5.0f, -0.1f);
	static rinfw::core::math::Float3 at(0.0f, 0.15f, 0.0f);
	static rinfw::core::math::Float3 up(0.0f, 1.0f, 0.0f);
	static rinfw::core::math::Float3 pos;
	static rinfw::core::math::Float3 scale(1.0f, 1.0f, 1.0f);
	static float fov = 45.0f;


	ImGui::Begin("Model Camera and World", nullptr, ImGuiWindowFlags_NoScrollbar);
	DEBUG(if (ImGui::Button("Wire")) { wire = !wire; });
	ImGui::DragFloat3("eye", (float*) &eye, 0.025f);
	ImGui::DragFloat3("at", (float*) &at, 0.025f);
	ImGui::DragFloat3("up", (float*) &up, 0.025f);

	ImGui::DragFloat3("pos", (float*) &pos, 0.025f);
	ImGui::DragFloat3("scale", (float*) &scale, 0.025f);

	ImGui::End();


	view = rinfw::core::math::matrix44LookAtLH(eye, at, up);

	D3D11_VIEWPORT viewport;
	WindowDevice &window = WindowDevice::getInstance();
	viewport.Width = (float) window.getWidth();		// ビューポートの幅
	viewport.Height = (float) window.getHeight();		// ビューポートの高さ
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

	this->Draw(model, view, projection, worldmtx, context, wire);
	return true;
}
bool GPLModel::Draw(core::PmxModel &model, rinfw::core::math::Matrix44 &viewmtx, rinfw::core::math::Matrix44 &projectionmtx, rinfw::core::math::Matrix44 &worldmtx, ID3D11DeviceContext *context, bool wireframe) {
	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));
	//this->BlendDebug();
	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer [] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // 頂点のサイズ
	UINT offset = 0;									// ずれの調整
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	this->vs.setShader(context);

	COMPTR(ID3D11Buffer) cameraconstantbuffer, worldconstantbuffer;
	cameraconstantbuffer = this->vs.getGPLSBuffer(0);
	worldconstantbuffer = this->vs.getGPLSBuffer(1);
	struct _Camera {
		rinfw::core::math::Matrix44 View;
		rinfw::core::math::Matrix44 Projection;
	};
	_Camera cameramtx;
	cameramtx.View = viewmtx;
	cameramtx.Projection = projectionmtx;

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(worldmtx), 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる

	this->vs.setBuffertoVS(context);
	if (gpldeferred.getActive()) {
		this->ps_deferred.setSamplerStatetoPS(context);
		gpldeferred.setStatus(context);
	}
	else {
		this->om.setStatus(context);
		this->ps.setSamplerStatetoPS(context);
	}

	unsigned long count = 0;
	for (UINT i = 0; i < (UINT) model.mesh.materialCount; i++) {
		if (model.mesh.materials[i].flag == 0x0) {
			count += model.mesh.materials[i].index_count;
			continue;
		}
		//#ifdef _Debug
				// 描画フラグ(8bit) - 各bit 0:OFF 1:ON 0x01:両面描画, 0x02:地面影, 0x04:セルフシャドウマップへの描画, 0x08:セルフシャドウの描画, 0x10:エッジ描画, 0x20:頂点カラー(※2.1拡張), 0x40:Point描画(※2.1拡張), 0x80:Line描画(※2.1拡張)
		if (!wireframe) {
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
		//#endif

		ID3D11ShaderResourceView *srv[4] = { nullptr };
		if ((unsigned) model.mesh.materials[i].diffuse_texture_index < model.mesh.texture.size()) {
			srv[0] = model.mesh.texture[model.mesh.materials[i].diffuse_texture_index].getSRV();
		}
		else {
			srv[0] = nullptr;
		}
		if ((unsigned) model.mesh.materials[i].sphere_texture_index < model.mesh.texture.size()) {
			srv[1] = model.mesh.texture[model.mesh.materials[i].sphere_texture_index].getSRV();
		}
		else {
			srv[1] = nullptr;
		}

		if ((unsigned) model.mesh.materials[i].toon_texture_index < model.mesh.texture.size()) {
			srv[2] = model.mesh.texture[model.mesh.materials[i].toon_texture_index].getSRV();
		}
		else {
			srv[2] = nullptr;
		}
		context->VSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->PSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);


		COMPTR(ID3D11Buffer) cons;
		cons = this->ps.getGPLSBuffer(0);

		D3D11_MAPPED_SUBRESOURCE ms; // Bufferを格納する為にとりあえずロックをかけないといけない。どこまでロックをかける？サブリソース データにアクセスできるようにする
		context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // アクセス先ms
		rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		context->Unmap(cons.GetInterfacePtr(), NULL); // ロック解除

		if (gpldeferred.getActive()) {
			this->ps_deferred.setShader(context);
		}
		else {
			this->ps.setShader(context);
		}

		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}

	DEBUG(device.getAnotation()->EndEvent());
	return true;
}
bool GPLModel::Draw(core::PmxModel &model, rinfw::core::math::Matrix44 &worldmtx, rinfw::graphics::Camera &camera, ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	DEBUG(DX11Device &device = DX11Device::getInstance());
	GPLDeferred &gpldeferred = GPLDeferred::getInstance();
	DEBUG(device.getAnotation()->BeginEvent(model.model_name.c_str()));
	//this->BlendDebug();
	context->ClearState();
	this->ia.setInputLayout(context);
	this->ia.setTopology(context);

	ID3D11Buffer *vertexbuffer [] = {
		model.getVertexBuffer()
	};
	UINT stride = sizeof(rinfw::core::ConstantVertex); // 頂点のサイズ
	UINT offset = 0;									// ずれの調整
	context->IASetVertexBuffers(0, 1, vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(model.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	this->vs.setShader(context);

	COMPTR(ID3D11Buffer) worldconstantbuffer;
	worldconstantbuffer = this->vs.getGPLSBuffer(1);
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(worldmtx), 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる
	ID3D11Buffer *constantbuf [] = {
		camera.getBuffer().GetInterfacePtr(),
		worldconstantbuffer,
	};
	context->VSSetConstantBuffers(0, 2, constantbuf);

	//Light
	if (!gpldeferred.getActive()) {
		ImGui::Begin("PointLight");
		ImGui::InputInt("Num", &this->point.num[0]);
		this->point.num[0] = (this->point.num[0] <= 0) ? 0 : this->point.num[0];

		for (int i = 0; i < this->point.num[0]; i++) {
			ImGui::PushID(&this->point.pos[i]);
			ImGui::DragFloat3("Pos", (float*) &this->point.pos[i], 0.001);
			ImGui::DragFloat("R", (float*) &this->point.pos[i].w, 0.001);
			ImGui::DragFloat("F", (float*) &this->point.color[i].w, 0.001);
			ImGui::DragFloat3("Color", (float*) &this->point.color[i], 0.001);
			ImGui::Text("-----------------------------------------------------");
			ImGui::PopID();
		}
		ImGui::End();

		rinfw::graphics::CameraResource cr = camera.getCameraResource();
		//座標変換
		PointLight p;
		p = this->point;
		for (int i = 0; i < this->point.num[0]; i++) {
			rinfw::core::math::Matrix44 pos;
			rinfw::core::math::matrix44Translation(pos, rinfw::core::math::Float3(this->point.pos[i].x, this->point.pos[i].y, this->point.pos[i].z));
			pos = rinfw::core::math::matrix44Transpose(pos);
			p.pos[i] = rinfw::core::math::Float4(pos._14, pos._24, pos._34, this->point.pos[i].w);
		}

		ImGui::Begin("SpotLight");
		ImGui::InputInt("Num", &this->spot.num[0]);
		this->spot.num[0] = (this->spot.num[0] <= 0) ? 0 : this->spot.num[0];
		for (int i = 0; i < this->spot.num[0]; i++) {
			ImGui::PushID(&this->spot.pos[i]);
			ImGui::DragFloat3("Pos", (float*) &this->spot.pos[i], 0.001);
			ImGui::DragFloat3("dir", (float*) &this->spot.dir[i], 0.001);
			ImGui::DragFloat("R", (float*) &this->spot.pos[i].w, 0.001);
			ImGui::DragFloat("F", (float*) &this->spot.color[i].w, 0.001);
			ImGui::DragFloat("cone", (float*) &this->spot.dir[i].w, 0.001);
			ImGui::DragFloat4("diffuse", (float*) &this->spot.color[i], 0.001);
			ImGui::Text("-----------------------------------------------------");
			ImGui::PopID();
		}
		ImGui::End();

		//座標変換
		SpotLight s;
		s = this->spot;
		for (int i = 0; i < this->spot.num[0]; i++) {
			rinfw::core::math::Matrix44 pos;
			rinfw::core::math::matrix44Translation(pos, rinfw::core::math::Float3(this->spot.pos[i].x, this->spot.pos[i].y, this->spot.pos[i].z));
			pos = rinfw::core::math::matrix44Transpose(pos);
			s.pos[i] = rinfw::core::math::Float4(pos._14, pos._24, pos._34, this->spot.pos[i].w);

			rinfw::core::math::Matrix44 vec;
			rinfw::core::math::matrix44Translation(vec, rinfw::core::math::Float3(this->spot.dir[i].x, this->spot.dir[i].y, this->spot.dir[i].z));
			vec = rinfw::core::math::matrix44Transpose(vec);
			s.dir[i] = rinfw::core::math::Float4(vec._14, vec._24, vec._34, this->spot.dir[i].w);
		}
		//p.num[1] = this->spot.num[0];
		COMPTR(ID3D11Buffer) buf = this->ps.getGPLSBuffer(1), buf2 = this->ps.getGPLSBuffer(2);
		context->UpdateSubresource(buf.GetInterfacePtr(), 0, NULL, &p, 0, 0);
		context->UpdateSubresource(buf2.GetInterfacePtr(), 0, NULL, &s, 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる


	}


	if (gpldeferred.getActive()) {
		this->ps_deferred.setSamplerStatetoPS(context);
		gpldeferred.setStatus(context);
	}
	else {
		this->om.setStatus(context);
		this->ps.setSamplerStatetoPS(context);
	}

	unsigned long count = 0;
	for (UINT i = 0; i < (UINT) model.mesh.materialCount; i++) {
		if (model.mesh.materials[i].flag == 0x0) {
			count += model.mesh.materials[i].index_count;
			continue;
		}
#ifdef _Debug
		// 描画フラグ(8bit) - 各bit 0:OFF 1:ON 0x01:両面描画, 0x02:地面影, 0x04:セルフシャドウマップへの描画, 0x08:セルフシャドウの描画, 0x10:エッジ描画, 0x20:頂点カラー(※2.1拡張), 0x40:Point描画(※2.1拡張), 0x80:Line描画(※2.1拡張)
		if (!camera.getWireframe()) {
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
		if ((unsigned) model.mesh.materials[i].diffuse_texture_index < model.mesh.texture.size()) {
			srv[0] = model.mesh.texture[model.mesh.materials[i].diffuse_texture_index].getSRV();
		}
		else {
			srv[0] = nullptr;
		}
		if ((unsigned) model.mesh.materials[i].sphere_texture_index < model.mesh.texture.size()) {
			srv[1] = model.mesh.texture[model.mesh.materials[i].sphere_texture_index].getSRV();
		}
		else {
			srv[1] = nullptr;
		}

		if ((unsigned) model.mesh.materials[i].toon_texture_index < model.mesh.texture.size()) {
			srv[2] = model.mesh.texture[model.mesh.materials[i].toon_texture_index].getSRV();
		}
		else {
			srv[2] = nullptr;
		}
		context->VSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);
		context->PSSetShaderResources(0, sizeof(srv) / sizeof(ID3D11ShaderResourceView*), srv);


		COMPTR(ID3D11Buffer) cons;
		cons = this->ps.getGPLSBuffer(0);

		D3D11_MAPPED_SUBRESOURCE ms; // Bufferを格納する為にとりあえずロックをかけないといけない。どこまでロックをかける？サブリソース データにアクセスできるようにする
		context->Map(cons.GetInterfacePtr(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // アクセス先ms
		rinfw::core::ConstantMaterial *pmaterial = (rinfw::core::ConstantMaterial*)ms.pData;
		memcpy(pmaterial->diffuse, model.mesh.materials[i].diffuse, sizeof(float) * 4);
		memcpy(pmaterial->specular, model.mesh.materials[i].specular, sizeof(float) * 3);
		memcpy(&pmaterial->specularlity, &model.mesh.materials[i].specularlity, sizeof(float));
		memcpy(pmaterial->ambient, model.mesh.materials[i].ambient, sizeof(float) * 3);
		memcpy(pmaterial->edge_color, model.mesh.materials[i].edge_color, sizeof(float) * 4);
		memcpy(&pmaterial->edge_size, &model.mesh.materials[i].edge_size, sizeof(float));
		context->Unmap(cons.GetInterfacePtr(), NULL); // ロック解除

		if (gpldeferred.getActive()) {
			this->ps_deferred.setShader(context);
		}
		else {
			this->ps.setBuffertoPS(context);
			this->ps.setShader(context);
		}

		context->DrawIndexed(model.mesh.materials[i].index_count, count, 0);
		count += model.mesh.materials[i].index_count;
	}

	DEBUG(device.getAnotation()->EndEvent());
	return true;
}

bool GPLModel::BlendDebug() {
	auto device = DX11Device::getInstance().getDevice();
	const char* BLEND_items [] =
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
	const char* BLEND_OP_items [] =
	{
		"D3D11_BLEND_OP_ADD",//1
		"D3D11_BLEND_OP_SUBTRACT",
		"D3D11_BLEND_OP_REV_SUBTRACT",
		"D3D11_BLEND_OP_MIN",
		"D3D11_BLEND_OP_MAX",
	};

	const char* COLOR_WRITE_ENABLE_items [] =
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
			BlendStateDesc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP) (BlendOp + 1);
			BlendStateDesc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP) (BlendOpAlpha + 1);
			BlendStateDesc.RenderTarget[0].SrcBlend = (D3D11_BLEND) (SrcBlend + 1);
			BlendStateDesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND) (SrcBlendAlpha + 1);
			BlendStateDesc.RenderTarget[0].DestBlend = (D3D11_BLEND) (DestBlend + 1);
			BlendStateDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND) (DestBlendAlpha + 1);
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


}
}
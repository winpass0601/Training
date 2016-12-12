#include <./GPL/Deferred/GPLDeferred.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

namespace rinfw {
namespace graphics {
void GPLDeferred::Release() {
	this->ia.Release();
	this->om.Release();
	this->rs.Release();
	this->vs.Release();
	this->ps.Release();

}
bool GPLDeferred::createPipeLine(ID3D11Device *device) {

	//this->shadowtex.Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	//this->depth.Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);


	if (device == nullptr) {
		return false;
	}
	//Create GBuffer
	UINT width = (UINT)WindowDevice::getInstance().getWidth(),height = (UINT)WindowDevice::getInstance().getHeight();
	this->gbuffer[0].Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	this->gbuffer[1].Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	this->gbuffer[2].Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	this->gbuffer[3].Create(width, height, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

	//Create VertexBuffer

	const float vertex[] = {
		+1.0f, +1.0f, 0.5f , 1.0f, 0.0f,
		-1.0f, +1.0f, 0.5f , 0.0f, 0.0f,
		+1.0f, -1.0f, 0.5f , 1.0f, 1.0f,
		-1.0f, -1.0f, 0.5f , 0.0f, 1.0f
	};
	//const float vertex[] = {
	//	  +0.0f, +0.5f, +0.5f ,  1.0f, 1.0f,
	//	  +0.5f, -0.5f, +0.5f ,  1.0f, 1.0f,
	//	  -0.5f, -0.5f, +0.5f ,  1.0f, 1.0f
	//};

	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(vertex);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = vertex;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* buf;
	device->CreateBuffer(&hBufferDesc, &hSubResourceData, &buf);
	this->ia.setGPLSVertex(0,buf,sizeof(float)*(3+2),0);




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
	return true;
}
bool GPLDeferred::createIA(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	this->ia.setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return true;
}
bool GPLDeferred::createOM(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	ID3D11BlendState* hpBlendState = NULL;
	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++) {

		//MMDブレンド方法
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

	//そのブレンディングをコンテキストに設定
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->om_final.setGPLSBlendState(hpBlendState, blendFactor, 0xffffffff);
	this->om_final.setGPLSRenderTargetView(0, DX11Device::getInstance().getCOMRTV());


	this->om.setGPLSRenderTargetView(0, this->gbuffer[0].getCOMRTV());
	this->om.setGPLSRenderTargetView(1, this->gbuffer[1].getCOMRTV());
	this->om.setGPLSRenderTargetView(2, this->gbuffer[2].getCOMRTV());
	this->om.setGPLSRenderTargetView(3, this->gbuffer[3].getCOMRTV());
	this->om.setGPLSDepthStencilView(DX11Device::getInstance().getCOMDSV());
	device->CreateBlendState(&BlendStateDesc, &hpBlendState);
	this->om.setGPLSBlendState(hpBlendState, blendFactor, 0xffffffff);


	return true;
}
bool GPLDeferred::createRS(ID3D11Device *device) {
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

	ID3D11RasterizerState* hpRasterizerState = NULL;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
			D3D11_FILL_SOLID,
			D3D11_CULL_FRONT,	//ポリゴンの裏表を無くす
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
bool GPLDeferred::createVS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC element[] = { // 入力アセンブラー ステージの単一の要素( HLSL セマンティクス,要素のセマンティクス インデックス,要素データのデータ型,入力アセンブラーを識別する整数値,各要素間のオフセット (バイト単位),単一の入力スロットの入力データ クラスを識別,インスタンス単位の同じデータを使用して描画するインスタンスの数)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },		// POS情報
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,		12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	// UV情報
	};

	ID3D11VertexShader *vs_buf;
	ID3D10Blob *vsblob; // 任意長のデータを返す際に使用
	// ファイルを元にエフェクトをコンパイル
	D3DX11CompileFromFile(TEXT("./Shader/Deferred/VS.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
	// blobを_bufに格納
	device->CreateVertexShader(vsblob->GetBufferPointer(), vsblob->GetBufferSize(), nullptr, &vs_buf); // コンパイル済みシェーダーから、頂点シェーダー オブジェクトを作成

	//頂点レイアウト作成
	ID3D11InputLayout* hpInputLayout = NULL;
	device->CreateInputLayout(element, ARRAYSIZE(element), vsblob->GetBufferPointer(), vsblob->GetBufferSize(), &hpInputLayout);
	vsblob->Release();

	//IA
	{
		this->ia.setGPLSInputLayout(hpInputLayout);
	}

	//VS
	{
		this->vs.setGPLSShader(vs_buf);
	}
	return true;
}
bool GPLDeferred::createPS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}
	ID3D11PixelShader  *ps_buf = nullptr;
	ID3D10Blob *psblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/Deferred/PS.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // ピクセル シェーダーを作成
	this->ps.setGPLSShader(ps_buf);
	psblob->Release();

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

	this->ps.setGPLSShaderResourceView(0, this->gbuffer[0].getCOMSRV());
	this->ps.setGPLSShaderResourceView(1, this->gbuffer[1].getCOMSRV());
	this->ps.setGPLSShaderResourceView(2, this->gbuffer[2].getCOMSRV());
	return true;
}
bool GPLDeferred::Clear(ID3D11DeviceContext *context) {
	for(int i = 0;i<4;i++)
		context->ClearRenderTargetView(this->gbuffer[i].getRTV(), this->clearcolor[i]);
	return true;
}
bool GPLDeferred::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr)
		return false;
	if (!this->getActive())
		return true;

	this->om.setStatus();


	return true;
}
bool GPLDeferred::Update() {
	if (!this->getActive())
		return true;
	ImGui::Begin("GPLDeferred");
	static float scale = 0.2f;
	ImGui::SliderFloat("Scale", &scale, 0.025f, 1.0f);
	for (int i = 0; i < 4; i++) {
		ImGui::PushID(this->clearcolor[i]);
		ImGui::SliderFloat4("Clear Color", this->clearcolor[i], 0.0f, 1.0f);
		ImGui::Image(this->gbuffer[i].getSRV(), ImVec2((float)WindowDevice::getInstance().getWidth()*scale,(float)WindowDevice::getInstance().getHeight()*scale));
		ImGui::PopID();
	}
	ImGui::End();
	return true;
}
bool GPLDeferred::Draw(ID3D11DeviceContext *context) {
	DEBUG(DX11Device::getInstance().getAnotation()->BeginEvent(L"GPLDeferred"));
	if (context == nullptr)
		return false;
	if (!this->getActive())
		return true;
	context->ClearState();




	this->ia.setStatus();
	this->rs.setStatus();
	this->om_final.setStatus();
	this->vs.setStatus();
	this->ps.setStatus();

	context->Draw(4, 0);
	DEBUG(DX11Device::getInstance().getAnotation()->EndEvent());
	return true;
}

bool GPLDeferred::setActive(bool enabled) {
	return (this->enabled = enabled);
}
}
}
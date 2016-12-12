#include <./GPL/Test/GPLTest.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

namespace rinfw {
namespace graphics {

//GPLSテスト用コード
GPLTest::GPLTest(){

}

void GPLTest::Release() {
	this->ia.Release();
	this->om.Release();
	this->rs.Release();
	this->vs.Release();
	this->ps.Release();
	
}
bool GPLTest::setPipeLine(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	context->ClearState();
	
	this->ia.setStatus(context);
	this->om.setStatus(context);
	this->rs.setStatus(context);
	this->vs.setStatus(context);
	this->ps.setStatus(context);
	return true;
}
bool GPLTest::createPipeLine(ID3D11Device *device) {
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


	struct Vertex3D {
		float pos[3];	//x-y-z
		float col[4];	//r-g-b-a
	};
	
	const int TYOUTEN = 4;	//ポリゴンの頂点数
	
	//頂点データ(三角ポリゴン1枚)
	Vertex3D hVectorData[TYOUTEN] = {
		{ { +0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }
	};
	D3D11_INPUT_ELEMENT_DESC hInElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4*3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	//頂点バッファ作成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(Vertex3D) * TYOUTEN;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);
	
	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = hVectorData;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;
	//
	ID3D11Buffer* hpBuffer;
	if(FAILED(device->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer))){
		
	}
	ID3D11VertexShader *vs_buf;
	ID3D10Blob *vsblob; // 任意長のデータを返す際に使用
	// ファイルを元にエフェクトをコンパイル
	D3DX11CompileFromFile(TEXT("./Shader/TestVS.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
	// blobを_bufに格納
	device->CreateVertexShader(vsblob->GetBufferPointer(), vsblob->GetBufferSize(), nullptr, &vs_buf); // コンパイル済みシェーダーから、頂点シェーダー オブジェクトを作成
	
	
	//その頂点バッファをコンテキストに設定
	UINT hStrides = sizeof(Vertex3D);
	UINT hOffsets = 0;
	//頂点レイアウト作成
	ID3D11InputLayout* hpInputLayout = NULL;
	device->CreateInputLayout(hInElementDesc, ARRAYSIZE(hInElementDesc), vsblob->GetBufferPointer(), vsblob->GetBufferSize(), &hpInputLayout);
	vsblob->Release();
	//
	//IA
	{
		this->ia.setGPLSInputLayout(hpInputLayout);
		this->ia.setGPLSVertex(0,hpBuffer, hStrides, hOffsets);
		COMPTR(ID3D11InputLayout) input;
	}
	
	//VS
	{
		this->vs.setGPLSShader(vs_buf);
	}

	return true;
}
bool GPLTest::createIA(ID3D11Device *device){
	if (device == nullptr) {
		return false;
	}
	this->ia.setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return true;
}
bool GPLTest::createOM(ID3D11Device *device){
	if (device == nullptr) {
		return false;
	}

	ID3D11BlendState* hpBlendState = NULL;
	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for(int i=0; i < 8; i++){
		BlendStateDesc.RenderTarget[i].BlendEnable = TRUE;
		BlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	device->CreateBlendState(&BlendStateDesc, &hpBlendState);

	//そのブレンディングをコンテキストに設定
	float blendFactor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
	this->om.setGPLSBlendState(hpBlendState, blendFactor, 0xffffffff);
	this->om.setGPLSRenderTargetView(0, DX11Device::getInstance().getCOMRTV());
	this->om.setGPLSDepthStencilView(0, DX11Device::getInstance().getCOMDSV());

	return true;
}
bool GPLTest::createRS(ID3D11Device *device){
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
	return true;
}
bool GPLTest::createVS(ID3D11Device *device){
	if (device == nullptr) {
		return false;
	}
	return true;
}
bool GPLTest::createPS(ID3D11Device *device){
	if (device == nullptr) {
		return false;
	}
	ID3D11PixelShader  *ps_buf = nullptr;
	ID3D10Blob *psblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/TestPS.hlsl"), 0, 0, "main", "ps_5_0", 0, 0, 0, &psblob, 0, 0);
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // ピクセル シェーダーを作成
	this->ps.setGPLSShader(ps_buf);

	psblob->Release();
	return true;
}
bool GPLTest::Draw(ID3D11DeviceContext *context) {
	DEBUG(DX11Device &device = DX11Device::getInstance());
	DEBUG(device.getAnotation()->BeginEvent(L"GPLTest"));
	this->setPipeLine(context);
	context->Draw(4,0);
	DEBUG(device.getAnotation()->EndEvent());
	return true;
}
}
}
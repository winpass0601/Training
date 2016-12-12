#include <./GPL/Line/GPLLine.h>
#include <boost/range/algorithm.hpp>
#include <Device.h>

//! test
#include <GUI.h>

namespace rinfw {
namespace graphics {

//GPLSテスト用コード
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

	D3D11_INPUT_ELEMENT_DESC element [] = { // 入力アセンブラー ステージの単一の要素( HLSL セマンティクス,要素のセマンティクス インデックス,要素データのデータ型,入力アセンブラーを識別する整数値,各要素間のオフセット (バイト単位),単一の入力スロットの入力データ クラスを識別,インスタンス単位の同じデータを使用して描画するインスタンスの数)
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// POS情報
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },// COLOR情報
	};

	ID3D11VertexShader *vs_buf;
	ID3D10Blob *vsblob; // 任意長のデータを返す際に使用
	// ファイルを元にエフェクトをコンパイル
	D3DX11CompileFromFile(TEXT("./Shader/Utility/vs_line.hlsl"), 0, 0, "main", "vs_5_0", 0, 0, 0, &vsblob, 0, 0);
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
	device->CreateBlendState(&BlendStateDesc, &hpBlendState);

	//そのブレンディングをコンテキストに設定
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
bool GPLLine::createVS(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}


	ID3D11Buffer *vertexBuffer = nullptr;
	D3D11_BUFFER_DESC bd; // 生成方法(バッファー リソース)
	ZeroMemory(&bd, sizeof(bd)); // 中身をゼロクリア
	// Bufferの生成方法の格納
	bd.Usage = D3D11_USAGE_DEFAULT; // バッファーで想定されている読み込みおよび書き込みの方法を識別
	bd.CPUAccessFlags = 0; // CPUからは書き込みのみ行います
	bd.ByteWidth = sizeof(Line) * LINE_MAX*2;  // バッファーのサイズ(バイト単位)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // なんのバッファですか？
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
	device->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &ps_buf);  // ピクセル シェーダーを作成
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
	viewport.Width = (float) window.getWidth();		// ビューポートの幅
	viewport.Height = (float) window.getHeight();		// ビューポートの高さ
	viewport.MinDepth = 0.0f;				// ビューポートの最小深度
	viewport.MaxDepth = 1.0f;				// ビューポートの最大深度
	viewport.TopLeftX = 0;				// ビューポートの左側の X 位置
	viewport.TopLeftY = 0;				// ビューポートの上部の Y 位置

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

	context->UpdateSubresource(cameraconstantbuffer.GetInterfacePtr(), 0, NULL, &cameramtx, 0, 0); // CPU によって、マッピング不可能なメモリー内に作成されたサブリソースにメモリーからデータがコピーされる
	context->UpdateSubresource(worldconstantbuffer.GetInterfacePtr(), 0, NULL, &rinfw::core::math::matrix44Transpose(world),0,0);
	this->vs.setBuffertoVS(context);

	context->Draw(this->lines.size(), 0);

	return true;
}
}
}
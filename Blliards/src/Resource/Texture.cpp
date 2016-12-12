#include <./Resource/Texture.h>
#include <./Device/DirectXDevice.h>
#include <./Utility/Utility.h>
#include "DirectXTex/DirectXTex.h"
Texture2D::Texture2D() {
}
Texture2D::~Texture2D() {
}
ULONG Texture2D::Create(UINT Width, UINT Height, D3D11_USAGE use, DXGI_FORMAT fmt, UINT flg, UINT CPU_ACCESS_FLAG) {
	ULONG ret = 0;
	DX11Device &device = DX11Device::getInstance();
	ID3D11Texture2D *tex = nullptr;
	ID3D11ShaderResourceView *srv = nullptr;
	ID3D11RenderTargetView *rtv = nullptr;

	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = fmt;
	desc.SampleDesc.Count = 1;
	desc.Usage = use;
	// シェーダリソースとして使う	
	desc.BindFlags = flg;
	// CPUからアクセスして書き込みOK
	desc.CPUAccessFlags = CPU_ACCESS_FLAG;
	//desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (FAILED(device.getDevice()->CreateTexture2D(&desc, nullptr, &tex))) {
		ret = -1;
		return ret;
	}
	this->tex.Attach(tex);

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	if (FAILED(device.getDevice()->CreateShaderResourceView(this->tex, &srvDesc, &srv))) {
		ret = -3;
		return ret;
	}
	this->srv.Attach(srv);

	if (flg&D3D11_BIND_RENDER_TARGET) {
		if (FAILED(device.getDevice()->CreateRenderTargetView(this->tex, nullptr, &rtv))) {
			ret = -2;
			return ret;
		}
		this->rtv.Attach(rtv);
	}
	return ret;
}
ULONG Texture2D::LoadFile(std::string str) {
	HRESULT hr = S_OK;
	DX11Device &device = DX11Device::getInstance();
	ID3D11ShaderResourceView *srv = nullptr;
	if (SUCCEEDED(D3DX11CreateShaderResourceViewFromFileA(device.getDevice(), str.c_str(), NULL, NULL, &srv, NULL))) {
		this->srv.Attach(srv);
		return 0;
	}
	std::wstring wstr;
	widen(str, wstr);
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	if (FAILED((hr = DirectX::LoadFromTGAFile(wstr.c_str(), &metadata, image)))) {
		if (FAILED((hr =DirectX::LoadFromWICFile(wstr.c_str(),0, &metadata, image)))) {
			if (FAILED((hr =DirectX::LoadFromDDSFile(wstr.c_str(),0, &metadata, image)))) {
				return -1;
			}
		}
	}
	if (FAILED(CreateShaderResourceView(device.getDevice(), image.GetImages(), image.GetImageCount(), metadata, &srv)) ){
		image.Release();
		return -1;
	}
	this->srv.Attach(srv);
	return 0;
}
ULONG Texture2D::LoadFile(std::wstring str) {
	HRESULT hr = S_OK;
	DX11Device &device = DX11Device::getInstance();
	ID3D11ShaderResourceView *srv = nullptr;
	if (SUCCEEDED((hr = D3DX11CreateShaderResourceViewFromFileW(device.getDevice(), str.c_str(), NULL, NULL, &srv, NULL)))) {
		this->srv.Attach(srv);
		return 0;
	}
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	if (FAILED((hr = DirectX::LoadFromTGAFile(str.c_str(), &metadata, image)))) {
		if (FAILED((hr =DirectX::LoadFromWICFile(str.c_str(),0, &metadata, image)))) {
			if (FAILED((hr =DirectX::LoadFromDDSFile(str.c_str(),0, &metadata, image)))) {
				return -1;
			}
		}
	}
	if (FAILED(CreateShaderResourceView(device.getDevice(), image.GetImages(), image.GetImageCount(), metadata, &srv)) ){
		image.Release();
		return -1;
	}
	this->srv.Attach(srv);
	return 0;
}












TextureDSV2D::TextureDSV2D() {

}
TextureDSV2D::~TextureDSV2D() {

}

ULONG TextureDSV2D::Create(UINT Width, UINT Height, D3D11_USAGE use, DXGI_FORMAT fmt, UINT flg, UINT CPU_ACCESS_FLAG) {
	unsigned long ret = 0;
	DX11Device &device = DX11Device::getInstance();
	D3D11_TEXTURE2D_DESC descDepth;            // 2D テクスチャー
	ID3D11Texture2D *tex;
	ID3D11DepthStencilView*		dsv; // ステンシルビュー(深度ステンシル テスト中にテクスチャー リソースにアクセス)
	//ステンシルビューの生成
	ZeroMemory(&descDepth, sizeof(descDepth));	// 構造体中身を０初期化
	descDepth.Width = Width; descDepth.Height = Height; // テクスチャーの幅と高さ
	descDepth.MipLevels = 1; // テクスチャー内のミップマップ レベルの最大数(1:マルチサンプリングされたテクスチャー 0:サブテクスチャーの全セットを生成する)
	descDepth.ArraySize = 1; // テクスチャー配列内のテクスチャーの数
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 深度チャンネルに 24 ビット、ステンシル チャンネルに 8 ビットを使用する 32 ビット Z バッファー フォーマット
	descDepth.SampleDesc.Count = 1; descDepth.SampleDesc.Quality = 0; // ピクセル単位のマルチサンプリングの数とイメージの品質レベル
	descDepth.Usage = D3D11_USAGE_DEFAULT; // テクスチャーの読み込みおよび書き込み方法を識別する値
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // テクスチャーを出力結合ステージの深度ステンシル ターゲットとしてバインド
	ret = device.getDevice()->CreateTexture2D(&descDepth, NULL, &tex); // 2D テクスチャーの配列
	
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView; // 深度ステンシル ビューからアクセス可能なテクスチャーのサブリソースを指定
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView)); // 構造体中身を０初期化
	descDepthStencilView.Format = descDepth.Format; // リソース データのフォーマット
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // (2D テクスチャーとしてアクセス)深度ステンシル リソースへのアクセス方法を指定
	descDepthStencilView.Texture2D.MipSlice = 0; // (最初に使用するミップマップ レベルのインデックス)2D テクスチャーのサブリソースを指定

	// リソース データへのアクセス用に深度ステンシル ビューを作成
	ret = device.getDevice()->CreateDepthStencilView(tex, &descDepthStencilView, &dsv);
	this->dsv = dsv;

	if (tex) { // 使用後の解放
		tex->Release();
		tex = nullptr;
	}
	return ret;

}

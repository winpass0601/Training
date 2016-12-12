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
	// �V�F�[�_���\�[�X�Ƃ��Ďg��	
	desc.BindFlags = flg;
	// CPU����A�N�Z�X���ď�������OK
	desc.CPUAccessFlags = CPU_ACCESS_FLAG;
	//desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (FAILED(device.getDevice()->CreateTexture2D(&desc, nullptr, &tex))) {
		ret = -1;
		return ret;
	}
	this->tex.Attach(tex);

	// ShaderResourceView�̏����쐬����
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
	D3D11_TEXTURE2D_DESC descDepth;            // 2D �e�N�X�`���[
	ID3D11Texture2D *tex;
	ID3D11DepthStencilView*		dsv; // �X�e���V���r���[(�[�x�X�e���V�� �e�X�g���Ƀe�N�X�`���[ ���\�[�X�ɃA�N�Z�X)
	//�X�e���V���r���[�̐���
	ZeroMemory(&descDepth, sizeof(descDepth));	// �\���̒��g���O������
	descDepth.Width = Width; descDepth.Height = Height; // �e�N�X�`���[�̕��ƍ���
	descDepth.MipLevels = 1; // �e�N�X�`���[���̃~�b�v�}�b�v ���x���̍ő吔(1:�}���`�T���v�����O���ꂽ�e�N�X�`���[ 0:�T�u�e�N�X�`���[�̑S�Z�b�g�𐶐�����)
	descDepth.ArraySize = 1; // �e�N�X�`���[�z����̃e�N�X�`���[�̐�
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// �[�x�`�����l���� 24 �r�b�g�A�X�e���V�� �`�����l���� 8 �r�b�g���g�p���� 32 �r�b�g Z �o�b�t�@�[ �t�H�[�}�b�g
	descDepth.SampleDesc.Count = 1; descDepth.SampleDesc.Quality = 0; // �s�N�Z���P�ʂ̃}���`�T���v�����O�̐��ƃC���[�W�̕i�����x��
	descDepth.Usage = D3D11_USAGE_DEFAULT; // �e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �e�N�X�`���[���o�͌����X�e�[�W�̐[�x�X�e���V�� �^�[�Q�b�g�Ƃ��ăo�C���h
	ret = device.getDevice()->CreateTexture2D(&descDepth, NULL, &tex); // 2D �e�N�X�`���[�̔z��
	
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView; // �[�x�X�e���V�� �r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X���w��
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView)); // �\���̒��g���O������
	descDepthStencilView.Format = descDepth.Format; // ���\�[�X �f�[�^�̃t�H�[�}�b�g
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // (2D �e�N�X�`���[�Ƃ��ăA�N�Z�X)�[�x�X�e���V�� ���\�[�X�ւ̃A�N�Z�X���@���w��
	descDepthStencilView.Texture2D.MipSlice = 0; // (�ŏ��Ɏg�p����~�b�v�}�b�v ���x���̃C���f�b�N�X)2D �e�N�X�`���[�̃T�u���\�[�X���w��

	// ���\�[�X �f�[�^�ւ̃A�N�Z�X�p�ɐ[�x�X�e���V�� �r���[���쐬
	ret = device.getDevice()->CreateDepthStencilView(tex, &descDepthStencilView, &dsv);
	this->dsv = dsv;

	if (tex) { // �g�p��̉��
		tex->Release();
		tex = nullptr;
	}
	return ret;

}

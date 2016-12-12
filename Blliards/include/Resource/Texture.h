#pragma once
#include <d3d11_1.h>
#include <Macro.h>

class Texture2D  {
public:
	Texture2D();
	~Texture2D();

	ULONG Create(UINT, UINT, D3D11_USAGE, DXGI_FORMAT, UINT = D3D11_BIND_SHADER_RESOURCE, UINT CPU_ACCESS_FLAG = 0);
	ULONG LoadFile(std::string file);
	ULONG LoadFile(std::wstring file);

	ID3D11Texture2D				*getTex() { return this->tex; }
	ID3D11ShaderResourceView	*getSRV() { return this->srv; }
	ID3D11RenderTargetView		*getRTV() { return this->rtv; }

	COMPTR(ID3D11Texture2D)				getCOMTEX() { return this->tex; }
	COMPTR(ID3D11ShaderResourceView)	getCOMSRV() { return this->srv; }
	COMPTR(ID3D11RenderTargetView)		getCOMRTV() { return this->rtv; }


protected:
private:
	COMPTR(ID3D11Texture2D)				tex;
	COMPTR(ID3D11ShaderResourceView)	srv;
	COMPTR(ID3D11RenderTargetView)		rtv;
};

class TextureDSV2D {
public:
	TextureDSV2D();
	~TextureDSV2D();

	ULONG Create(UINT, UINT, D3D11_USAGE, DXGI_FORMAT, UINT = D3D11_BIND_SHADER_RESOURCE, UINT CPU_ACCESS_FLAG = 0);

	ID3D11DepthStencilView *getDSV() { return this->dsv; }

protected:
private:

	COMPTR(ID3D11DepthStencilView)	dsv;

};
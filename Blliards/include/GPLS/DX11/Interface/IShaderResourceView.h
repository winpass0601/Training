#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
class IShaderResourceView {
public:
	IShaderResourceView() {
	}
	IShaderResourceView(const IShaderResourceView &obj) {
		this->SRVRelease();
		for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			this->srv[i] = obj.srv[i];
		}
	}
	void SRVRelease() {
		for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			if (this->srv[i].GetInterfacePtr() != nullptr)
			{
				this->srv[i].Release();
				this->srv[i].Detach();
			}
		}
	}

	bool setShaderResourceView(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11ShaderResourceView *srv[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			srv[i] = this->srv[i].GetInterfacePtr();
		}
		context->VSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, srv);
		return true;
	}
	bool getShaderResourceView(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11ShaderResourceView *srv[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		context->VSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, srv);
		for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			if (this->srv[i].GetInterfacePtr() != nullptr) {
				this->srv[i].Release();
				this->srv[i].Detach();
			}
			if (srv[i] != nullptr) {
				this->srv[i].Attach(srv[i]);
			}
		}
		return true;
	}

	bool setGPLSShaderResourceView(UINT idx, ID3D11ShaderResourceView *srv, bool write = true) {
		if (idx >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT || (!write&&this->srv[idx].GetInterfacePtr() != nullptr)) {
			return false;
		}
		if (this->srv[idx].GetInterfacePtr() != nullptr) {
			this->srv[idx].Release();
			this->srv[idx].Detach();
		}
		if (srv != nullptr) {
			this->srv[idx].Attach(srv);
		}
		return true;
	}
	bool setGPLSShaderResourceView(UINT idx, COMPTR(ID3D11ShaderResourceView) srv, bool write = true) {
		if (idx >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT || (!write&&this->srv[idx].GetInterfacePtr() != nullptr)) {
			return false;
		}
		if (this->srv[idx].GetInterfacePtr() != nullptr) {
			this->srv[idx].Release();
			this->srv[idx].Detach();
		}
		if (srv.GetInterfacePtr() != nullptr) {
			this->srv[idx] = srv;
		}
		return true;

	}
	COMPTR(ID3D11ShaderResourceView)  getGPLSShaderResourceView(UINT idx) {
		if (idx >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) {
			return false;
		}
		return this->srv[idx];
	}
protected:
	COMPTR(ID3D11ShaderResourceView) srv[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
};
}
}
}
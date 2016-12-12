#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>
namespace rinfw {
namespace graphics {
namespace gplsi {
class IDepthStencilState {
public:
	IDepthStencilState() {}
	IDepthStencilState(const IDepthStencilState& obj) {
		this->DepthStencilStateRelease();
		this->dss = obj.dss;
	}
	void DepthStencilStateRelease() {
		this->stencilref = 0;
		if (this->dss) {
			this->dss.Release();
			this->dss.Detach();
		}
	}

	bool setDepthStencilState(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		context->OMSetDepthStencilState(this->dss.GetInterfacePtr(), this->stencilref);
		return true;
	}
	bool getDepthStencilState(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11DepthStencilState *dss = nullptr;
		context->OMGetDepthStencilState(&dss, &this->stencilref);
		if (this->dss.GetInterfacePtr() != nullptr) {
			this->dss.Release();
			this->dss.Detach();
		}
		if (dss != nullptr) {
			this->dss.Attach(dss);
		}
		return true;
	}

	bool setGPLSDepthStencilState(ID3D11DepthStencilState *dss, UINT stencilref, bool write = true) {
		if (!write&&this->dss.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->dss.GetInterfacePtr() != nullptr) {
			this->dss.Release();
			this->dss.Detach();
		}
		if (dss != nullptr) {
			this->dss.Attach(dss);
			this->stencilref = stencilref;
		}
		return true;
	}
	bool setGPLSDepthStencilState(COMPTR(ID3D11DepthStencilState) dss, UINT stencilref, bool write = true) {
		if (!write&&this->dss.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->dss.GetInterfacePtr() != nullptr) {
			this->dss.Release();
			this->dss.Detach();
		}
		if (dss.GetInterfacePtr() != nullptr) {
			this->dss = dss;
			this->stencilref = stencilref;
		}
		return true;
	}
	COMPTR(ID3D11DepthStencilState) getGPLSDepthStencilState(UINT &stencilref) {
		stencilref = this->stencilref;
		return this->dss;
	}

protected:
	COMPTR(ID3D11DepthStencilState) dss;
	UINT stencilref;
};
}
}
}
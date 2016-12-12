#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
class IRasterizerState {
public:
	IRasterizerState(){}
	IRasterizerState(const IRasterizerState& obj) {
		this->RasterizerStateRelease();
		this->rasterizerstate = obj.rasterizerstate;
	}
	void RasterizerStateRelease() {
		if (this->rasterizerstate) {
			this->rasterizerstate.Release();
			this->rasterizerstate.Detach();
		}
	}
	bool setRasterizerState(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		context->RSSetState(this->rasterizerstate.GetInterfacePtr());
		return true;
	}
	bool getRasterizerState(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		if (this->rasterizerstate.GetInterfacePtr() != nullptr) {
			this->rasterizerstate.Release();
			this->rasterizerstate.Detach();
		}
		ID3D11RasterizerState *rasterizerstate = nullptr;
		context->RSGetState(&rasterizerstate);
		if (this->rasterizerstate.GetInterfacePtr() != nullptr) {
			this->rasterizerstate.Release();
			this->rasterizerstate.Detach();
		}
		if (rasterizerstate != nullptr) {
			this->rasterizerstate.Attach(rasterizerstate);
		}
		return true;
	}

	bool setGPLSRasterizerState(ID3D11RasterizerState *rasterizerstate, bool write = true) {
		if (!write && this->rasterizerstate.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->rasterizerstate.GetInterfacePtr() != nullptr) {
			this->rasterizerstate.Release();
			this->rasterizerstate.Detach();
		}
		this->rasterizerstate.Attach(rasterizerstate);
		return true;
	}
	bool setGPLSRasterizerState(COMPTR(ID3D11RasterizerState) rasterizerstate, bool write = true) {
		if (!write && this->rasterizerstate.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->rasterizerstate.GetInterfacePtr() != nullptr) {
			this->rasterizerstate.Release();
			this->rasterizerstate.Detach();
		}
		if (rasterizerstate.GetInterfacePtr() != nullptr) {
			this->rasterizerstate = rasterizerstate;
		}
		return true;
	}
	COMPTR(ID3D11RasterizerState) getGPLSRasterizerState() {
		return this->rasterizerstate;
	}
protected:
	COMPTR(ID3D11RasterizerState) rasterizerstate;
};
}
}
}
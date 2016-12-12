#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>
namespace rinfw {
namespace graphics {
namespace gplsi {
class IInputLayout {
public:
	IInputLayout() {}
	IInputLayout(const IInputLayout &obj) {
		this->InputLayoutRelease();
		this->inputlayout = obj.inputlayout;
	}
	void InputLayoutRelease() {
		if (this->inputlayout) {
			this->inputlayout.Release();
			this->inputlayout.Detach();
		}
	}
	bool setInputLayout(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		context->IASetInputLayout(this->inputlayout.GetInterfacePtr());
		return true;
	}
	bool getInputLayout(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11InputLayout *inputlayout = nullptr;
		context->IAGetInputLayout(&inputlayout);
		if (this->inputlayout.GetInterfacePtr() != nullptr) {
			this->inputlayout.Release();
			this->inputlayout.Detach();
		}
		if (inputlayout != nullptr) {
			this->inputlayout.Attach(inputlayout);
		}
		return true;
	}

	bool setGPLSInputLayout(ID3D11InputLayout *inputlayout, bool write = true) {
		if (!write&&this->inputlayout.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->inputlayout.GetInterfacePtr() != nullptr) {
			this->inputlayout.Release();
			this->inputlayout.Detach();
		}
		if (inputlayout != nullptr) {
			this->inputlayout.Attach(inputlayout);
		}
		return true;
	}
	bool setGPLSInputLayout(COMPTR(ID3D11InputLayout) inputlayout, bool write = true) {
		if (!write&&this->inputlayout.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->inputlayout.GetInterfacePtr() != nullptr) {
			this->inputlayout.Release();
			this->inputlayout.Detach();
		}
		if (inputlayout.GetInterfacePtr() != nullptr) {
			this->inputlayout = inputlayout;
		}
		return true;
	}
	COMPTR(ID3D11InputLayout) getGPLSInputLayout() {
		return this->inputlayout;
	}
protected:
	COMPTR(ID3D11InputLayout) inputlayout;
};
}
}
}
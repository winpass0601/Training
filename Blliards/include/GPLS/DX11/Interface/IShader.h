#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
template<typename T>
class IShader {
public:
	IShader() {}
	IShader(const IShader &obj) {
		this->ShaderRelease();
		this->shader = obj.shader;
	}

	void ShaderRelease() {
		if (this->shader) {
			this->shader.Release();
			this->shader.Detach();
		}
	}

	virtual bool setShader(ID3D11DeviceContext *context) = 0;
	virtual bool getShader(ID3D11DeviceContext *context) = 0;

	bool setGPLSShader(T *shader, bool write = true) {
		if (!write&&this->shader.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader != nullptr) {
			this->shader.Attach(shader);
		}
		return true;
	}
	bool setGPLSShader(COMPTR(T), bool write = true) {
		if (!write&&this->shader.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader.GetInterfacePtr() != nullptr) {
			this->shader = shader;
		}
		return true;
	}
	COMPTR(T) getGPLSShader() {
		return this->shader;
	}
protected:
	COMPTR(T) shader;
	//ˆÈ‰º–¢ŽÀ‘•
	//ID3D11ClassInstance *const *ppClassInstances,
	//UINT NumClassInstances
};
class IComputeShader : public IShader<ID3D11ComputeShader> {
public:
	bool setShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		context->CSSetShader(this->shader.GetInterfacePtr(), nullptr, 0);
		return true;
	}
	bool getShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		ID3D11ComputeShader *shader = nullptr;
		ID3D11ClassInstance *pClassInstances = nullptr;
		UINT NumClassInstances;
		context->CSGetShader(&shader, &pClassInstances, &NumClassInstances);

		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader != nullptr) {
			this->shader.Attach(shader);
		}
		return true;
	}
};
class IVertexShader : public IShader<ID3D11VertexShader> {
public:
	bool setShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		context->VSSetShader(this->shader.GetInterfacePtr(), nullptr, 0);
		return true;
	}
	bool getShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		ID3D11VertexShader *shader = nullptr;
		ID3D11ClassInstance *pClassInstances = nullptr;
		UINT NumClassInstances;
		context->VSGetShader(&shader, &pClassInstances, &NumClassInstances);

		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader != nullptr) {
			this->shader.Attach(shader);
		}
		return true;
	}
};
class IHullShader : public IShader<ID3D11HullShader> {
public:
	bool setShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		context->HSSetShader(this->shader.GetInterfacePtr(), nullptr, 0);
		return true;
	}
	bool getShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		ID3D11HullShader *shader = nullptr;
		ID3D11ClassInstance *pClassInstances = nullptr;
		UINT NumClassInstances;
		context->HSGetShader(&shader, &pClassInstances, &NumClassInstances);

		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader != nullptr) {
			this->shader.Attach(shader);
		}
		return true;
	}
};
class IDomainShader : public IShader<ID3D11DomainShader> {
public:
	bool setShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		context->DSSetShader(this->shader.GetInterfacePtr(), nullptr, 0);
		return true;
	}
	bool getShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		ID3D11DomainShader *shader = nullptr;
		ID3D11ClassInstance *pClassInstances = nullptr;
		UINT NumClassInstances;
		context->DSGetShader(&shader, &pClassInstances, &NumClassInstances);

		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader != nullptr) {
			this->shader.Attach(shader);
		}
		return true;
	}
};


class IPixelShader : public IShader<ID3D11PixelShader> {
public:
	bool setShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		context->PSSetShader(this->shader.GetInterfacePtr(), nullptr, 0);
		return true;
	}
	bool getShader(ID3D11DeviceContext *context)final {
		if (context == nullptr) {
			return false;
		}
		ID3D11PixelShader *shader = nullptr;
		ID3D11ClassInstance *pClassInstances = nullptr;
		UINT NumClassInstances;
		context->PSGetShader(&shader, &pClassInstances, &NumClassInstances);

		if (this->shader.GetInterfacePtr() != nullptr) {
			this->shader.Release();
			this->shader.Detach();
		}
		if (shader != nullptr) {
			this->shader.Attach(shader);
		}
		return true;
	}
};
}
}
}
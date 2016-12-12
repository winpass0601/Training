#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
class ISamplerState {
public:
	ISamplerState() {}
	ISamplerState(const ISamplerState &obj) {

	}
	void SamplerStateRelease() {

	}

	bool setSamplerStatetoVS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			samplers[i] = this->samplers[i].GetInterfacePtr();
		}
		context->VSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		return true;
	}
	bool getSamplerStatetoVS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		context->VSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			if (this->samplers[i].GetInterfacePtr() != nullptr) {
				this->samplers[i].Release();
				this->samplers[i].Detach();
			}
			if (samplers[i] != nullptr) {
				this->samplers[i].Attach(samplers[i]);
			}
		}
		return true;
	}
	bool setSamplerStatetoHS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			samplers[i] = this->samplers[i].GetInterfacePtr();
		}
		context->HSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		return true;
	}
	bool getSamplerStatetoHS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		context->HSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			if (this->samplers[i].GetInterfacePtr() != nullptr) {
				this->samplers[i].Release();
				this->samplers[i].Detach();
			}
			if (samplers[i] != nullptr) {
				this->samplers[i].Attach(samplers[i]);
			}
		}
		return true;
	}
	bool setSamplerStatetoDS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			samplers[i] = this->samplers[i].GetInterfacePtr();
		}
		context->DSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		return true;
	}
	bool getSamplerStatetoDS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		context->DSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			if (this->samplers[i].GetInterfacePtr() != nullptr) {
				this->samplers[i].Release();
				this->samplers[i].Detach();
			}
			if (samplers[i] != nullptr) {
				this->samplers[i].Attach(samplers[i]);
			}
		}
		return true;
	}
	bool setSamplerStatetoPS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			samplers[i] = this->samplers[i].GetInterfacePtr();
		}
		context->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		return true;
	}
	bool getSamplerStatetoPS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11SamplerState *samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		context->PSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			if (this->samplers[i].GetInterfacePtr() != nullptr) {
				this->samplers[i].Release();
				this->samplers[i].Detach();
			}
			if (samplers[i] != nullptr) {
				this->samplers[i].Attach(samplers[i]);
			}
		}
		return true;
	}

	bool setGPLSSamplerState(UINT idx, ID3D11SamplerState *sampler, bool write = true) {
		if (idx >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT || (!write&&this->samplers[idx].GetInterfacePtr() != nullptr)) {
			return false;
		}
		if (this->samplers[idx].GetInterfacePtr() != nullptr) {
			this->samplers[idx].Release();
			this->samplers[idx].Detach();
		}
		if (sampler != nullptr) {
			this->samplers[idx].Attach(sampler);
		}
		return true;
	}
	bool setGPLSSamplerState(UINT idx, COMPTR(ID3D11SamplerState) sampler, bool write = true) {
		if (idx >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT || (!write&&this->samplers[idx].GetInterfacePtr() != nullptr)) {
			return false;
		}
		if (this->samplers[idx].GetInterfacePtr() != nullptr) {
			this->samplers[idx].Release();
			this->samplers[idx].Detach();
		}
		if (sampler.GetInterfacePtr() != nullptr) {
			this->samplers[idx] = sampler;
		}
		return true;
	}
	COMPTR(ID3D11SamplerState) getGPLSSamplerState(UINT idx) {
		if (idx >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) {
			return false;
		}
		return this->samplers[idx];
	}
protected:
	COMPTR(ID3D11SamplerState) samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
};
}
}
}
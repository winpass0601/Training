#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
class IBlendState {
public:
	IBlendState(){}
	IBlendState(const IBlendState &obj) {
		this->BlendStateRelease();
		this->blendstate = obj.blendstate;
		this->blendfactor[0] = obj.blendfactor[0];
		this->blendfactor[1] = obj.blendfactor[1];
		this->blendfactor[2] = obj.blendfactor[2];
		this->blendfactor[3] = obj.blendfactor[3];
		this->samplemask = obj.samplemask;
	}
	void BlendStateRelease() {
		this->blendfactor[0] = 0.0f;
		this->blendfactor[1] = 0.0f;
		this->blendfactor[2] = 0.0f;
		this->blendfactor[3] = 0.0f;
		this->samplemask = 0xffffffff;
		if (this->blendstate) {
			this->blendstate.Release();
			this->blendstate.Detach();
		}
	}

	bool setBlendState(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		context->OMSetBlendState(this->blendstate.GetInterfacePtr(), this->blendfactor, this->samplemask);
		return true;
	}
	bool getBlendState(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11BlendState *blendstate = nullptr;
		context->OMGetBlendState(&blendstate, this->blendfactor, &this->samplemask);

		if (this->blendstate.GetInterfacePtr() != nullptr) {
			this->blendstate.Release();
			this->blendstate.Detach();
		}
		if (blendstate != nullptr) {
			this->blendstate.Attach(blendstate);
		}
		return true;
	}

	bool setGPLSBlendState(ID3D11BlendState *blendstate, const FLOAT blendfactor[4], UINT samplemask, bool write = true) {
		if (!write && this->blendstate.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->blendstate.GetInterfacePtr() != nullptr) {
			this->blendstate.Release();
			this->blendstate.Detach();
		}
		if (blendstate != nullptr) {
			this->blendstate.Attach(blendstate);
			this->blendfactor[0] = blendfactor[0];
			this->blendfactor[1] = blendfactor[1];
			this->blendfactor[2] = blendfactor[2];
			this->blendfactor[3] = blendfactor[3];
			this->samplemask = samplemask;
		}
		return true;
	}
	bool setGPLSBlendState(COMPTR(ID3D11BlendState)blendstate, const FLOAT blendfactor[4], UINT samplemask, bool write = true) {
		if (!write && this->blendstate.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->blendstate.GetInterfacePtr() != nullptr) {
			this->blendstate.Release();
			this->blendstate.Detach();
		}
		if (blendstate.GetInterfacePtr() != nullptr) {
			this->blendstate = blendstate;
			this->blendfactor[0] = blendfactor[0];
			this->blendfactor[1] = blendfactor[1];
			this->blendfactor[2] = blendfactor[2];
			this->blendfactor[3] = blendfactor[3];
			this->samplemask = samplemask;
		}
		return true;
	}
	COMPTR(ID3D11BlendState) getGPLSBlendState(FLOAT(&blendfactor)[4], UINT &samplemask) {
		blendfactor[0] = this->blendfactor[0];
		blendfactor[1] = this->blendfactor[1];
		blendfactor[2] = this->blendfactor[2];
		blendfactor[3] = this->blendfactor[3];
		samplemask = this->samplemask;
		return this->blendstate;
	}

protected:
	COMPTR(ID3D11BlendState) blendstate;
	FLOAT blendfactor[4];
	UINT samplemask;
};
}
}
}
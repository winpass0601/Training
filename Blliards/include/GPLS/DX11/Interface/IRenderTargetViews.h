#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
class IRenderTargetViews {
public:
	IRenderTargetViews() {}
	IRenderTargetViews(const IRenderTargetViews &obj) {
		this->RenderTargetViewsRelease();
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			this->rtv[i] = obj.rtv[i];
		}
		this->dsv = obj.dsv;
	}
	void RenderTargetViewsRelease() {

		for (auto &v : this->rtv) {
			if (v) {
				v.Release();
				v.Detach();
			}
		}
		if (this->dsv) {
			this->dsv.Release();
			this->dsv.Detach();
		}
	}

	bool setRenderTargetView(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11RenderTargetView *rtv[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			rtv[i] = this->rtv[i].GetInterfacePtr();
		}
		context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, rtv, this->dsv.GetInterfacePtr());

		return true;
	}
	bool getRenderTargetView(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11RenderTargetView *rendertargetview[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
		ID3D11DepthStencilView *dsv = nullptr;
		context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, rendertargetview, &dsv);
		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			this->rtv[i].Attach(rendertargetview[i]);
		}
		if (this->dsv.GetInterfacePtr() != nullptr) {
			this->dsv.Release();
			this->dsv.Detach();
		}
		if (dsv != nullptr) {
			this->dsv.Attach(dsv);
		}
		return true;
	}

	bool setGPLSRenderTargetView(UINT idx, ID3D11RenderTargetView *rendertargetview, bool write = true) {
		if (!write
			&&idx < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
			&&this->rtv[idx].GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->rtv[idx].GetInterfacePtr() != nullptr) {
			this->rtv[idx].Release();
			this->rtv[idx].Detach();
		}
		if (rendertargetview != nullptr) {
			this->rtv[idx].Attach(rendertargetview);
		}
		return true;
	}
	bool setGPLSRenderTargetView(UINT idx, COMPTR(ID3D11RenderTargetView) rendertargetview, bool write = true) {
		if (!write
			&&idx < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
			&&this->rtv[idx].GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->rtv[idx].GetInterfacePtr() != nullptr) {
			this->rtv[idx].Release();
			this->rtv[idx].Detach();
		}
		if (rendertargetview != nullptr) {
			this->rtv[idx] = rendertargetview;
		}
		return true;
	}
	COMPTR(ID3D11RenderTargetView) getGPLSRenderTargetView(UINT idx) {
		if (idx < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) {
			return false;
		}
		return this->rtv[idx];
	}

	bool setGPLSDepthStencilView(ID3D11DepthStencilView *dsv, bool write = true) {
		if (!write&&this->dsv.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->dsv.GetInterfacePtr() != nullptr) {
			this->dsv.Release();
			this->dsv.Detach();
		}
		if (dsv != nullptr) {
			this->dsv.Attach(dsv);
		}
		return true;
	}
	bool setGPLSDepthStencilView(COMPTR(ID3D11DepthStencilView) dsv, bool write = true) {
		if (!write&&this->dsv.GetInterfacePtr() != nullptr) {
			return false;
		}
		if (this->dsv.GetInterfacePtr() != nullptr) {
			this->dsv.Release();
			this->dsv.Detach();
		}
		if (dsv != nullptr) {
			this->dsv = dsv;
		}
		return true;
	}
	COMPTR(ID3D11DepthStencilView) getGPLSDepthStencilView() {
		return this->dsv;
	}

protected:
	COMPTR(ID3D11RenderTargetView) rtv[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	COMPTR(ID3D11DepthStencilView) dsv;
};
}
}
}
#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
	class IViewPort {
	public:
		IViewPort() {}
		IViewPort(const IViewPort &obj) {
			for (int i = 0; i < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; i++) {
				this->viewport[i] = obj.viewport[i];
			}
		}
		void ViewPortRelease() {
			for (int i = 0; i < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; i++) {
				this->viewport[i].Width = 0.0f;
				this->viewport[i].Height = 0.0f;
				this->viewport[i].TopLeftX = 0.0f;
				this->viewport[i].TopLeftY = 0.0f;
				this->viewport[i].MinDepth = 0.0f;
				this->viewport[i].MaxDepth = 0.0f;
			}
		}

		bool setViewPort(ID3D11DeviceContext *context) {
			if (context == nullptr) {
				return false;
			}
			context->RSSetViewports(D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, this->viewport);
			return true;
		}
		bool getViewPort(ID3D11DeviceContext *context) {
			if (context == nullptr) {
				return false;
			}
			UINT viewportnum = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
			context->RSGetViewports(&viewportnum, viewport);
			return true;
		}

		bool setGPLSViewPort(UINT idx, const D3D11_VIEWPORT viewport) {
			if (idx >= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) {
				return false;
			}
			this->viewport[idx] = viewport;
			return true;
		}
		bool getGPLSViewPort(UINT idx, D3D11_VIEWPORT &viewport) {
			if (idx <= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) {
				return false;
			}
			viewport = this->viewport[idx];
			return true;
		}

	protected:
		D3D11_VIEWPORT viewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

	};
}
}
}
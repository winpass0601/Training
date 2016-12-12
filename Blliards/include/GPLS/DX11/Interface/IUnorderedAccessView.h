#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {
class IUnorderedAccessView {
public:
	IUnorderedAccessView() {}
	IUnorderedAccessView(const IUnorderedAccessView &obj) {
	}
	void UnorderedAccessViewRelease() {
		for (UINT i = 0; i < D3D11_1_UAV_SLOT_COUNT; i++) {
			if (this->unorderedaccessviews[i].GetInterfacePtr() != nullptr) {
				this->unorderedaccessviews[i].Release();
				this->unorderedaccessviews[i].Detach();
			}
		}
	}

	bool setUnorderedAccessView(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}

		ID3D11UnorderedAccessView *unorderedaccessviews[D3D11_1_UAV_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_1_UAV_SLOT_COUNT; i++) {
			unorderedaccessviews[i] = this->unorderedaccessviews[i].GetInterfacePtr();
		}
		UINT uavInits[] = { 0 };
		context->CSSetUnorderedAccessViews(0, D3D11_1_UAV_SLOT_COUNT, unorderedaccessviews, uavInits);
		return true;
	}
	bool getUnorderedAccessView(ID3D11DeviceContext *context){
		if (context == nullptr) {
			return false;
		}
		ID3D11UnorderedAccessView *unorderedaccessviews[D3D11_1_UAV_SLOT_COUNT];
		context->CSGetUnorderedAccessViews(0, D3D11_1_UAV_SLOT_COUNT, unorderedaccessviews);
		for (UINT i = 0; i < D3D11_1_UAV_SLOT_COUNT; i++) {
			if (this->unorderedaccessviews[i].GetInterfacePtr() != nullptr) {
				this->unorderedaccessviews[i].Release();
				this->unorderedaccessviews[i].Detach();
			}
			if (unorderedaccessviews[i] != nullptr) {
				this->unorderedaccessviews[i].Attach(unorderedaccessviews[i]);
			}
		}
		return true;
	}


	bool setGPLSUnorderedAccessView(UINT idx,ID3D11UnorderedAccessView *unorderedaccessview, bool write = true);
	bool setGPLSUnorderedAccessView(UINT idx,COMPTR(ID3D11UnorderedAccessView), bool write = true);
	COMPTR(ID3D11UnorderedAccessView) getGPLSUnorderedAccessView(UINT idx) {
		if (idx >= D3D11_1_UAV_SLOT_COUNT) {
			return nullptr;
		}
		return this->unorderedaccessviews[idx];
	}

protected:
	COMPTR(ID3D11UnorderedAccessView) unorderedaccessviews[D3D11_1_UAV_SLOT_COUNT];
};
}
}
}
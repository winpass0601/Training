#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <D3D11_1.h>
#include <./GPLS/DX11/Interfaces.h>

namespace rinfw {
namespace graphics {
namespace gpls {

class OM :
	public gplsi::IBlendState,
	public gplsi::IDepthStencilState,
	public gplsi::IRenderTargetViews{
public:
	OM();
	OM(const OM &om);
	OM(const OM &om,bool blendstate, bool dss, bool rtv, bool depthstencilvie);

	void Release();
	bool setStatus();
	bool setStatus(ID3D11DeviceContext *context);
	bool getStatus();
	bool getStatus(ID3D11DeviceContext *context);


protected:
private:
};

}
}
}
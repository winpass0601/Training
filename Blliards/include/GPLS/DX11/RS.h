#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <D3D11_1.h>
#include <./GPLS/DX11/Interfaces.h>

namespace rinfw {
namespace graphics {
namespace gpls {
class RS :
	public gplsi::IViewPort,
	public gplsi::IRasterizerState{
public:
	RS();
	RS(const RS &rs);
	RS(const RS &rs,bool viewport,bool rasterizerstate);

	void Release();
	bool setStatus();
	bool setStatus(ID3D11DeviceContext *context);
	bool getStatus();
	bool getStatus(ID3D11DeviceContext *context);
};
}
}
}
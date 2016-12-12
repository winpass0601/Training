#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <D3D11_1.h>
#include <./GPLS/DX11/Interfaces.h>

namespace rinfw {
namespace graphics {
namespace gpls {
class CS :
	public gplsi::IComputeShader,
	public gplsi::IBuffer,
	public gplsi::IShaderResourceView,
	public gplsi::IUnorderedAccessView{
public:
	CS();
	CS(const CS &CS);
	CS(const CS &CS,bool shader,bool buffers,bool unorderedaccessview,bool srv);

	void Release();
	bool setStatus();
	bool setStatus(ID3D11DeviceContext *context);
	bool getStatus();
	bool getStatus(ID3D11DeviceContext *context);



private:


};

}
}
}
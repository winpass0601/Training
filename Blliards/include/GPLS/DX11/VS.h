#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <D3D11_1.h>
#include <./GPLS/DX11/Interfaces.h>

namespace rinfw {
namespace graphics {
namespace gpls {

class VS :
	public gplsi::IVertexShader,
	public gplsi::IBuffer,
	public gplsi::ISamplerState,
	public gplsi::IShaderResourceView{
public:
	VS();
	VS(const VS &vs);
	VS(const VS &vs,bool vertexshader,bool buffers,bool samplers,bool srv);

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
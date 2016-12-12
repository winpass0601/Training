#pragma once
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./GPLS/GPLS.h>
namespace rinfw {
namespace graphics {


//GPLSテスト用コード
class GPLTest {
public:
	GPLTest();
	void Release();
	bool setPipeLine(ID3D11DeviceContext *context);
	bool createPipeLine(ID3D11Device *device);
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);
	bool Draw(ID3D11DeviceContext *context);
protected:
private:
	std::string name;
	gpls::IA ia;
	gpls::OM om;
	gpls::RS rs;
	gpls::VS vs;
	gpls::PS ps;
};
}
}
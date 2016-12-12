#pragma once
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./GPLS/GPLS.h>
#include <./Component/Model/PmxModel.h>
#include <./Core/Math/Math.h>

//! PMX用シャドウ描画専用

namespace rinfw {
namespace graphics {
class GPLShadow {
public:
	static GPLShadow &getInstance() {
		static GPLShadow ins;
		return ins;
	}
	bool createPipeLine(ID3D11Device *device);
	void Release();
	bool Clear(ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world);
	bool Draw(core::PmxModel &model,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world,ID3D11DeviceContext *context);

	Texture2D &getShadowMap();

protected:
private:
	GPLShadow();
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);
	DEBUG(bool BlendDebug());

	gpls::IA ia;
	gpls::OM om;
	gpls::RS rs;
	//gpls::RS rs;
	gpls::VS vs;
	gpls::PS ps;

	core::math::Float4 height;
	Texture2D shadowtex;
	TextureDSV2D depth;
};

}
}
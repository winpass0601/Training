#pragma once
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./GPLS/GPLS.h>
#include <./Component/Model/PmxModel.h>
#include <./Core/Math/Math.h>

//! PmxópÉÇÉfÉãï`âÊêÍóp

namespace rinfw {
namespace graphics {


class GPLDisplacementMap {
public:
	static GPLDisplacementMap &getInstance() {
		static GPLDisplacementMap ins;
		return ins;
	}
	bool createPipeLine(ID3D11Device *device);
	void Release();
	bool Clear(ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world);
	bool Draw(core::PmxModel &model,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world,ID3D11DeviceContext *context);
	bool DrawShadow(core::PmxModel &model,rinfw::core::math::Matrix44 &world);
	bool DrawShadow(core::PmxModel &model,rinfw::core::math::Matrix44 &world,ID3D11DeviceContext *context);

	void setHeight(core::math::Float4 height);
protected:
private:
	GPLDisplacementMap();
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createHS(ID3D11Device *device);
	bool createDS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);
	DEBUG(bool BlendDebug());

	gpls::IA ia;
	gpls::OM om;
	gpls::RS rs;
	gpls::RS rs_cullnone;
	gpls::RS rs_cullwire;
	//gpls::RS rs;
	gpls::VS vs;
	gpls::HS hs;
	gpls::DS ds;
	gpls::PS ps;

	gpls::PS shadow_ps;
	gpls::OM shadow_om;

	core::math::Float4 height;


	Texture2D shadowtex;
	TextureDSV2D depth;
	struct Camera {
		rinfw::core::math::Matrix44 View;
		rinfw::core::math::Matrix44 Projection;
	};
	Camera shadow;
};

}
}
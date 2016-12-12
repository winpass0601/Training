#pragma once
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./GPLS/GPLS.h>
#include <./Component/Model/PmxModel.h>
#include <./Core/Math/Math.h>
#include <./Graphics/Camera.h>

//! PmxópÉÇÉfÉãï`âÊêÍóp

namespace rinfw {
namespace graphics {


class GPLModel {
public:
	static GPLModel &getInstance() {
		static GPLModel ins;
		return ins;
	}
	bool createPipeLine(ID3D11Device *device);
	void Release();
	bool Draw(core::PmxModel &model);
	bool Draw(core::PmxModel &model,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world,rinfw::graphics::Camera &camera);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &worldmtx,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &worldmtx,rinfw::graphics::Camera &camera,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &viewmtx,rinfw::core::math::Matrix44 &projectionmtx,rinfw::core::math::Matrix44 &worldmtx,ID3D11DeviceContext *context ,bool wireframe = false);

protected:
private:
	GPLModel();
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);
	bool BlendDebug();

#pragma pack(push,1)
	struct PointLight {
		rinfw::core::math::Float4 pos[30];
		rinfw::core::math::Float4 color[30];
		int num[4];
	};
	struct SpotLight {
		rinfw::core::math::Float4 pos[4];
		rinfw::core::math::Float4 color[4];
		rinfw::core::math::Float4 dir[4];
		int num[4];
	};
#pragma pack(pop)
	PointLight point;
	SpotLight spot;

	gpls::IA ia;
	gpls::OM om;
	gpls::RS rs;
	gpls::RS rs_cullnone;
	gpls::RS rs_cullwire;
	//gpls::RS rs;
	gpls::VS vs;
	gpls::PS ps;
	gpls::PS ps_deferred;

};

}
}
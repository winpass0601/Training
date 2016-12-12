#pragma once
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./GPLS/GPLS.h>
#include <./Component/Model/PmxModel.h>
#include <./Core/Math/Math.h>
#include <./Graphics/Camera.h>

namespace rinfw {
namespace graphics {


class GPLPointLight {
public:
	static GPLPointLight &getInstance() {
		static GPLPointLight ins;
		return ins;
	}
	bool createPipeLine(ID3D11Device *device);
	void Release();
	bool Draw(core::PmxModel &model);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world);
	bool Draw(core::PmxModel &model,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &worldmtx,ID3D11DeviceContext *context);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &viewmtx,rinfw::core::math::Matrix44 &projectionmtx,rinfw::core::math::Matrix44 &worldmtx,ID3D11DeviceContext *context ,bool wireframe = false);

	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &world,rinfw::graphics::Camera &camera);
	bool Draw(core::PmxModel &model,rinfw::core::math::Matrix44 &worldmtx,rinfw::graphics::Camera &camera,ID3D11DeviceContext *context);

protected:
private:
	struct PointStruct {
		float window[2];//uv•ÏŠ·‚ÉŽg‚¤
		rinfw::core::math::Float3 pos;//
		rinfw::core::math::Float3 color;//
		float lightlen;
		float dummy[3];
	};
	GPLPointLight();
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);

	gpls::IA ia;
	gpls::OM om;
	gpls::RS rs;
	gpls::RS rs_wire;
	gpls::VS vs;
	gpls::PS ps;

};

}
}
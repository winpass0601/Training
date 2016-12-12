#pragma once
#include <./Macro.h>
#include <./GPLS/GPLS.h>
#include <./Core/Math/Math.h>
#include <vector>
//! Lineï`âÊêÍóp

#define LINE_MAX (256)

namespace rinfw {
namespace graphics {

struct Line {
	rinfw::core::math::Point pos;
	rinfw::core::math::Float3 color;
};

class GPLLine {
public:
	static GPLLine &getInstance() {
		static GPLLine ins;
		return ins;
	}
	bool createPipeLine(ID3D11Device *device);
	void Release();
	bool Draw();
	bool Draw(rinfw::core::math::Matrix44 & world);

protected:
private:
	GPLLine();
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);
	DEBUG(bool BlendDebug());

	gpls::IA ia;
	gpls::OM om;
	gpls::RS rs;
	gpls::VS vs;
	gpls::PS ps;

	std::vector<Line> lines;
};

}
}
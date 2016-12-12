#pragma once
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./GPLS/GPLS.h>
#include <./Core/Math/Math.h>
#include <./Resource/Texture.h>

//! Pmx—pƒ‚ƒfƒ‹•`‰æê—p

namespace rinfw {
namespace graphics {
class GPLDeferred{
public:
	static GPLDeferred &getInstance() {
		static GPLDeferred ins;
		return ins;
	}
	bool createPipeLine(ID3D11Device *device);
	void Release();
	bool Clear(ID3D11DeviceContext *context);
	bool setStatus(ID3D11DeviceContext *context);
	bool Update();
	bool Draw(ID3D11DeviceContext *context);

	bool setActive(bool enabled);
	bool getActive() { return this->enabled; }
	COMPTR(ID3D11RenderTargetView) getRTVLightBuffer() { return this->gbuffer[2].getCOMRTV(); }
	COMPTR(ID3D11ShaderResourceView) getSRVNormalBuffer() { return this->gbuffer[1].getCOMSRV(); }
	COMPTR(ID3D11ShaderResourceView) getSRVWorldBuffer() { return this->gbuffer[3].getCOMSRV(); }
private:
	bool createIA(ID3D11Device *device);
	bool createOM(ID3D11Device *device);
	bool createRS(ID3D11Device *device);
	bool createVS(ID3D11Device *device);
	bool createPS(ID3D11Device *device);


	gpls::IA ia;
	gpls::OM om;
	gpls::OM om_final;
	gpls::RS rs;
	gpls::VS vs;
	gpls::PS ps;
	float clearcolor[4][4];
	Texture2D gbuffer[4];//albed,(normal rgb,depth a),(Lighitmap rgb , shadow a),worldpos(float3) depth
	COMPTR(ID3D11Buffer) vertex;
	bool enabled;
};

}
}
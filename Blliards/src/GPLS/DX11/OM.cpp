#pragma once
#include<./GPLS/DX11/OM.h>
#include<./Device/DirectXDevice.h>

namespace rinfw {
namespace graphics {
namespace gpls {

OM::OM() {
	this->blendfactor[0] = 0.0f;
	this->blendfactor[1] = 0.0f;
	this->blendfactor[2] = 0.0f;
	this->blendfactor[3] = 0.0f;
	this->stencilref = 0;
}
OM::OM(const OM &om) {
	this->blendstate = this->blendstate;
	this->blendfactor[0] = om.blendfactor[0];
	this->blendfactor[1] = om.blendfactor[1];
	this->blendfactor[2] = om.blendfactor[2];
	this->blendfactor[3] = om.blendfactor[3];
	this->samplemask = om.samplemask;

	this->dss = om.dss;
	this->stencilref = om.stencilref;

	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		this->rtv[i] = om.rtv[i];
	}
	this->dsv = dsv;
}
OM::OM(const OM &om,bool blendstate, bool dss, bool rtv, bool depthstencilvie) {
	if (blendstate) {
		this->blendstate = this->blendstate;
		this->blendfactor[0] = om.blendfactor[0];
		this->blendfactor[1] = om.blendfactor[1];
		this->blendfactor[2] = om.blendfactor[2];
		this->blendfactor[3] = om.blendfactor[3];
		this->samplemask = om.samplemask;
	}

	if (dss) {
		this->dss = om.dss;
		this->stencilref = om.stencilref;
	}

	if (rtv) {
		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			this->rtv[i] = om.rtv[i];
		}
	}

	if (depthstencilvie) {
		this->dsv = dsv;
	}
}

void OM::Release() {
	this->BlendStateRelease();
	this->DepthStencilStateRelease();
	this->RenderTargetViewsRelease();
}
bool OM::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool OM::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setBlendState(context)) {
		return false;
	}
	if (!this->setDepthStencilState(context)) {
		return false;
	}
	if (!this->setRenderTargetView(context)) {
		return false;
	}
	return true;
}
bool OM::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool OM::getStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->getBlendState(context)) {
		return false;
	}
	if (!this->getDepthStencilState(context)) {
		return false;
	}
	if (!this->getRenderTargetView(context)) {
		return false;
	}
	return true;
}

}
}
}
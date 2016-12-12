#pragma once
#include<./GPLS/DX11/RS.h>
#include<./Device/DirectXDevice.h>
#include<./Device/WindowDevice.h>
namespace rinfw {
namespace graphics {
namespace gpls {

RS::RS() {
	WindowDevice &window = WindowDevice::getInstance();
	D3D11_VIEWPORT vp;
	vp.Width = (float)window.getWidth();
	vp.Height = (float)window.getHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	for (auto &v : this->viewport) {
		v = vp;
	}
}
RS::RS(const RS &rs) {
	for (UINT i = 0; i < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; i++) {
		this->viewport[i] = rs.viewport[i];
	}
	this->rasterizerstate = rs.rasterizerstate;
}
RS::RS(const RS &rs, bool viewport, bool rasterizerstate) {
	if (viewport) {
		for (UINT i = 0; i < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; i++) {
			this->viewport[i] = rs.viewport[i];
		}
	}
	if (rasterizerstate) {
		this->rasterizerstate = rs.rasterizerstate;
	}
}

void RS::Release() {
	this->ViewPortRelease();
	this->RasterizerStateRelease();

}
bool RS::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool RS::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setViewPort(context)) {
		return false;
	}
	if (!this->setRasterizerState(context)) {
		return false;
	}
	return true;
}
bool RS::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool RS::getStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->getViewPort(context)) {
		return false;
	}
	if (!this->getRasterizerState(context)) {
		return false;
	}
	return true;
}




}
}
}
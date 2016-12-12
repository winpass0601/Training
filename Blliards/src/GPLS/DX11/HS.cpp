#pragma once
#include<./GPLS/DX11/HS.h>
#include<./Device/DirectXDevice.h>

using namespace  rinfw::graphics::gpls;

HS::HS() {

}
HS::HS(const HS & hs){

	this->shader = hs.shader;

	for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
		this->buffers[i] = hs.buffers[i];
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
		this->samplers[i] = hs.samplers[i];
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
		this->srv[i] = hs.srv[i];
	}
}
HS::HS(const HS &hs, bool shader, bool buffers, bool samplers, bool srv) {
	if (shader) {
		this->shader = hs.shader;
	}
	if (buffers) {
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			this->buffers[i] = hs.buffers[i];
		}
	}
	if(samplers){
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			this->samplers[i] = hs.samplers[i];
		}
	}
	if(srv){
		for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			this->srv[i] = hs.srv[i];
		}
	}
}

void HS::Release() {

	if (this->shader) {
		this->shader.Release();
		this->shader.Detach();
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
		if(this->buffers[i]){
			this->buffers[i].Release();
			this->buffers[i].Detach();
		}
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
		if (this->samplers[i]) {
			this->samplers[i].Release();
			this->samplers[i].Detach();
		}
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
		if (this->srv[i]) {
			this->srv[i].Release();
			this->srv[i].Detach();
		}
	}
}
bool HS::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool HS::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setShader(context)) {
		return false;
	}
	if (!this->setBuffertoHS(context)) {
		return false;
	}
	if (!this->setSamplerStatetoHS(context)) {
		return false;
	}
	if (!this->setShaderResourceView(context)) {
		return false;
	}
	return true;
}
bool HS::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool HS::getStatus(ID3D11DeviceContext *context){
	if (context == nullptr) {
		return false;
	}
	if (!this->getShader(context)) {
		return false;
	}
	if (!this->getBuffertoHS(context)) {
		return false;
	}
	if (!this->getSamplerStatetoHS(context)) {
		return false;
	}
	if (!this->getShaderResourceView(context)) {
		return false;
	}
	return true;
}
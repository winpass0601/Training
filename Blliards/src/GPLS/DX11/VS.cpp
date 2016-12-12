#pragma once
#include<./GPLS/DX11/VS.h>
#include<./Device/DirectXDevice.h>

using namespace rinfw::graphics::gpls;

VS::VS() {

}
VS::VS(const VS & vs){

	this->shader = vs.shader;

	for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
		this->buffers[i] = vs.buffers[i];
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
		this->samplers[i] = vs.samplers[i];
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
		this->srv[i] = vs.srv[i];
	}
}
VS::VS(const VS &vs, bool shader, bool buffers, bool samplers, bool srv) {
	if (shader) {
		this->shader = vs.shader;
	}
	if (buffers) {
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			this->buffers[i] = vs.buffers[i];
		}
	}
	if(samplers){
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			this->samplers[i] = vs.samplers[i];
		}
	}
	if(srv){
		for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			this->srv[i] = vs.srv[i];
		}
	}
}

void VS::Release() {

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

	this->SRVRelease();
}
bool VS::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool VS::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setShader(context)) {
		return false;
	}
	if (!this->setBuffertoVS(context)) {
		return false;
	}
	if (!this->setSamplerStatetoVS(context)) {
		return false;
	}
	if (!this->setShaderResourceView(context)) {
		return false;
	}
	return true;
}
bool VS::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool VS::getStatus(ID3D11DeviceContext *context){
	if (context == nullptr) {
		return false;
	}
	if (!this->getShader(context)) {
		return false;
	}
	if (!this->getBuffertoVS(context)) {
		return false;
	}
	if (!this->getSamplerStatetoVS(context)) {
		return false;
	}
	if (!this->getShaderResourceView(context)) {
		return false;
	}
	return true;
}

#pragma once
#include<./GPLS/DX11/PS.h>
#include<./Device/DirectXDevice.h>

using namespace rinfw::graphics::gpls;
PS::PS() {

}
PS::PS(const PS & vs){

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
PS::PS(const PS &vs, bool shader, bool buffers, bool samplers, bool srv) {
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

void PS::Release() {

	if(this->shader){
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
bool PS::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool PS::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setShader(context)) {
		return false;
	}
	if (!this->setBuffertoPS(context)) {
		return false;
	}
	if (!this->setSamplerStatetoPS(context)) {
		return false;
	}
	if (!this->setShaderResourceView(context)) {
		return false;
	}
	return true;
}
bool PS::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool PS::getStatus(ID3D11DeviceContext *context){
	if (context == nullptr) {
		return false;
	}
	if (!this->getShader(context)) {
		return false;
	}
	if (!this->getBuffertoPS(context)) {
		return false;
	}
	if (!this->getSamplerStatetoPS(context)) {
		return false;
	}
	if (!this->getShaderResourceView(context)) {
		return false;
	}
	return true;
}

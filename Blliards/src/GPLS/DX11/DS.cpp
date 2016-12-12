#pragma once
#include<./GPLS/DX11/DS.h>
#include<./Device/DirectXDevice.h>

namespace rinfw {
namespace graphics {
namespace gpls {

DS::DS() {

}
DS::DS(const DS & ds){

	this->shader = ds.shader;

	for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
		this->buffers[i] = ds.buffers[i];
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
		this->samplers[i] = ds.samplers[i];
	}

	for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
		this->srv[i] = ds.srv[i];
	}
}
DS::DS(const DS &ds, bool shader, bool buffers, bool samplers, bool srv) {
	if (shader) {
		this->shader = ds.shader;
	}
	if (buffers) {
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			this->buffers[i] = ds.buffers[i];
		}
	}
	if(samplers){
		for (UINT i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++) {
			this->samplers[i] = ds.samplers[i];
		}
	}
	if(srv){
		for (UINT i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++) {
			this->srv[i] = ds.srv[i];
		}
	}
}

void DS::Release() {

	this->ShaderRelease();
	this->BufferRelease();
	this->SamplerStateRelease();
	this->SRVRelease();

}
bool DS::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool DS::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setShader(context)) {
		return false;
	}
	if (!this->setBuffertoDS(context)) {
		return false;
	}
	if (!this->setSamplerStatetoDS(context)) {
		return false;
	}
	if (!this->setShaderResourceView(context)) {
		return false;
	}
	return true;
}
bool DS::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool DS::getStatus(ID3D11DeviceContext *context){
	if (context == nullptr) {
		return false;
	}
	if (!this->getShader(context)) {
		return false;
	}
	if (!this->getBuffertoDS(context)) {
		return false;
	}
	if (!this->getSamplerStatetoDS(context)) {
		return false;
	}
	if (!this->getShaderResourceView(context)) {
		return false;
	}
	return true;
}
}
}
}
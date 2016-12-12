#pragma once
#include<./GPLS/DX11/IA.h>
#include<./Device/DirectXDevice.h>

namespace rinfw {
namespace graphics {
namespace gpls {

IA::IA() {
	//‰ŠúÝ’è
	ZeroMemory(this->vertex_strides, sizeof(UINT)*D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
	ZeroMemory(this->vertex_offsets, sizeof(UINT)*D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);

	this->idx_format = DXGI_FORMAT_R32_UINT;
	this->idx_offset = 0;

	this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
IA::IA(const IA &ia) {

	this->inputlayout = ia.inputlayout;

	for (UINT i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++) {
		this->vertex[i] = ia.vertex[i];
		this->vertex_strides[i] = ia.vertex_strides[i];
		this->vertex_offsets[i] = ia.vertex_offsets[i];
	}

	this->idxvertex = ia.idxvertex;
	this->idx_format = ia.idx_format;
	this->idx_offset = ia.idx_offset;

	this->topology = ia.topology;
}
IA::IA(const IA &ia, bool inputlayout, bool vertex, bool index, bool topology) {

	if(inputlayout){
		this->inputlayout = ia.inputlayout;
	}

	if(vertex){
		for (UINT i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++) {
			this->vertex[i] = ia.vertex[i];
			this->vertex_strides[i] = ia.vertex_strides[i];
			this->vertex_offsets[i] = ia.vertex_offsets[i];
		}
	}

	if(index){
		this->idxvertex = ia.idxvertex;
		this->idx_format = ia.idx_format;
		this->idx_offset = ia.idx_offset;
	}

	if(topology){
		this->topology = ia.topology;
	}
}

void IA::Release() {

	if (this->inputlayout) {
		this->inputlayout.Release();
		this->inputlayout.Detach();
	}

	ZeroMemory(this->vertex_strides, sizeof(UINT)*D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
	ZeroMemory(this->vertex_offsets, sizeof(UINT)*D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
	for (auto &v : this->vertex) {
		if (v) {
			v->Release();
			v.Detach();
		}
	}

	if(this->idxvertex){
		this->idxvertex.Release();
		this->idxvertex.Detach();
	}
	this->idx_format = DXGI_FORMAT_R32_UINT;
	this->idx_offset = 0;

	this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

}

bool IA::setStatus() {
	return this->setStatus(DX11Device::getInstance().getContext());
}
bool IA::setStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->setInputLayout(context)) {
		return false;
	}
	if (!this->setVertex(context)) {
		return false;
	}
	if (!this->setIndex(context)) {
		return false;
	}
	if (!this->setTopology(context)) {
		return false;
	}
	return true;
}

bool IA::getStatus() {
	return this->getStatus(DX11Device::getInstance().getContext());
}
bool IA::getStatus(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	if (!this->getInputLayout(context)) {
		return false;
	}
	if (!this->getVertex(context)) {
		return false;
	}
	if (!this->getIndex(context)) {
		return false;
	}
	if (!this->getTopology(context)) {
		return false;
	}
	return true;
}
//ŠO•”


bool IA::setVertex() {
	return this->setVertex(DX11Device::getInstance().getContext());
}
bool IA::setVertex(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	ID3D11Buffer *vertex[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
	for (UINT i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++) {
		vertex[i] = this->vertex[i].GetInterfacePtr();
	}
	context->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, vertex, this->vertex_strides, this->vertex_offsets);
	return true;
}
bool IA::getVertex() {
	return this->getVertex(DX11Device::getInstance().getContext());
}
bool IA::getVertex(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	for (UINT i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++) {
		ID3D11Buffer *vertex = nullptr;
		context->IAGetVertexBuffers(i, 1, &vertex, &this->vertex_strides[i], &this->vertex_offsets[i]);

		if (this->vertex[i].GetInterfacePtr() != nullptr) {
			this->vertex[i].Release();
			this->vertex[i].Detach();
		}
		if (vertex != nullptr) {
			this->vertex[i].Attach(vertex);
		}
	}
	return true;
}

bool IA::setIndex() {
	return this->setIndex(DX11Device::getInstance().getContext());
}
bool IA::setIndex(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	context->IASetIndexBuffer(this->idxvertex.GetInterfacePtr(), this->idx_format, this->idx_offset);
	return true;
}
bool IA::getIndex() {
	return this->getIndex(DX11Device::getInstance().getContext());
}
bool IA::getIndex(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	ID3D11Buffer *idxvertex = nullptr;
	context->IAGetIndexBuffer(&idxvertex, &this->idx_format, &this->idx_offset);
	if (this->idxvertex.GetInterfacePtr() != nullptr) {
		this->idxvertex.Release();
		this->idxvertex.Detach();
	}
	if (idxvertex != nullptr) {
		this->idxvertex.Attach(idxvertex);
	}
	return true;
}

bool IA::setTopology() {
	return this->setTopology(DX11Device::getInstance().getContext());
}
bool IA::setTopology(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	context->IASetPrimitiveTopology(this->topology);
	return true;
}
bool IA::getTopology() {
	return this->getTopology(DX11Device::getInstance().getContext());
}
bool IA::getTopology(ID3D11DeviceContext *context) {
	if (context == nullptr) {
		return false;
	}
	context->IAGetPrimitiveTopology(&this->topology);
	return true;
}

//“à•”

bool IA::setGPLSVertex(UINT slot,ID3D11Buffer *vertexbuffer,UINT Strides,UINT Offsets,bool write){
	if (slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
		|| (!write && this->vertex[slot].GetInterfacePtr() != nullptr)) {
		return false;
	}
	if (this->vertex[slot].GetInterfacePtr() != nullptr) {
		this->vertex[slot].Release();
		this->vertex[slot].Detach();
	}
	if (vertexbuffer != nullptr) {
		this->vertex[slot].Attach(vertexbuffer);
		this->vertex_strides[slot] = Strides;
		this->vertex_offsets[slot] = Offsets;
	}
	return true;
}
bool IA::setGPLSVertex(UINT slot, COMPTR(ID3D11Buffer) vertexbuffer, UINT Strides, UINT Offsets, bool write) {
	if (slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
		|| (!write && this->vertex[slot].GetInterfacePtr() != nullptr)) {
		return false;
	}
	if (this->vertex[slot].GetInterfacePtr() != nullptr) {
		this->vertex[slot].Release();
		this->vertex[slot].Detach();
	}
	if (vertexbuffer.GetInterfacePtr() != nullptr) {
		this->vertex[slot] = vertexbuffer;
		this->vertex_strides[slot] = Strides;
		this->vertex_offsets[slot] = Offsets;
	}
	return true;
}
COMPTR(ID3D11Buffer) IA::getGPLSVertex(UINT slot) {
	if (slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT) {
		return false;
	}
	return this->vertex[slot];
}

bool IA::setGPLSIndex(ID3D11Buffer *indexbuffer, DXGI_FORMAT format, UINT offset,bool write) {
	if (!write && this->idxvertex.GetInterfacePtr() != nullptr) {
		return false;
	}
	if (this->idxvertex.GetInterfacePtr() != nullptr) {
		this->idxvertex.Release();
		this->idxvertex.Detach();
	}
	if (indexbuffer != nullptr) {
		this->idxvertex.Attach(indexbuffer);
		this->idx_format = format;
		this->idx_offset = offset;
	}
	return true;
}
bool IA::setGPLSIndex(COMPTR(ID3D11Buffer)indexbuffer, DXGI_FORMAT format, UINT offset, bool write) {
	if (!write && this->idxvertex.GetInterfacePtr() != nullptr) {
		return false;
	}
	if (this->idxvertex.GetInterfacePtr() != nullptr) {
		this->idxvertex.Release();
		this->idxvertex.Detach();
	}
	if (indexbuffer.GetInterfacePtr() != nullptr) {
		this->idxvertex = indexbuffer;
		this->idx_format = format;
		this->idx_offset = offset;
	}
	return true;
}
COMPTR(ID3D11Buffer) IA::getGPLSIndex() {
	return this->idxvertex;
}

void IA::setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
	this->topology = topology;
}
D3D11_PRIMITIVE_TOPOLOGY IA::getGPLSTopology() {
	return this->topology;
}

}
}
}

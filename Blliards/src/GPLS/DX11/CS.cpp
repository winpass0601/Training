#pragma once
#include<./GPLS/DX11/CS.h>
#include<./Device/DirectXDevice.h>
using namespace rinfw::graphics::gpls;


CS::CS() {

}
CS::CS(const CS &cs) {
	this->shader = cs.shader;

}
CS::CS(const CS &cs,bool shader,bool buffers,bool unorderedaccessview,bool srv) {
	if (shader) {
		this->shader = cs.shader;
	}

	if (buffers) {
		for (int i = 0; i < ARRAYSIZE(this->buffers); i++) {
			this->buffers[i] = cs.buffers[i];
		}
	}

	if (unorderedaccessview) {
		for (int i = 0; i < ARRAYSIZE(this->unorderedaccessviews); i++) {
			this->unorderedaccessviews[i] = cs.unorderedaccessviews[i];
		}
	}

	if (srv) {
		for (int i = 0; i < ARRAYSIZE(this->srv); i++) {
			this->srv[i] = cs.srv[i];
		}
	}
}

void CS::Release() {
	this->ShaderRelease();
	this->BufferRelease();
	this->UnorderedAccessViewRelease();
	this->SRVRelease();
}
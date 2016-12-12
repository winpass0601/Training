#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <d3d11_1.h>

namespace rinfw {
namespace graphics {
namespace gplsi {

class IBuffer {
public:
	IBuffer() {}
	IBuffer(const IBuffer &obj) {
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			this->buffers[i] = obj.buffers[i];
		}
	}
	void BufferRelease() {
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			if (this->buffers[i]) {
				this->buffers[i].Release();
				this->buffers[i].Detach();
			}
		}
	}

	bool setBuffertoVS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			buffers[i] = this->buffers[i].GetInterfacePtr();
		}
		context->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		return true;
	}
	bool getBuffertoVS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		context->VSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			if (this->buffers[i].GetInterfacePtr() != nullptr) {
				this->buffers[i].Release();
				this->buffers[i].Detach();
			}
			if (buffers[i] != nullptr) {
				this->buffers[i].Attach(buffers[i]);
			}
		}
		return true;
	}
	bool setBuffertoHS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			buffers[i] = this->buffers[i].GetInterfacePtr();
		}
		context->HSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		return true;
	}
	bool getBuffertoHS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		context->HSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			if (this->buffers[i].GetInterfacePtr() != nullptr) {
				this->buffers[i].Release();
				this->buffers[i].Detach();
			}
			if (buffers[i] != nullptr) {
				this->buffers[i].Attach(buffers[i]);
			}
		}
		return true;
	}
	bool setBuffertoDS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			buffers[i] = this->buffers[i].GetInterfacePtr();
		}
		context->DSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		return true;
	}
	bool getBuffertoDS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		context->DSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			if (this->buffers[i].GetInterfacePtr() != nullptr) {
				this->buffers[i].Release();
				this->buffers[i].Detach();
			}
			if (buffers[i] != nullptr) {
				this->buffers[i].Attach(buffers[i]);
			}
		}
		return true;
	}
	bool setBuffertoPS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			buffers[i] = this->buffers[i].GetInterfacePtr();
		}
		context->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		return true;
	}
	bool getBuffertoPS(ID3D11DeviceContext *context) {
		if (context == nullptr) {
			return false;
		}
		ID3D11Buffer *buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		context->PSGetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, buffers);
		for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++) {
			if (this->buffers[i].GetInterfacePtr() != nullptr) {
				this->buffers[i].Release();
				this->buffers[i].Detach();
			}
			if (buffers[i] != nullptr) {
				this->buffers[i].Attach(buffers[i]);
			}
		}
		return true;
	}

	bool setGPLSBuffer(UINT idx, ID3D11Buffer *buffer, bool write = true) {
		if (idx >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT || (!write&&this->buffers[idx].GetInterfacePtr() != nullptr)) {
			return false;
		}
		if (this->buffers[idx].GetInterfacePtr() != nullptr) {
			this->buffers[idx].Release();
			this->buffers[idx].Detach();
		}
		if (buffer != nullptr) {
			this->buffers[idx].Attach(buffer);
		}
		return true;
	}
	bool setGPLSBuffer(UINT idx, COMPTR(ID3D11Buffer) buffer, bool write = true) {
		if (idx >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT || (!write&&this->buffers[idx].GetInterfacePtr() != nullptr)) {
			return false;
		}
		if (this->buffers[idx].GetInterfacePtr() != nullptr) {
			this->buffers[idx].Release();
			this->buffers[idx].Detach();
		}
		if (buffer.GetInterfacePtr() != nullptr) {
			this->buffers[idx] = buffer;
		}
		return true;
	}
	COMPTR(ID3D11Buffer) getGPLSBuffer(UINT idx) {
		if (idx >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) {
			return false;
		}
		return this->buffers[idx];
	}

protected:
	COMPTR(ID3D11Buffer) buffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
};
}
}
}
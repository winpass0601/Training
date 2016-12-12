#pragma once
#include <./Core/Manager.h>

#include <./Device/DirectXDevice.h>

#include <./Macro.h>


namespace rinfw {
namespace core {
class Model : public rinfw::core::Component {

protected:
	COMPTR(ID3D11Buffer)vertexBuffer;
	COMPTR(ID3D11Buffer)indexBuffer;

	COMPTR(ID3D11Buffer)materialBuffer;

public:

	ID3D11Buffer* getVertexBuffer(){
		return vertexBuffer.GetInterfacePtr();
	}
	ID3D11Buffer* getIndexBuffer() {
		return indexBuffer.GetInterfacePtr();
	}
	ID3D11Buffer* getMaterialBuffer() {
		return materialBuffer.GetInterfacePtr();
	}

	void Delete()override{};
	void Release()override{};
	void Create()override{};
};
}
}
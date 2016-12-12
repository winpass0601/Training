#pragma once
#pragma warning(disable:4005)
#include<./Macro.h>
#include <D3D11_1.h>
#include <./GPLS/DX11/Interfaces.h>

namespace rinfw {
namespace graphics{
namespace gpls {
class IA :
	public gplsi::IInputLayout {
public:
	IA();
	IA(const IA &ia);
	IA(const IA &ia, bool inputlayout, bool vertex, bool index, bool topology);

	void Release();
	bool setStatus();
	bool setStatus(ID3D11DeviceContext *context);
	bool getStatus();
	bool getStatus(ID3D11DeviceContext *context);

	//ŠO•”

	bool setVertex();
	bool setVertex(ID3D11DeviceContext *context);
	bool getVertex();
	bool getVertex(ID3D11DeviceContext *context);


	bool setIndex();
	bool setIndex(ID3D11DeviceContext *context);
	bool getIndex();
	bool getIndex(ID3D11DeviceContext *context);

	bool setTopology();
	bool setTopology(ID3D11DeviceContext *context);
	bool getTopology();
	bool getTopology(ID3D11DeviceContext *context);


	//“à•”
	bool setGPLSVertex(UINT slot, ID3D11Buffer*, UINT Strides, UINT Offsets, bool write = true);
	bool setGPLSVertex(UINT slot, COMPTR(ID3D11Buffer), UINT Strides, UINT Offsets, bool write = true);
	COMPTR(ID3D11Buffer) getGPLSVertex(UINT slot);

	//bool setVertexs

	bool setGPLSIndex(ID3D11Buffer*, DXGI_FORMAT format, UINT offset, bool write = true);
	bool setGPLSIndex(COMPTR(ID3D11Buffer), DXGI_FORMAT format, UINT offset, bool write = true);
	COMPTR(ID3D11Buffer) getGPLSIndex();

	void setGPLSTopology(D3D11_PRIMITIVE_TOPOLOGY);
	D3D11_PRIMITIVE_TOPOLOGY getGPLSTopology();


protected:
private:

	//IASetVertexBuffers
	COMPTR(ID3D11Buffer) vertex[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	UINT vertex_strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	UINT vertex_offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

	//IASetIndexBuffer
	COMPTR(ID3D11Buffer) idxvertex;
	DXGI_FORMAT idx_format;
	UINT idx_offset;

	//IASetPrimitiveTopology
	D3D11_PRIMITIVE_TOPOLOGY topology;
};
}
}
}
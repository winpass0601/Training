#ifndef _HEADER_GFW_DIRECTX_
#define _HEADER_GFW_DIRECTX_
#pragma warning(push)
#pragma warning(disable:4005)

#include <D3D11_1.h>
#include <D3DX11.h>
#ifdef _Debug
#include <dxgidebug.h>
#endif
#include <Macro.h>

class DX11Device {
public:
	virtual ~DX11Device();
	static DX11Device &getInstance() {
		static DX11Device ins;
		return ins;
	}

	unsigned long Init(HWND, unsigned long, unsigned long, bool);
	ID3D11Device *getDevice() {
		return this->device;
	}
	ID3D11DeviceContext *getContext() {
		return this->context;
	}
	IDXGISwapChain *getSwapChain() {
		return this->swapchain;
	}
#ifdef _Debug
	ID3D11Debug *getDebug() {
		return this->debug;
	}
	ID3DUserDefinedAnnotation *getAnotation() {
		return this->anotation;
	}
#endif
	ID3D11RenderTargetView *getRTV() { return this->rendertargetview; }
	COMPTR(ID3D11RenderTargetView) &getCOMRTV() { return this->rendertargetview; }
	ID3D11DepthStencilView *getDSV() { return this->dsv; }
	COMPTR(ID3D11DepthStencilView) &getCOMDSV() { return this->dsv; }
protected:
private:
	DX11Device();
	D3D_DRIVER_TYPE drivertype;
	D3D_FEATURE_LEVEL feature;
	COMPTR(ID3D11Device) device;
	//COMPTR(ID3D11VideoDevice) video;
	COMPTR(ID3D11DeviceContext) context;
	COMPTR(IDXGISwapChain) swapchain;
	DEBUG(COMPTR(ID3D11Debug) debug);
	DEBUG(COMPTR(ID3DUserDefinedAnnotation) anotation);

	COMPTR(ID3D11RenderTargetView)		rendertargetview;// レンダーターゲットビュー
	COMPTR(ID3D11DepthStencilView)		dsv;// ステンシルビュー
};

#pragma warning(pop)

#endif
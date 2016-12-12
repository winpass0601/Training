#include <./GUI.h>
#include <./Device.h>

bool rinfw::gui::GuiInit() {
	DX11Device &device = DX11Device::getInstance();
	WindowDevice &window = WindowDevice::getInstance();
	ImGui_ImplDX11_Init(window.getHandle(), device.getDevice(), device.getContext());
	return true;
}
bool rinfw::gui::GuiUpdateBegin() {
	ImGui_ImplDX11_NewFrame();

	return true;
}
bool rinfw::gui::GuiUpdateEnd() {
	DX11Device &device = DX11Device::getInstance();
//#ifdef _Debug
	//device.getAnotation()->BeginEvent(L"ImGui");
	ID3D11RenderTargetView *rtv[] = {
		device.getRTV(),
	};
	device.getContext()->OMSetRenderTargets(1, rtv, nullptr);
	ImGui::Render();
//#endif
	return true;
}
bool rinfw::gui::GuiRelease() {
	ImGui_ImplDX11_Shutdown();
	return true;
}

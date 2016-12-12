#include <Windows.h>
#include <stdio.h>
#include <comip.h>
#include <./Core/Manager.h>
#include <./Core/ResourceManager.h>
#include <./Device.h>
#include <./GUI.h>

#include <./Core/FPSManager.h>

//各シーン
#include <./Scene/GameMain.h>
using namespace rinfw::core;
using namespace rinfw::gui;
using namespace rinfw::graphics;
using namespace rinfw::scene;



int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	DEBUG(_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));


	WindowDevice &window = WindowDevice::getInstance();
	DX11Device &device = DX11Device::getInstance();
	auto seq = SequenceManager::getInstance();

	window.Init(hInstance, nCmdShow, TEXT("rinrin Blliards"), 1280, 720, true);
	device.Init(window.getHandle(), window.getWidth(), window.getHeight(), window.getWindowMode());

	GuiInit();
	//+ 各シーンの作成
	SharedPtr<Scene> gamemain(new GameMain);
	seq.Attach(gamemain);
	seq.setCurrent(gamemain);

	seq.Init();
	int ret = 0;

	do {
		float clear [] = { 0.2f, 0.2f, 0.2f, 0.0f };
		device.getContext()->ClearRenderTargetView(device.getRTV(), clear);
		device.getContext()->ClearDepthStencilView(device.getDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		GuiUpdateBegin();
		ImGui::Text("Application.average %.3f ms/frame(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // FPS
		ImGui::Text("FPS:%f", FPSManager::getInstance().getFPS()); // FPS
		ImGui::Text("deltaTime:%f", FPSManager::getInstance().getDeltaTime()); // FPS

		seq.Update();
		seq.Draw();

		GuiUpdateEnd();
		FPSManager::getInstance().waitFPS();
		device.getSwapChain()->Present(0, 0);
		ret = window.MessageLoop();
	} while (ret != WM_QUIT);
	seq.Release();
	GuiRelease();

	//DEBUG(device.getDebug()->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY));
	return ret;
}
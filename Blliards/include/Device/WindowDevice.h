#ifndef _HEADER_GFW_WINDOW_
#define _HEADER_GFW_WINDOW_
#pragma warning(push)
#include <Macro.h>
#include <Windows.h>


class WindowDevice {
public:
	virtual ~WindowDevice();
	static WindowDevice &getInstance() {
		static WindowDevice ins;
		return ins;
	}
	unsigned long Init(HINSTANCE,int,String, unsigned long, unsigned long,bool = true);

	void setTitle(String);
	String getTitle();
	void ReSize(ULONG, ULONG);
	bool getRunFlg() { return this->runflg; }
	ULONG MessageLoop();
	HWND getHandle() { return this->handle; }
	unsigned long getWidth() { return this->width; }
	unsigned long getHeight() { return this->height; }
	bool getWindowMode() { return this->windowmode; }
protected:
private:
	WindowDevice();

	HWND handle;
	WNDCLASSEX wcex;
	String title;
	bool windowmode;
	UINT width, height;

	volatile bool runflg;
};

#pragma warning(pop)
#endif
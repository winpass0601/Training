#pragma once
#include <Macro.h>

//#ifdef _Debug
#include <./GUI/imgui.h>
#include <./GUI/imgui_internal.h>
#include <./GUI/imgui_impl_dx11.h>
//#else
//#endif

namespace rinfw {
namespace gui {
bool GuiInit();
bool GuiUpdateBegin();
bool GuiUpdateEnd();
bool GuiRelease();



}
}
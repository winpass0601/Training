#include <Device.h>
#include <./Graphics/Camera.h>
#ifdef _Debug
#include <./GUI.h>
#endif

namespace rinfw {
namespace graphics {

Camera::Camera() {
	this->perspective = true;
	this->wireframe = false;
	this->fov = 45.0f;
	this->nearz = 0.1f;
	this->farz = 1.0f;
	this->width = (float)WindowDevice::getInstance().getWidth();
	this->height = (float)WindowDevice::getInstance().getHeight();
	this->pos = rinfw::core::math::Float3(0, 0, 0);
	this->at = rinfw::core::math::Float3(0, 0, 1);
	this->up = rinfw::core::math::Float3(0, 1, 0);
	DEBUG(this->showdebug = false);
}
Camera::Camera(const Camera &camera) {
	this->pos = camera.pos;
	this->at = camera.at;
	this->up = camera.up;
	this->fov = camera.fov;
	this->width = camera.width;
	this->height = camera.height;
	this->nearz = camera.nearz;
	this->farz = camera.farz;
	this->perspective = camera.perspective;
	if (this->buffer.GetInterfacePtr() == nullptr) {
		this->Create();
	}
}

bool Camera::Create() {
	return this->Create(DX11Device::getInstance().getDevice());
}
bool Camera::Create(ID3D11Device *device) {
	if (device == nullptr) {
		return false;
	}

	ID3D11Buffer *constantbuffer = nullptr;
	D3D11_BUFFER_DESC bd; // 生成方法(バッファー リソース)
	ZeroMemory(&bd, sizeof(bd)); // 中身をゼロクリア
	// Bufferの生成方法の格納
	bd.Usage = D3D11_USAGE_DEFAULT; // バッファーで想定されている読み込みおよび書き込みの方法を識別
	bd.CPUAccessFlags = 0; // CPUからは書き込みのみ行います
	bd.ByteWidth = sizeof(CameraResource);  // バッファーのサイズ(バイト単位)
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // なんのバッファですか？
	bd.StructureByteStride = sizeof(float);
	if (FAILED(device->CreateBuffer(&bd, nullptr, &constantbuffer))) {
		return false;
	}
	this->buffer.Attach(constantbuffer);
	return true;
}

bool Camera::Update() {
	return this->Update(DX11Device::getInstance().getContext());
}
bool Camera::Update(ID3D11DeviceContext *context) {
	if (context == nullptr || this->buffer.GetInterfacePtr() == nullptr) {
		return false;
	}
	this->cameraresource.view = rinfw::core::math::matrix44Transpose(
		rinfw::core::math::matrix44LookAtLH(this->pos, this->at, this->up)
	);
	this->cameraresource.projection = rinfw::core::math::matrix44Transpose(
		(this->perspective)?
		rinfw::core::math::matrix44PerspectiveFovLH(D3DXToRadian(this->fov), this->width / this->height, this->nearz, this->farz):
		rinfw::core::math::matrix44OrthoGraphicLH(this->width , this->height, this->nearz, this->farz)
	);
	context->UpdateSubresource(this->buffer.GetInterfacePtr(), 0, NULL, &this->cameraresource, 0, 0);
	return true;
}

#ifdef _Debug
void Camera::Debug(std::string str) {

	if (ImGui::Begin((str + " Camera Debug").c_str(), &this->showdebug)) {
	ImGui::PushID(this);
	ImGui::DragFloat3("pos", (float*)&this->pos, 0.005f);
	ImGui::DragFloat3("at", (float*)&this->at, 0.005f);
	ImGui::DragFloat3("up", (float*)&this->up, 0.005f);
	ImGui::Checkbox("Perspective", &this->perspective);
	ImGui::Checkbox("WireFrame", &this->wireframe);
	ImGui::DragFloat("Width", &this->width);
	ImGui::DragFloat("Height", &this->height);
	ImGui::DragFloat("Fov", &this->fov);
	ImGui::DragFloat("NearZ", &this->nearz);
	ImGui::DragFloat("FarZ", &this->farz);

	ImGui::PopID();
	ImGui::End();
	}

}
#endif
}
}
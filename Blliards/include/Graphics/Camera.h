#pragma once
#pragma warning(disable:4005)
#include <D3D11_1.h>
#include <./Macro.h>
#include <boost/shared_ptr.hpp>
#include <./Core/Math/Math.h>

namespace rinfw {
namespace graphics {
struct CameraResource{
	rinfw::core::math::Matrix44 view;
	rinfw::core::math::Matrix44 projection;
};
class Camera {
public:
	Camera();
	Camera(const Camera&);

	bool Create();
	bool Create(ID3D11Device *device);

	bool Update();
	bool Update(ID3D11DeviceContext *context);

	DEBUG(void showDebug() { this->showdebug = true; });
	DEBUG(void Debug(std::string = "Unknown"));
	CameraResource getCameraResource() { return this->cameraresource; }
	rinfw::core::math::Float3 &getPos() { return this->pos; }
	void setPos(rinfw::core::math::Float3 pos) { this->pos = pos; }
	rinfw::core::math::Float3 &getAt() { return this->at; }
	void setAt(rinfw::core::math::Float3 at) { this->at = at; }
	rinfw::core::math::Float3 &getUp() { return this->up; }
	void setUp(rinfw::core::math::Float3 up) { this->up = up; }
	float &getNearZ() { return this->nearz; }
	void setNearZ(float nearz) { this->nearz = nearz; }
	float &getFarZ() { return this->farz; }
	void setFarZ(float farz) { this->farz = farz; }
	float &getFov() { return this->fov; }
	void setFov(float fov) { this->fov = fov; }
	float &getWidth() { return this->width; }
	void setWidth(float width) { this->width = width; }
	float &getHeight() { return this->height; }
	void setHeight(float height) { this->height = height; }
	bool &getPerspective() { return this->perspective; }
	void setPerspective(bool perspective) { this->perspective = perspective; }
	bool &getWireframe() { return this->wireframe; }
	void setWireframe(bool wireframe) { this->wireframe = wireframe; }


	COMPTR(ID3D11Buffer) getBuffer() { return this->buffer; }
protected:
private:
	CameraResource cameraresource;
	rinfw::core::math::Float3 pos;
	rinfw::core::math::Float3 at;
	rinfw::core::math::Float3 up;
	float fov,width,height,nearz,farz;
	COMPTR(ID3D11Buffer) buffer;
	bool wireframe;
	bool perspective;
	DEBUG(bool showdebug);
};
}
}
#include <./GPL/TileRenderer/TileRenderer.h>
#include <./Device.h>
using namespace rinfw::graphics::gpl;

TileRenderer::TileRenderer() {

}

bool TileRenderer::Init(ID3D11Device *device,unsigned int tileWidth,unsigned int TileHeight) {
	if (device == nullptr)
		return false;
	WindowDevice &window = WindowDevice::getInstance();

	this->tileWidth = tileWidth;
	this->TileHeight = TileHeight;

	this->numCellX = (window.getWidth() + this->tileWidth - 1) / this->tileWidth;
	this->numCellY = (window.getHeight() + this->tileWidth - 1) / this->tileWidth;

	ID3D11ComputeShader *cs_buf = nullptr;
	ID3D10Blob *csblob = nullptr;

	D3DX11CompileFromFile(TEXT("./Shader/TileRenderer/CS.hlsl"), 0, 0, "main", "cs_5_0", 0, 0, 0, &csblob, 0, 0);
	device->CreateComputeShader(csblob->GetBufferPointer(), csblob->GetBufferSize(), nullptr, &cs_buf);

	this->cs.setGPLSShader(cs_buf);
	return true;
}

bool TileRenderer::Release() {
	this->cs.Release();
	SAFE_RELEASE(this->srv);
	return true;
}

bool TileRenderer::Dispatch(ID3D11DeviceContext* context) {
	if (this->srv == nullptr)
		return false;

	//Ý’è
	{

		//ID3D11Buffer* buffers[] = { hScene->GetConstScene() };
		//context->CSSetConstantBuffers( 0, ARRAYSIZE(buffers), buffers );
		//
		//ID3D11UnorderedAccessView* uavs[] = {
		//	hScene->GetLightIndicesUAV()
		//};
		//UINT uavInits[] = { 0 };
		//context->CSSetUnorderedAccessViews( 0, ARRAYSIZE(uavs), uavs, uavInits );
		//
		////context->CSSetShader( m_pCShader[0]->GetShader(), NULL, 0 );
		//if (!this->cs.setComputeShader()) {
		//	return false;
		//}
		//ID3D11ShaderResourceView*	views[] = {
		//	hScene->GetLightView(),
		//	hScene->GetFrameBuffer( Scene::FBUFFER_NORMAL_DEPTH )->GetRenderBuffer()->GetResourceView(),
		//};
		//context->CSSetShaderResources( 0, ARRAYSIZE(views), views );
	}

	//ŽÀs
	context->Dispatch(this->numCellX, this->numCellY, 1);

	//I—¹ˆ—
	{
		ID3D11UnorderedAccessView* uavs[] = { NULL };
		UINT uavInits[] = { 0 };
		context->CSSetUnorderedAccessViews( 0, ARRAYSIZE(uavs), uavs, uavInits );
		ID3D11ShaderResourceView*	views[] = { NULL, NULL };
		context->CSSetShaderResources( 0, ARRAYSIZE(views), views );
	}
	return false;
}
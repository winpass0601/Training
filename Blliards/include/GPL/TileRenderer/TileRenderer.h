#pragma once
#include <Macro.h>
#include <./GPLS/GPLS.h>

namespace rinfw {
namespace graphics {
namespace gpl {
//ForwardPlusBase
class TileRenderer {
public:
	TileRenderer();
	~TileRenderer() { this->Release(); }
	bool Init(ID3D11Device* device,unsigned int tileWidth,unsigned int TileHeight);
	bool Release();

	bool Dispatch(ID3D11DeviceContext* context);
private:

	COMPTR(ID3D11ShaderResourceView) srv;
	gpls::CS cs;

	unsigned int numCellX, numCellY;
	unsigned int tileWidth, TileHeight;

};
}
}
}
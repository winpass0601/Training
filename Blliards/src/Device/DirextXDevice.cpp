#include <./Device/DirectXDevice.h>

DX11Device::DX11Device() {

}
DX11Device::~DX11Device() {
}
unsigned long DX11Device::Init(HWND handle, unsigned long width, unsigned long height, bool windowmode) {
	unsigned long ret = 0;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // Direct3D ���\�[�X�Ƃ̑��݉^�p���� Direct2D �Ŏ������邽�߂ɕK�v
	DEBUG(creationFlags |= D3D11_CREATE_DEVICE_DEBUG);	   // �f�o�b�O ���C���[���T�|�[�g����f�o�C�X���쐬



	//------buffer�̐ݒ�--------//
	DXGI_SWAP_CHAIN_DESC desc = {}; // �o�b�N �o�b�t�@�[�̕\�����[�h��\�� 
	desc.BufferCount = 1; // �X���b�v �`�F�[���̃o�b�t�@�[����\���l(�t�����g �o�b�t�@�[���܂�)
	desc.BufferDesc.Width = width;   // �𑜓x�̕�
	desc.BufferDesc.Height = height; // �𑜓x�̍���
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 4 �����A32 �r�b�g�����Ȃ������t�H�[�}�b�g
	//���t���b�V�� ���[�g (Hz �P��) 
	desc.BufferDesc.RefreshRate.Numerator = 60;    // �L�����̍ő�l��\�������Ȃ������l�y����z
	desc.BufferDesc.RefreshRate.Denominator = 1;   // �L�����̍ŏ��l��\�������Ȃ������l�y���q�z
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �T�[�t�F�X�܂��̓��\�[�X���o�̓����_�[ �^�[�Q�b�g�Ƃ��Ďg�p
	// ���\�[�X�̃}���`�T���v�����O �p�����[�^�[(�A���`�G�C���A�V���O�Ȃ��̃f�t�H���g �T���v�� ���[�h)
	desc.SampleDesc.Count = 1;   // �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
	desc.SampleDesc.Quality = 0; // �C���[�W�̕i�����x��
	desc.Windowed = windowmode;  // �o���̓E�B���h�E ���[�h�̏ꍇ�� TRUE ����ȊO�̏ꍇ�� FALSE
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // IDXGISwapChain::ResizeTarget ���Ăяo���ă��[�h��؂�ւ�����悤�ɂ��邽�߂ɐݒ�
	desc.OutputWindow = handle; // �o�̓E�B���h�E�ւ� HWND �n���h��
	// ���ɉ����Đ؂�ւ���\���̂𑢂�
	D3D_FEATURE_LEVEL features[] = { // ver���Ƃɐݒ�i�D��x�͏ォ��jDirect3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	// �h���C�o�[�̎�ނ̃I�v�V���� GPU���g����CPU���g����
	D3D_DRIVER_TYPE devicetype[] = {
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // �n�[�h�E�F�A �A�u�X�g���N�V���� ���C���[ (HAL) 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE, // �\�t�g�E�F�A �h���C�o�[
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_REFERENCE // ���t�@�����X ���X�^���C�U�[(�@�\�̃e�X�g�A�f���A�f�o�b�O�p�j
	};
	// DX11�K�v�ϐ��̏������Q
	ID3D11Device *device = nullptr;         // �����_�����O����у��\�[�X�̍쐬�Ɏg�p
	ID3D11DeviceContext *context = nullptr; // �����_�����O �R�}���h�𐶐�����f�o�C�X �R���e�L�X�g
	IDXGISwapChain *swapchain = nullptr;    // �����_�����O���ꂽ�f�[�^���o�͂ɕ\������O�Ɋi�[���邽�߂� 1 �܂��͕����̃T�[�t�F�X
	for (auto dt : devicetype) {
		if (SUCCEEDED(
			D3D11CreateDeviceAndSwapChain(  // �f�B�X�v���C �A�_�v�^�[��\���f�o�C�X�ƃ����_�����O�Ɏg�p����X���b�v �`�F�[�����쐬
			nullptr,						// �f�o�C�X�̍쐬���Ɏg�p����r�f�I �A�_�v�^�[�ւ̃|�C���^�[(NULL�ŋK��̃A�_�v�^�[)
			dt,								// �쐬����f�o�C�X�̎��
			nullptr,						// �\�t�g�E�F�A ���X�^���C�U�[���������� DLL �̃n���h��
			creationFlags,					// �L���ɂ��郉���^�C�� ���C���[(�l�̓r�b�g�P�ʂ� OR ���Z�Ŏw��)
			features,						// �쐬�����݂�@�\���x���̏������w�肷�� D3D_FEATURE_LEVEL �̔z��ւ̃|�C���^�[
			ARRAYSIZE(features),			// pFeatureLevels �̗v�f��
			D3D11_SDK_VERSION,				// SDK �̃o�[�W���� D3D11_SDK_VERSION ���w��
			&desc,							// �X���b�v �`�F�[���̏������p�����[�^�[���i�[����|�C���^
			&swapchain,						// �����_�����O�Ɏg�p����X���b�v �`�F�[���̃|�C���^�A�h���X��Ԃ�
			&device,						// �쐬���ꂽ�f�o�C�X�ւ̃|�C���^�̃A�h���X��Ԃ�
			&this->feature,					// �@�\���x���̔z��ɂ���ŏ��̗v�f��\���|�C���^��Ԃ�
			&context						// �f�o�C�X �R���e�L�X�g�̃|�C���^�̃A�h���X��Ԃ�
			)
			)) { // ��L���������ꂽ�e�@�\����
			this->device.Attach(device);
			this->context.Attach(context);
			this->swapchain.Attach(swapchain);
			this->drivertype = dt;
			break;
		}
	}
	// �f�o�C�XNULL�`�F�b�N
	if (!device) {
		ret = -1;
		return ret;
	}

	//�f�o�b�O�p�f�o�C�X�̐���
	DEBUG(this->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&this->debug)));
	DEBUG(this->context->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)&this->anotation));

	//BuckBuffer�̐���
	D3D11_TEXTURE2D_DESC descDepth;            // 2D �e�N�X�`���[
	ID3D11Texture2D* pBackBuffer = NULL;	   // �\�������ꂽ�������[�ł���e�N�Z�� �f�[�^���Ǘ�
	ID3D11RenderTargetView*		rendertargetview; // �����_�[�^�[�Q�b�g�r���[(�����_�����O���ɃA�N�Z�X�\�ȃ����_�[ �^�[�Q�b�g�̃T�u���\�[�X������)
	ID3D11DepthStencilView*		dsv; // �X�e���V���r���[(�[�x�X�e���V�� �e�X�g���Ƀe�N�X�`���[ ���\�[�X�ɃA�N�Z�X)
	// comptr�݂��Ȃ���)GetBuffer(�Q�ƃJ�E���^1������
	ret = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(ret)) {
		return ret;
	}
	// BackBuffer�̎擾���@�iCreateRenderTargetView)
	ret = this->device->CreateRenderTargetView(pBackBuffer, NULL, &rendertargetview);
	if (FAILED(ret)) {
		return ret;
	}
	// ret��Create���i�[�����̂ŎQ�ƃJ�E���^��0�ɂ���i�Q�ɂȂ邩��j
	pBackBuffer->Release();
	pBackBuffer = nullptr;

	//�X�e���V���r���[�̐���
	ZeroMemory(&descDepth, sizeof(descDepth));	// �\���̒��g���O������
	descDepth.Width = width; descDepth.Height = height; // �e�N�X�`���[�̕��ƍ���
	descDepth.MipLevels = 1; // �e�N�X�`���[���̃~�b�v�}�b�v ���x���̍ő吔(1:�}���`�T���v�����O���ꂽ�e�N�X�`���[ 0:�T�u�e�N�X�`���[�̑S�Z�b�g�𐶐�����)
	descDepth.ArraySize = 1; // �e�N�X�`���[�z����̃e�N�X�`���[�̐�
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// �[�x�`�����l���� 24 �r�b�g�A�X�e���V�� �`�����l���� 8 �r�b�g���g�p���� 32 �r�b�g Z �o�b�t�@�[ �t�H�[�}�b�g
	descDepth.SampleDesc.Count = 1; descDepth.SampleDesc.Quality = 0; // �s�N�Z���P�ʂ̃}���`�T���v�����O�̐��ƃC���[�W�̕i�����x��
	descDepth.Usage = D3D11_USAGE_DEFAULT; // �e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �e�N�X�`���[���o�͌����X�e�[�W�̐[�x�X�e���V�� �^�[�Q�b�g�Ƃ��ăo�C���h
	ret = this->device->CreateTexture2D(&descDepth, NULL, &pBackBuffer); // 2D �e�N�X�`���[�̔z��

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView; // �[�x�X�e���V�� �r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X���w��
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView)); // �\���̒��g���O������
	descDepthStencilView.Format = descDepth.Format; // ���\�[�X �f�[�^�̃t�H�[�}�b�g
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // (2D �e�N�X�`���[�Ƃ��ăA�N�Z�X)�[�x�X�e���V�� ���\�[�X�ւ̃A�N�Z�X���@���w��
	descDepthStencilView.Texture2D.MipSlice = 0; // (�ŏ��Ɏg�p����~�b�v�}�b�v ���x���̃C���f�b�N�X)2D �e�N�X�`���[�̃T�u���\�[�X���w��

	// ���\�[�X �f�[�^�ւ̃A�N�Z�X�p�ɐ[�x�X�e���V�� �r���[���쐬
	ret = this->device->CreateDepthStencilView(pBackBuffer, &descDepthStencilView, &dsv);
	if (pBackBuffer) { // �g�p��̉��
		pBackBuffer->Release();
		pBackBuffer = nullptr;
	}
	// // ��L���������ꂽ�e�@�\����
	this->rendertargetview.Attach(rendertargetview);
	this->dsv.Attach(dsv);
	// �r���[�|�[�g
	D3D11_VIEWPORT vp;				// �r���[�|�[�g�̐��@���`
	vp.Width = (float)width;		// �r���[�|�[�g�̕�
	vp.Height = (float)height;		// �r���[�|�[�g�̍���
	vp.MinDepth = 0.0f;				// �r���[�|�[�g�̍ŏ��[�x
	vp.MaxDepth = 1.0f;				// �r���[�|�[�g�̍ő�[�x
	vp.TopLeftX = 0;				// �r���[�|�[�g�̍����� X �ʒu
	vp.TopLeftY = 0;				// �r���[�|�[�g�̏㕔�� Y �ʒu
	this->context->RSSetViewports(1, &vp); // �p�C�v���C���̃��X�^���C�U�[ �X�e�[�W�Ƀr���[�|�[�g�̔z����o�C���h

	//�����_�[�^�[�Q�b�g�̐ݒ�
	ID3D11RenderTargetView *rtv[] = { // �����_�����O���ɃA�N�Z�X�\�ȃ����_�[ �^�[�Q�b�g�̃T�u���\�[�X������
		this->getRTV(),
	};
	this->context->OMSetRenderTargets(1, rtv, nullptr); // 1 �ȏ�̃����_�[ �^�[�Q�b�g���A�g�~�b�N�Ƀo�C���h���A�o�͌����X�e�[�W�ɐ[�x�X�e���V�� �o�b�t�@�[���o�C���h
	return ret;
}


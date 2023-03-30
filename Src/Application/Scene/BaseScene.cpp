#include "../Headers.h"


BaseScene::~BaseScene()
{
}

void BaseScene::Frame()
{
	//�X�V����
	Update();

	// �o�b�N�o�b�t�@�N���A
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBA��0-1�͈̔͂�
	// Z�o�b�t�@�N���A
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//�J�����E���E�ݒ�
	SetCamera();

	//�`�揈��
	Draw();
}

void BaseScene::SetCamera()
{
}

void BaseScene::Update()
{
}

void BaseScene::Draw()
{
}

void BaseScene::ImGuiUpdate()
{
}

#include "../Headers.h"


BaseScene::~BaseScene()
{
}

void BaseScene::Frame()
{
	//更新処理
	Update();

	// バックバッファクリア
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.0f, 0.0f, 1.0f, 1));//RGBAを0-1の範囲で
	// Zバッファクリア
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//カメラ・視界設定
	SetCamera();

	//描画処理
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

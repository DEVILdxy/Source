#include "../Headers.h"

TitleScene::TitleScene()
{
	m_tex.Load("Data/Texture/Title.png");
	m_enterTex.Load("Data/Texture/PressSpace.png");


	m_soundBgm = std::make_shared<KdSoundEffect>();
	m_soundBgm->Load("Data/Sound/BGM.wav");
	m_soundBgmInst = m_soundBgm->CreateInstance(false);
	m_soundBgmInst->SetVolume(0.1f);
	m_soundBgmInst->Play(true);
}

TitleScene::~TitleScene()
{
	m_tex.Release();
	m_enterTex.Release();
	m_soundBgmInst->Pause();
}

void TitleScene::Draw()
{
	SHADER.m_spriteShader.Begin();

	
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle(0, 0, 1280, 720), 1.0f);


	Math::Matrix mat = DirectX::XMMatrixTranslation(150, -200, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(&m_enterTex, Math::Rectangle(0, 0, 700, 150), 1.0f);



	SHADER.m_spriteShader.End();
}

void TitleScene::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (SYSTEM.GetEscapeKeyFlg() == false)
		{
			SYSTEM.SetEscapeKeyFlg(true);
			APP.m_window.Release();
		}
	}
	else
	{
		SYSTEM.SetEscapeKeyFlg(false);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (SYSTEM.GetSpaceKeyFlg() == false)	//íºëOÇ‹Ç≈ÉLÅ[ÇâüÇµÇ¢Ç»Ç©Ç¡ÇΩ 
		{
			SYSTEM.SetSpaceKeyFlg(true);
			SCENEMANAGER.ChangeScene(new GameScene());		
		}
	}
	else
	{
		SYSTEM.SetSpaceKeyFlg(false);
	}

}
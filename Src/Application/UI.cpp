#include"Headers.h"

UI::UI(GameScene* _gameScene)
{
	m_aimTex = TEXTUREMANAGER.GetTexture("Data/Texture/Aim1.png");
	m_aimMat = Math::Matrix::Identity;
	m_aimAnime = 0;	

	m_waterTex = TEXTUREMANAGER.GetTexture("Data/Texture/Water_UI.png");
	m_waterMat = Math::Matrix::Identity;
	m_waterAnime = 0;

	m_frameTex = TEXTUREMANAGER.GetTexture("Data/Texture/Water_Frame.png");
	m_frameMat = Math::Matrix::Identity;

	m_arrowTex = TEXTUREMANAGER.GetTexture("Data/Texture/arrow.png");
	m_arrowMat = Math::Matrix::Identity;
	
	m_gameScene = _gameScene;
	m_inLimit = false;
	m_openChest = false;
	
	m_spriteFont = new DirectX::SpriteFont(D3D.GetDev(), L"Data/SpriteFont.dat");
}

UI::~UI()
{

}

void UI::Draw()
{
	const Player& player = m_gameScene->GetPlayer();	
	float wallDis = player.GetGroundDis();
	Math::Vector3 playerPos = player.GetPos();
	float boardDis = player.GetAimRayDis();
	constexpr float limitDis = 34.0f;

	if (m_gameScene->GetPlayer().GetAimShow() == true)
	{
		if (boardDis <= limitDis)
		{
			Draw_Aim(AimState::AnimeAim);
			m_inLimit = true;
		}
		else
		{
			Draw_Aim(AimState::NormalAim);
			m_inLimit = false;
		}
	}

	const Chest& chest = m_gameScene->GetChest();
	Math::Vector3 playerToChest = chest.GetPos() - player.GetPos();

	if (m_openChest == false && playerToChest.Length()<=3)
	{
		SHADER.m_spriteShader.Begin();

		std::string str = "Eキーを押す";
		SHADER.m_spriteShader.DrawString(m_spriteFont, 0, 20, &str[0], Math::Color(1, 1, 1, 1));

		SHADER.m_spriteShader.End();

		if (GetAsyncKeyState('E') & 0x8000)
		{
			m_openChest = true;
		}
	}

	if (m_openChest == true)
	{
		Draw_Arrow();
		Draw_Water();
		Draw_Frame();
	}
	
	
}

void UI::Update_Water()
{
	float height = m_gameScene->GetWater().GeMat().Translation().y+5;
	Math::Matrix transMat = DirectX::XMMatrixTranslation(-570,-125+height,0);
	m_waterMat = transMat;
	m_waterAnime += 0.5f;
	if (m_waterAnime >= 300)
	{
		m_waterAnime = 0;
	}
}

void UI::Draw_Water()
{
	float height = m_gameScene->GetWater().GeMat().Translation().y + 5;
	height *= 2;
	SHADER.m_spriteShader.Begin();

	SHADER.m_spriteShader.SetMatrix(m_waterMat);
	SHADER.m_spriteShader.DrawTex(m_waterTex, Math::Rectangle(m_waterAnime, 0, 100, 50+height), 1.0f);

	SHADER.m_spriteShader.End();
}

void UI::Draw_Frame()
{
	Math::Matrix transMat = DirectX::XMMatrixTranslation(-570, 0, 0);
	m_frameMat = transMat;
	SHADER.m_spriteShader.Begin();

	SHADER.m_spriteShader.SetMatrix(m_frameMat);
	SHADER.m_spriteShader.DrawTex(m_frameTex, Math::Rectangle(0, 0, 100, 300), 1.0f);

	SHADER.m_spriteShader.End();
}

void UI::Draw_Aim(AimState _aimState)
{
	SHADER.m_spriteShader.Begin();

	if (_aimState == AimState::NormalAim)
	{
		SHADER.m_spriteShader.SetMatrix(m_aimMat);
		SHADER.m_spriteShader.DrawTex(m_aimTex, Math::Rectangle(0, 0, 64, 64), 1.0f);
	}
	if (_aimState == AimState::AnimeAim)
	{
		SHADER.m_spriteShader.SetMatrix(m_aimMat);
		SHADER.m_spriteShader.DrawTex(m_aimTex, Math::Rectangle((int)m_aimAnime * 64, 0, 64, 64), 1.0f);
	}
	

	SHADER.m_spriteShader.End();
}

void UI::Draw_Arrow()
{
	float ratio = 300.0f / 380.0f; // 比率変換
	float height = m_gameScene->GetPlayer().GetPos().y - 150;// 高さ正規化
	height *= ratio;
	Math::Matrix transMat = DirectX::XMMatrixTranslation(-490, height, 0);
	m_arrowMat = transMat;
	SHADER.m_spriteShader.Begin();

	SHADER.m_spriteShader.SetMatrix(m_arrowMat);
	SHADER.m_spriteShader.DrawTex(m_arrowTex, Math::Rectangle(0, 0, 64, 64), 1.0f);

	SHADER.m_spriteShader.End();
}

void UI::Update()
{
	m_aimAnime += 0.1f;
	if (m_aimAnime >= 4)
	{
		m_aimAnime = 0;
	}

	Update_Water();

}

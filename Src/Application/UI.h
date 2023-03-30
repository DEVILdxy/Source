#pragma once
#include"Headers.h"

class GameScene;

class UI
{
public:
	UI(GameScene* _gameScene);
	~UI();

	void Update();

	void Draw();
	
	const bool GetLimit()const { return m_inLimit; }
	const bool GetOpenChest()const { return m_openChest; }
	

private:

	enum class AimState
	{
		NormalAim,
		AnimeAim
	};

	void Update_Water();

	void Draw_Water();
	void Draw_Frame();
	void Draw_Aim(AimState _aimState); // 照準マークの描画
	void Draw_Arrow();// キャラクターの位置を提示用

	

	// 照準用
	KdTexture* m_aimTex;
	Math::Matrix m_aimMat;	
	float m_aimAnime;

	// 水面UI
	KdTexture* m_waterTex;
	Math::Matrix m_waterMat;
	float m_waterAnime;

	// 框
	KdTexture* m_frameTex;
	Math::Matrix m_frameMat;

	// 高さ提示用
	KdTexture* m_arrowTex;
	Math::Matrix m_arrowMat;


	bool m_inLimit; // 飛べる範囲内

	bool m_openChest;

	GameScene* m_gameScene;
	DirectX::SpriteFont* m_spriteFont;
};

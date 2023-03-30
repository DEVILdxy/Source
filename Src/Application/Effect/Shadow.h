#pragma once
class GameScene;
class Shadow :public Effect
{
public:
	Shadow(GameScene* _gameScene,float _size);
	~Shadow();
	void Update(Math::Vector3 _objPos);

private:

	float DecideHeight(Math::Vector3 _objPos); // ‰e‚ÌY²À•W‚ğ‹‚ß‚é

	Vertex m_v[4];

	float m_shadowPosY;

	float m_minDist; // ƒŒƒC”»’è‚É’n–Ê‚Æ‚ÌÅ¬‹——£

	GameScene* m_gameScene;
};
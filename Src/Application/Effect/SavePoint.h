#pragma once
class GameScene;
class SavePoint :public Effect
{
public:
	SavePoint(GameScene* _gameScene,Math::Vector3& _pos);
	~SavePoint();
	void Update()override;
	void Draw()override;

private:

	KdTexture* m_texSub; // 副テクスチャ
	Math::Matrix m_matSub;

	GameScene* m_gameScene;
};
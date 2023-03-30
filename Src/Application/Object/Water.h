#pragma once
class GameScene;
class Water:public Object
{
public:
	Water(GameScene* _gameScene);
	~Water();

	void Update()override;
	void Draw()override;

	const Math::Matrix& GeMat()const { return m_mat; }

private:

	GameScene* m_gameScene;

	KdTexture m_normalTex;//–@ü
};
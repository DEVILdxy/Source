#pragma once
class GameScene;
class Shadow :public Effect
{
public:
	Shadow(GameScene* _gameScene,float _size);
	~Shadow();
	void Update(Math::Vector3 _objPos);

private:

	float DecideHeight(Math::Vector3 _objPos); // �e��Y�����W�����߂�

	Vertex m_v[4];

	float m_shadowPosY;

	float m_minDist; // ���C����ɒn�ʂƂ̍ŏ�����

	GameScene* m_gameScene;
};
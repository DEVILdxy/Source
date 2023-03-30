#pragma once

class GameScene;
class Enemy;
class Appear :public BaseState
{
public:

	Appear(GameScene* _gameScene, Enemy* _enemy, Math::Matrix& _mat);

	

	BaseState* Action(GameScene* _gameScene, Math::Matrix& _mat)override;


private:


	GameScene* m_gameScene;

	float m_appearHeight;
};
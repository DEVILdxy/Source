#pragma once

class GameScene;
class Track :public BaseState
{
public:
	Track();

	BaseState* Action(GameScene* _gameScene, Math::Matrix& _mat)override;

private:

	GameScene* m_gameScene;

	int m_attackPrepareTime;
};
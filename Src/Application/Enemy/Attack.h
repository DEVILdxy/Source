#pragma once
class GameScene;
class Attack :public BaseState
{
public:
	Attack();

	BaseState* Action(GameScene* _gameScene, Math::Matrix& _mat)override;


private:

	void SetTargetPos(Math::Vector3 _pos);

	void pushPlayer(GameScene* _gameScene,Math::Vector3 _pos);

	float m_movePow;

	float m_pushPow;

	
	int m_returnTrackTime;

};
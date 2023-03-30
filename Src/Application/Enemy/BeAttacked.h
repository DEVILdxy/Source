#pragma once
class GameScene;
class BeAttacked :public BaseState
{
public:
	BeAttacked();

	BaseState* Action(GameScene* _gameScene, Math::Matrix& _mat)override;

private:
	void AddEnemyPos(Math::Vector3 _addPos, Math::Matrix& _mat);

	float m_hitDis;

	float m_pushPow=0;

	int m_returnTrackTime;

	GameScene* m_gameScene;

	bool m_hitFlg;
};
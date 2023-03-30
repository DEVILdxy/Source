#pragma once


class GameScene;
class Arm_Right :public Arm
{
public:
	Arm_Right(GameScene* _gameScene);
	void Update()override;
	const Math::Matrix GetMat()const { return m_mat; }


private:
	GameScene* m_gameScene;
};
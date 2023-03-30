#pragma once

class GameScene;
class Arm_Left :public Arm 
{
public:
	Arm_Left(GameScene* _gameScene);
	void Update()override;

	const Math::Matrix GetMat()const { return m_mat; }
	//キーフレームアニメーション関数

private:
	//キーフレームアニメ用
	Math::Matrix m_startMat;//アニメ開始行列
	Math::Matrix m_endMat;  //アニメ終了行列
	float m_animeFrame; //進捗具合（０～１）

	GameScene* m_gameScene;
};
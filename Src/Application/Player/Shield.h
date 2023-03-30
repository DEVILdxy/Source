#pragma once

class GameScene;

class Shield
{
public:
	Shield(GameScene* _gameScene);
	~Shield();
	void Update();
	void Draw();

private:
	KdModel* m_model;
	Math::Matrix m_mat;

	//キーフレームアニメ用
	Math::Matrix m_startMat;//アニメ開始行列
	Math::Matrix m_endMat;  //アニメ終了行列

	float m_animeFrame; //進捗具合（０～１）


	GameScene* m_gameScene;


};

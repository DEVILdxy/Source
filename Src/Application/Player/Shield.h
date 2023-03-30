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

	//�L�[�t���[���A�j���p
	Math::Matrix m_startMat;//�A�j���J�n�s��
	Math::Matrix m_endMat;  //�A�j���I���s��

	float m_animeFrame; //�i����i�O�`�P�j


	GameScene* m_gameScene;


};

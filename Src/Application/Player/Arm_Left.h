#pragma once

class GameScene;
class Arm_Left :public Arm 
{
public:
	Arm_Left(GameScene* _gameScene);
	void Update()override;

	const Math::Matrix GetMat()const { return m_mat; }
	//�L�[�t���[���A�j���[�V�����֐�

private:
	//�L�[�t���[���A�j���p
	Math::Matrix m_startMat;//�A�j���J�n�s��
	Math::Matrix m_endMat;  //�A�j���I���s��
	float m_animeFrame; //�i����i�O�`�P�j

	GameScene* m_gameScene;
};
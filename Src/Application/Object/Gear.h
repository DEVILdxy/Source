#pragma once

class GameScene;
class Gear
{
public:
	Gear(GameScene* _gameScene);
	~Gear();
	virtual void Update();
	void Draw();

	void SetOnGearFlg(bool _flg);
	void MoveTogether();// �L�����N�^�[�ƍ��̃v���W�F�N�g�ꏏ�ɉ�]����
	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec,  float& _dis) const;
private:
	GameScene* m_gameScene;

	KdModel* m_model;
	Math::Matrix m_mat;
	float m_ang;

	bool m_isOnFlg; // �L�����N�^�[�����t���O
	Math::Vector3 m_saveVec;
	float m_saveAng;
};
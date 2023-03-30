#pragma once

class GameScene;
class Platform;
class Wheel:public Object
{
public:
	Wheel(GameScene *_gameScene, float _ang,const Math::Matrix& _transMat,const Math::Matrix& _rotMat);
	~Wheel();
	void Update()override;
	void Draw()override;

	void MoveTogether();// �L�����N�^�[�ƍ��̃v���W�F�N�g�ꏏ�ɉ�]����

	void SetOnWheelFlg(bool _flg) { m_isOnFlg = _flg; }

	const Math::Matrix GetMat()const { return m_mat; }

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;

	

private:
	float m_ang;
	float m_plusAng;
	GameScene* m_gameScene;
	Math::Matrix m_transMat;
	Math::Matrix m_rotMat;
	std::shared_ptr<Platform>m_platform[5];

	bool m_isOnFlg; // �L�����N�^�[�����t���O

	Math::Vector3 m_saveVec;
	float m_saveAng;
	
};
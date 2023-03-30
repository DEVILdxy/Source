#pragma once
class GameScene;
class Ball :public Object
{
public:
	Ball(GameScene* _gameScene);

	void Update()override;

	

	void SetPos(Math::Vector3 _pos);
	const Math::Vector3& GetPos()const { return m_mat.Translation(); }

	const Math::Vector3& GetOldPos()const { return m_oldPos; }
	const Math::Vector3& GetNowPos()const { return m_nowPos; }
	const Math::Vector3& GetMoveVec()const { return m_moveVec; }


private:
	
	void Move_Slope();// �Ζʏ�̈ړ�����
	void Roll(Math::Vector3& _dir); // ��]�@�ړ�������ball�̒��a�̒l��n��
	void Move();// ���˂��܂߂Ĉړ�����
	void Move(const Math::Vector3& _vec);// ���[���h���W��ς���

	Math::Vector3 m_oldPos;
	Math::Vector3 m_nowPos;

	Math::Vector3 m_moveVec;// �ړ��x�N�g��	
	float m_length;// �ړ�����

	GameScene* m_gameScene;
};
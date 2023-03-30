#include"../Headers.h"

Ball::Ball(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/Ball.gltf");
	m_mat = DirectX::XMMatrixTranslation(3, 330, -101); // 0, 400, 0
	m_gameScene = _gameScene;
	m_moveVec = { 0.3f,0,0 };

	m_nowPos = {};
	m_oldPos = m_mat.Translation();
}

void Ball::Update()
{
	// �d��
	float gravity = -0.5f;
	m_mat._42 += gravity;

	m_oldPos = m_mat.Translation();
	Move();

	Move_Slope();


	Math::Vector3 moveVec = m_nowPos - m_oldPos;
	Roll(moveVec);


}

void Ball::Move_Slope()
{
	const Map& map = m_gameScene->GetMap();
	Math::Vector3 mapNormal;
	Math::Vector3 downVec = { 0,-1,0 };
	float hitDis;
	bool hit = map.CheckBoard(m_mat.Translation(), downVec, hitDis, mapNormal);
	if (hit) // ���ɔ�΂����C��map����������
	{
		Math::Vector3 dot;
		dot = DirectX::XMVector3Dot(mapNormal, -downVec);
		constexpr float awayDistance = 3.0f;

		float limit;
		limit = awayDistance / dot.x;
		//��������֎~�G���A�̕�/�R�T�C���Ɓ������~�܂�ׂ�����
		if (limit < 0) //�ǂ̗�������N�������ꍇ
		{
			limit *= -1;//�������v���X�̒l�ɕϊ�
		}

		//Math::Vector3 moveVec;
		if (hitDis < limit)  //�ǂ܂ł̋������u�����~�܂�ׂ������v���Z��
		{
			m_mat._41 += (mapNormal * ((limit - hitDis) * dot.x)).x;
			m_mat._42 += (mapNormal * ((limit - hitDis) * dot.x)).y;
			m_mat._43 += (mapNormal * ((limit - hitDis) * dot.x)).z;
			//�@�������ɓK�؂ȋ����������Ԃ�
			m_nowPos = m_mat.Translation();
		}
	}
}

void Ball::Roll(Math::Vector3& _dir)
{
	constexpr float ballDiameter = 6.0f;// �{�[���̒��a�̒���
	float ang = (_dir.Length() / (ballDiameter * M_PI) * 360); // ��]�p�x���i�s�����ƃ{�[���̎����̔䗦��360���̂����錋��

	Math::Vector3 wRotAxis = DirectX::XMVector3Cross(_dir, Math::Vector3(0, 1, 0));// ���[���h�̉�]��	

	if (wRotAxis != Math::Vector3::Zero)
	{
		Math::Matrix invMat;
		invMat = DirectX::XMMatrixInverse(nullptr, m_mat); // �t�s��
		Math::Vector3 lRotAxis = DirectX::XMVector3TransformNormal(wRotAxis, invMat); // ���[�J���̉�]��

		Math::Matrix rotMat = DirectX::XMMatrixRotationAxis(lRotAxis, DirectX::XMConvertToRadians(-ang));

		m_mat = rotMat * m_mat;
	}
}

void Ball::Move()
{
	if (m_moveVec != Math::Vector3::Zero)
	{
		Math::Vector3 rayVec = m_moveVec;
		rayVec.Normalize();
		const Wall_Reflect& WR = m_gameScene->GetWallReflect();
		Math::Vector3 mapNormal;
		float hitDis;
		bool hit = WR.CheckBoard(m_mat.Translation(), rayVec, hitDis, mapNormal);
		if (hit) // �i�s�����ɏ�Q��������ꍇ
		{
			float moveLen;
			moveLen = m_moveVec.Length();

			Math::Vector3 dot;
			dot = DirectX::XMVector3Dot(mapNormal, -rayVec);

			float limit;
			limit = 3.0f / dot.x;// �e�̔��a���߂荞�܂Ȃ����߂̌��E����
			if (limit < 0)
			{
				limit *= -1;
			}

			if ((hitDis - limit) <= moveLen)// ��Q���܂ł̋������\��ړ��ʂ��Z��
			{
				Math::Vector3 vec;
				vec = rayVec * (hitDis - limit);

				Move(vec);

				m_moveVec += mapNormal * (moveLen * dot.x) * 2;
			
				// �VmoveVec = ��moveVec + �Փ˖ʂ̖@�� x ���p�O�p�`�̒�ӂ̒��� x 2
			}
			else
			{
				Move(m_moveVec);// �ʏ�ړ�
			}
		}
		else
		{
			Move(m_moveVec);// �ʏ�ړ�
		}
	}

}

void Ball::Move(const Math::Vector3& _vec)
{
	m_mat._41 += _vec.x;
	//m_mat._42 += _vec.y;
	m_mat._43 += _vec.z;
}

void Ball::SetPos(Math::Vector3 _pos)
{
	m_mat._41 = _pos.x;
	m_mat._42 = _pos.y;
	m_mat._43 = _pos.z;
}



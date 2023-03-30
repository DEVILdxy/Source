#include"../Headers.h"

Enemy::Enemy(GameScene* _gameScene, Math::Vector3 _pos)
{
	m_gameScene = _gameScene;
	m_modelDraw = MODELMANAGER.GetModel("Data/Models/Enemy/Enemy.gltf");
	m_modelUpdate = MODELMANAGER.GetModel("Data/Models/Enemy/Enemy_Update.gltf");
	m_aliveFlg = true;
	const Map& map = m_gameScene->GetMap();
	float dist = -1;
	map.CheckBoard(_pos, Math::Vector3{ 0,-1,0 }, dist);

	const float modelHeight = 1.0f;

	float y = _pos.y - dist + modelHeight;
	m_mat = DirectX::XMMatrixTranslation(_pos.x, y, _pos.z);


	m_currentState = new Appear(m_gameScene, this, m_mat);

	m_aimCone = new AimCone(this);

}

Enemy::~Enemy()
{

	if (m_currentState != nullptr)
	{
		delete m_currentState;
	}
}

void Enemy::Update()
{
	float gravity = -0.3f;
	//�d��
	m_mat._42 += gravity;
	if (m_mat._42 <= m_nowGround)
	{
		m_mat._42 = m_nowGround;
		gravity = 0;
	}


	Math::Vector3 vecFront, vecBack, vecLeft, vecRight, vecUp;
	vecFront = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), m_mat);
	vecBack = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -1), m_mat);
	vecLeft = DirectX::XMVector3TransformNormal(Math::Vector3(-1, 0, 0), m_mat);
	vecRight = DirectX::XMVector3TransformNormal(Math::Vector3(1, 0, 0), m_mat);

	CheckHit(vecFront);
	CheckHit(vecBack);
	CheckHit(vecLeft);
	CheckHit(vecRight);

	Check_Ground();

	m_aimCone->Update();

	if (m_currentState != nullptr)
	{
		BaseState* nextState;
		nextState = m_currentState->Action(m_gameScene, m_mat);

		//�ʂ̃X�e�[�g�ւ̈ڍs��]���Ԃ��Ă�����
		if (nextState != nullptr)
		{
			//���݃X�e�[�g���폜
			delete m_currentState;

			//�ڍs������݂̃X�e�[�g�Ƃ���
			m_currentState = nextState;
		}
	}




}


void Enemy::Draw()
{
	SHADER.m_standardShader.SetAppearEnable(true);

	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_modelDraw);

	SHADER.m_standardShader.SetAppearEnable(false);

	if (m_gameScene->GetPlayer().GetEnemySpotFlg() == true && m_gameScene->GetUI().GetLimit() == true)
	{
		m_aimCone->Draw();
	}


}


bool Enemy::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_modelUpdate, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}

void Enemy::Check_Ground()
{
	const Map& map = m_gameScene->GetMap();
	float groundHitDis;
	if (map.CheckBoard(m_mat.Translation(), Math::Vector3(0, -1, 0), groundHitDis))
	{
		m_nowGround = m_mat._42 + (1.0f - groundHitDis);
	}
}

void Enemy::CheckHit(Math::Vector3 _vec)
{
	const Map& map = m_gameScene->GetMap();
	Math::Vector3 mapNormal;
	bool mapHit;//���C��������
	mapHit = map.CheckBoard(m_mat.Translation(), _vec, m_hitDis, mapNormal);
	PushBack(mapHit,mapNormal, _vec);

	const Obstacle& obstacle = m_gameScene->GetObstacle();
	Math::Vector3 obsNormal;
	bool obsHit;
	obsHit = obstacle.CheckBoard(m_mat.Translation(), _vec, m_hitDis, obsNormal);
	PushBack(obsHit,obsNormal, _vec);
}

void Enemy::PushBack(bool _hit,Math::Vector3& _normal, Math::Vector3 _vec)
{
	if (_hit) // �i�H��ɕǂ�����
	{

		Math::Vector3 dot;
		dot = DirectX::XMVector3Dot(_normal, -_vec);

		// �}�W�b�N�i���o�[�@/���l /���l
		float limit;
		limit = 3.0f / dot.x;
		// ��������֎~�G���A�̕�/�R�T�C���Ɓ������~�܂�ׂ�����
		if (limit < 0) //�ǂ̗�������N�������ꍇ
		{
			limit *= -1;//�������v���X�̒l�ɕϊ�
		}
		if (m_hitDis < limit)  //�ǂ܂ł̋������u�����~�܂�ׂ������v���Z��
		{
			m_mat._41 += (_normal * ((limit - m_hitDis) * dot.x)).x;
			m_mat._42 += (_normal * ((limit - m_hitDis) * dot.x)).y;
			m_mat._43 += (_normal * ((limit - m_hitDis) * dot.x)).z;
			//�@�������ɓK�؂ȋ����������Ԃ�
		}
	}

}

void Enemy::AddPos(Math::Vector3 _addPos)
{
	m_mat._41 += _addPos.x;
	m_mat._42 += _addPos.y;
	m_mat._43 += _addPos.z;
}


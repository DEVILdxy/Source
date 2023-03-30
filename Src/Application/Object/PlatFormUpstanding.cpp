#include"../Headers.h"

PlatFormUpstanding::PlatFormUpstanding(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/PlatFormMove.gltf");
	m_gameScene = _gameScene;
	m_mat = Math::Matrix::Identity;
	m_ang = 0;
	m_speed = 0;
	m_dirFlg = true;
	m_isOnFlg = false;

}

PlatFormUpstanding::~PlatFormUpstanding()
{

}

void PlatFormUpstanding::Update()
{
	Math::Matrix transMat = DirectX::XMMatrixTranslation(171, 166, 1.2f);


	Move();


	Math::Matrix lRotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90)); // Œü‚±‚¤‚ð90‹‚É‰ñ“]
	Math::Matrix wRotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));// ƒ[ƒ‹ƒh‰ñ“]

	m_mat = lRotMatY * transMat * wRotMatY;

}

void PlatFormUpstanding::MoveTogether()
{
	if (m_isOnFlg == false)
	{
		m_savePos = m_mat.Translation() - m_gameScene->GetPlayer().GetPos();
		m_isOnFlg = true;
	}
	else
	{
		Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 0, -1);
		Math::Matrix mat = transMat * m_mat;
		m_gameScene->GetPlayer().SetMat(mat);
		m_gameScene->GetPlayer().SetPos(mat.Translation());
	}
	//Math::Matrix transMat = DirectX::XMMatrixTranslation(m_savePos.x, m_savePos.y, m_savePos.z);
}

void PlatFormUpstanding::Move()
{
	const float ang = 90.0f; // Šp“x•Ï‚¦‚é‹æŠÔ
	const float angMax = 0;
	const float angMin = -90;
	float sub;
	if (m_dirFlg == true)
	{
		sub = abs(m_ang - angMin);
		if (sub < 0.3 * ang)  // 
		{
			m_speed = -sub * 0.04f;
		}
		else if (sub > 0.7 * ang)
		{
			m_speed -= 0.01f;
		}

		if (sub <= 0.1f)
		{
			m_ang = angMin;
			ChangeDir();
			m_speed = 0;

		}
	}

	if (m_dirFlg == false)
	{
		sub = abs(m_ang - angMax);
		if (sub < 0.3 * ang)
		{
			m_speed = sub * 0.04f;
		}
		else if (sub > 0.7 * ang)
		{
			m_speed += 0.01f;
		}

		if (sub <= 0.1f)
		{
			m_ang = angMax;
			ChangeDir();
			m_speed = 0;

		}
	}
	m_ang += m_speed;
}

void PlatFormUpstanding::ChangeDir()
{
	if (m_dirFlg == true)
	{
		m_dirFlg = false;
	}
	else
	{
		m_dirFlg = true;
	}
}

bool PlatFormUpstanding::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_model, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}

bool PlatFormUpstanding::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const
{

	bool hit;
	hit = ModelIntersects_IncludeNormal(*m_model, m_mat, _rayPos, _rayVec, _dis, _normalVec);

	return hit;
}

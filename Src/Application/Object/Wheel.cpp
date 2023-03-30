#include "../Headers.h"


Wheel::Wheel(GameScene* _gameScene, float _ang, const Math::Matrix& _transMat, const Math::Matrix& _rotMat)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/Wheel.gltf");
	m_ang = 0;
	m_plusAng = _ang;
	m_gameScene = _gameScene;
	m_transMat = _transMat;
	m_rotMat = _rotMat;
	for (int i = 0; i < 5; i++)
	{
		m_platform[i] = std::make_shared<Platform>(_gameScene);
	}

	m_isOnFlg = false;
}

Wheel::~Wheel()
{

}

void Wheel::Update()
{
	const Chest& chest = m_gameScene->GetChest();
	if (chest.GetOpenFlg() == true)
	{
		m_ang += m_plusAng;
	}
	if (m_ang >= 360)
	{
		m_ang -= 360;
	}
	Math::Matrix rotMat;
	rotMat = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_ang));
	m_mat = rotMat * m_rotMat * m_transMat;

	{
		Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 50.1f, 0);

		Math::Matrix tmpMat = transMat * m_mat;
		m_platform[0]->SetPos(tmpMat.Translation());

	}

	{


		Math::Matrix transMat = DirectX::XMMatrixTranslation(47.5f, 15.3f, 0);
		Math::Matrix tmpMat = transMat * m_mat;
		m_platform[1]->SetPos(tmpMat.Translation());

	}

	{

		Math::Matrix transMat = DirectX::XMMatrixTranslation(29.3f, -40.4f, 0);
		Math::Matrix tmpMat = transMat * m_mat;
		m_platform[2]->SetPos(tmpMat.Translation());


	}

	{

		Math::Matrix transMat = DirectX::XMMatrixTranslation(-29.3f, -40.4f, 0);
		Math::Matrix tmpMat = transMat * m_mat;
		m_platform[3]->SetPos(tmpMat.Translation());


	}
	{

		Math::Matrix transMat = DirectX::XMMatrixTranslation(-47.5f, 15.3f, 0);
		Math::Matrix tmpMat = transMat * m_mat;
		m_platform[4]->SetPos(tmpMat.Translation());

	}

	for (int i = 0; i < 5; i++)
	{
		m_platform[i]->Update();

	}




}

void Wheel::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
	for (int i = 0; i < 5; i++)
	{
		m_platform[i]->Draw();
	}

}

void Wheel::MoveTogether()
{

	if (m_isOnFlg == false)
	{
		m_isOnFlg = true;
		m_saveVec = m_gameScene->GetPlayer().GetPos() - m_mat.Translation();
		m_saveAng = m_ang;
	}
	else  
	{
		float plusAng = m_ang - m_saveAng;
		Math::Matrix rotMat;
		rotMat = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(plusAng));
		Math::Vector3 vec;
		vec = DirectX::XMVector3TransformNormal(m_saveVec, rotMat);
		Math::Vector3 plusPos;
		plusPos = vec - m_saveVec;

		m_gameScene->GetPlayer().AddPos(plusPos);

		m_saveVec = m_gameScene->GetPlayer().GetPos() - m_mat.Translation();
		m_saveAng = m_ang;
	}

}


bool Wheel::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec,float& _dis)const
{
	float minDis = FLT_MAX;
	float dist;
	for (int i = 0; i < 5; i++)
	{

		bool hit = m_platform[i]->CheckBoard(_rayPos, _rayVec, _dis);

		if (hit == false)continue;

		dist = _dis;

		if (dist < minDis)
		{
			minDis = dist;
		}
	}

	if (minDis != FLT_MAX)
	{
		_dis = minDis;
		return true;
	}
	else
	{
		return false;
	}

}



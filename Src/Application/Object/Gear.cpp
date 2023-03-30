#include"../Headers.h"

Gear::Gear(GameScene* _gameScene)
{
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/Gear.gltf");
	m_mat = Math::Matrix::Identity;
	m_ang = 0;
	m_gameScene = _gameScene;
	m_isOnFlg = false;
}

Gear::~Gear()
{
	
}

void Gear::Update()
{
	m_ang += 0.18f;
	if (m_ang <= -360)
	{
		m_ang += 360;
	}

	Math::Matrix transMat = DirectX::XMMatrixTranslation(-98.3f, 86.3f, -61.7f);
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));

	m_mat = rotMatY * transMat;
}

void Gear::Draw()
{
	SHADER.m_standardShader.SetUVTiling({ 2,2 });
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
	SHADER.m_standardShader.SetUVTiling({ 1,1 });
}

void Gear::SetOnGearFlg(bool _flg)
{
	m_isOnFlg = _flg;
}

void Gear::MoveTogether()
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
		rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(plusAng));
		Math::Vector3 vec;
		vec = DirectX::XMVector3TransformNormal(m_saveVec, rotMat);
		Math::Vector3 plusPos;
		plusPos = vec - m_saveVec;

		m_gameScene->GetPlayer().AddPos(plusPos);

		m_saveVec = m_gameScene->GetPlayer().GetPos() - m_mat.Translation();
		m_saveAng = m_ang;
	}
}

bool Gear::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec,  float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_model, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}

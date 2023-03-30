#include"../Headers.h"

Obstacle::Obstacle(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/Rock.gltf");
	m_mat = DirectX::XMMatrixTranslation(60, 169, 0);
	m_gameScene = _gameScene;
	m_hp = 3;
	m_aliveFlg = true;
	m_hitFlg = false;
}

Obstacle::~Obstacle()
{
	//m_model->Release();
}

void Obstacle::Update()
{
	if (m_aliveFlg != true)
	{
		return;
	}

	if (m_hp <= 0)
	{
		m_aliveFlg = false;
	}


}

void Obstacle::Draw()
{
	if (m_aliveFlg != true)
	{
		return;
	}

	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
}

bool Obstacle::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const
{
	bool hit;
	hit = ModelIntersects_IncludeNormal(*m_model, m_mat, _rayPos, _rayVec, _dis, _normalVec);

	return hit;
}

void Obstacle::Hit()
{
	m_hp--;
	Math::Matrix rotMatZ = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians((float)(rand() % 360)));
	m_gameScene->GetEffectManager().Add(new Crack(rotMatZ, m_mat));

}



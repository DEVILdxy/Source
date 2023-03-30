#include"../Headers.h"

Shield::Shield(GameScene* _gameScene)
{
	m_model=MODELMANAGER.GetModel("Data/Models/Characters/shield.gltf");
	m_mat = Math::Matrix::Identity;
	m_gameScene = _gameScene;
	
}

Shield::~Shield()
{

}

void Shield::Update()
{	
	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, -0.5f, 0);
	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
	m_mat = rotMatX * transMat * m_gameScene->GetLeftArm().GetMat();
	
	
}

void Shield::Draw()
{	
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
}



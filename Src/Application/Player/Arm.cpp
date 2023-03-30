#include"../Headers.h"

Arm::Arm()
{
	m_model = MODELMANAGER.GetModel("Data/Models/Characters/Arm.gltf");
	m_mat = Math::Matrix::Identity;
}

Arm::~Arm()
{
	
}

void Arm::Update()
{	
	
}

void Arm::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
}

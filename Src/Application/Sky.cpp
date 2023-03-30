#include"Headers.h"

Sky::Sky()
{
	m_model=MODELMANAGER.GetModel("Data/Sky/Sky.gltf");
	m_mat = DirectX::XMMatrixTranslation(0, 0, 0);
	Math::Matrix scalMat = DirectX::XMMatrixScaling(50, 50, 50);
	m_mat = scalMat * m_mat;
}

Sky::~Sky()
{
	m_model->Release();
}

void Sky::Draw()
{
	SHADER.m_standardShader.SetLightEnable(false);
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
	SHADER.m_standardShader.SetLightEnable(true);
}

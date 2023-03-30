#include"../Headers.h"

void Object::Update()
{
}

void Object::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
}

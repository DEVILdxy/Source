#include "../Headers.h"

void Effect::Update()
{
}

void Effect::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//�g���Ăق������_�o�b�t�@
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//���_�̗��p�@(�v���~�e�B�u�g�|���W�[)���Z�b�g 
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`�����Z�b�g							�@�@
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex->GetSRViewAddress());
	//---------------------------
	//�{���\��
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//���܂łɐݒ肵�����e�𔽉f
	//�`��					  �����g�����H
	D3D.GetDevContext()->Draw(4, 0);
}

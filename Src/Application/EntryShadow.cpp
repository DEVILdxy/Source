#include"Headers.h"

EntryShadow::EntryShadow()
{
	m_tex.Load("Data/Texture/Black.png");

	m_v[0].uv = { 0,0 };
	m_v[1].uv = { 1,0 };
	m_v[2].uv = { 0,1 };
	m_v[3].uv = { 1,1 };	

	m_mat = DirectX::XMMatrixTranslation(0, 0, 0);

	m_depth = 0;
	m_alpha = 1;

}

EntryShadow::~EntryShadow()
{
	m_vb.Release();
	m_tex.Release();
}

void EntryShadow::Update()
{
	m_v[0].pos = { -5,5,m_depth };
	m_v[1].pos = { 5,5,m_depth };
	m_v[2].pos = { -5,-5,m_depth };
	m_v[3].pos = { 5,-5,m_depth };

	m_v[0].color = { 1,1,1,m_alpha };
	m_v[1].color = { 1,1,1,m_alpha };
	m_v[2].color = { 1,1,1,m_alpha };
	m_v[3].color = { 1,1,1,m_alpha };

	D3D11_SUBRESOURCE_DATA srd = {};//�[���N���A(���g�S�ă[��)
	srd.pSysMem = m_v;//&v[0]�@���_�f�[�^�������Ă���擪�A�h���X
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(m_v), D3D11_USAGE_DEFAULT, &srd);
}

void EntryShadow::Draw()
{
	//�|��============================================
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//�g���Ăق������_�o�b�t�@
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//���_�̗��p�@(�v���~�e�B�u�g�|���W�[)���Z�b�g ���n��?
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`�����Z�b�g							�@�@
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex.GetSRViewAddress());
	//---------------------------
	//�{���\��
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//���܂łɐݒ肵�����e�𔽉f
	//�`��					  �����g�����H
	D3D.GetDevContext()->Draw(4, 0);
}

void EntryShadow::SetParameter(float _alpha, float _depth)
{
	m_alpha = _alpha;
	m_depth = _depth;
}

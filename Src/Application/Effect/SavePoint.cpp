#include "../Headers.h"

SavePoint::SavePoint(GameScene* _gameScene, Math::Vector3& _pos)
{
	m_tex = TEXTUREMANAGER.GetTexture("Data/Texture/Light.png");
	m_texSub = TEXTUREMANAGER.GetTexture("Data/Texture/Light.png");
	Math::Matrix transMat = DirectX::XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	m_mat = transMat;
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90));
	m_matSub = rotMatY * transMat;


	Vertex v[4];
	v[0].pos = { -3,3,0 };
	v[1].pos = { 3,3,0 };
	v[2].pos = { -3,-3,0 };
	v[3].pos = { 3,-3,0 };

	v[0].color = { 1,1,1,0.5f };
	v[1].color = { 1,1,1,0.5f };
	v[2].color = { 1,1,1,0.5f };
	v[3].color = { 1,1,1,0.5f };

	v[0].uv = { 0,0 };
	v[1].uv = { 1,0 };
	v[2].uv = { 0,1 };
	v[3].uv = { 1,1 };

	D3D11_SUBRESOURCE_DATA srd = {};//�[���N���A(���g�S�ă[��)
	srd.pSysMem = v;//&v[0]�@���_�f�[�^�������Ă���擪�A�h���X

	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);
}

SavePoint::~SavePoint()
{
}

void SavePoint::Update()
{
	
}

void SavePoint::Draw()
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
	//�g���Ăق������_�o�b�t�@
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//���_�̗��p�@(�v���~�e�B�u�g�|���W�[)���Z�b�g 
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`�����Z�b�g							�@�@
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_texSub->GetSRViewAddress());
	//---------------------------
	//�{���\��
	SHADER.m_effectShader.SetWorldMatrix(m_matSub);
	SHADER.m_effectShader.WriteToCB();//���܂łɐݒ肵�����e�𔽉f
	//�`��					  �����g�����H
	D3D.GetDevContext()->Draw(4, 0);
}

#include"Headers.h"

Board::Board()
{
	m_mat = DirectX::XMMatrixTranslation(10, 10, 60);

	//�|���̏�����
	Vertex v[4];
	//��4����6�̎l�p�`�{���S��
	//���W
	v[0].pos = { -10,10,0 };//��Z����??�荿?
	v[1].pos = { 10,10,0 };
	v[2].pos = { -10,-10,0 };
	v[3].pos = { 10,-10,0 };

	//�F�ݒ�@	  *RGB A
	v[0].color = { 1,0,0,1 };
	v[1].color = { 1,0,0,1 };
	v[2].color = { 1,0,0,1 };
	v[3].color = { 1,0,0,1 };

	D3D11_SUBRESOURCE_DATA srd = {};//�[���N���A(���g�S�ă[��)
	srd.pSysMem = v;//&v[0]�@���_�f�[�^�������Ă���擪�A�h���X
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	//�ǂ̃��[���h���W���v�Z���ĕۑ����Ă���
	m_wallPos[0] = DirectX::XMVector3TransformCoord(v[0].pos, m_mat);
	m_wallPos[1] = DirectX::XMVector3TransformCoord(v[1].pos, m_mat);
	m_wallPos[2] = DirectX::XMVector3TransformCoord(v[2].pos, m_mat);
	m_wallPos[3] = DirectX::XMVector3TransformCoord(v[3].pos, m_mat);
}

Board::~Board()
{
	m_vb.Release();
}

void Board::Draw()
{

	//���_buffer�Z�b�g
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//UINT(unsigned int)
	//�g���Ăق������_�o�b�t�@
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//���_�̗��p�@(�v���~�e�B�u�g�|���W�[)���Z�b�g ���n��?
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`�����Z�b�g							�@�@�@�����e�N�X�`��
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());
	//---------------------------

	//�{���\��
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//���܂łɐݒ肵�����e�𔽉f
	//�`��					  �����g�����H
	D3D.GetDevContext()->Draw(4, 0);
}

bool Board::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	//���C����(���C���ˈʒu�A���C���˕���[�����P]�A�|���R���̒��_���W�R��,������������������ϐ�)
	if (DirectX::TriangleTests::Intersects(_rayPos, _rayVec, m_wallPos[0], m_wallPos[1], m_wallPos[2], _dis) ||
		DirectX::TriangleTests::Intersects(_rayPos, _rayVec, m_wallPos[1], m_wallPos[2], m_wallPos[3], _dis))   //?���O�p�`
	{
		//2���̂����ǂ��炩�̃|���S���Ƀ��C����������
		return true;
	}
	return false;
}

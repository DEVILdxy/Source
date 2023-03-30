#include"../Headers.h"

Spray::Spray()
{
	m_tex = TEXTUREMANAGER.GetTexture("Data/Texture/Cloud.bmp");

	m_v[0].uv = { 0,0 };
	m_v[1].uv = { 1,0 };
	m_v[2].uv = { 0,1 };
	m_v[3].uv = { 1,1 };

	m_mat = DirectX::XMMatrixTranslation(0, 0, 0);

	m_pos = {};


	//���Z������������
	m_blendAlpha = D3D.CreateBlendState(KdBlendMode::Alpha);
	m_blendAdd = D3D.CreateBlendState(KdBlendMode::Add);

	m_aliveFlg = true;

	//m_water =_water;
}

Spray::~Spray()
{
	m_vb.Release();

	m_blendAlpha->Release();
	m_blendAdd->Release();

}

void Spray::Update()
{
	//�r���{�[�h�쐬
	//====================
	Math::Matrix tmpMat = SHADER.m_cb7_Camera.Work().mV; //�J�����̍s����擾(View�s��)

	//�J�����̍��W�͕s�v�Ȃ̂Ŗ����ɂ�����
	tmpMat._41 = 0; //X
	tmpMat._42 = 0; //Y
	tmpMat._43 = 0; //Z

	//�t�s��ɕϊ�
	tmpMat = tmpMat.Invert();

	//===================================
	//Math::Matrix waterMat = m_water->GeMat();
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat =tmpMat* transMat;
	static float tmpPos = 0;
	static float tmpAlpha = 0;

	tmpPos += 0.01f;
	tmpAlpha -= 0.001f;
	const int size = 10; // �܂�̃T�C�Y
	const float alpha = 0.6f;

	m_v[0].pos = { -size - tmpPos,size + tmpPos,0 };
	m_v[1].pos = { size + tmpPos,size + tmpPos,0 };
	m_v[2].pos = { -size - tmpPos,-size - tmpPos,0 };
	m_v[3].pos = { size + tmpPos,-size - tmpPos,0 };

	m_v[0].color = { 1,1,1,alpha + tmpAlpha };
	m_v[1].color = { 1,1,1,alpha + tmpAlpha };
	m_v[2].color = { 1,1,1,alpha + tmpAlpha };
	m_v[3].color = { 1,1,1,alpha + tmpAlpha };

	if ((alpha + tmpAlpha) <= 0.0f) // �܂��alpha�l��0�̎�
	{
		tmpAlpha = 0;
		tmpPos = 0;
		m_aliveFlg = false;
	}

	D3D11_SUBRESOURCE_DATA srd = {};//�[���N���A(���g�S�ă[��)
	srd.pSysMem = m_v;//&v[0]�@���_�f�[�^�������Ă���擪�A�h���X
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(m_v), D3D11_USAGE_DEFAULT, &srd);

	
}

void Spray::Draw()
{	//z�o�b�t�@�ւ̏������݂����Ȃ�
	D3D.GetDevContext()->OMSetDepthStencilState(
		D3D.CreateDepthStencilState(true, false), 0);

	//���Z�������ɕύX
	D3D.GetDevContext()->OMSetBlendState(m_blendAdd, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);


	//�|��============================================
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//�g���Ăق������_�o�b�t�@
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//���_�̗��p�@(�v���~�e�B�u�g�|���W�[)���Z�b�g ���n��?
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`�����Z�b�g							�@�@
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex->GetSRViewAddress());
	//---------------------------
	//�{���\��
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//���܂łɐݒ肵�����e�𔽉f
	//�`��					  �����g�����H
	D3D.GetDevContext()->Draw(4, 0);



	D3D.GetDevContext()->OMSetBlendState(m_blendAlpha, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	//���Z����OFF

	//�p���Ȃ��Ȃ����猳�ɖ߂�
	D3D.GetDevContext()->OMSetDepthStencilState(
		D3D.CreateDepthStencilState(true, true), 0);


	


}

void Spray::SetParameter(Math::Vector3 _pos)
{
	m_pos = _pos;
}

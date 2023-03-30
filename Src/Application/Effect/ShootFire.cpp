#include "../Headers.h"

ShootFire::ShootFire(const Math::Vector3& _pos)
{
	m_tex=TEXTUREMANAGER.GetTexture("Data/Texture/Smoke.png");
	m_mat = Math::Matrix::Identity;
	m_pos = _pos;
	m_anime = 0;
	m_aliveFlg = true;

	
}

ShootFire::~ShootFire()
{
	delete m_tex;
	m_vb.Release();
}

void ShootFire::Update()
{
	Vertex v[4];
	v[0].pos = { -0.8,0.8,0 };
	v[1].pos = { 0.8,0.8,0 };
	v[2].pos = { -0.8,-0.8,0 };
	v[3].pos = { 0.8,-0.8,0 };

	v[0].color = { 1,1,1,1 };
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	m_anime++;
	if (m_anime >= 64) {
		m_anime = 0;
		m_aliveFlg = false;
	}

	//           ↓表示したい画像番号
	float W1 = (m_anime % 8) * 32.0f / m_tex->GetWidth();
	//                   ↑横に並んでいる画像の数
	float H1 = (m_anime / 8) * 32.0f / m_tex->GetHeight();
	float W2 = (m_anime % 8 + 1) * 32.0f / m_tex->GetWidth();
	float H2 = (m_anime / 8 + 1) * 32.0f / m_tex->GetHeight();

	v[0].uv = { W1,H1 };
	v[1].uv = { W2,H1 };
	v[2].uv = { W1,H2 };
	v[3].uv = { W2,H2 };

	D3D11_SUBRESOURCE_DATA srd = {};//ゼロクリア(中身全てゼロ)
	srd.pSysMem = v;//&v[0]　頂点データが入っている先頭アドレス

	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	//ビルボード作成
	//====================
	Math::Matrix tmpMat = SHADER.m_cb7_Camera.Work().mV; //カメラの行列を取得(View行列)を取得

	//カメラの座標は不要なので無効にしたい
	tmpMat._41 = 0; //X
	tmpMat._42 = 0; //Y
	tmpMat._43 = 0; //Z

	//逆行列に変換
	tmpMat = tmpMat.Invert();

	//===================================
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = tmpMat * transMat;
}



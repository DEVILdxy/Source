#include"Headers.h"

Board::Board()
{
	m_mat = DirectX::XMMatrixTranslation(10, 10, 60);

	//板ポリの初期化
	Vertex v[4];
	//幅4高さ6の四角形ボリゴン
	//座標
	v[0].pos = { -10,10,0 };//以Z字走??定坐?
	v[1].pos = { 10,10,0 };
	v[2].pos = { -10,-10,0 };
	v[3].pos = { 10,-10,0 };

	//色設定　	  *RGB A
	v[0].color = { 1,0,0,1 };
	v[1].color = { 1,0,0,1 };
	v[2].color = { 1,0,0,1 };
	v[3].color = { 1,0,0,1 };

	D3D11_SUBRESOURCE_DATA srd = {};//ゼロクリア(中身全てゼロ)
	srd.pSysMem = v;//&v[0]　頂点データが入っている先頭アドレス
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	//壁のワールド座標を計算して保存しておく
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

	//頂点bufferセット
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//UINT(unsigned int)
	//使ってほしい頂点バッファ
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//頂点の利用法(プリミティブトポロジー)をセット 原始拓?
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャをセット							　　　↓白テクスチャ
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());
	//---------------------------

	//板ボリ表示
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//今までに設定した内容を反映
	//描画					  ↓何個使うか？
	D3D.GetDevContext()->Draw(4, 0);
}

bool Board::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	//レイ判定(レイ発射位置、レイ発射方向[長さ１]、ポリコムの頂点座標３つ,当たった距離を入れる変数)
	if (DirectX::TriangleTests::Intersects(_rayPos, _rayVec, m_wallPos[0], m_wallPos[1], m_wallPos[2], _dis) ||
		DirectX::TriangleTests::Intersects(_rayPos, _rayVec, m_wallPos[1], m_wallPos[2], m_wallPos[3], _dis))   //?个三角形
	{
		//2枚のうちどちらかのポリゴンにレイが当たった
		return true;
	}
	return false;
}

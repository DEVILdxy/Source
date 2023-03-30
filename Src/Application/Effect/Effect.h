#pragma once


class Effect
{
public:
	Effect() {  }
	~Effect() {}

	virtual void Update();
	virtual void Draw();

	const bool GetAliveFlg()const { return m_aliveFlg; }

protected:
	KdBuffer m_vb;
	KdTexture *m_tex;
	Math::Matrix m_mat;
	int m_anime;
	bool m_aliveFlg;

};
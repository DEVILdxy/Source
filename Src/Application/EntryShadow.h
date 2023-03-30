#pragma once

class EntryShadow
{
public:
	EntryShadow();
	~EntryShadow();
	void Update();
	void Draw();
	void SetParameter(float _alpha,float _depth);

private:

	KdBuffer m_vb;
	Math::Matrix m_mat;
	KdTexture m_tex;

	Vertex m_v[4];
	float m_alpha;
	float m_depth;

};
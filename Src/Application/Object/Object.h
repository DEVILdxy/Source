#pragma once
class Object
{
public:
	Object() { m_mat = Math::Matrix::Identity; }
	~Object() {}

	virtual void Update();
	virtual void Draw();

protected:
	KdModel* m_model;
	Math::Matrix m_mat;
};
#pragma once
class GameScene;
class Board
{
public:
	Board();
	~Board();
	void Draw();

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;


private:

	KdBuffer m_vb;//vertex buffer
	Math::Matrix m_mat;

	Math::Vector3 m_wallPos[4]; //壁の４頂点のワールド座標

};
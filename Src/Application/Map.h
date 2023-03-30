#pragma once

class GameScene;
class Map :public Object
{
public:
	Map(GameScene* _gameScene);
	~Map();
	void Init(std::string _fname, int _mapNo);

	void Draw()override;
	void Update()override;

	void CamMove(float _camDist,Math::Vector3& _camPos, Math::Vector3& _playerPos,Math::Vector3& _vec, Player& _player); // カメラとマップを当たった時　カメラを移動させる


	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec)const;
	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;

	const KdModel& GetModel()const { return *m_model; }
	const Math::Matrix& GetMat()const { return m_mat; }
	const int& GetLoadedMap()const { return m_loadedMap; }

	const Fence& GetFence()const { return *m_fence; }

private:

	int m_loadedMap;
	GameScene* m_gameScene;
	Fence* m_fence;

	//bool 


};
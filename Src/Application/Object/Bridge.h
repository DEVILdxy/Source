#pragma once
class GameScene;
class Bridge :public Object
{
public:
	Bridge(GameScene *_gameScene, std::string _fileName, float _maxHeight, float _upSpeed);
	~Bridge();
	void Update()override;
	
	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;

private:

	GameScene* m_gameScene;
	float m_upHeight;  // 上昇高さ
	float m_upSpeed;   // 上昇速さ
	float m_maxHeight;
};
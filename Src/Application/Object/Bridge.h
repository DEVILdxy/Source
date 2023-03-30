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
	float m_upHeight;  // �㏸����
	float m_upSpeed;   // �㏸����
	float m_maxHeight;
};
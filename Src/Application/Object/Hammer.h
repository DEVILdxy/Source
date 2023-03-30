#pragma once
class GameScene;
class Hammer :public Object
{
public:
	Hammer(GameScene* _gameScene, float _speed, int _towards, float _range, Math::Vector3 _pos);// _speed�͕p�x�A_towards�͕����A_range�͕�
	~Hammer();
	void Update()override;
	void Draw()override;

	void Push(Math::Vector3 _pos);

protected:
	float m_tmpAng=0; // �h���p�x
	float m_ang;
	GameScene* m_gameScene;
	float m_pushPow;
	int m_towards;// ����
	Math::Vector3 m_pos;
	float m_speed;// �p�x
	float m_range;

	std::shared_ptr<Shadow> m_shadow; // �e
};
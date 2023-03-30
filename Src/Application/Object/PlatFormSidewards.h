#pragma once
class GameScene;
class PlatFormSidewards :public Object
{
public:
	PlatFormSidewards(GameScene* _gameScene);
	~PlatFormSidewards();

	void Update()override;

	void MoveTogether();
	void SetIsOnFlg(bool _flg) { m_isOnFlg = _flg; }

	bool CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis)const;

private:

	Math::Vector3 m_startPos;
	Math::Vector3 m_endPos;
	Math::Vector3 m_pos;
	Math::Vector3 m_savePos;

	float m_speed;
	float m_distance;// ‘S‘Ì‚Ì‹——£

	bool m_isOnFlg;

	GameScene* m_gameScene;

};
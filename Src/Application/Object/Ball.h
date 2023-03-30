#pragma once
class GameScene;
class Ball :public Object
{
public:
	Ball(GameScene* _gameScene);

	void Update()override;

	

	void SetPos(Math::Vector3 _pos);
	const Math::Vector3& GetPos()const { return m_mat.Translation(); }

	const Math::Vector3& GetOldPos()const { return m_oldPos; }
	const Math::Vector3& GetNowPos()const { return m_nowPos; }
	const Math::Vector3& GetMoveVec()const { return m_moveVec; }


private:
	
	void Move_Slope();// 斜面上の移動処理
	void Roll(Math::Vector3& _dir); // 回転　移動方向とballの直径の値を渡す
	void Move();// 反射を含めて移動処理
	void Move(const Math::Vector3& _vec);// ワールド座標を変える

	Math::Vector3 m_oldPos;
	Math::Vector3 m_nowPos;

	Math::Vector3 m_moveVec;// 移動ベクトル	
	float m_length;// 移動長さ

	GameScene* m_gameScene;
};
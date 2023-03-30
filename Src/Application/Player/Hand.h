#pragma once

class GameScene;

class Player;
class Hand
{
public:
	Hand(GameScene* _gameScene);
	~Hand();
	void Update();
	void Update_Line();
	
	void Draw();
	void Draw_Chain(float _length);
	void Draw_Frie();
	
	void CheckHit(Math::Vector3 _vec); //�����蔻��֐�

	const bool GetMapHitState()const { return m_mapHit; }
	const bool GetEnemyHitState()const { return m_enemyHit; }
	const bool GetPlatFormMoveHitState()const { return m_PFMhit; }
	const bool GetPlatFormInvisibleHitState()const { return m_PFIhit; }
	const Math::Matrix GetMat()const { return m_mat; }
	const bool GetShootState()const { return m_isShoot; }
	void SetShootState(bool _state) { m_isShoot = _state; }
	void SetMapHitState(bool _state) { m_mapHit = _state; }
	void SetEnemyHitState(bool _state) { m_enemyHit = _state; }
	void SetPlatFormMoveHitState(bool _state) { m_PFMhit = _state; }
	void SetPlatFormInvisibleHitState(bool _state) { m_PFIhit = _state; }

	const float GetHandDis()const { return m_hitMapDis; }

private:

	void Move();
	
	//���˕�
	KdModel* m_model;
	Math::Matrix m_mat;
	bool m_isShoot;//�E�蔭�˃t���O
	bool m_mapHit;  //���map�̐ڐG�t���O
	bool m_enemyHit;// ���enemy�̐ڐG�t���O
	bool m_PFMhit;// ��Ɨ����̐ڐG�t���O
	bool m_PFIhit;// ��ƉB������̐ڐG�t���O
	float m_hitMapDis = 2077; // ��map�̓����蔻�苗��
	float m_hitEnemyDis = 2077;// ��enemy�̓����蔻�苗��
	float m_hitPFMDis = 2077;// ��PlatFormMove�̓����蔻�苗��
	Math::Vector3 m_moveVec = { 0,0,0 };
	
	//�`�F�[��==========
	KdModel* m_chainModel;
	Math::Matrix m_chainMat[50];
	Math::Vector3 v_hand_rightArm;

	//����=============
	Math::Matrix m_fireMat;
	KdBuffer m_fireVb;
	KdTexture* m_fireTex;
	int m_anime;      //�����A�j������

	static const int lineNum = 50;
	Math::Matrix m_lineMat[lineNum];
	//==================

	GameScene* m_gameScene;

	//���˃T�E���h
	bool m_isSoundPlay;
	int m_timer;
	bool m_isTimeKeep;
	std::shared_ptr<KdSoundEffect>m_sound;
	std::shared_ptr<KdSoundInstance>m_soundInst;
};
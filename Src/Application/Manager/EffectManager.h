#pragma once

class Effect;

class EffectManager
{
public:
	// ������
	EffectManager();
	// �I������
	~EffectManager();
	// �X�V����
	void Update();
	// �`�揈��
	void Draw();

	//�I�u�W�F�N�g��ǉ����鏈��
	void Add(Effect* _obj);



private:

	std::list<Effect*> m_list;

	


};
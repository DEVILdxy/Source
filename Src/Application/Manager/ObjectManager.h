#pragma once
class Object;
class ObjectManager
{
public:
	// ������
	ObjectManager() { m_List.clear(); }
	// �I������
	~ObjectManager();
	// �X�V����
	void Update();
	// �`�揈��
	void Draw();

	//�I�u�W�F�N�g��ǉ����鏈��
	void Add(Object* _obj);

private:

	std::vector<Object*>m_List;

};
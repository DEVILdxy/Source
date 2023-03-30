#pragma once
class Object;
class ObjectManager
{
public:
	// 初期化
	ObjectManager() { m_List.clear(); }
	// 終了処理
	~ObjectManager();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	//オブジェクトを追加する処理
	void Add(Object* _obj);

private:

	std::vector<Object*>m_List;

};
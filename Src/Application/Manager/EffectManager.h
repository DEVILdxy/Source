#pragma once

class Effect;

class EffectManager
{
public:
	// 初期化
	EffectManager();
	// 終了処理
	~EffectManager();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	//オブジェクトを追加する処理
	void Add(Effect* _obj);



private:

	std::list<Effect*> m_list;

	


};
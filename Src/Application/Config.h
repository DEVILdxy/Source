#pragma once

class PlayerSetting
{
public:
	static constexpr float movePow = 0.4f;// 通常状態に移動速度
	static constexpr float movePowTPS = 0.2f;// 照準状態に移動速度
	static constexpr float toEnemyDis = 1.5f;// と敵の間の距離
	static constexpr float pushPow = 0.8f;// 推す力
	static constexpr float toPlatformUpDisLimit = 3.0f;// 立ち足場から影響受ける距離
	static constexpr float inMovieMoveDis = 0.3f;// 演出に移動速度
	static constexpr float height = 1.5f;// キャラクターの高さ
	static constexpr float awayDist = 1.0f; // 立ち入り禁止エリアの幅 
	static constexpr float ToObjectDist = 1.0f; // と目標の距離
};
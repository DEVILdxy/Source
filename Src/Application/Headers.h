#pragma once
#include "main.h"
#include "define.h"

bool ModelIntersects_IncludeNormal(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec);
bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis);
float GetVecAngY(Math::Vector3 _vec);
float TurnToAng(Math::Matrix& _mat, const float _targetAng, const float _plusAng);


#include"System.h"
#include"Config.h"

#include"Manager/TextureManager.h"
#include"Manager/ModelManager.h"

#include"Player/Shield.h"
#include"Player/Hand.h"
#include"Player/Arm.h"
#include"Player/Arm_Left.h"
#include"Player/Arm_Right.h"
#include"Player/Player.h"


#include"Enemy/BaseState.h"
#include"Enemy/Appear.h"
#include"Enemy/Track.h"
#include"Enemy/Attack.h"
#include"Enemy/BeAttacked.h"

#include"Enemy/Enemy.h"
#include"Manager/EnemyManager.h"

#include"Object/Object.h"
#include"Object/Bridge.h"
#include"Object/Chest.h"
#include"Object/Fence.h"
#include"Object/Water.h"
#include"Sky.h"
#include"Map.h"
#include"Object/Door_Left.h"
#include"Object/Door_Right.h"	
#include"Object/Gear.h"
#include"Object/Wheel.h"
#include"Object/Platform.h"
#include"Object/Hammer.h"
#include"Object/WaterFall.h"
#include"Object/AimCone.h"
#include"Object/Obstacle.h"
#include"Object/Ball.h"
#include"Object/PlatFormUpstanding.h"
#include"Object/PlatFormInvisible.h"
#include"Object/PlatFormSidewards.h"
#include"Object/Wall_Reflect.h"
#include"Manager/ObjectManager.h"

#include"Board.h"

#include"Effect/Effect.h"
#include"Effect/Spray.h"
#include"Effect/ShootFire.h"
#include"EntryShadow.h"
#include"Effect/MagicCircle.h"
#include"Effect/Crack.h"
#include"Effect/Shadow.h"
#include"Effect/SavePoint.h"
#include"Manager/EffectManager.h"

#include"UI.h"

#include"Scene/BaseScene.h"
#include"Scene/GameScene.h"
#include"Scene/TitleScene.h"
#include"Scene/GameOverScene.h"
#include"Manager/SceneManager.h"
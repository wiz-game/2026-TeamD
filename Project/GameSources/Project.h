/*!
@file Project.h
@brief コンテンツ用のヘッダをまとめる
*/

#pragma once


#include "Scene.h"

// エディター用クラス
#include "StageObjectInterface.h"
#include "DebugLog.h"
#include "BinaryFile.h"
#include "Conversion.h"
#include "Gizmo.h"

// マネージャークラス
#include "GameManager.h"
#include "SoundManager.h"

// オリジナルコンポーネント
#include "Move.h"
#include "FurBubble.h"

// キャラクター
#include "Character.h"

// ステージオブジェクト
#include "SkyBox.h"
#include "Ground.h"
#include "Tree.h"
#include "Mushroom.h"
#include "Dirt.h"

// エネミー
#include "EnemyBase.h"
#include "EnemyAlpaca.h"

#include "Bubble.h"
#include "TrampolineBubbles.h"
#include "Player.h"

#include "MyCamera.h"

// UI
#include "UIBase.h"
#include "UIBlinking.h"

// ステージ
#include "TitleStage.h"
#include "SelectStage.h"
#include "GameStage.h"

#include "StageEditor.h"

#include "InputManager.h"
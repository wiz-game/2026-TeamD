/*!
@file Project.h
@brief コンテンツ用のヘッダをまとめる
*/

#pragma once

// mediaファイルの読み込みクラス
#include "LoadMedia.h"

#include "Scene.h"

// 汎用的な関数や構造体ヘッダーファイル
#include "Utils.h"

// エディター用クラス
#include "StageObjectInterface.h"
#include "DebugLog.h"
#include "BinaryFile.h"
#include "Gizmo.h"

// マネージャークラス
#include "GameManager.h"
#include "SoundManager.h"
#include "MenuManager.h"
#include "EffectManager.h"
#include "EffectUpdateDrawManager.h"

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
#include "Stone.h"
#include "FallenTree.h"
#include "FirTree.h"
#include "Slope.h"
#include "Fence.h"
#include "LongMushroom.h"
#include "Foothold.h"
#include "Board.h"
#include "Apple.h"
#include "Shrub.h"

// エネミー
#include "EnemyBase.h"
#include "EnemyAlpaca.h"

#include "Bubble.h"
#include "TrampolineBubbles.h"
#include "Player.h"

// アイテム
#include "ItemBase.h"
#include "PowerUpSoap.h"

// UI
#include "UIBase.h"
#include "UIBlinking.h"
#include "UISlide.h"
#include "UITransitionSlide.h"

// ステージ
#include "TitleStage.h"
#include "SelectStage.h"
#include "GameStage.h"
#include "GameClearStage.h"
#include "GameOverStage.h"

#include "MyCamera.h"

#include "StageEditor.h"

#include "InputManager.h"
/*!
@file Move.cpp
@brief 移動など
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	Move::Move(const shared_ptr<GameObject>& gameObject) :
		Component(gameObject),
		m_Speed(10.0f) 
	{
	}

	//初期化
	void Move::OnCreate()
	{
		
	}

	void Move::OnUpdate()
	{
	}

	void Move::OnDraw()
	{

	}

	void Move::VectorMove(Vec3 Input)
	{
		// 現在のステージを取得する
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();

		// 経過時間を取得する
		auto deltaTime = App::GetApp()->GetElapsedTime();

		// Transformのあるゲームオブジェクトを取得する
		auto transComp = GetGameObject()->GetComponent<Transform>();
		auto initPos = transComp->GetPosition();
		
		// 移動ベクトルを格納するための変数
		float rotX = 0.0f, rotZ = 0.0f;

		// ステージ情報が有効なとき
		if (stage)
		{
			// ステージのカメラを取得する
			auto camera = stage->GetView()->GetTargetCamera();

			// カメラの現在の情報をそれぞれ取得する
			auto at = camera->GetAt();
			auto eye = camera->GetEye();

			// 正面を計算する「見てる場所 - カメラの位置」
			Vec3 forward = at - eye;
			forward.normalize();	// カメラの傾きによって移動が早くなったり遅くなったりするのを防ぐ

			// 向いている方向に行くようにする
			Vec3 forwardMove = Vec3(forward.z, 0.0f, -forward.x);

			// 移動の結果を格納するための変数
			float vectorx = 0.0f;
			float vectorz = 0.0f;

			// 前後移動
			vectorx += forward.x * Input.z * deltaTime * m_Speed;
			vectorz += forward.z * Input.z * deltaTime * m_Speed;

			// 左右移動
			vectorx += forwardMove.x * Input.x * deltaTime * m_Speed;
			vectorz += forwardMove.z * Input.x * deltaTime * m_Speed;

			// 前後左右で移動した計算結果をVectorに代入する
			initPos.x += vectorx;
			initPos.z += vectorz;
			rotX = vectorx;
			rotZ = vectorz;
		}
		// 移動の結果を求める
		transComp->SetPosition(initPos);

		auto mycamera = stage->GetView()->GetTargetCamera()->GetThis<MyCamera>();
		auto aim = mycamera->GetIsAiming();
		if (aim == false)
		{
			// 入力ベクトルに基づき、キャラクターが進行方向を向くように回転を設定する
			float angle = atan2f(rotX, rotZ);
			transComp->SetRotation(0.0f, angle, 0.0f);
		}
	}
}
//end basecross

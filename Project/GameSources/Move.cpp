/*!
@file Move.cpp
@brief 移動など
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	//初期化
	void Move::OnCreate()
	{
		
	}

	void Move::OnUpdate()
	{
		// ゲームオブジェクトの位置情報などを取得する
		auto transComp = GetGameObject()->GetComponent<Transform>();
		// 現在の位置を取得する
		auto transPos = transComp->GetPosition();
		Vec3 movePos = VectorMove(transPos);
		transComp->SetPosition(movePos);
	}

	void Move::OnDraw()
	{

	}

	Vec3 Move::VectorMove(Vec3 Vector)
	{
		// コントローラーの接続
		auto device = App::GetApp()->GetInputDevice();
		auto control = device.GetControlerVec()[0];

		// 現在のステージを取得する
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();

		// 経過時間を取得する
		auto deltaTime = App::GetApp()->GetElapsedTime();

		// Transformのあるゲームオブジェクトを取得する
		auto transComp = GetGameObject()->GetComponent<Transform>();

		auto player = stage->GetSharedGameObject<Player>(L"Player");

		// 左スティックの傾き具合
		float stickLPulsX = .7f, stickLMinusX = -.7f;
		float stickLPulsY = .7f, stickLMinusY = -.7f;

		// 左スティックの傾き
		float stickLX = control.fThumbLX;
		float stickLY = control.fThumbLY;

		// コントローラが接続されているかどうか
		if (control.bConnected)
		{
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

				// 移動速度
				float speed = 3.0f;
				
				// 前後移動
				vectorx += forward.x * stickLY * deltaTime * speed;
				vectorz += forward.z * stickLY * deltaTime * speed;

				// 左右移動
				vectorx += forwardMove.x * stickLX * deltaTime * speed;
				vectorz += forwardMove.z * stickLX * deltaTime * speed;

				// 前後左右で移動した計算結果をVectorに代入する
				Vector.x += vectorx;
				Vector.z += vectorz;

				if (player->GetTargetMode() == false)
				{
					// 移動している方向に向かせるようにする
					float angle = atan2f(vectorx, vectorz);
					transComp->SetRotation(0.0f, angle, 0.0f);
				}
			}
		}
		return Vector;
	}
}
//end basecross

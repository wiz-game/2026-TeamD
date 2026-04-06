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
		auto transComp = GetGameObject()->GetComponent<Transform>();
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
		auto control = device.GetControlerVec();

		// 左スティックの傾き具合
		float stickLPulsX = .7f, stickLMinusX = -.7f;
		float stickLPulsY = .7f, stickLMinusY = -.7f;

		// コントローラが接続されているかどうか
		if (control[0].bConnected)
		{
			// X軸
			if (control[0].fThumbLX >= stickLPulsX)
			{
				Vector.x += App::GetApp()->GetElapsedTime();
			}
			else if (control[0].fThumbLX <= stickLMinusX)
			{
				Vector.x -= App::GetApp()->GetElapsedTime();
			}
			// Y軸
			if (control[0].fThumbLY >= stickLPulsY)
			{
				Vector.z += App::GetApp()->GetElapsedTime();
			}
			else if (control[0].fThumbLY <= stickLMinusY)
			{
				Vector.z -= App::GetApp()->GetElapsedTime();
			}
		}
		return Vector;
	}
}
//end basecross

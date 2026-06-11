
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MyCamera::MyCamera()
	{
		m_viewPointMoveSpeed = 5.0f;
	}

	MyCamera::~MyCamera()
	{
	}

	void MyCamera::OnCreate()
	{
		Camera::OnCreate();
	}
	
	void MyCamera::OnUpdate()
	{
		switch (GameManager::Instance().GetGameMode())
		{
		default:
			break;
		case ENUM_GameMode::Play:
			UpdatePlayMode();
			break;
		case ENUM_GameMode::Menu:
			UpdateMenuMode();
			break;
		case ENUM_GameMode::Editor:
			UpdateEditorMode();
			break;
		}
		
		// CameraクラスのUpdateを最後に呼ぶ
		Camera::OnUpdate();
	}

	void MyCamera::PlayModeMove
	(
		float stickrx,
		float stickry,
		float sticklx,
		float stickly
	)
	{
		// InputManagerからの入力を受け取り、カメラを操作する処理
		// InputManagerのMoveCamera()から呼び出される想定
		// 引数は必要に応じ追加してください
		// Lトリガー(LT)（現在でいうXボタン）を押している時の処理はメンバ変数のm_isAimingで切り替えるようにしてください

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage)
		{
			return;
		}

		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (!player)
		{
			return;
		}

		auto playerComp = player->GetComponent<Transform>();
		auto targetPos = playerComp->GetPosition();
		auto targetMode = player->GetTargetMode();

		// カメラの傾きの上限
		const float MAX_RY = 3.0f, MIN_RY = -0.5f;

		Vec3 at, eye;

		// 傾き具合
		float slope = 5.5f;

		// カメラの注視点（At）とカメラの位置（Eye）を計算
		at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
		eye = targetPos + Vec3
		(
			slope,
			14.0f,
			slope
		);
		Vec3 forward = at - eye;

		float vectorx = 0.0f, vectorz = 0.0f;

		Vec3 forwardMove = Vec3(forward.z, 0.0f, -forward.x);
	
		Vec3 originEye = eye;
		float minT = 1.0f;

		for (auto& obj : stage->GetGameObjectVec())
		{
			if (obj == player)
			{
				continue;
			}

			auto drawComp = obj->GetComponent<PNTStaticDraw>(false);
			if (drawComp)
			{
				Vec3 hitPoint;
				TRIANGLE tri;
				size_t index;

				// 注視点(at)から本来のカメラ位置(eye)までの線分と、オブジェクトのメッシュとの衝突判定
				if (drawComp->HitTestStaticMeshSegmentTrianglesToAffine(at, originEye, hitPoint, tri, index))
				{
					if (obj->FindTag(L"Ground"))
					{
						// 衝突点までの距離の割合を計算
						float dist = bsm::length(hitPoint - at);
						float tDist = bsm::length(originEye - at);
						float t = dist / tDist;

						if (t < minT)
						{
							minT = t;
						}
					}
				}
			}
		}

		// 衝突があった場合、カメラの衝突点の手前に移動
		if (minT < 1.0f)
		{
			// 壁に埋まらないように衝突点より少し注視点に寄せる
			eye = at + (originEye - at) * (minT * 0.9f);
		}

		// カメラに設定を反映
		this->SetAt(at);
		this->SetEye(eye);
	}

	void MyCamera::FocusFixedViewPointMove(const Point2D<int> mousePoint)
	{
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		m_yaw -= mousePoint.x * elapsedTime;
		m_pitch += mousePoint.y * elapsedTime;
		
		float curDist = (GetEye() - GetAt()).length();
		if (curDist < 1e-6f) return;

		float cosP = cosf(m_pitch);
		float offsetX = curDist * cosP * cosf(m_yaw);
		float offsetY = curDist * sinf(m_pitch);
		float offsetZ = curDist * cosP * sinf(m_yaw);

		SetEye(GetAt() + Vec3(offsetX, offsetY, offsetZ));
	}

	void MyCamera::CameraFixedViewPointMove(const Point2D<int> mousePoint)
	{
		Vec3 viewDir = normalize(GetAt() - GetEye());
		Vec3 viewXZ = normalize(Vec3{ viewDir.x, 0.0f, viewDir.z });

		// 右ベクトルを計算
		Vec3 worldUp = Vec3{ 0.0f, 1.0f, 0.0f };
		Vec3 right = normalize(cross(viewXZ, worldUp));

		// マウスの移動量に基づいてカメラを移動
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 deltaXZ = right * (mousePoint.x * elapsedTime * m_viewPointMoveSpeed);
		Vec3 deltaY = Vec3{ 0.0f, mousePoint.y * elapsedTime * m_viewPointMoveSpeed, 0.0f };

		SetAt(GetAt() + deltaXZ + deltaY);
		SetEye(GetEye() + deltaXZ + deltaY);
	}

	void MyCamera::WheelCameraDistance(int wheelDelta)
	{
		Vec3 at = GetAt();
		Vec3 eye = GetEye();

		Vec3 dir = at - eye;
		float curDist = dir.length();
		if (curDist < 1e-6f) return;

		Vec3 dirN = dir / curDist;

		m_distance = curDist - wheelDelta * 1.0f;
		
		Vec3 newEye = at - dirN * m_distance;

		SetEye(newEye);
	}
	
	void MyCamera::UpdatePlayMode()
	{
		// プレイヤーに追従する処理
		PlayModeMove(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	void MyCamera::UpdateMenuMode()
	{
	}

	void MyCamera::UpdateEditorMode()
	{
	}
}
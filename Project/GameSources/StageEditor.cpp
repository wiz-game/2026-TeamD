#include "stdafx.h"
#include "Project.h"
#include "StageEditor.h"

namespace basecross
{
	void StageEditor::Initialize()
	{
	}

	void StageEditor::StartEditor()
	{
		// デバッグログを作成
		m_sPtrStageLog = App::GetApp()->GetScene<Scene>()->GetActiveStage()->AddGameObject<DebugLog>(L"-EditorMenu-\n");
		m_sPtrStageLog->SetTextRect(Rect2D<float>(1070.0f, 10.0f, 200.0f, 400.0f));

		m_editorMode = ENUM_EditorMode::Position;
		m_isSelectedObj = false;
	}

	void StageEditor::EndEditor()
	{
		// デバッグログを破棄
		m_sPtrStageLog->DestroyGameObject();
		m_sPtrStageLog = nullptr;

		m_editorMode = ENUM_EditorMode::Position;
		m_isSelectedObj = false;
	}

	void StageEditor::PressedLMouseButton(const Point2D<int> mousePoint)
	{
		SerectObj(mousePoint);
	}

	void StageEditor::PressedDelete()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		auto player = dynamic_pointer_cast<Player>(m_selectedObj);
		if (player) return; // プレイヤーは削除できないように

		if (!m_selectedObj) return;

		if (!m_isSelectedObj) return;
		
		stage->RemoveGameObject<GameObject>(m_selectedObj);

		m_selectedObj = nullptr;
		m_isSelectedObj = false;
	}

	void StageEditor::SerectObj(const Point2D<int> mousePoint)
	{
		Vec3 startPos, endPos;
		Vec3 hitPoint;
		TRIANGLE retTri;
		size_t retIndex;

		// マウス位置からレイを作成
		GetMouseRey(startPos, endPos, mousePoint);

		auto objVec = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec();
		for (auto& obj : objVec)
		{
			// メッシュとレイの衝突をテスト
			auto staticDrawComp = obj->GetComponent<PNTStaticDraw>(false);
			// PNTStaticDrawのみテスト
			if (staticDrawComp)
			{
				auto isHit = staticDrawComp
					->HitTestStaticMeshSegmentTrianglesToAffine(startPos, endPos, hitPoint, retTri, retIndex);

				// 衝突していたら選択
				if (isHit)
				{
					DeselectObj();
					m_selectedObj = obj;
					m_isSelectedObj = true;
					staticDrawComp->SetDiffuse(m_selectedObjColor);
					break;
				}
			}
		}
	}

	void StageEditor::GetMouseRey(Vec3& startPos, Vec3& endPos, const Point2D<int> mousePoint)
	{
		Mat4x4 world, view, proj;
		world.affineTransformation
		(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto PtrCamera = stage->GetView()->GetTargetCamera();
		view = PtrCamera->GetViewMatrix();
		proj = PtrCamera->GetProjMatrix();

		auto viewport = stage->GetView()->GetTargetViewport();

		auto width = viewport.Width;
		auto height = viewport.Height;

		// フルスクリーンだったら
		auto winLong = GetWindowLong(App::GetApp()->GetHWnd(), GWL_STYLE);
		if (!(winLong & WS_OVERLAPPEDWINDOW))
		{
			width = (float)GetSystemMetrics(SM_CXSCREEN);
			height = (float)GetSystemMetrics(SM_CYSCREEN);
		}

		startPos = XMVector3Unproject
		(
			Vec3((float)mousePoint.x, (float)mousePoint.y, 0),
			viewport.TopLeftX,
			viewport.TopLeftY,
			width,
			height,
			viewport.MinDepth,
			viewport.MaxDepth,
			proj,
			view,
			world
		);

		endPos = XMVector3Unproject
		(
			Vec3((float)mousePoint.x, (float)mousePoint.y, 1.0),
			viewport.TopLeftX,
			viewport.TopLeftY,
			width,
			height,
			viewport.MinDepth,
			viewport.MaxDepth,
			proj,
			view,
			world
		);
	}

	void StageEditor::DeselectObj()
	{
		if (m_selectedObj)
		{
			// 選択しているオブジェクトの色を元に戻す
			auto staticDrawComp = m_selectedObj->GetComponent<PNTStaticDraw>(false);
			if (staticDrawComp)
			{
				staticDrawComp->SetDiffuse(m_defaultObjColor);
			}
			m_selectedObj = nullptr;
			m_isSelectedObj = false;
		}
	}

	void StageEditor::AddGameObject()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		switch (m_addedObj)
		{
		default:
			break;
		case ENUM_AddedObj::Ground:
			stage->AddGameObject<Ground>(Vec3(1.0f), Quat(), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case ENUM_AddedObj::Mushroom:
			break;
		case ENUM_AddedObj::Tree:
			break;
		}
	}
}
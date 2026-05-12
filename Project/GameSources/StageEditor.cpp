#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void StageEditor::RegisterEditorMenuLog(const wstring& logName, const wstring& debugLog)
	{
		if (m_sPtrEditorMenuLog)
			m_sPtrEditorMenuLog->AddDebugStr(logName, debugLog);
	}

	void StageEditor::Initialize()
	{
	}
	
	void StageEditor::WriteStageData()
	{
		auto dataPath = GetMediaDataDir() + m_stageDatasDir + m_stageDataPath;
		BinaryClear(dataPath);

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto gameObjs = stage->GetGameObjectVec();

		for (auto& gameObj : gameObjs)
		{
			if (!gameObj) continue;
			if (!gameObj->GetIsEditorSave()) continue;
			auto transform = gameObj->GetComponent<Transform>();
			if (!transform) continue;
			
			BinaryWriteAdd(dataPath, gameObj->GetID());
			BinaryWriteAdd(dataPath, transform->GetScale());
			BinaryWriteAdd(dataPath, transform->GetQuaternion());
			BinaryWriteAdd(dataPath, transform->GetPosition());
		}

		AddEditorMenuLog(L"Save", L"OK");
	}

	void StageEditor::ReadStageData(const string& stageDataPath, const shared_ptr<Stage>& stage)
	{
		m_stageDataPath = stageDataPath;
		auto dataPath = GetMediaDataDir() + m_stageDatasDir + m_stageDataPath;
		
		vector<STRUCT_ObjectParam> objParams;
		BinaryAllReadDataUnit(dataPath, objParams);

		for (const auto& objParam : objParams)
		{
			switch (objParam.GetID())
			{
			case ENUM_ObjectID::Ground:
				stage->AddGameObject<Ground>(objParam);
				break;
			case ENUM_ObjectID::Mushroom:
				stage->AddGameObject<Mushroom>(objParam);
				break;
			case ENUM_ObjectID::Tree:
				stage->AddGameObject<Tree>(objParam);
				break;
			case ENUM_ObjectID::Dirt:
				stage->AddGameObject<Dirt>(objParam);
				break;
			default:
				break;
			}
		}
	}
		
	void StageEditor::StartEditor()
	{
		// デバッグログを作成
		m_sPtrEditorMenuLog = App::GetApp()->GetScene<Scene>()->GetActiveStage()->AddGameObject<DebugLog>(L"-EditorMenu-");
		m_sPtrEditorMenuLog->SetTextRect(Rect2D<float>(1070.0f, 10.0f, 200.0f, 400.0f));

		AddEditorMenuLog(L"SelectObject", static_cast<int>(m_objID));

		m_editorMode = ENUM_EditorMode::Position;
		m_isSelectedObj = false;
	}

	void StageEditor::EndEditor()
	{
		// デバッグログを破棄
		m_sPtrEditorMenuLog->DestroyGameObject();
		m_sPtrEditorMenuLog = nullptr;
		
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

		STRUCT_ObjectParam objParam(m_objID);
		switch (m_objID)
		{
		case ENUM_ObjectID::Ground:
			stage->AddGameObject<Ground>(objParam);
			break;
		case ENUM_ObjectID::Mushroom:
			stage->AddGameObject<Mushroom>(objParam);
			break;
		case ENUM_ObjectID::Tree:
			stage->AddGameObject<Tree>(objParam);
			break;
		case ENUM_ObjectID::Dirt:
			stage->AddGameObject<Dirt>(objParam);
			break;
		default:
			break;
		}
	}

	void StageEditor::ChangeObject(bool yKeyPressed, bool uKeyPressed)
	{
		int changeObjNum = 0;

		if (yKeyPressed)
		{
			// 前のオブジェクトに切り替える
			changeObjNum = (static_cast<int>(m_objID) - 1);

			// 最小値未満になったら最後のオブジェクトに切り替える
			if (changeObjNum < 0) changeObjNum = static_cast<int>(ENUM_ObjectID::Max) - 1;
		}
		else if (uKeyPressed)
		{
			// 次のオブジェクトに切り替える
			changeObjNum = (static_cast<int>(m_objID) + 1);

			// 最大値以上になったら最初のオブジェクトに切り替える
			if (changeObjNum >= static_cast<int>(ENUM_ObjectID::Max)) changeObjNum = 0;
		}

		m_objID = static_cast<ENUM_ObjectID>(changeObjNum);

		AddEditorMenuLog(L"SelectObject", changeObjNum);
	}
}
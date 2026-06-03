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
		m_gizmos.resize(static_cast<size_t>(ENUM_Axis::Max));
		m_operationAmount = 10.0f;
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
			case ENUM_ObjectID::Stone:
				stage->AddGameObject<Stone>(objParam);
				break;
			case ENUM_ObjectID::FallenTree:
				stage->AddGameObject<FallenTree>(objParam);
				break;
			case ENUM_ObjectID::FirTree:
				stage->AddGameObject<FirTree>(objParam);
				break;
			case ENUM_ObjectID::EnemyAlpaca:
				stage->AddGameObject<EnemyAlpaca>(objParam);
				break;
			case ENUM_ObjectID::Slope:
				stage->AddGameObject<Slope>(objParam);
				break;
			case ENUM_ObjectID::Fence:
				stage->AddGameObject<Fence>(objParam);
				break;
			case ENUM_ObjectID::LongMushroom:
				stage->AddGameObject<LongMushroom>(objParam);
				break;
			case ENUM_ObjectID::Foothold:
				stage->AddGameObject<Foothold>(objParam);
				break;
			case ENUM_ObjectID::Board:
				stage->AddGameObject<Board>(objParam);
				break;
			case ENUM_ObjectID::Apple:
				stage->AddGameObject<Apple>(objParam);
				break;
			case ENUM_ObjectID::Shrub:
				stage->AddGameObject<Shrub>(objParam);
				break;
			case ENUM_ObjectID::CollisionObb:
				stage->AddGameObject<CollisionObbObject>(objParam);
				break;
			case ENUM_ObjectID::CollisionSphere:
				stage->AddGameObject<CollisionSphereObject>(objParam);
				break;
			case ENUM_ObjectID::CollisionCapsule:
				stage->AddGameObject<CollisionCapsuleObject>(objParam);
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

		AddEditorMenuLog(L"EditorMode", L"Position");
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
		
		DeselectObj();
		GizmoDrawActive(false);
	}

	void StageEditor::SelectEditorMode(const ENUM_EditorMode& editorMode)
	{
		m_editorMode = editorMode;
		switch (m_editorMode)
		{
		case ENUM_EditorMode::Position:
			AddEditorMenuLog(L"EditorMode", L"Position");
			break;
		case ENUM_EditorMode::Scale:
			AddEditorMenuLog(L"EditorMode", L"Scale");
			break;
		case ENUM_EditorMode::Quaternion:
			AddEditorMenuLog(L"EditorMode", L"Quaternion");
			break;
		default:
			break;
		}
	}

	void StageEditor::PressedLMouseButton(const Point2D<int> mousePoint)
	{
		bool isGizmoSelect = false;
		if (m_isSelectedObj)
		{
			isGizmoSelect = GizmoSelect(mousePoint);
		}

		if (!isGizmoSelect)
		{
			SelectObj(mousePoint);
		}
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

		GizmoDrawActive(false);
	}

	void StageEditor::SelectObj(const Point2D<int>& mousePoint)
	{
		Vec3 startPos, endPos;
		Vec3 hitPoint;
		TRIANGLE retTri;
		size_t retIndex;

		// マウス位置からレイを作成
		GetMouseRey(startPos, endPos, mousePoint);

		auto objVec = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec();
		shared_ptr<GameObject> risultObj = nullptr;
		
		Vec3 targetVec = Vec3();

		for (auto& obj : objVec)
		{
			// ギズモは選択できない
			if (dynamic_pointer_cast<Gizmo>(obj)) continue;

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
					auto hitTargetVec = hitPoint - startPos;
					if (hitTargetVec.length() < targetVec.length())
					{
						targetVec = hitTargetVec;
						risultObj = obj;
					}
					else if (targetVec == Vec3())
					{
						targetVec = hitTargetVec;
						risultObj = obj;
					}
				}
			}
		}

		if (risultObj == nullptr) return;

		DeselectObj();
		m_selectedObj = risultObj;
		m_isSelectedObj = true;
		risultObj->GetComponent<PNTStaticDraw>(false)->SetDiffuse(m_selectedObjColor);

		// ギズモを生成
		if (!m_gizmos[0])
		{
			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			for (int i = 0; i < static_cast<int>(ENUM_Axis::Max); i++)
			{
				auto sharedPtr = stage->AddGameObject<Gizmo>(static_cast<ENUM_Axis>(i), m_selectedObj->GetComponent<Transform>());
				m_gizmos[i] = sharedPtr;
			}
		}
		// 生成していたら位置を変更する
		else
		{
			for (const auto& gizmo : m_gizmos)
			{
				gizmo->SetUpdateActive(true);
				auto selectPos = m_selectedObj->GetComponent<Transform>()->GetPosition();
				gizmo->GetComponent<Transform>()->SetPosition(selectPos);
			}

			GizmoDrawActive(true);
		}
	}

	void StageEditor::GetMouseRey(Vec3& startPos, Vec3& endPos, const Point2D<int>& mousePoint)
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

	void StageEditor::GizmoDrawActive(const bool& isDraw)
	{
		if (!m_gizmos[0]) return;
		for (const auto& gizmo : m_gizmos)
		{
			gizmo->GetComponent<PNTStaticDraw>()->SetDrawActive(isDraw);
		}
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

	void StageEditor::DeselectGizmo()
	{
		if (m_selectedGizmo) m_selectedGizmo = nullptr;
	}

	bool StageEditor::GizmoSelect(const Point2D<int>& mousePoint)
	{
		if (!m_isSelectedObj) return false;
		if (!m_selectedObj) return false;
		if (m_selectedGizmo) return false;

		Vec3 startPos, endPos;
		Vec3 hitPoint;
		TRIANGLE retTri;
		size_t retIndex;

		// マウス位置からレイを作成
		GetMouseRey(startPos, endPos, mousePoint);

		if (!m_gizmos[0]) return false;
		for (auto& gizmo : m_gizmos)
		{
			// ギズモとの衝突をテスト
			auto staticDrawComp = gizmo->GetComponent<PNTStaticDraw>(false);
			if (!staticDrawComp) continue;

			auto isHit = staticDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startPos, endPos, hitPoint, retTri, retIndex);
			if (!isHit) continue;

			m_selectedGizmo = gizmo;
			return true;
		}

		m_selectedGizmo = nullptr;
		return false;
	}

	void StageEditor::ObjectOperation(const Point2D<int>& mousePoint)
	{
		if (!m_selectedGizmo) return;

		switch (m_editorMode)
		{
		case ENUM_EditorMode::Position:
			PositionOperation(mousePoint);
			break;
		case ENUM_EditorMode::Quaternion:
			QuaternionOperation(mousePoint);
			break;
		case ENUM_EditorMode::Scale:
			ScaleOperation(mousePoint);
			break;
		default:
			break;
		}
	}

	void StageEditor::SelectCopy()
	{
		if (!m_selectedObj) return;

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		STRUCT_ObjectParam originalObjParam
		(
			m_selectedObj->GetID(),
			m_selectedObj->GetComponent<Transform>()->GetScale(),
			m_selectedObj->GetComponent<Transform>()->GetQuaternion(),
			m_selectedObj->GetComponent<Transform>()->GetPosition()
		);

		switch (m_selectedObj->GetID())
		{
		case ENUM_ObjectID::Ground:
			stage->AddGameObject<Ground>(originalObjParam);
			break;
		case ENUM_ObjectID::Mushroom:
			stage->AddGameObject<Mushroom>(originalObjParam);
			break;
		case ENUM_ObjectID::Tree:
			stage->AddGameObject<Tree>(originalObjParam);
			break;
		case ENUM_ObjectID::Dirt:
			stage->AddGameObject<Dirt>(originalObjParam);
			break;
		case ENUM_ObjectID::Stone:
			stage->AddGameObject<Stone>(originalObjParam);
			break;
		case ENUM_ObjectID::FallenTree:
			stage->AddGameObject<FallenTree>(originalObjParam);
			break;
		case ENUM_ObjectID::FirTree:
			stage->AddGameObject<FirTree>(originalObjParam);
			break;
		case ENUM_ObjectID::EnemyAlpaca:
			stage->AddGameObject<EnemyAlpaca>(originalObjParam);
			break;
		case ENUM_ObjectID::Slope:
			stage->AddGameObject<Slope>(originalObjParam);
			break;
		case ENUM_ObjectID::Fence:
			stage->AddGameObject<Fence>(originalObjParam);
			break;
		case ENUM_ObjectID::LongMushroom:
			stage->AddGameObject<LongMushroom>(originalObjParam);
			break;
		case ENUM_ObjectID::Foothold:
			stage->AddGameObject<Foothold>(originalObjParam);
			break;
		case ENUM_ObjectID::Board:
			stage->AddGameObject<Board>(originalObjParam);
			break;
		case ENUM_ObjectID::Apple:
			stage->AddGameObject<Apple>(originalObjParam);
			break;
		case ENUM_ObjectID::Shrub:
			stage->AddGameObject<Shrub>(originalObjParam);
			break;
		case ENUM_ObjectID::CollisionObb:
			stage->AddGameObject<CollisionObbObject>(originalObjParam);
			break;
		case ENUM_ObjectID::CollisionSphere:
			stage->AddGameObject<CollisionSphereObject>(originalObjParam);
			break;
		case ENUM_ObjectID::CollisionCapsule:
			stage->AddGameObject<CollisionCapsuleObject>(originalObjParam);
			break;
		default:
			break;
		}
	}

	void StageEditor::PositionOperation(const Point2D<int>& mousePoint)
	{
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		auto selectedTrans = m_selectedObj->GetComponent<Transform>();
		auto nowPos = selectedTrans->GetPosition();

		switch (m_selectedGizmo->GetAxis())
		{
		case ENUM_Axis::X:
			nowPos.x += mousePoint.x * elapsedTime * m_operationAmount;
			nowPos.x -= mousePoint.y * elapsedTime * m_operationAmount;
			selectedTrans->SetPosition(nowPos);
			break;
		case ENUM_Axis::Y:
			selectedTrans->SetPosition(Vec3(nowPos.x, nowPos.y -= mousePoint.y * elapsedTime * m_operationAmount, nowPos.z));
			break;
		case ENUM_Axis::Z:
			nowPos.z += mousePoint.x * elapsedTime * m_operationAmount;
			nowPos.z -= mousePoint.y * elapsedTime * m_operationAmount;
			selectedTrans->SetPosition(nowPos);
			break;
		default:
			break;
		}

		// ギズモの位置も更新する
		for (auto& gizmo : m_gizmos)
		{
			gizmo->GetComponent<Transform>()->SetPosition(selectedTrans->GetPosition());
		}
	}

	void StageEditor::QuaternionOperation(const Point2D<int>& mousePoint)
	{
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		auto selectedTrans = m_selectedObj->GetComponent<Transform>();
		auto nowQuat = selectedTrans->GetQuaternion();

		switch (m_selectedGizmo->GetAxis())
		{
		case ENUM_Axis::X:
			nowQuat.x += mousePoint.x * elapsedTime * m_operationAmount;
			nowQuat.x -= mousePoint.y * elapsedTime * m_operationAmount;
			selectedTrans->SetQuaternion(nowQuat);
			break;
		case ENUM_Axis::Y:
			selectedTrans->SetQuaternion(Quat(nowQuat.x, nowQuat.y -= mousePoint.y * elapsedTime * m_operationAmount, nowQuat.z, nowQuat.w));
			break;
		case ENUM_Axis::Z:
			nowQuat.z += mousePoint.x * elapsedTime * m_operationAmount;
			nowQuat.z -= mousePoint.y * elapsedTime * m_operationAmount;
			selectedTrans->SetQuaternion(nowQuat);
			break;
		default:
			break;
		}
	}

	void StageEditor::ScaleOperation(const Point2D<int>& mousePoint)
	{
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		auto selectedTrans = m_selectedObj->GetComponent<Transform>();
		auto nowScale = selectedTrans->GetScale();

		switch (m_selectedGizmo->GetAxis())
		{
		case ENUM_Axis::X:
			nowScale.x += mousePoint.x * elapsedTime * m_operationAmount;
			nowScale.x -= mousePoint.y * elapsedTime * m_operationAmount;
			selectedTrans->SetScale(nowScale);
			break;
		case ENUM_Axis::Y:
			selectedTrans->SetScale(Vec3(nowScale.x, nowScale.y -= mousePoint.y * elapsedTime * m_operationAmount, nowScale.z));
			break;
		case ENUM_Axis::Z:
			nowScale.z += mousePoint.x * elapsedTime * m_operationAmount;
			nowScale.z -= mousePoint.y * elapsedTime * m_operationAmount;
			selectedTrans->SetScale(nowScale);
			break;
		default:
			break;
		}
	}

	void StageEditor::AddGameObject()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		shared_ptr<GameObject> newObj = nullptr;

		STRUCT_ObjectParam objParam(m_objID);
		switch (m_objID)
		{
		case ENUM_ObjectID::Ground:
			newObj = stage->AddGameObject<Ground>(objParam);
			break;
		case ENUM_ObjectID::Mushroom:
			newObj = stage->AddGameObject<Mushroom>(objParam);
			break;
		case ENUM_ObjectID::Tree:
			newObj = stage->AddGameObject<Tree>(objParam);
			break;
		case ENUM_ObjectID::Dirt:
			newObj = stage->AddGameObject<Dirt>(objParam);
			break;
		case ENUM_ObjectID::Stone:
			newObj = stage->AddGameObject<Stone>(objParam);
			break;
		case ENUM_ObjectID::FallenTree:
			newObj = stage->AddGameObject<FallenTree>(objParam);
			break;
		case ENUM_ObjectID::FirTree:
			newObj = stage->AddGameObject<FirTree>(objParam);
			break;
		case ENUM_ObjectID::EnemyAlpaca:
			newObj = stage->AddGameObject<EnemyAlpaca>(objParam);
			break;
		case ENUM_ObjectID::Slope:
			newObj = stage->AddGameObject<Slope>(objParam);
			break;
		case ENUM_ObjectID::Fence:
			newObj = stage->AddGameObject<Fence>(objParam);
			break;
		case ENUM_ObjectID::LongMushroom:
			newObj = stage->AddGameObject<LongMushroom>(objParam);
			break;
		case ENUM_ObjectID::Foothold:
			newObj = stage->AddGameObject<Foothold>(objParam);
			break;
		case ENUM_ObjectID::Board:
			newObj = stage->AddGameObject<Board>(objParam);
			break;
		case ENUM_ObjectID::Apple:
			newObj = stage->AddGameObject<Apple>(objParam);
			break;
		case ENUM_ObjectID::Shrub:
			newObj = stage->AddGameObject<Shrub>(objParam);
			break;
		case ENUM_ObjectID::CollisionObb:
			newObj = stage->AddGameObject<CollisionObbObject>(objParam);
			break;
		case ENUM_ObjectID::CollisionSphere:
			newObj = stage->AddGameObject<CollisionSphereObject>(objParam);
			break;
		case ENUM_ObjectID::CollisionCapsule:
			newObj = stage->AddGameObject<CollisionCapsuleObject>(objParam);
			break;
		default:
			break;
		}

		if (!newObj) return;

		newObj->SetUpdateActive(false);
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
/*!
@file StageEditor.h
@brief ステージエディター
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class ENUM_EditorMode
	{
		Position,
		Rotation,
		Scale,
		Terrain
	};

	class StageEditor
	{
	private:
		shared_ptr<DebugLog> m_sPtrEditorMenuLog = nullptr;

		ENUM_ObjectID m_objID = ENUM_ObjectID::Ground;
		ENUM_EditorMode m_editorMode = ENUM_EditorMode::Position;

		shared_ptr<GameObject> m_selectedObj = nullptr;

		string m_stageDatasDir = "StageDatas/";
		string m_stageDataPath = "Stage_1.bin";

		Vec4 m_selectedObjColor = Vec4(1.0f, 1.0f, 0.0f, 0.8f);
		Vec4 m_defaultObjColor = Vec4(0.6f, 0.6f, 0.6f, 1.0f);
		bool m_isSelectedObj = false;

		vector<shared_ptr<Gizmo>> m_gizmos;
	private:
		StageEditor() {}
		virtual ~StageEditor() {}

		string GetMediaDataDir() { return to_string(App::GetApp()->GetDataDirWString()); }
		void RegisterEditorMenuLog(const wstring& logName, const wstring& debugLog);
	public:
		static StageEditor& Instance()
		{
			static StageEditor instance;
			return instance;
		}

		void Initialize();

		void WriteStageData();
		void ReadStageData(const string& stageDataPath, const shared_ptr<Stage>& stage);
		
		void AddEditorMenuLog(const wstring& logName, const wstring& debugLog) { RegisterEditorMenuLog(logName, debugLog); }
		void AddEditorMenuLog(const wstring& logName, const int& debugLog)	   { RegisterEditorMenuLog(logName, to_wstring(debugLog)); }
		void AddEditorMenuLog(const wstring& logName, const float& debugLog)   { RegisterEditorMenuLog(logName, to_wstring(debugLog)); }

		void StartEditor();
		void EndEditor();

		void PressedLMouseButton(const Point2D<int> mousePoint);
		void PressedDelete();

		void SerectObj(const Point2D<int>& mousePoint);
		void GetMouseRey(Vec3& startPos, Vec3& endPos, const Point2D<int> mousePoint);
		void DeselectObj();
		void ObjectOperation(const Point2D<int>& mousePoint);
		void AddGameObject();
		void ChangeObject(bool yKeyPressed, bool uKeyPressed);
	};

}

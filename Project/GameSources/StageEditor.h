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
		shared_ptr<DebugLog> m_sPtrStageLog = nullptr;

		ENUM_EditorMode m_editorMode = ENUM_EditorMode::Position;

		shared_ptr<GameObject> m_selectedObj = nullptr;

		Vec4 m_selectedObjColor = Vec4(1.0f, 1.0f, 0.0f, 0.8f);
		Vec4 m_defaultObjColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		bool m_isSelectedObj = false;
	private:
		StageEditor() {}
		virtual ~StageEditor() {}
	public:
		static StageEditor& Instance()
		{
			static StageEditor instance;
			return instance;
		}

		void Initialize();

		void StartEditor();
		void EndEditor();

		void PressedLMouseButton(const Point2D<int> mousePoint);

		void SerectObj(const Point2D<int> mousePoint);
		void GetMouseRey(Vec3& startPos, Vec3& endPos, const Point2D<int> mousePoint);
	};

}

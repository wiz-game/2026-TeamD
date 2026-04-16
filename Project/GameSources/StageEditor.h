/*!
@file StageEditor.h
@brief ステージエディター
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class StageEditor
	{
	private:
		shared_ptr<DebugLog> m_sPtrStageLog = nullptr;

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
	};

}

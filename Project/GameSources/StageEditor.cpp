#include "stdafx.h"
#include "Project.h"

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
	}

	void StageEditor::EndEditor()
	{
		// デバッグログを破棄
		m_sPtrStageLog->DestroyGameObject();
		m_sPtrStageLog = nullptr;
	}
}
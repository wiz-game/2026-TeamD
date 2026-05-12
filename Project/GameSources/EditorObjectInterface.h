/*!
@file EditorObjectInterface.h
@brief エディター用オブジェクトのインターフェース
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class EditorObjectInterface
	{
	private:
		int m_ID = 0;
		bool m_isEditorSave = false;

	protected:
		void SetID(int id = 0) { m_ID = id; }
		void SetIsEditorSave(const bool& isEditorSave) { m_isEditorSave = isEditorSave; }
	
	public:
		EditorObjectInterface(const int& Id = 0) : m_ID(Id) { m_isEditorSave = false; }
		virtual ~EditorObjectInterface() {}

		int GetID() const { return m_ID; }
		bool GetIsEditorSave()const { return m_isEditorSave; }
	};
}
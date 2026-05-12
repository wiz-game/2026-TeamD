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
	public:
		EditorObjectInterface(const int& Id = 0) : m_ID(Id) {}
		virtual ~EditorObjectInterface() {}

		void SetID(int id = 0) { m_ID = id; }
		int GetID() const { return m_ID; }
	};
}
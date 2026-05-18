/*!
@file UIBase.h
@brief UIの基底クラス
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class UIBase : public GameObject
	{
	private:
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCTSpriteDraw> m_sPtrDraw;
		shared_ptr<Transform> m_sPtrTrans;

		wstring m_meshName;
		Vec3 m_position;
		Col4 m_color;
		float m_width;
		float m_height;
	public:
		UIBase(const shared_ptr<Stage>& stage, const wstring& meshName, const Vec3& position, const Vec2& imageSize = Vec2(1200, 800));
		virtual ~UIBase() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}

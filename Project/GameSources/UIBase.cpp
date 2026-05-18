/*!
@file UIBase.cpp
@brief UIの基底クラス
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UIBase::UIBase(const shared_ptr<Stage>& stage, const wstring& meshName, const Vec3& position, const Vec2& imageSize)
		: GameObject(stage),
		m_meshName(meshName),
		m_position(position),
		m_width(imageSize.x),
		m_height(imageSize.y),
		m_color(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}

	void UIBase::OnCreate()
	{
		m_vertices =
		{
			{ Vec3(-m_width * 0.5f,  m_height * 0.5f, 0), m_color, Vec2(0.0f, 0.0f) },
			{ Vec3( m_width * 0.5f,  m_height * 0.5f, 0), m_color, Vec2(1.0f, 0.0f) },
			{ Vec3(-m_width * 0.5f, -m_height * 0.5f, 0), m_color, Vec2(0.0f, 1.0f) },
			{ Vec3( m_width * 0.5f, -m_height * 0.5f, 0), m_color, Vec2(1.0f, 1.0f) }
		};

		m_indices = { 0,1,2, 2,1,3 };
		m_sPtrDraw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_sPtrDraw->SetTextureResource(m_meshName);
		m_sPtrDraw->SetSamplerState(SamplerState::LinearWrap);
		m_sPtrDraw->SetDiffuse(m_color);
		SetAlphaActive(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetPosition(m_position);
	}

	void UIBase::OnUpdate()
	{
	}
}
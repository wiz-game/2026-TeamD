/*!
@file UIBase.cpp
@brief UIの基底クラス
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UIBase::UIBase(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam)
		: GameObject(stage),
		m_uiParam(uiParam),
		m_width(1920.0f * m_uiParam.ImageSizeRatio),
		m_height(1080.0f * m_uiParam.ImageSizeRatio)
	{
	}

	void UIBase::OnCreate()
	{
		m_vertices =
		{
			{ Vec3(-m_width * 0.5f,  m_height * 0.5f, 0), m_uiParam.Color, Vec2(0.0f, 0.0f) },
			{ Vec3( m_width * 0.5f,  m_height * 0.5f, 0), m_uiParam.Color, Vec2(1.0f, 0.0f) },
			{ Vec3(-m_width * 0.5f, -m_height * 0.5f, 0), m_uiParam.Color, Vec2(0.0f, 1.0f) },
			{ Vec3( m_width * 0.5f, -m_height * 0.5f, 0), m_uiParam.Color, Vec2(1.0f, 1.0f) }
		};

		m_indices = { 0,1,2, 2,1,3 };
		m_sPtrDraw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_sPtrDraw->SetTextureResource(m_uiParam.MeshName);
		m_sPtrDraw->SetSamplerState(SamplerState::LinearWrap);
		m_sPtrDraw->SetDiffuse(m_uiParam.Color);
		SetAlphaActive(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetPosition(m_uiParam.Position);
		SetDrawLayer(2);
	}

	void UIBase::OnUpdate()
	{
	}

	void UIBase::SetImageSizeRatio(float ratio)
	{
		m_uiParam.ImageSizeRatio = ratio;

		m_width = 1920.0f * ratio;
		m_height = 1080.0f * ratio;

		m_vertices =
		{
			{ Vec3(-m_width * 0.5f,  m_height * 0.5f, 0), m_uiParam.Color, Vec2(0.0f, 0.0f) },
			{ Vec3(m_width * 0.5f,  m_height * 0.5f, 0), m_uiParam.Color, Vec2(1.0f, 0.0f) },
			{ Vec3(-m_width * 0.5f, -m_height * 0.5f, 0), m_uiParam.Color, Vec2(0.0f, 1.0f) },
			{ Vec3(m_width * 0.5f, -m_height * 0.5f, 0), m_uiParam.Color, Vec2(1.0f, 1.0f) }
		};

		RemoveComponent<PCTSpriteDraw>();
		m_sPtrDraw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_sPtrDraw->SetTextureResource(m_uiParam.MeshName);
		m_sPtrDraw->SetSamplerState(SamplerState::LinearWrap);
		m_sPtrDraw->SetDiffuse(m_uiParam.Color);

	}
}
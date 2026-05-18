/*!
@file UIBase.h
@brief UIの基底クラス
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	struct STRUCT_UIParam
	{
		wstring MeshName;
		Vec3 Position;
		float ImageSizeRatio;

		STRUCT_UIParam(const wstring& meshName, const Vec3& position, const float& imageSizeRatio = 1.0f)
			: MeshName(meshName), 
			Position(position), 
			ImageSizeRatio(imageSizeRatio)
		{
		}
	};

	class UIBase : public GameObject
	{
	protected:
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		shared_ptr<PCTSpriteDraw> m_sPtrDraw;
		shared_ptr<Transform> m_sPtrTrans;
		STRUCT_UIParam m_uiParam;

		Col4 m_color;
		float m_width;
		float m_height;
	public:
		UIBase(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam);
		virtual ~UIBase() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}

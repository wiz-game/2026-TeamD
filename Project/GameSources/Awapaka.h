#pragma once
#include "stdafx.h"

namespace basecross
{
	class Awapaka : public GameObject
	{
	private:
		shared_ptr<Transform> m_transform;
		shared_ptr<PNTBoneModelDraw> m_pntDraw;

		Vec3 m_position;
		Vec3 m_rotation;
		wstring m_animetion;

	public:
		Awapaka(const std::shared_ptr<Stage>& stage, const Vec3& position, const Vec3& rotation, const wstring& animetion) :
			GameObject(stage),
			m_position(position),
			m_rotation(rotation),
			m_animetion(animetion)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
	};
}
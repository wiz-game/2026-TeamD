/*!
@file MyCamera.h
@brief オリジナルカメラ
@copyright 菅野 皐、
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class MyCamera : public Camera
	{
	private:
		bool m_isAiming = false;

		float m_yaw = 0.0f;
		float m_pitch = 0.0f;
	public:
		MyCamera();
		virtual ~MyCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PlayModeMove();
		void FocusFixedViewPointMove(const Point2D<int> mousePoint);
		void CameraFixedViewPointMove(const Point2D<int> mousePoint);

		bool GetIsAiming() const { return m_isAiming; }
		void SetIsAiming(bool isAiming) { m_isAiming = isAiming; }
	private:
		void UpdatePlayMode();
		void UpdateMenuMode();
		void UpdateEditorMode();
	};
}
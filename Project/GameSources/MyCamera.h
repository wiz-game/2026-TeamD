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
		float m_distance = 1.0f;

		float m_viewPointMoveSpeed;

		// 半透明処理用の変数
		float m_fadeTime;
		float m_fadeAlpha;
		float m_notFadeAlpha = 1.0f;
		float m_fadeLength;
	public:
		MyCamera();
		virtual ~MyCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PlayModeMove
		(
			float stickrx,
			float stickry,
			float sticklx,
			float stickly
		);
		void FocusFixedViewPointMove(const Point2D<int> mousePoint);
		void CameraFixedViewPointMove(const Point2D<int> mousePoint);
		void WheelCameraDistance(int wheelDelta);
		
		bool GetIsAiming() const { return m_isAiming; }
		void SetIsAiming(bool isAiming) { m_isAiming = isAiming; }

		float GetDistance() const { return m_distance; }
	private:
		void UpdatePlayMode();
		void UpdateMenuMode();
		void UpdateEditorMode();
		void FadeStageObjectAlpha();
	};
}
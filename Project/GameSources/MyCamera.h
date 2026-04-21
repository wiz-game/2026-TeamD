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
	public:
		MyCamera();
		virtual ~MyCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PlayModeMove();

		bool GetIsAiming() const { return m_isAiming; }
		void SetIsAiming(bool isAiming) { m_isAiming = isAiming; }
	private:
		void UpdatePlayMode();
		void UpdateMenuMode();
		void UpdateEditorMode();
	};
}
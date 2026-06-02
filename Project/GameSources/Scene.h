/*!
@file Scene.h
@brief ÉVÅ[Éì
*/
#pragma once

#include "stdafx.h"

namespace basecross
{
	class Scene : public SceneBase
	{
	private:
	public:
		Scene() :SceneBase(){}
		virtual ~Scene();

		virtual void OnCreate() override;
		virtual void OnCreate2();
		virtual void OnUpdate() override;
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		void ChangeStage(const wstring& stageName) 
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), stageName);
		}
	};
}
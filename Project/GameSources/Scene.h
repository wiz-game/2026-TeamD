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
		void RegisterMediaFiles(const wstring& Directory);
		bool ShouldSkipFolder(const wstring& ParentDir, const wstring& FolderName);
		void RegisterSingleModel(const wstring& Key, const wstring& ModelDir, const wstring& ModelFile, bool AccessWrite = false);
	public:
		Scene() :SceneBase(){}
		virtual ~Scene();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	};
}
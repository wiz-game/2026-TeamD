/*!
@file Scene.cpp
@brief シーン実装
*/

#include "stdafx.h"
#include "Project.h"
#include "DXModelResource.h"
#include "JoltManager.h"
#include "Scene.h"

namespace basecross
{
	void Scene::OnCreate()
	{
		try 
		{
			App::GetApp()->SetFullScreenMode();

			JoltManager::StaticInitialize();

			// 背景色を設定
			SetClearColor(Col4(0.0f, 0.11328125f, 0.2578125, 1.0f)); // ミッドナイトブルー
			
			//リソース作成
			RegisterMediaFiles(App::GetApp()->GetDataDirWString());

			GameManager::Instance().Initialize();
			InputManager::Instance().Initialize();
			StageEditor::Instance().Initialize();

			//自分自身にイベントを送る
			//これによりゲームステージのオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) 
		{
			throw;
		}
	}

	void Scene::OnUpdate()
	{
		// InputManagerを先に更新する
		InputManager::Instance().Update();

		SceneBase::OnUpdate();
	}


	Scene::~Scene()
	{
		JoltManager::StaticTerminate();
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		if (event->m_MsgStr == L"TitleStage")
		{
			
		}
		else if (event->m_MsgStr == L"GameStage_1") 
		{
			//ゲームステージの設定
			ResetActiveStage<GameStage>();
		}
	}
	
	void Scene::RegisterMediaFiles(const wstring& Directory)
	{
		// ファイルパスを作成
		auto filePath = Directory + L"\\*";
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile(filePath.c_str(), &findFileData);

		// ハンドルのオープンチェック
		if (hFind == INVALID_HANDLE_VALUE)
		{
			throw BaseException
			(
				L"ディレクトリを開けませんでした",
				L"if (hFind == INVALID_HANDLE_VALUE)",
				L"Scene::RegisterMediaFiles"
			);
			return;
		}

		do
		{
			// 特殊なエントリはスキップ
			if (wcscmp(findFileData.cFileName, L".") == 0 || wcscmp(findFileData.cFileName, L"..") == 0) continue;

			// 現在のエントリのフルパスを作成
			wstring fullPath = Directory + L"\\" + findFileData.cFileName;

			// エントリがディレクトリの場合
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// スキップするフォルダかチェック
				if (ShouldSkipFolder(Directory, findFileData.cFileName))
				{
					continue;
				}

				// サブフォルダなので再帰
				RegisterMediaFiles(fullPath);
			}
			// エントリがファイルの場合
			else
			{
				// ファイル名を取得
				wstring fileNamePath = findFileData.cFileName;
				// pos(.)を取得
				size_t pos = fileNamePath.rfind(L'.');
				// 取得できなかった場合チェックしない
				if (pos != wstring::npos)
				{
					// pos(.)含めて拡張子を取得し一致するregister関数を呼ぶ
					wstring ext = fileNamePath.substr(pos);
					// pos(.)より前を登録名として取得
					wstring registerName = fileNamePath.substr(0, pos);
					if (ext == L".png")
					{
						App::GetApp()->RegisterTexture(registerName, fullPath);
					}
					else if (ext == L".bmf")
					{
						RegisterSingleModel(registerName, Directory + L"\\", findFileData.cFileName);
					}
					//else if (ext == L".efk")
					//{
					//	EffectManager::Instance().RegisterEffect(registerName, fullPath);
					//}
					else if (ext == L".wav")
					{
						App::GetApp()->RegisterWav(registerName, fullPath);
					}
				}
			}

		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	bool Scene::ShouldSkipFolder(const wstring& ParentDir, const wstring& FolderName)
	{
		// 直下でスキップするフォルダ名
		if (FolderName == L"Shaders")
		{
			return true;
		}

		// 親フォルダを取得
		size_t pos = ParentDir.rfind(L'\\');
		wstring parentFolder = (pos != wstring::npos) ? ParentDir.substr(pos + 1) : ParentDir;

		// Effects内のTextureフォルダはスキップする
		if (parentFolder == L"Effects" && FolderName == L"Texture")
		{
			return true;
		}

		return false;
	}

	void Scene::RegisterSingleModel(const wstring& Key, const wstring& ModelDir, const wstring& ModelFile, bool AccessWrite)
	{
		try
		{
			wstring modelData = ModelDir + ModelFile;
			BinaryReader reader(modelData);
			BinaryReader modelTypeCheckReader(modelData);
			// ヘッダーの読込
			auto pHeader = reader.ReadArray<char>(16);
			pHeader = modelTypeCheckReader.ReadArray<char>(16);
			// マルチかどうかの分岐
			string str = pHeader;
			if (str != "BDV1.0")
			{
				throw BaseException
				(
					L"データ形式がシングルモデルではないです\n",
					modelData,
					L"Scene::RegisterSingleModel()"
				);
			}
			// 頂点を読み込んでタイプによってモデル形式を変える
			BlockHeader blockHeader = modelTypeCheckReader.Read<BlockHeader>();
			// 頂点がVertexかVertexWithTangentならStaticモデルとして読み込む
			if (blockHeader.m_Type == BlockType::Vertex)
			{
				auto sPtrStaticModel = MeshResource::CreateStaticModelMeshBase(reader, ModelDir, ModelFile, AccessWrite);
				App::GetApp()->RegisterResource(Key, sPtrStaticModel);
			}
			// SkinedならBone付きモデルとして読み込む
			else if (blockHeader.m_Type == BlockType::SkinedVertex)
			{
				auto sPtrBoneModel = MeshResource::CreateBoneModelMeshBase(reader, ModelDir, ModelFile, AccessWrite);
				App::GetApp()->RegisterResource(Key, sPtrBoneModel);
			}
		}
		catch (...)
		{
			throw;
		}
	}
}
//end basecross

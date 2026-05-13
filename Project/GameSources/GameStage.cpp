/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void GameStage::CreateViewLight() 
	{
		// カメラの設定
		auto camera = ObjectFactory::Create<MyCamera>();
		camera->SetEye(Vec3(0.0f, 0.0f, -8.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		// マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); // デフォルトのライティングを指定
	}

	void GameStage::OnCreate() 
	{
		try 
		{
			auto& app = App::GetApp();

			// JoltPhysicsを初期化する
			m_jphManger.Initialize();

			//ビューとライトの作成
			CreateViewLight();
			CreatePlayer();
			AddGameObject<EnemyBase>(10);

			StageEditor::Instance().ReadStageData("Stage_1.bin", GetThis<GameStage>());

			//auto pash = App::GetApp()->GetDataDirWString() + L"StageDatas/";
			//wstring stageObjData = pash + L"Stage" + to_wstring(0) + L".csv";
			//ifstream ifsStageObjData(stageObjData);
			//if (!ifsStageObjData)
			//{
			//	throw BaseException
			//	(
			//		L"ステージオブジェクトファイルが存在しません",
			//		L"ifsStageObjData",
			//		L"GameStageBase::CreateAllStageObject(wstring StageObjData)"
			//	);
			//}
			//string rowBuf;

			//// 一行読む
			//// 1ループごとに1つオブジェクトを追加する
			//for (int row = 0; getline(ifsStageObjData, rowBuf); row++)
			//{
			//	istringstream issObjParam(rowBuf);
			//	string colBuf;

			//	// 一列読んでオブジェクトタイプをみる
			//	getline(issObjParam, colBuf, ',');
			//	// タイプに応じた生成を行う
			//	switch ((ENUM_ObjType)stoi(colBuf))
			//	{
			//	default:
			//		break;
			//	case ENUM_ObjType::Fixed:
			//		AddFixedObj(GetFixedParams(issObjParam, colBuf));
			//		break;
			//	}
			//}

			//AddGameObject<Tree>
			//(
			//	2,
			//	Vec3(1.0f, 1.0f, 1.0f),
			//	Quat(0.0f, 0.0f, 0.0f, 1.0f),
			//	Vec3(165.0f, 88.6f, 0.0f)
			//);

			//AddGameObject<Mushroom>
			//(
			//	1,
			//	Vec3(1.0f, 1.0f, 1.0f),
			//	Quat(0.0f, 0.0f, 0.0f, 1.0f),
			//	Vec3(161.0f, 88.6f, 0.0f)
			//);
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// プレイヤーの位置を中心に当たり判定の範囲を設定する
		m_collManagerPos = GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		AABB collRange = AABB(m_collManagerPos - m_collisionRange, m_collManagerPos + m_collisionRange);
		GetCollisionManager()->SetRootAABB(collRange);

		StageEditor::Instance().AddEditorMenuLog(L"FPS", 1.0f / app->GetElapsedTime() );
	}

	void GameStage::OnUpdate2()
	{
		m_jphManger.Update(1.0f / 60.0f);
	}

	void GameStage::OnDraw()
	{
	}


	void GameStage::SetStageObjBaseParams(STRUCT_StageObjBaseParams& StageObjBaseParams, istringstream& IssObjParam, string& ColBuf)
	{
		for (int col = 0; getline(IssObjParam, ColBuf, ','); col++)
		{
			// 構造体に各パラメータを格納
			switch (col)
			{
			default:
				break;
				// オブジェクト番号
			case 0:
				StageObjBaseParams.ObjNum = stoi(ColBuf);
				break;
				// サイズ情報
			case 1:
				StageObjBaseParams.Scale.setX(stof(ColBuf));
				break;
			case 2:
				StageObjBaseParams.Scale.setY(stof(ColBuf));
				break;
			case 3:
				StageObjBaseParams.Scale.setZ(stof(ColBuf));
				break;
				// 回転情報
			case 4:
				StageObjBaseParams.Quaternion.setX(stof(ColBuf));
				break;
			case 5:
				StageObjBaseParams.Quaternion.setY(stof(ColBuf));
				break;
			case 6:
				StageObjBaseParams.Quaternion.setZ(stof(ColBuf));
				break;
			case 7:
				StageObjBaseParams.Quaternion.setW(stof(ColBuf));
				break;
				// 位置情報
			case 8:
				StageObjBaseParams.Position.setX(stof(ColBuf));
				break;
			case 9:
				StageObjBaseParams.Position.setY(stof(ColBuf));
				break;
			case 10:
				StageObjBaseParams.Position.setZ(stof(ColBuf));
				return;
			}
		}
	}

	GameStage::STRUCT_FixedParams GameStage::GetFixedParams(istringstream& IssObjParam, string& ColBuf)
	{
		STRUCT_FixedParams objParams;
		// StageObjBaseで共通のパラメータを読み込む
		SetStageObjBaseParams(objParams.StageObjParams, IssObjParam, ColBuf);

		// このオブジェクト専用パラメータを読み込む
		for (int col = 0; getline(IssObjParam, ColBuf, ','); col++)
		{
			switch (col)
			{
			default:
				break;
			case 0:
				objParams.VersionDeff = stoi(ColBuf);
				break;
			}
		}

		return objParams;
	}

	void GameStage::AddFixedObj(const STRUCT_FixedParams ObjParams)
	{
		switch (ObjParams.StageObjParams.ObjNum)
		{
		default:
			break;
			// 床
		case 0:
			//AddGameObject<Ground>
			//	(
			//		0,
			//		ObjParams.StageObjParams.Scale,
			//		ObjParams.StageObjParams.Quaternion,
			//		ObjParams.StageObjParams.Position
			//	);
			break;
		}
	}

	// Playerを作成する
	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>();
		SetSharedGameObject(L"Player", player);
	}
}
//end basecross

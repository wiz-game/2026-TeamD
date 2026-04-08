/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
#include "JoltManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage
	{
		JoltManager m_jphManger; // Jolt Physics マネージャー

		void CreateViewLight(); //ビューの作成

		// エディター用///////////////////////////////////////////////
		enum ENUM_ObjType
		{
			Fixed,
		};

		struct STRUCT_StageObjBaseParams
		{
			int ObjNum = 0;
			Vec3 Scale;
			Quat Quaternion;
			Vec3 Position;
		};

		struct STRUCT_FixedParams
		{
			STRUCT_StageObjBaseParams StageObjParams;
			int VersionDeff = 0;
		};

		void SetStageObjBaseParams(STRUCT_StageObjBaseParams& StageObjBaseParams, istringstream& IssObjParam, string& ColBuf);
		STRUCT_FixedParams GetFixedParams(istringstream& IssObjParam, string& ColBuf);
		void AddFixedObj(const STRUCT_FixedParams ObjParams);
		// エディター用///////////////////////////////////////////////
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override; //初期化
		virtual void OnUpdate()override; //更新
		virtual void OnUpdate2() override;
		virtual void OnDraw()override; //描画
	};
}
//end basecross


/*!
@file StageObjectInterface.h
@brief エディター用オブジェクトのインターフェース
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class ENUM_ObjectID
	{
		Ground,
		Mushroom,
		Tree,
		Dirt,
		Stone,
		FallenTree,
		FirTree,
		EnemyAlpaca,
		Slope,
		Fence,
		LongMushroom,
		Foothold,
		Board,
		Apple,
		Shrub,
		CollisionObject,
		SoilWall,
		SoilWallLong,
		SoilWallShort,
		Max
	} ;

	struct STRUCT_ObjectParam
	{
	private:
		ENUM_ObjectID ID;
		bsm::Vec3 Scale;
		bsm::Quat Quaternion;
		bsm::Vec3 Position;

	public:
		STRUCT_ObjectParam
		(
			const ENUM_ObjectID& id = ENUM_ObjectID::Max,
			const bsm::Vec3& scale = bsm::Vec3(1.0f),
			const bsm::Quat& quaternion = bsm::Quat(),
			const bsm::Vec3& position = bsm::Vec3(0.0f)
		) :
			ID(id),
			Scale(scale),
			Quaternion(quaternion),
			Position(position)
		{}

		ENUM_ObjectID GetID() const { return ID; }
		bsm::Vec3 GetScale() const { return Scale; }
		bsm::Quat GetQuaternion() const { return Quaternion; }
		bsm::Vec3 GetPosition() const { return Position; }

	protected:
		void SetID(const ENUM_ObjectID& id) { ID = id; }
		void SetScale(const bsm::Vec3& scale) { Scale = scale; }
		void SetQuaternion(const bsm::Quat& quaternion) { Quaternion = quaternion; }
		void SetPosition(const bsm::Vec3& position) { Position = position; }
	};

	struct STRUCT_StageObjectFade
	{
	private:
		bsm::Col4 m_fadeStartDiffuse = bsm::Col4();
		float m_elapsedFadeTime = 0.0f;
		float m_targetTime = 0.0f;
		float m_targetAlpha = 1.0f;
		bool m_isFade = false;
	public:
		void Fade(const bsm::Col4& fadeStartDiffuse, const float& targetAlpha, const float& targetTime)
		{
			m_fadeStartDiffuse = fadeStartDiffuse;
			m_targetAlpha = targetAlpha;
			m_targetTime = targetTime;
			m_elapsedFadeTime = 0.0f;
			m_isFade = true;
		}

		void UpdateFade(const shared_ptr<PNTStaticDraw>& sPtrDrawComp, const float& elapsedTime)
		{
			if (!m_isFade) return;

			m_elapsedFadeTime += elapsedTime;
			if (m_elapsedFadeTime >= m_targetTime || m_targetTime <= 0.0f)
			{
				sPtrDrawComp->SetDiffuse(bsm::Col4(m_fadeStartDiffuse.x, m_fadeStartDiffuse.y, m_fadeStartDiffuse.z, m_targetAlpha));
				m_isFade = false;
			}
			else
			{
				float t = m_elapsedFadeTime / m_targetTime;
				float newVolume = m_fadeStartDiffuse.w + (m_targetAlpha - m_fadeStartDiffuse.w) * t;
				sPtrDrawComp->SetDiffuse(bsm::Col4(m_fadeStartDiffuse.x, m_fadeStartDiffuse.y, m_fadeStartDiffuse.z, newVolume));
			}
		}
	};

	class StageObjectInterface
	{
	protected:
		STRUCT_ObjectParam m_objectParam;
		bool m_isEditorSave;
		
		void SetIsEditorSave(const bool& isEditorSave) { m_isEditorSave = isEditorSave; }
	public:
		STRUCT_StageObjectFade m_stageObjectFade;

		StageObjectInterface(const STRUCT_ObjectParam& objectParams) : m_objectParam(objectParams) { m_isEditorSave = false; }
		virtual ~StageObjectInterface() {}

		STRUCT_ObjectParam GetObjectParam() const { return m_objectParam; };
		ENUM_ObjectID GetID() const { return m_objectParam.GetID(); }
		bool GetIsEditorSave() const { return m_isEditorSave; }
	};
}
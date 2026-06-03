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
		CollisionObb,
		CollisionSphere,
		CollisionCapsule,
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

	class StageObjectInterface
	{
	protected:
		STRUCT_ObjectParam m_objectParam;
		bool m_isEditorSave;

		void SetIsEditorSave(const bool& isEditorSave) { m_isEditorSave = isEditorSave; }
	public:
		StageObjectInterface(const STRUCT_ObjectParam& objectParams) : m_objectParam(objectParams) { m_isEditorSave = false; }
		virtual ~StageObjectInterface() {}

		STRUCT_ObjectParam GetObjectParam() const { return m_objectParam; };
		ENUM_ObjectID GetID() const { return m_objectParam.GetID(); }
		bool GetIsEditorSave() const { return m_isEditorSave; }
	};
}
/*!
@file SoundManager.h
@brief BGMとSEを管理するクラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class SoundManager {
	//--------------------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------------------
	private:
		weak_ptr<SoundItem> m_bgm;
		vector<weak_ptr<SoundItem>> m_seList;
		weak_ptr<XAudio2Manager> m_xAudioMana;
		
		float m_bgmVolume;
		float m_seVolume;
		float m_bgmMasterVolume;
		float m_seMasterVolume;
	public:
	//--------------------------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------------------------
	private:
		//----------------------------------------------------------------------------------
		/*
			コンストラクタ
		*/
		SoundManager();
		//----------------------------------------------------------------------------------
		/*
			デストラクタ
		*/
		virtual ~SoundManager() {}
	public:
		//----------------------------------------------------------------------------------
		/*
			インスタンス
		*/
		static SoundManager& Instance();
		//----------------------------------------------------------------------------------
		/*
			BGMを再生する
			@param BGMKey 再生するBGMのキー
			@return なし
		*/
		void PlayBGM(wstring BGMKey);
		//----------------------------------------------------------------------------------
		/*
			BGMを止める
			@param なし
			@return なし
		*/
		void StopBGM();
		//----------------------------------------------------------------------------------
		/*
			SEを再生する
			@param SEKey 再生するSEのキー
			@param IsLoop ループ再生するか
			@return seItem 再生するSEアイテムのポインタ
		*/
		weak_ptr<SoundItem> PlaySE(wstring SEKey, bool IsLoop = false);
		//----------------------------------------------------------------------------------
		/*
			SEを止める
			@param WPtrSEItem 止めるSEのシェアポインタ
			@return なし
		*/
		void StopSE(weak_ptr<SoundItem> WPtrSEItem);
		//----------------------------------------------------------------------------------
		/*
			リストからnullになっているSEアイテムを削除する
			@param なし
			@return なし
		*/
		void AutoEreseSE();
		//----------------------------------------------------------------------------------
		/*
			全てのSEを停止する
			@param なし
			@return なし
		*/
		void AllStopSE();
		//----------------------------------------------------------------------------------
		/*
			BGMの音量を設定する
			@param SetVolume 設定する音量
			@return なし
		*/
		void SetBGMVolume(float SetVolume);
		//----------------------------------------------------------------------------------
		/*
			BGMの音量を取得する
			@param なし
			@return m_bgmVolume
		*/
		float GetBGMVolume();
		//----------------------------------------------------------------------------------
		/*
			SEの音量を設定する
			@param SetVolume 設定する音量
			@return なし
		*/
		void SetSEVolume(float SetVolume);
		//----------------------------------------------------------------------------------
		/*
			SEの音量を取得する
			@param なし
			@return m_seVolume
		*/
		float GetSEVolume();
	};
}
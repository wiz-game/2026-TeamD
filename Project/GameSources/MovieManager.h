#pragma once
#include "stdafx.h"

namespace basecross
{
	class Player;
	class Dirt;
	struct CameraKeyframe
	{
		float time;
		Vec3 eye;
		Vec3 lookat;
		float armLength;
		float fov;
		enum Easing { Linear = 0, EaseInOut = 1 } easing = Linear;
	};

	struct LocalCamKey
	{
		float time;
		float distance;
		float height;
		float lookAtHeight;
		float fov;
		CameraKeyframe::Easing easing;
	};

	struct CameraTrack
	{
		vector<CameraKeyframe> keys;
		void Evaluate(float& t,Vec3& outEye,Vec3& outLookAt,float& outArmLength,float& outFov)const;
	};

	enum MovieType
	{
		None,
		Title,
		Select,
		Play,
		EnemySpotted,
		Cleaned,
		GameClear,
		GameOver,
		PlayMovieEnd,
		GameClearMovieEnd,
		GameOverMovieEnd,
		Max
	};

	using MovieCallback = function<void()>;
	class MyCamera;
	struct MovieEvent
	{
		// イベントの長さ
		float duration = 0.0f;
		// カメラトラック
		optional<CameraTrack> cameraTrack;
		// イベント完了時に呼ぶ
		MovieCallback onComplete = nullptr;
		// 割り込み許可フラグ
		bool interruptible = true;

		vector<pair<float, MovieCallback>> keyCallbacks;
		vector<char> keyCalled;

		bool onCompleteCalled = false;
		bool keysAreLocal = false;
		bool keysConverted = false;
	};
	
	class MovieManager
	{
	private:
		MovieType m_currentMovie;
		bool m_isPlayMove;
		Timer m_timer;

		vector<vector<MovieEvent>> m_eventsPerMovie;
	
		size_t m_currentEventIndex;
		float m_currentEventTime;

		shared_ptr<Player> m_player;
		shared_ptr<MyCamera> m_camera;
		shared_ptr<Stage> m_stage;
		vector<shared_ptr<Dirt>> m_dirts;
		Vec3 m_cachedPlayerPos;
		Vec3 m_cachedPlayerForward;
		bool m_initialized = false;
		vector<bool> m_createdPerType;
		// カメラ当たり判定パラメータ
		// カメラ球の半径
		float m_cameraRadius;
		// 壁から離す余裕
		float m_safetyMargin;
		// プレイヤーに最小限寄せる距離
		float m_minCamDistance;

		MovieManager();
		~MovieManager();
	public:
		static MovieManager& Instance()
		{
			static MovieManager instance;
			return instance;
		}

		void Initialize();
		void OnUpdate();

		// 制御
		void PlayMovie(const MovieType& movie);
		void StopMovie();	
		// Movieのキー定義
		void DefineEventTiming(MovieType type);
		// Movieを最初から再生させる
		void InitializeMovie(const MovieType& movie);

		// 全てのゲームオブジェクトの更新フラグを変更する
		void SetAllExceptPlayerUpdateActive(bool isActive);
		static Vec3 GetForwardXZ(const Vec3& fwd);
		pair<Vec3, Vec3> MoveCameraInFrontPlayer(float distance, float height, float lookAtHeight);
		
		// カメラ関連
		void SetCamera(shared_ptr<MyCamera>& camera) { m_camera = camera; }
		shared_ptr<MyCamera> GetStageCamera() { return m_camera; }

		// プレイヤー関連
		void SetPlayer(shared_ptr<Player>& obj) { m_player = obj; }
		shared_ptr<Player> GetPlayer() { return m_player; }

		// ステージ関連
		void SetStage(const shared_ptr<Stage>& stage) { m_stage = stage; }
		shared_ptr<Stage> GetStage() { return m_stage; }
		
		// 汚れの関連
		void SetDirt(const vector<shared_ptr<Dirt>>& dirts) { m_dirts = dirts; }
		vector<shared_ptr<Dirt>> GetDirt();

		void SetMovieType(MovieType gameMode);
		void EnterMovieType(MovieType gameMode);
		void ExitMovieType(MovieType gameMode);
	};
}

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void CameraTrack::Evaluate(float& t, Vec3& outEye, Vec3& outLookAt, float& outArmLength, float& outFov)const
	{
		if (keys.empty()) return;
		if (t <= keys.front().time)
		{
			outEye = keys.front().eye;
			outLookAt = keys.front().lookat;
			outArmLength = keys.front().armLength;
			outFov = keys.front().fov;
			return;
		}
		if (t >= keys.back().time)
		{
			outEye = keys.back().eye;
			outLookAt = keys.back().lookat;
			outArmLength = keys.back().armLength;
			outFov = keys.back().fov;
			return;
		}

		for (size_t i = 0; i + 1 < keys.size(); ++i)
		{
			const CameraKeyframe& a = keys[i];
			const CameraKeyframe& b = keys[i + 1];

			if (t >= a.time && t <= b.time)
			{
				float localTime = (t - a.time) / (b.time - a.time);
				if (a.easing == CameraKeyframe::EaseInOut)
				{
					localTime = localTime * localTime * (3.0f - 2.0f * localTime);
				}
				outEye = a.eye + (b.eye - a.eye) * localTime;
				outLookAt = a.lookat + (b.lookat - a.lookat) * localTime;
				outArmLength = a.armLength + (b.armLength - a.armLength) * localTime;
				outFov = a.fov + (b.fov - a.fov) * localTime;
				return;
			}
		}
	}

	MovieManager::MovieManager():
		m_currentMovie(MovieType::None),
		m_isPlayMove(false),
		m_timer(0.0f),
		m_currentEventIndex(0),
		m_currentEventTime(0.0f),
		m_cachedPlayerPos(Vec3(0.0f)),
		m_cachedPlayerForward(Vec3(0.0f, 0.0f, 1.0f)),
		m_cameraRadius(0.3f),
		m_safetyMargin(0.05f),
		m_minCamDistance(0.5f)
	{
	}

	MovieManager::~MovieManager()
	{
	}

	void MovieManager::Initialize()
	{
		// イベント登録
		size_t movieCount = static_cast<size_t>(MovieType::Max);
		m_eventsPerMovie.resize(movieCount);

		// Sample
		{
			MovieEvent ev;
			ev.duration = 2.0f;
			CameraTrack t;

			// GetPlayer() を使って直接カメラ位置を計算
			Vec3 eyeStart, atStart, eyeEnd, atEnd;
			if (auto player = GetPlayer())
			{
				auto playerPos = player->GetComponent<Transform>()->GetPosition();
				auto playerForward = player->GetComponent<Transform>()->GetForward();
				if (playerForward.lengthSqr() > 0.0001f) playerForward.normalize();
				Vec3 fwdXZ = GetForwardXZ(playerForward);

				eyeStart = playerPos + fwdXZ * 8.0f + Vec3(0.0f, 3.0f, 0.0f);
				atStart = playerPos + Vec3(0.0f, 1.5f, 0.0f);

				eyeEnd = playerPos + fwdXZ * 5.0f + Vec3(0.0f, 1.0f, 0.0f);
				atEnd = playerPos + Vec3(0.0f, 0.0f, 0.0f);
			}
			else
			{
				// フォールバック
				eyeStart = Vec3(15.0f, 10.0f, 0.0f);
				atStart = Vec3(0.0f, 4.0f, 0.0f);
				eyeEnd = Vec3(5.0f, 4.0f, 0.0f);
				atEnd = Vec3(0.0f, 2.0f, 0.0f);
			}

			t.keys = {
				{0.0f, eyeStart, atStart, 20.0f, 60.0f, CameraKeyframe::EaseInOut},
				{2.0f, eyeEnd,   atEnd,   15.0f, 60.0f, CameraKeyframe::Linear}
			};
			ev.cameraTrack = t;

			// 特定のキーの時にイベントを起こす
			ev.keyCallbacks = {
				//{0.5f, []() {SoundManager::Instance().PlaySE(L"GameClear_BGM"); }}
			};

			// 次のキーが無かったらする処理 
			//ev.onComplete = []() {
			//	GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
			//	};
			m_eventsPerMovie[static_cast<size_t>(MovieType::Title)] = { ev };
		}

		// GameClear
		{
			MovieEvent ev;
			ev.duration = 2.0f;
			CameraTrack t;

			// GetPlayer() を使って直接カメラ位置を計算
			Vec3 eyeStart, atStart, eyeEnd, atEnd;
			if (auto player = GetPlayer())
			{
				auto playerPos = player->GetComponent<Transform>()->GetPosition();
				auto playerForward = player->GetComponent<Transform>()->GetForward();
				if (playerForward.lengthSqr() > 0.0001f) playerForward.normalize();
				Vec3 fwdXZ = GetForwardXZ(playerForward);

				eyeStart = playerPos + fwdXZ * 8.0f + Vec3(0.0f, 3.0f, 0.0f);
				atStart = playerPos + Vec3(0.0f, 1.5f, 0.0f);

				eyeEnd = playerPos + fwdXZ * 5.0f + Vec3(0.0f, 1.0f, 0.0f);
				atEnd = playerPos + Vec3(0.0f, 0.0f, 0.0f);
			}
			else
			{
				// フォールバック
				eyeStart = Vec3(15.0f, 10.0f, 0.0f);
				atStart = Vec3(0.0f, 4.0f, 0.0f);
				eyeEnd = Vec3(5.0f, 4.0f, 0.0f);
				atEnd = Vec3(0.0f, 2.0f, 0.0f);
			}

			t.keys = {
				{0.0f, eyeStart, atStart, 20.0f, 60.0f, CameraKeyframe::EaseInOut},
				{2.0f, eyeEnd,   atEnd,   15.0f, 60.0f, CameraKeyframe::Linear}
			};
			ev.cameraTrack = t;

			// 特定のキーの時にイベントを起こす
			ev.keyCallbacks = {
				//{0.5f, []() {SoundManager::Instance().PlaySE(L"GameClear_BGM"); }}
			};

			// 次のキーが無かったらする処理 
			//ev.onComplete = []() {
			//	GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
			//	};
			m_eventsPerMovie[static_cast<size_t>(MovieType::GameClear)] = { ev };
		}

		// GameOver
		{
			MovieEvent ev;
			ev.duration = 2.0f;
			CameraTrack t;

			// GetPlayer() を使って直接カメラ位置を計算
			Vec3 eyeStart, atStart, eyeEnd, atEnd;
			if (auto player = GetPlayer())
			{
				auto playerPos = player->GetComponent<Transform>()->GetPosition();
				auto playerForward = player->GetComponent<Transform>()->GetForward();
				if (playerForward.lengthSqr() > 0.0001f) playerForward.normalize();
				Vec3 fwdXZ = GetForwardXZ(playerForward);

				eyeStart = playerPos + fwdXZ * 8.0f + Vec3(0.0f, 3.0f, 0.0f);
				atStart = playerPos + Vec3(0.0f, 1.5f, 0.0f);

				eyeEnd = playerPos + fwdXZ * 5.0f + Vec3(0.0f, 1.0f, 0.0f);
				atEnd = playerPos + Vec3(0.0f, 0.0f, 0.0f);
			}
			else
			{
				// フォールバック
				eyeStart = Vec3(15.0f, 10.0f, 0.0f);
				atStart = Vec3(0.0f, 4.0f, 0.0f);
				eyeEnd = Vec3(5.0f, 4.0f, 0.0f);
				atEnd = Vec3(0.0f, 2.0f, 0.0f);
			}

			t.keys = {
				{0.0f, eyeStart, atStart, 20.0f, 60.0f, CameraKeyframe::EaseInOut},
				{2.0f, eyeEnd,   atEnd,   15.0f, 60.0f, CameraKeyframe::Linear}
			};
			ev.cameraTrack = t;

			// 特定のキーの時にイベントを起こす
			ev.keyCallbacks = {
				//{0.5f, []() {SoundManager::Instance().PlaySE(L"GameClear_BGM"); }}
			};

			// 次のキーが無かったらする処理 
			//ev.onComplete = []() {
			//	GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
			//	};
			m_eventsPerMovie[static_cast<size_t>(MovieType::GameOver)] = { ev };
		}
	}

	void MovieManager::OnUpdate()
	{
		if (!m_isPlayMove) return;
		float dt = App::GetApp()->GetElapsedTime();
		size_t movieIdx = static_cast<size_t>(m_currentMovie);
		if (movieIdx >= m_eventsPerMovie.size())
		{
			StopMovie();
			return;
		}

		auto& events = m_eventsPerMovie[movieIdx];
		if (m_currentEventIndex >= events.size())
		{
			StopMovie();
			return;
		}

		// 毎フレームプレイヤーの位置・前方をキャッシュ更新（存在すれば）
		if (m_player)
		{
			if (auto tr = m_player->GetComponent<Transform>())
			{
				m_cachedPlayerPos = tr->GetPosition();
				m_cachedPlayerForward = tr->GetForward();
				if (m_cachedPlayerForward.lengthSqr() > 0.0001f) m_cachedPlayerForward.normalize();
			}
		}
		else
		{
			// もし m_player が未設定なら GetPlayer() を試す（外部で SetPlayer が呼ばれていないケース）
			if (auto p = GetPlayer())
			{
				if (auto tr = p->GetComponent<Transform>())
				{
					m_cachedPlayerPos = tr->GetPosition();
					m_cachedPlayerForward = tr->GetForward();
					if (m_cachedPlayerForward.lengthSqr() > 0.0001f) m_cachedPlayerForward.normalize();
				}
			}
		}

		m_currentEventTime += dt;
		MovieEvent& ev = events[m_currentEventIndex];

		// イベント内時間を渡す
		auto cam = GetStageCamera();
		if (ev.cameraTrack.has_value() && cam)
		{
			Vec3 eye, lookAt;
			float armLen = -1.0f, fov = -1.0f;
			// Evaluate は安全化されている前提（戻り値がある場合はチェックしても良い）
			ev.cameraTrack->Evaluate(m_currentEventTime, eye, lookAt, armLen, fov);
			cam->SetEye(eye.x, eye.y, eye.z);
			cam->SetAt(lookAt.x, lookAt.y, lookAt.z);
		}
		else if (cam)
		{
			// cameraTrack が無い場合はプレイヤー前方にカメラを配置するロジックを使う
			// 距離・高さ・注視高さはイベント側で指定できるようにしていないので、デフォルト値を使用
			float distance = 8.0f;
			float height = 3.0f;
			float lookAtHeight = 1.5f;

			// ev に distance/height 情報があればそれを使う（なければ上のデフォルト）
			// ここでは単純に MoveCameraInFrontPlayer を使う
			auto [eye, lookAt] = MoveCameraInFrontPlayer(distance, height, lookAtHeight);
			cam->SetEye(eye.x, eye.y, eye.z);
			cam->SetAt(lookAt.x, lookAt.y, lookAt.z);
		}

		// keyCalled の初期化判定（バグ修正）
		if (ev.keyCalled.size() < ev.keyCallbacks.size())
		{
			ev.keyCalled.assign(ev.keyCallbacks.size(), false);
		}

		// keyCallbacks の到達チェック
		for (size_t k = 0; k < ev.keyCallbacks.size(); ++k)
		{
			float keyTime = ev.keyCallbacks[k].first;
			if (!ev.keyCalled[k] && m_currentEventTime >= keyTime)
			{
				if (ev.keyCallbacks[k].second) ev.keyCallbacks[k].second();
				ev.keyCalled[k] = true;
			}
		}

		// イベント終了判定
		if (m_currentEventTime >= ev.duration)
		{
			if (!ev.onCompleteCalled && ev.onComplete)
			{
				ev.onCompleteCalled = true;
				ev.onComplete();
			}

			++m_currentEventIndex;
			m_currentEventTime = 0.0f;

			// 次イベントの初期化
			if (m_currentEventIndex < events.size())
			{
				events[m_currentEventIndex].keyCalled.assign(events[m_currentEventIndex].keyCallbacks.size(), false);
				events[m_currentEventIndex].onCompleteCalled = false;
			}
			else
			{
				StopMovie();
			}
		}
	}

	void MovieManager::InitializeMovie(const MovieType& movie)
	{
		m_currentMovie = movie;
		m_isPlayMove = true;
		m_currentEventIndex = 0;
		m_currentEventTime = 0.0f;

		auto& events = m_eventsPerMovie[static_cast<size_t>(m_currentMovie)];
		for (auto& ev : events)
		{
			ev.keyCalled.assign(ev.keyCallbacks.size(), 0);
			ev.onCompleteCalled = false;
		}
	}

	void MovieManager::FinalizeMovie()
	{
		m_isPlayMove = false;
		m_currentMovie = MovieType::None;
		m_timer.SetCounter();
	}

	void MovieManager::PlayMovie(const MovieType& movie)
	{
		SetAllGameObjectsUpdateActive(false);
		if (movie == MovieType::None) return;
		InitializeMovie(movie);
	}

	void MovieManager::StopMovie()
	{
		SetAllGameObjectsUpdateActive(true);
		m_isPlayMove = false;
		m_currentEventIndex = 0;
		m_currentEventTime = 0.0f;
	}

	void MovieManager::SetMovieEvents(MovieType movie, const vector<MovieEvent>& events)
	{
		size_t idx = static_cast<size_t>(movie);
		if (idx >= m_eventsPerMovie.size()) return;
		m_eventsPerMovie[idx] = events;
	}

	void MovieManager::PlayMovieCamera()
	{
		auto myCamera = GetStageCamera();
		if (!myCamera) return;
		auto& events = m_eventsPerMovie[static_cast<size_t>(m_currentMovie)];
		if (!events.empty() && events[0].cameraTrack.has_value() && !events[0].cameraTrack->keys.empty())
		{
			const CameraKeyframe& k = events[0].cameraTrack->keys.front();
			myCamera->SetEye(k.eye.x, k.eye.y, k.eye.z);
		}
	}

	void MovieManager::SetAllGameObjectsUpdateActive(bool isActive)
	{
		for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
		{
			if (!gameObject->FindTag(L"Player"))
			{
				gameObject->SetUpdateActive(isActive);
			}
		}
	}

	Vec3 MovieManager::GetForwardXZ(const Vec3& fwd)
	{
		Vec3 f = Vec3(fwd.x, 0.0f, fwd.z);
		float len = f.length();
		if (len <= 1e-5f)
		{
			return Vec3(0.0f, 0.0f, 1.0f);
		}
		return f / len;
	}

	pair<Vec3,Vec3> MovieManager::MoveCameraInFrontPlayer(float distance, float height, float lookAtHeight)
	{
		// 必要な前提チェック
		if (!m_player) {
			// プレイヤーがいなければデフォルトの位置を返す
			Vec3 defaultEye(0.0f, height, -distance);
			Vec3 defaultLookAt(0.0f, lookAtHeight, 0.0f);
			return { defaultEye, defaultLookAt };
		}

		// キャッシュしているプレイヤー位置・前方ベクトルを使う（または m_player から取得）
		Vec3 plPos = m_cachedPlayerPos;
		Vec3 plFwd = m_cachedPlayerForward;
		if (plFwd.length() < 1e-6f) plFwd = Vec3(0.0f, 0.0f, 1.0f);
		Vec3 fwdNorm = plFwd.normalize();

		// 目標カメラ位置（プレイヤーの前方に distance、かつ高さを加える）
		Vec3 desiredEye = plPos + fwdNorm * distance + Vec3(0.0f, height, 0.0f);

		// 注視点（プレイヤーの位置に lookAtHeight を足す）
		Vec3 lookAt = plPos + Vec3(0.0f, lookAtHeight, 0.0f);

		// ステージのオブジェクトを走査して最短ヒット点を探す
		float minDist = FLT_MAX;
		Vec3 bestHitPos = Vec3(0.0f, 0.0f, 0.0f);
		bool anyHit = false;

		shared_ptr<Stage> stage;
	
		if (!stage && m_player) {
		}

		if (!stage) {
			return { desiredEye, lookAt };
		}

		auto objVec = stage->GetGameObjectVec();
		for (auto& obj : objVec)
		{
			if (!obj) continue;
			// 必要ならタグでフィルタ（CameraObsNotDiffuse / CameraObsDiffuse 等）
			// bool isCamObs = obj->FindTag(L"CameraObsNotDiffuse") || obj->FindTag(L"CameraObsDiffuse");
			// if (!isCamObs) continue;

			auto draw = obj->GetComponent<SmBaseDraw>();
			if (!draw) continue;

			TRIANGLE tri;
			size_t triNum = 0;
			Vec3 hitPos(0.0f, 0.0f, 0.0f);

			draw->HitTestStaticMeshSegmentTriangles(plPos, desiredEye, hitPos, tri, triNum);

			// HitTest が Vec3(0,0,0) を返す仕様ならそれを無視
			if (hitPos != Vec3(0.0f, 0.0f, 0.0f))
			{
				float hitDist = (hitPos - plPos).length(); // Euclidean distance
				if (hitDist < minDist)
				{
					minDist = hitDist;
					bestHitPos = hitPos;
					anyHit = true;
				}
			}
		}

		if (!anyHit)
		{
			// 障害物なし
			return { desiredEye, lookAt };
		}

		// 障害物あり：安全距離を計算してカメラを寄せる
		float safeDist = minDist - (m_cameraRadius + m_safetyMargin);
		if (safeDist < m_minCamDistance) safeDist = m_minCamDistance;

		Vec3 safeEye = plPos + fwdNorm * safeDist + Vec3(0.0f, height, 0.0f);

		return { safeEye, lookAt };
	}

	Vec3 MovieManager::Lerp(const Vec3& a, const Vec3& b, float t)
	{
		return a + (b - a) * t;
	}

	void MovieManager::SetPlayer(shared_ptr<Player>& player)
	{
		if (player == nullptr) return;
		m_player = player;
	}

	shared_ptr<Player> MovieManager::GetPlayer()
	{
		return m_player;
	}

	shared_ptr<MyCamera> MovieManager::GetStageCamera()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return nullptr;
		auto camera = stage->GetView()->GetTargetCamera();
		auto myCamara = dynamic_pointer_cast<MyCamera>(camera);

		return myCamara;
	}
}
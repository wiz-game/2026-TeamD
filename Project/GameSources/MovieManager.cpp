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
		m_currentEventTime(0.0f)
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
			// 2秒で一回転
			ev.duration = 5.0f;
			CameraTrack t;
			Vec3 center = Vec3(0.0f, 4.0f, 0.0f);
			float radius = 15.0f;
			float height = 10.0f;
			// 多いほど滑らか
			int segments = 32; 

			t.keys.clear();
			for (int i = 0; i <= segments; ++i)
			{
				// 0..1
				float u = static_cast<float>(i) / segments;
				float time = u * ev.duration;
				// 0..2π
				float theta = u * 2.0f * 3.14159265358979323846f; 
				float x = center.x + radius * cosf(theta);
				float z = center.z + radius * sinf(theta);
				t.keys.push_back({ time, Vec3(x, height, z), center, radius, 60.0f, CameraKeyframe::Linear });
			}

			ev.cameraTrack = t;

			ev.onComplete = []()
			{
				GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
			};

			m_eventsPerMovie[static_cast<size_t>(MovieType::Title)] = { ev };
		}

		// GameClear
		{
			MovieEvent ev;
			ev.duration = 2.0f;
			CameraTrack t;

			//auto [eyeStart, atStart] = MoveCameraInFrontPlayer(8.0f, 3.0f, 1.5f);
			//auto [eyeEnd, atEnd] = MoveCameraInFrontPlayer(5.0f, 2.0f, 1.5f);

			t.keys = {
				{0.0f,Vec3(15.0f,10.0f,0.0f),Vec3(0.0f,4.0f,0.0f),20.0f,60.0f, CameraKeyframe::EaseInOut},
				{2.0f, Vec3(5.0f,4.0f,0.0f),  Vec3(0.0f,2.0f,0.0f), 15.0f, 60.0f, CameraKeyframe::Linear}
			};
			ev.cameraTrack = t;

			// 特定のキーの時にイベントを起こす
			ev.keyCallbacks = {
				{0.5f, []() {SoundManager::Instance().PlaySE(L"GameClear_BGM"); }}
				// {1.2f, []() { SoundManager::Instance().PlaySE(L"Spark_SE"); }}
			};


			// 次のキーが無かったらする処理 
			ev.onComplete = []() {
				GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
				};
			m_eventsPerMovie[static_cast<size_t>(MovieType::GameClear)] = { ev };
		}

		// GameOver
		{
			MovieEvent ev;
			ev.duration = 2.0f;
			CameraTrack t;
			t.keys = {
				{0.0f,Vec3(15.0f,10.0f,0.0f),Vec3(0.0f,4.0f,0.0f),20.0f,60.0f, CameraKeyframe::EaseInOut},
				{2.0f, Vec3(5.0f,4.0f,0.0f),  Vec3(0.0f,2.0f,0.0f), 15.0f, 60.0f, CameraKeyframe::Linear}
			};
			ev.cameraTrack = t;

			// 特定のキーの時にイベントを起こす
			ev.keyCallbacks = {
				{0.5f, []() {SoundManager::Instance().PlaySE(L"GameClear_BGM");}}
				// {1.2f, []() { SoundManager::Instance().PlaySE(L"Spark_SE"); }}
			};
			// 呼ばれフラグを初期化（必須）
			ev.keyCalled.assign(ev.keyCallbacks.size(), 0);
			
			// 次のキーが無かったらする処理 
			ev.onComplete = []() {
				GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
			};
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
		} ;

		auto& events = m_eventsPerMovie[movieIdx];
		if (m_currentEventIndex >= events.size())
		{
			StopMovie();
			return;
		}

		m_currentEventTime += dt;
		MovieEvent& ev = events[m_currentEventIndex];

		// イベント内時間を渡す
		if (ev.cameraTrack.has_value())
		{
			Vec3 eye, lookAt;
			float armLen = -1.0f, fov = -1.0f;
			ev.cameraTrack->Evaluate(m_currentEventTime, eye, lookAt, armLen, fov);
			auto cam = GetStageCamera();
			if (cam)
			{
				cam->SetEye(eye.x, eye.y, eye.z);
				cam->SetAt(lookAt.x, lookAt.y, lookAt.z);
			}
		}

		if (ev.keyCalled.size() < ev.keyCalled.size())
		{
			// keyCalled が初期化されていない場合は初期化
			ev.keyCalled.assign(ev.keyCallbacks.size(), false);
		}

		// 追加: keyCallbacks の到達チェック
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


			// 全イベント終了
			if (m_currentEventIndex < events.size())
			{
				events[m_currentEventIndex].keyCalled.assign(events[m_currentEventIndex].keyCallbacks.size(), 0);
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

		PlayMovieCamera();
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
			gameObject->SetUpdateActive(isActive);
		}
	}

	pair<Vec3,Vec3> MovieManager::MoveCameraInFrontPlayer(float distance, float height, float lookAtHeight)
	{
		Vec3 defaultEye(15.0f, 10.0f, 0.0f), defaultAt(0.0f, 4.0f, 0.0f);
		auto& app = App::GetApp();
		if (!app) return { defaultEye, defaultAt };
		auto scene = app->GetScene<Scene>();
		if (!scene) return { defaultEye, defaultAt };
		auto stage = scene->GetActiveStage();
		if (!stage) return { defaultEye, defaultAt };
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		auto playerPos = player->GetComponent<Transform>()->GetPosition();
		auto forward = player->GetComponent<Transform>()->GetForward();

		if (forward.lengthSqr() <= 0.0001f)
		{
			forward = Vec3(0.0f, 0.0f, 1.0f);
		}
		forward = forward.normalize();

		Vec3 eye = playerPos + forward * distance + Vec3(0.0f, height, 0.0f);
		Vec3 at = playerPos + Vec3(0.0f, lookAtHeight, 0.0f);
		return { eye,at };
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
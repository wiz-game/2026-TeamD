#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"

namespace basecross
{
    void EnemyBase::OnCreate()
    {

    }

    void EnemyBase::OnUpdate()
    {

    }

    // ガチ徘徊
    void EnemyBase::Move
    (
        const shared_ptr<GameObject>& gameObject,
        float speed
    )
    {
        // そのゲームオブジェクトの情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // 時間（乱数）
        float randTime = rand() % 3 + 1;
        float randRotation = rand() % 361;

        // ゼロ
        const float ZERO_TIME = 0.0f;

        // タイマースピード
        float timerSpeed = 1.0f;

        // 経過時間を取得する
        auto deltaTime = App::GetApp()->GetElapsedTime();

        // 待機時間
        if (m_isStand == true)
        {
            //m_InitialStandTime = standingTime;
            if (m_InitialStandTime > ZERO_TIME)
            {
                m_InitialStandTime -= App::GetApp()->GetElapsedTime();
            }
            else if (m_InitialStandTime <= ZERO_TIME)
            {
                m_isStand = false;
                m_isWandering = true;

                m_InitialWanderingTime = randTime;
                m_RandRotation = randRotation;
            }
        }
        // 徘徊時間
        else if (m_isWandering == true)
        {
            if (m_InitialWanderingTime > ZERO_TIME)
            {
                m_InitialWanderingTime -= App::GetApp()->GetElapsedTime();
                float rad = XMConvertToRadians(m_RandRotation);
                // 徘徊をランダムにする
                transPos.x += cosf(rad) * deltaTime * speed;
                transPos.z += sinf(rad) * deltaTime * speed;
            }
            else if (m_InitialWanderingTime <= ZERO_TIME)
            {
                m_isWandering = false;
                m_isStand = true;

                m_InitialStandTime = randTime;
            }
        }

        // 敵の位置を最終的に更新する
        transComp->SetPosition(transPos);
    }

    // 円状に動く
    void EnemyBase::CircleMove
    (
        const shared_ptr<GameObject>& gameObject,
        float radius,
        float moveSpeed
    )
    {
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // 角度の値を増やす
        m_angle += moveSpeed * App::GetApp()->GetElapsedTime();

        // 現在の位置を更新する（カメラの回転を参考にしました）
        // radius：回転する半径を決めます
        transPos.x = cosf(m_angle) * radius;
        transPos.z = sinf(m_angle) * radius;

        transComp->SetPosition(transPos.x, transPos.y, transPos.z);

        float angle = atan2f(transPos.x, transPos.z);
        transComp->SetRotation(0.0f, angle, 0.0f);
    }

    // 4つのポイントを置いてランダムに動く
    void EnemyBase::PointMove
    (
        const shared_ptr<GameObject>& gameObject,
        float speed
    )
    {
        // そのゲームオブジェクトの情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // 時間（乱数）
        float randTime = rand() % 3 + 1;
        float randRotation = rand() % 361;

        // ゼロ
        const float ZERO_TIME = 0.0f;

        // タイマースピード
        float timerSpeed = 1.0f;

        // 移動距離
        float distance = 3.0f;

        // 経過時間を取得する
        auto deltaTime = App::GetApp()->GetElapsedTime();

        // 初期位置を取得したらそれ以降は絶対に位置を更新してはイケない
        if (m_isFirstTime == true)
        {
            m_InitialPosition = transPos;
            m_isFirstTime = false;
        }

        // 待機時間
        if (m_isStand == true)
        {
            //m_InitialStandTime = standingTime;
            if (m_InitialStandTime > ZERO_TIME)
            {
                m_InitialStandTime -= deltaTime;
            }
            else if (m_InitialStandTime <= ZERO_TIME)
            {
                m_isStand = false;
                m_isWandering = true;

                m_InitialWanderingTime = randTime;
                // 一度だけ乱数を格納する
                m_NumPoint = static_cast<Point>(rand() % 4);

                // 初期値を格納する
                m_TargetPosition = m_InitialPosition;

                switch (m_NumPoint)
                {
                case Point0: // 原点
                    m_TargetPosition = m_InitialPosition;
                    break;

                case Point1: // 原点から右
                    m_TargetPosition.x += distance;
                    break;

                case Point2: // 原点から下
                    m_TargetPosition.z -= distance;
                    break;

                case Point3: // 原点から斜め
                    m_TargetPosition.x += distance;
                    m_TargetPosition.z -= distance;
                    break;

                default:
                    break;
                }
            }
        }
        // 徘徊時間
        else if (m_isWandering == true)
        {
            // ターゲット（点）までのベクトルを計算
            float diffX = m_TargetPosition.x - transPos.x;
            float diffZ = m_TargetPosition.z - transPos.z;

            // ターゲットまでの距離を計算
            float distance = sqrtf(diffX * diffX + diffZ * diffZ);

            if (distance > 0.05f)
            {
                float forward = atan2f(diffX, diffZ);
                transComp->SetRotation(0.0f, forward, 0.0f);

                transPos.x += (diffX / distance) * speed * deltaTime;
                transPos.z += (diffZ / distance) * speed * deltaTime;
            }
            // どれかの点に到達したとき
            else
            {
                // 強制的に点に重ねる
                transPos.x = m_TargetPosition.x;
                transPos.z = m_TargetPosition.z;

                // また待機時間に戻す
                m_isWandering = false;
                m_isStand = true;

                m_InitialStandTime = randTime;
            }

            // 敵の位置を最終的に更新する
            transComp->SetPosition(transPos);
        }
    }

    void EnemyBase::FunctionGravity(const shared_ptr<GameObject>& gameObject)
    {
        auto gravity = gameObject->GetComponent<Gravity>();
        if (gravity == nullptr)
        {
            return;
        }
    }

    void EnemyBase::DebugString()
    {
        auto transComp = GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        GameManager::Instance().AddDebugStr(L"Detection", m_Detection);
    }

    void EnemyBase::Died(const shared_ptr<GameObject>& gameObject)
    {
        // 自身の位置を取得する
        auto objComp = gameObject->GetComponent<Transform>();
        auto objPos = objComp->GetPosition();

        const float DIED_HP = 0.0f;
        if (m_EnemyHP <= DIED_HP)
        {
            auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
            stage->RemoveGameObject<GameObject>(GetThis<GameObject>());

            // 石鹸を出す
            auto soap = stage->AddGameObject<PowerUpSoap>();
            auto soapComp = soap->GetComponent<Transform>();
            soap->SetVecPosition(Vec3(objPos.x, objPos.y + 1, objPos.z));
            //soapComp->SetPosition(objPos.x, objPos.y + 1, objPos.z);
        }
    }

    // 索敵範囲
    void EnemyBase::DetectionRange(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        auto player = stage->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }
        auto playerPos = player->GetComponent<Transform>()->GetPosition();

        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        auto distancePos_X = playerPos.x - transPos.x;
        auto distancePos_Y = playerPos.y - transPos.y;
        auto distancePos_Z = playerPos.z - transPos.z;

        float distance = sqrt((distancePos_X * distancePos_X) + (distancePos_Y * distancePos_Y) + (distancePos_Z * distancePos_Z));

        const float radius = 10.0f;

        if (distance < radius)
        {
            m_Detection = true;
            float forward = atan2f(distancePos_X, distancePos_Z);
            transComp->SetRotation(0.0f, forward, 0.0f);
        }
        else
        {
            m_Detection = false;
        }
    }

    // ストーカー
    void EnemyBase::Stalker(const shared_ptr<GameObject>& gameObject, float stalkerSpeed)
    {
        auto player = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }

        auto playerComp = player->GetComponent<Transform>();
        auto playerPos = playerComp->GetPosition();

        auto objComp = gameObject->GetComponent<Transform>();
        auto objPos = objComp->GetPosition();

        float stalkerX = objPos.x + (playerPos.x - objPos.x) * stalkerSpeed * App::GetApp()->GetElapsedTime();
        //float stalkerY = objPos.y + (playerPos.y - objPos.y) * stalkerSpeed * App::GetApp()->GetElapsedTime();
        float stalkerZ = objPos.z + (playerPos.z - objPos.z) * stalkerSpeed * App::GetApp()->GetElapsedTime();

        objComp->SetPosition(stalkerX, 0.0f, stalkerZ);
    }

    // 泡を吐く
    void EnemyBase::DropDirt(const shared_ptr<GameObject>& gameObject)
    {
        auto objTrans = gameObject->GetComponent<Transform>();
        auto objPos = objTrans->GetPosition();
        STRUCT_ObjectParam objParm = STRUCT_ObjectParam(ENUM_ObjectID::Dirt,Vec3(1.0f),Quat(0.0f),Vec3(objPos));
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        m_LaunchofDirtCoolDown -= App::GetApp()->GetElapsedTime();
        if (m_LaunchofDirtCoolDown <= 0.0f)
        {
            auto dirt = stage->AddGameObject<Dirt>(objParm);
            m_LaunchofDirtCoolDown = m_InitCoolDown;
        }
    }

    void EnemyBase::MazeWandering(const shared_ptr<GameObject>& gameObject)
    {
        auto objTrans = gameObject->GetComponent<Transform>();
        auto objPos = objTrans->GetPosition();
        auto objRot = objTrans->GetRotation();

        float rotDistance = 90.0f;
        const float ZERO_ROTATION = 0.0f, MAX_ROTATION = 360.0f;

        float rotation = objRot.y;

        // 向いている方向に壁がある場合は90°回転する
        if (m_isRotated == true)
        {
            // 無限に回転をするので、一度だけにする
            objRot.y += 90.0f;

            // 360°を越えたら0°に戻す
            if (objRot.y >= MAX_ROTATION)
            {
                objRot.y -= MAX_ROTATION ;
            }
            objTrans->SetRotation(0.0f, objRot.y, 0.0f);
            m_isRotated = false;
        }
        else
        {
            // 向いている方向に移動させる
            float angleRot = objRot.y;
            float rotX = sinf(angleRot);
            float rotZ = -cosf(angleRot);
            objPos.x += rotX * App::GetApp()->GetElapsedTime() * 3.0f;
            objPos.z += rotZ * App::GetApp()->GetElapsedTime() * 3.0f;
            objTrans->SetPosition(objPos);
        }
    }

    void EnemyBase::aStar(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        auto player = stage->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }

        auto objTrans = gameObject->GetComponent<Transform>();
        auto objPos = objTrans->GetPosition();
        auto playerTrans = player->GetComponent<Transform>();
        auto playerPos = playerTrans->GetPosition();

        // 距離を計算するヘルパーラムダ
        auto getDistance = [](const auto& a, const auto& b) {
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            float dz = a.z - b.z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
            };

        // 直線追従（フォールバック用の移動処理）
        auto fallbackChase = [&]() {
            auto dir = playerPos - objPos;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
            if (len > 0.05f)
            {
                dir.x /= len; dir.y /= len; dir.z /= len;
                float speed = 3.0f;
                float step = speed * App::GetApp()->GetElapsedTime();
                auto newPos = objPos;
                newPos.x += dir.x * step;
                newPos.y += dir.y * step;
                newPos.z += dir.z * step;
                objTrans->SetPosition(newPos);
            }
            };

        // -------------------------------------------------------------
        // 1. StageCellMap の検索と取得
        // -------------------------------------------------------------
        shared_ptr<StageCellMap> cellMap = nullptr;
        for (auto& obj : stage->GetGameObjectVec())
        {
            cellMap = dynamic_pointer_cast<StageCellMap>(obj);
            if (cellMap != nullptr)
            {
                break;
            }
        }

        // セルマップが存在しない場合は直線追従
        if (cellMap == nullptr)
        {
            fallbackChase();
            return;
        }

        // -------------------------------------------------------------
        // 2. 経路のキャッシュと再計算タイマー
        // -------------------------------------------------------------
        static std::vector<decltype(objPos)> cachedPath;
        static float recalculateTimer = 0.0f;

        recalculateTimer -= App::GetApp()->GetElapsedTime();

        if (cachedPath.empty() || recalculateTimer <= 0.0f)
        {
            recalculateTimer = 0.4f; // 0.4秒に1回経路を再計算
            cachedPath.clear();

            // 自分とプレイヤーが現在いるセルのインデックスを取得
            CellIndex startCellIdx;
            CellIndex targetCellIdx;
            cellMap->FindNearCell(objPos, startCellIdx);
            cellMap->FindNearCell(playerPos, targetCellIdx);

            // 2D配列（セル情報）の取得
            auto& cellVec = cellMap->GetCellVec();
            int maxX = (int)cellVec.size();
            int maxZ = maxX > 0 ? (int)cellVec[0].size() : 0;

            if (maxX > 0 && maxZ > 0)
            {
                // CellIndexを1次元の整数IDに変換するラムダ
                // ※お使いの環境のCellIndexのメンバ変数名（m_X, m_Z等）に合わせてください。
                auto to1D = [maxX](const CellIndex& idx) {
                    return idx.x + idx.z * maxX;
                    };

                int startId = to1D(startCellIdx);
                int targetId = to1D(targetCellIdx);

                if (startId != targetId)
                {
                    // --- グリッド A* アルゴリズムの実行 ---
                    const float INF = (std::numeric_limits<float>::max)();
                    int numCells = maxX * maxZ;

                    std::vector<float> gScore(numCells, INF);
                    std::vector<float> fScore(numCells, INF);
                    std::vector<int> parent(numCells, -1);
                    std::vector<bool> inClosedSet(numCells, false);
                    std::vector<int> openSet;

                    gScore[startId] = 0.0f;
                    fScore[startId] = getDistance(objPos, playerPos);
                    openSet.push_back(startId);

                    bool found = false;

                    // 隣接4方向（前後左右）のオフセット
                    int dx[] = { 0, 0, -1, 1 };
                    int dz[] = { -1, 1, 0, 0 };

                    while (!openSet.empty())
                    {
                        // F値が最小のノードを選択
                        auto bestIt = openSet.begin();
                        for (auto it = openSet.begin(); it != openSet.end(); ++it) {
                            if (fScore[*it] < fScore[*bestIt]) {
                                bestIt = it;
                            }
                        }
                        int currentId = *bestIt;

                        if (currentId == targetId) {
                            found = true;
                            break;
                        }

                        openSet.erase(bestIt);
                        inClosedSet[currentId] = true;

                        int currentX = currentId % maxX;
                        int currentZ = currentId / maxX;

                        for (int i = 0; i < 4; ++i)
                        {
                            int neighborX = currentX + dx[i];
                            int neighborZ = currentZ + dz[i];

                            // 範囲外チェック
                            if (neighborX < 0 || neighborX >= maxX || neighborZ < 0 || neighborZ >= maxZ) 
                            {
                                continue;
                            }

                            int neighborId = neighborX + neighborZ * maxX;
                            if (inClosedSet[neighborId]) 
                            {
                                continue;
                            }

                            // 進入制限（コスト）チェック
                            const auto& piece = cellVec[neighborX][neighborZ];

                            // ★コストが一定以上（例：100以上）、または負の値なら壁（進入不可）として扱う
                            // お使いのプロジェクトにおける「壁」のコストに合わせて数値を変更してください。
                            if (piece.m_Cost >= 100 || piece.m_Cost < 0) 
                            {
                                continue;
                            }

                            // 隣接セルへの移動コスト（基本の1.0 + セル自体のコスト）
                            float tentativeG = gScore[currentId] + 1.0f + (float)piece.m_Cost;

                            if (tentativeG < gScore[neighborId])
                            {
                                parent[neighborId] = currentId;
                                gScore[neighborId] = tentativeG;

                                // H値（ゴールセルまでのマンハッタン距離）
                                int targetX = targetId % maxX;
                                int targetZ = targetId / maxX;
                                float h = (float)(std::abs(neighborX - targetX) + std::abs(neighborZ - targetZ));

                                fScore[neighborId] = gScore[neighborId] + h;

                                if (std::find(openSet.begin(), openSet.end(), neighborId) == openSet.end()) {
                                    openSet.push_back(neighborId);
                                }
                            }
                        }
                    }

                    // 経路の復元
                    if (found)
                    {
                        int currId = targetId;
                        while (currId != -1)
                        {
                            int x = currId % maxX;
                            int z = currId / maxX;

                            // AABBから中心座標を求める
                            CellIndex tempIdx;
                            tempIdx.x = x;
                            tempIdx.z = z;

                            AABB aabb;
                            if (cellMap->FindAABB(tempIdx, aabb))
                            {
                                // AABBの中心座標を計算（m_Min と m_Max がある場合の標準的な式です）
                                // もし aabb.GetCenter() が使える場合はそれに書き換えてください。
                                auto center = aabb.m_Min + aabb.m_Max;
                                center.x *= 0.5f;
                                center.y *= 0.5f;
                                center.z *= 0.5f;

                                // 敵が浮かないように、Y座標は現在の敵の高さに合わせます
                                center.y = objPos.y;

                                cachedPath.push_back(center);
                            }
                            currId = parent[currId];
                        }
                        std::reverse(cachedPath.begin(), cachedPath.end());
                        cachedPath.push_back(playerPos); // 最終地点に直接プレイヤーの位置を追加
                    }
                }
            }
        }

        // -------------------------------------------------------------
        // 3. 経路をたどって実際に移動する処理
        // -------------------------------------------------------------
        decltype(objPos) finalTargetPos = playerPos;

        if (!cachedPath.empty())
        {
            finalTargetPos = cachedPath[0];

            float distToNode = getDistance(objPos, finalTargetPos);
            if (distToNode < 0.3f)
            {
                cachedPath.erase(cachedPath.begin());
                if (!cachedPath.empty()) {
                    finalTargetPos = cachedPath[0];
                }
                else {
                    finalTargetPos = playerPos;
                }
            }
        }

        // 目標座標への移動を適用
        auto dir = finalTargetPos - objPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

        if (len > 0.05f)
        {
            dir.x /= len;
            dir.y /= len;
            dir.z /= len;

            float speed = 3.0f;
            float step = speed * App::GetApp()->GetElapsedTime();

            auto newPos = objPos;
            newPos.x += dir.x * step;
            newPos.y += dir.y * step;
            newPos.z += dir.z * step;

            objTrans->SetPosition(newPos);
        }
    }

    void EnemyBase::OnCollisionEnter(shared_ptr<GameObject>& Other)
    {
        if (Other->FindTag(L"Ground"))
        {
            m_isRotated = true;
        }

        auto bubble = dynamic_pointer_cast<Bubble>(Other);
        if (bubble == nullptr)
        {
            return;
        }

        auto force = bubble->GetBubbleHP();
        if (Other->FindTag(L"Bubble"))
        {
            m_EnemyHP -= force;
        }
    }

    void EnemyBase::OnCollisionExecute(shared_ptr<GameObject>& Other)
    {

    }

    void EnemyBase::OnCollisionExit(shared_ptr<GameObject>& Other)
    {

    }
}
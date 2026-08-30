#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"

namespace basecross
{
    EnemyBase::EnemyBase(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam) :
        GameObject(stage, objectParam),
        m_InitialWanderingTime(0.0f),
        m_RandRotation(0.0f),
        m_isWandering(false),
        m_InitialStandTime(0.0f),
        m_isStand(true),
        m_TargetPosition(),
        m_isFirstTime(true),
        m_Detection(true),
        m_InitCoolDown(3.0f),
        m_isRotated(false),
        m_rotationSpeed(2.0f),
        m_rotY(0.0f),
        m_canGoLeft(true),
        m_canGoRight(true),
        m_canGoForward(true),
        m_targetRotY(0.0f),
        m_rayRange(1.6f),
        m_rayDistanceRange(2.4f),
        m_ExpectRange(3.0f),

        // 移動速度
        m_Speed(2.5),
        m_DefaultSpeed(2.5),
        
        m_rotToHeadLeap(0.5f),

        // 壁を回避中かどうか
        m_isAvoiding(false),
        m_avoidTimer(0.0f),
        m_InitavoidTimer(0.5f),

        m_targetVec(),

        // 壁に触れたかどうか
        m_isHitWall(false),

        m_isContactofBubble(false),
        m_StunTime(0.0f),
        m_InitStunTime(3.0f),

        m_stuckTimer(0.0f)
    {}

    void EnemyBase::OnCreate()
    {

    }

    void EnemyBase::OnUpdate()
    {

    }

    void EnemyBase::Stun()
    {
        const float TIMER_ZERO = 0.0f, TIMER_SPEED = 1.0f;

        // バブルに触れたとき
        if (m_isContactofBubble)
        {
            // スタンのタイマーが0.0秒以下のとき
            if (m_StunTime <= TIMER_ZERO)
            {
                m_StunTime = m_InitStunTime;
            }

            // 移動できなくする
            m_Speed = 0.0f;
            auto elapsedTime = App::GetApp()->GetElapsedTime();
            m_StunTime -= elapsedTime * TIMER_SPEED;

            if (m_StunTime <= TIMER_ZERO)
            {
                m_Speed = m_DefaultSpeed;
                m_isContactofBubble = false;
                m_StunTime = TIMER_ZERO;
            }
        }
        else
        {
            m_Speed = m_DefaultSpeed;
        }

    }

    // 索敵範囲
    void EnemyBase::DetectionRange(const shared_ptr<GameObject>& gameObject)
    {
        // 現在のステージの情報を取得する
        auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
        auto playerPos = player->GetComponent<Transform>()->GetPosition();

        // 引数の情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();

        // プレイヤーと自身の位置を計算する
        auto distancePos = playerPos - transPos;

        float distance = sqrt((distancePos.x * distancePos.x) + (distancePos.y * distancePos.y) + (distancePos.z * distancePos.z));

        float angleX = distancePos.x / distance;
        float angleY = distancePos.y / distance;
        float angleZ = distancePos.z / distance;

        auto forward = transComp->GetForward();
        forward.normalize();

        auto dot = (forward.x * angleX) + (forward.y * angleY) + (forward.z * angleZ);

        const float radius = 7.0f, rangeHeight = 0.3f;

        Vec3 startReyPos = Vec3(transPos.x, rangeHeight, transPos.z);

        Vec3 endPSp(playerPos.x, transPos.y + rangeHeight, playerPos.z);

        float dotAngle = XMConvertToRadians(50.0f);

        // プレイヤーが視界内に入っていたら
        if (distance < radius && dot >= cosf(dotAngle))
        {
            m_Detection = true;
            // 全てのオブジェクトを探す
            for (auto& obj : GetStage()->GetGameObjectVec())
            {
                // Wallのタグを持っているオブジェクトのみ判定をする
                if (!obj->FindTag(L"Wall"))
                {
                    continue;
                }

                // コンポーネントが静的だったら
                auto drawComp = obj->GetComponent<PNTStaticDraw>(false);
                if (drawComp)
                {
                    Vec3 hitPoint;  // レイがメッシュと衝突した座標
                    TRIANGLE tri;   // メッシュ
                    size_t index;   //
                    // 対象のオブジェクトと自分のレイの間に障害物があったら視界に入っていない
                    if (drawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endPSp, hitPoint, tri, index))
                    {
                        m_Detection = false;
                        break;
                    }
                }
            }
        }
        else
        {
            m_Detection = false;
        }
    }

    // 追跡AI
    void EnemyBase::Tracking(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        // ステージ情報を取得する
        auto player = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }

        auto playerComp = player->GetComponent<Transform>();
        // プレイヤーの位置を取得する
        auto playerPos = playerComp->GetPosition();

        // 引数の情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // プレイヤーの位置から自分の位置を引いて正規化する
        Vec3 targetVec = playerPos - transPos;
        targetVec.y = 0.0f;
        if (targetVec.length() > 0.0001f)
        {
            targetVec.normalize();
        }
        else
        {
            targetVec = transComp->GetForward();
        }

        float distanceRange = m_rayRange;
        float distanceRangeFormat = 4.0f;
        float rangeHeight = 0.3f;

        Vec3 startReyPos = Vec3(transPos.x, transPos.y + rangeHeight, transPos.z);

        // 正面
        float endX = startReyPos.x + (targetVec.x * distanceRange);
        float endZ = startReyPos.z + (targetVec.z * distanceRange);
        Vec3 endSp(endX, transPos.y + rangeHeight, endZ);

        float currentAngle = atan2f(targetVec.x, targetVec.z);
        float angleR = currentAngle + XMConvertToRadians(45.0f);
        float angleL = currentAngle - XMConvertToRadians(45.0f);

        // targetVecに寄せた計算をする-------------------------------------------------------------
        Vec3 dirF = targetVec;
        // 右
        Vec3 dirR(sinf(angleR), 0.0f, cosf(angleR));
        dirR.normalize();
        float endRX = startReyPos.x + (dirR.x * distanceRange);
        float endRZ = startReyPos.z + (dirR.z * distanceRange);
        Vec3 endRSp(endRX, transPos.y + rangeHeight, endRZ);

        // 左
        Vec3 dirL(sinf(angleL), 0.0f, cosf(angleL));
        dirL.normalize();
        float endLX = startReyPos.x + (dirL.x * distanceRange);
        float endLZ = startReyPos.z + (dirL.z * distanceRange);
        Vec3 endLSp(endLX, transPos.y + rangeHeight, endLZ);
        //------------------------------------------------------------------------------------------

        m_canGoForward = true;
        m_canGoLeft = true;
        m_canGoRight = true;

        for (auto& obj : stage->GetGameObjectVec())
        {
            if (obj == gameObject)
            {
                continue;
            }

            auto modelObj = obj->GetComponent<PNTStaticDraw>(false);
            if (modelObj)
            {
                if (obj->FindTag(L"Wall"))
                {
                    if (IsWallHit(modelObj, startReyPos, endSp, transPos))
                    {
                        m_canGoForward = false;
                    }

                    if (IsWallHit(modelObj, startReyPos, endLSp, transPos))
                    {
                        m_canGoLeft = false;
                    }

                    if (IsWallHit(modelObj, startReyPos, endRSp, transPos))
                    {
                        m_canGoRight = false;
                    }
                }
            }
        }

        float timerZero = 0.0f;
        float delta = App::GetApp()->GetElapsedTime();
        if (!m_isAvoiding)
        {
            if (!m_canGoForward)
            {
                m_isAvoiding = true;
                m_avoidTimer = m_InitavoidTimer;

                m_NumRandRot = static_cast<ENUM_RANDOMROT>(rand() % RandomRotNum);
            }
        }

        if (m_isAvoiding)
        {
            m_avoidTimer -= delta;
            if (m_avoidTimer <= timerZero)
            {
                m_isAvoiding = false;
            }
            else
            {
                if (m_canGoLeft && m_canGoRight)
                {

                    switch (m_NumRandRot)
                    {
                    case RandomRotL:
                    default:
                        targetVec = dirL;
                        break;

                    case RandomRotR:
                        targetVec = dirR;
                        break;
                    }
                }

                // 回避方向の決定
                else if (m_canGoLeft && !m_canGoRight)
                {
                    targetVec = dirL;
                }
                else if (m_canGoRight && !m_canGoLeft)
                {
                    targetVec = dirR;
                }
                else
                {
                    targetVec = -dirF;
                }
            }
        }


        transPos += targetVec * m_Speed * App::GetApp()->GetElapsedTime();
        transComp->SetPosition(transPos);
        GetBehavior<UtilBehavior>()->RotToHead(targetVec, m_rotToHeadLeap);
    }

    void EnemyBase::MazeWandering(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // forward_Xとforward_Zを格納するための変数
        float rot_X = 0.0f, rot_Z = 0.0f;
        float startReyPos_Y = 0.3f;

        if (m_isRotated == false && m_canGoForward == false)
        {
            // 左右どちらも行けるならどちらかをランダムに決定して移動
            if ((m_canGoLeft == true && m_canGoRight == true) && m_isRotated == false)
            {
                m_NumRandRot = static_cast<ENUM_RANDOMROT>(rand() % RandomRotNum);
                switch (m_NumRandRot)
                {
                case RandomRotL:
                    //m_rotY -= XMConvertToRadians(90.0f);
                    m_targetRotY = m_rotY - XMConvertToRadians(90.0f);
                    break;

                case RandomRotR:
                    //m_rotY += XMConvertToRadians(90.0f);
                    m_targetRotY = m_rotY + XMConvertToRadians(90.0f);
                    break;

                default:
                    break;
                }
                m_isRotated = true;
            }
            // 片方のみならその方向に決定して移動
            // 左回転
            else if (m_canGoLeft == true)
            {
                //m_rotY -= XMConvertToRadians(90.0f);
                m_targetRotY = m_rotY - XMConvertToRadians(90.0f);
                m_isRotated = true;
            }
            // 右回転
            else if (m_canGoRight == true)
            {
                //m_rotY += XMConvertToRadians(90.0f);
                m_targetRotY = m_rotY + XMConvertToRadians(90.0f);
                m_isRotated = true;
            }
            // どちらもダメなら自分の真後ろに決定して移動
            else
            {
                //m_rotY += XMConvertToRadians(180.0f);
                m_targetRotY = m_rotY + XMConvertToRadians(180.0f);
                m_isRotated = true;
            }
        }

        if (m_isRotated == false)
        {
            const float STUCK_RANGE = 0.85f;
            const float TIMER_SPEED = 1.0f,STUCK_TIMERLIMIT = 3.0f,RESET_TIMER = 0.0f;
            float delta = App::GetApp()->GetElapsedTime();
            if (m_stuckTimer == RESET_TIMER)
            {
                m_lastPosition = transPos;
            }

            if ((transPos - m_lastPosition).length() < STUCK_RANGE)
            {
                m_stuckTimer += TIMER_SPEED * delta;
                if (m_stuckTimer >= STUCK_TIMERLIMIT)
                {
                    m_stuckTimer = RESET_TIMER;
                    m_targetRotY = m_rotY + XMConvertToRadians(180.0f);
                    m_isRotated = true;
                    m_lastPosition = transPos;
                }
            }
            else
            {
                m_stuckTimer = RESET_TIMER;
            }

        }

        

        const float LIMIT_ANGLE = 360.0f, ZERO_ANGLE = 0.0f;
        if (m_isRotated)
        {
            // 現在の角度より目標の角度が大きい場合
            if (m_rotY < m_targetRotY)
            {
                m_rotY += App::GetApp()->GetElapsedTime() * m_rotationSpeed;

                // 目標の角度を超過してしまった場合
                if (m_rotY >= m_targetRotY)
                {
                    m_rotY = m_targetRotY;
                    m_isRotated = false;
                    m_canGoForward = true;
                    m_canGoLeft = true;
                    m_canGoRight = true;
                }
            }

            // 目標の角度より現在の角度が大きい場合
            if (m_rotY > m_targetRotY)
            {
                m_rotY -= App::GetApp()->GetElapsedTime() * m_rotationSpeed;

                // 目標の角度より小さくなってしまった場合
                if (m_rotY <= m_targetRotY)
                {
                    m_rotY = m_targetRotY;
                    m_isRotated = false;
                    m_canGoForward = true;
                    m_canGoLeft = true;
                    m_canGoRight = true;
                }
            }

            // 360°以上になったら0°に戻す
            if (m_rotY >= XMConvertToRadians(LIMIT_ANGLE))
            {
                m_rotY = XMConvertToRadians(ZERO_ANGLE);
            }

            if (m_rotY <= XMConvertToRadians(-LIMIT_ANGLE))
            {
                m_rotY = XMConvertToRadians(ZERO_ANGLE);
            }

            transComp->SetRotation(rot_X, m_rotY, rot_Z);
        }
        else
        {
            transPos.x = transPos.x + (sinf(m_rotY) * m_Speed * App::GetApp()->GetElapsedTime());
            transPos.z = transPos.z + (cosf(m_rotY) * m_Speed * App::GetApp()->GetElapsedTime());
            transComp->SetPosition(transPos);
        }

        transComp->SetRotation(rot_X, m_rotY, rot_Z);
    }

    // 順番に動く
    void EnemyBase::PointMove(const shared_ptr<GameObject>& gameObject, float speed)
    {
        // そのゲームオブジェクトの情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();

        // 時間（乱数）
        float randTime = (float)(rand() % 3 + 1);
        float randRotation = (float)(rand() % 361);

        // ゼロ
        const float ZERO_TIME = 0.0f;

        // タイマースピード
        float timerSpeed = 1.0f;

        // 移動距離
        float distance = 3.0f, distanceArrow = 0.05f;

        // 次のポイントに行くための変数
        int pointNext = 1;

        // 経過時間を取得する
        auto deltaTime = App::GetApp()->GetElapsedTime();

        // 初期位置を取得したらそれ以降は絶対に位置を更新してはイケない
        if (m_isFirstTime == true)
        {
            // 位置を初期化する
            m_InitialPosition = transPos;
            m_isFirstTime = false;
        }

        // 待機時間
        if (m_isStand == true)
        {
            if (m_InitialStandTime > ZERO_TIME)
            {
                m_InitialStandTime -= deltaTime;
            }
            else if (m_InitialStandTime <= ZERO_TIME)
            {
                m_isStand = false;
                m_isWandering = true;

                m_InitialWanderingTime = randTime;

                // 
                m_NumPoint = static_cast<Point>((static_cast<int>(m_NumPoint) + pointNext) % m_PointPositions.size());

                // 初期値を格納する
                m_TargetPosition = m_InitialPosition;

                switch (m_NumPoint)
                {
                case Point0: // 原点
                    m_TargetPosition = m_InitialPosition;
                    break;

                case Point1:
                    m_TargetPosition = m_PointPositions[1];
                    break;

                case Point2:
                    m_TargetPosition = m_PointPositions[2];
                    break;

                case Point3:
                    m_TargetPosition = m_PointPositions[3];
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

            if (distance > distanceArrow)
            {
                float forward = atan2f(diffX, diffZ);
                transComp->SetRotation(transRot.x, forward, transRot.z);

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

    Vec3 EnemyBase::CalculateEndPointRayAngle(const Vec3& startPos, const float& forwardAngle, const float& angle, float rayRange)
    {
        float targetAngle = forwardAngle + XMConvertToRadians(angle);

        Vec3 dir(sinf(targetAngle), 0.0f, cosf(targetAngle));
        dir.normalize();

        float endPosX = startPos.x + (dir.x * rayRange);
        float endPosZ = startPos.z + (dir.z * rayRange);

        return Vec3(endPosX, startPos.y, endPosZ);
    }

    void EnemyBase::PointPosition(int number, const Vec3& pos)
    {
        if (number >= m_PointPositions.size())
        {
            m_PointPositions.resize(number + 1);
        }
        m_PointPositions[number] = pos;
    }

    const bool EnemyBase::IsWallHit(const shared_ptr<PNTStaticDraw>& staticDraw, const Vec3& startPos, const Vec3& endPos, const Vec3& basePos)
    {
        Vec3 hitPoint;
        TRIANGLE tri;
        size_t index;

        if (staticDraw->HitTestStaticMeshSegmentTrianglesToAffine(startPos, endPos, hitPoint, tri, index))
        {
            Vec3 dir(hitPoint.x - basePos.x, 0.0f, hitPoint.z - basePos.z);
            return dir.length() >= 0.1f && dir.length() <= m_rayRange;
        }
        return false;
    }

}
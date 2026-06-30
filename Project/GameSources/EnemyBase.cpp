#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"

namespace basecross
{
    EnemyBase::EnemyBase(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam) :
        GameObject(stage, objectParam),
        m_InitialWanderingTime(0.0f),
        m_RandRotation(0.0f),
        m_WanderSpeed(1.0f),
        m_isWandering(false),
        m_InitialStandTime(0.0f),
        m_isStand(true),
        m_angle(0.0f),
        m_TargetPosition(),
        m_isFirstTime(true),
        m_Detection(true),
        m_LaunchofDirtCoolDown(0.0f),
        m_InitCoolDown(3.0f),
        m_isRotated(false),
        m_rotationSpeed(2.0f),
        m_rotY(0.0f),
        m_canGoLeft(true),
        m_canGoRight(true),
        m_canGoForward(true),
        m_targetRotY(0.0f),
        m_rayRange(1.6f),

        // 移動速度
        m_Speed(1.4f),
        m_rotToHeadLeap(0.5f),

        // 座標を格納するための変数
        m_Point1Position(),
        m_Point2Position(),
        m_Point3Position(),

        // 壁を回避中かどうか
        m_isAvoiding(false),
        m_avoidTimer(0.0f),
        m_InitavoidTimer(0.5f),

        m_targetVec()
    {}

    void EnemyBase::OnCreate()
    {

    }

    void EnemyBase::OnUpdate()
    {

    }

    void EnemyBase::DebugString()
    {
        auto transComp = GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();

        GameManager::Instance().AddDebugStr(L"Detection", m_Detection);
        GameManager::Instance().AddDebugStr(L"isRotated", m_isRotated);
        GameManager::Instance().AddDebugStr(L"EnemyRotY", transRot.y);
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
        }
    }

    // 索敵範囲
    void EnemyBase::DetectionRange(const shared_ptr<GameObject>& gameObject)
    {
        // 現在のステージの情報を取得する
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        // プレイヤーの情報を取得する
        auto player = stage->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }
        auto playerPos = player->GetComponent<Transform>()->GetPosition();

        // 引数の情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();

        // プレイヤーと自身の位置を計算する
        auto distancePos_X = playerPos.x - transPos.x;
        auto distancePos_Y = playerPos.y - transPos.y;
        auto distancePos_Z = playerPos.z - transPos.z;

        float distance = sqrt((distancePos_X * distancePos_X) + (distancePos_Y * distancePos_Y) + (distancePos_Z * distancePos_Z));

        float angleX = distancePos_X / distance;
        float angleY = distancePos_Y / distance;
        float angleZ = distancePos_Z / distance;

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
            for (auto obj : stage->GetGameObjectVec())
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
    void EnemyBase::Tracking(const shared_ptr<GameObject>& gameObject, float speed)
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
        targetVec.normalize();

        if (m_isAvoiding)
        {
            targetVec = transComp->GetForward();
            targetVec.y = 0.0f;
            targetVec.normalize();
        }
       
        float distanceRange = m_rayRange; // ここは最終的にメンバ変数にする
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
                Vec3 hitPoint;
                TRIANGLE tri;
                size_t index;
                float dirX = 0.0f, dirZ = 0.0f, len = 0.0f;

                if (obj->FindTag(L"Wall"))
                {
                    if (modelObj->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endSp, hitPoint, tri, index))
                    {
                        // 正面
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, 0.0f, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoForward = false;
                        }
                    }

                    if (modelObj->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endLSp, hitPoint, tri, index))
                    {
                        // 左
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, 0.0f, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoLeft = false;
                        }
                    }

                    if (modelObj->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endRSp, hitPoint, tri, index))
                    {
                        // 右
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, 0.0f, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoRight = false;
                        }
                    }
                }
            }
        }
        
        float timerZero = 0.0f;
        float delta = App::GetApp()->GetElapsedTime();
        if (!m_isAvoiding)
        {
            if (m_canGoForward == false)
            {
                m_isAvoiding = true;
                m_avoidTimer = m_InitavoidTimer;

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
                if (m_canGoForward)
                {
                    targetVec = transComp->GetForward();
                }
                // 回避方向の決定
                else if (m_canGoLeft)
                {
                    targetVec = dirL;
                    //targetVec.normalize();
                }
                else if (m_canGoRight)
                {
                    targetVec = dirR;
                    //targetVec.normalize();
                }
            }
        }

        transPos.x += targetVec.x * speed * App::GetApp()->GetElapsedTime();
        transPos.z += targetVec.z * speed * App::GetApp()->GetElapsedTime();
        transComp->SetPosition(transPos);
        GetBehavior<UtilBehavior>()->RotToHead(targetVec, m_rotToHeadLeap);

        // デバッグ文字
        GameManager::Instance().AddDebugStr(L"m_isAvoiding", m_isAvoiding);
        GameManager::Instance().AddDebugStr(L"m_avoidTimer", m_avoidTimer);
    }

    void EnemyBase::MazeWandering(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();
        auto drawComp = gameObject->GetComponent<PNTBoneModelDraw>();

        float forward_X = 0.0f, forward_Z = 0.0f;
        // forward_Xとforward_Zを格納するための変数
        float rot_X = 0.0f, rot_Z = 0.0f;
        float startReyPos_Y = 0.3f;

        m_canGoLeft = true;
        m_canGoRight = true;
        m_canGoForward = true;

        Vec3 startReyPos = Vec3(transPos.x, transPos.y + startReyPos_Y, transPos.z);

        // レイ
        // 正面
        float targetAngleFloat = m_rotY;
        float forward_FX = transPos.x + (sinf(targetAngleFloat) * m_rayRange);
        float forward_FZ = transPos.z + (cosf(targetAngleFloat) * m_rayRange);
        Vec3 endFSp(forward_FX, transPos.y, forward_FZ);

        // 右
        float targetAngleRight = m_rotY + XMConvertToRadians(90.0f);
        float forward_RX = transPos.x + ((sinf(targetAngleRight))) * m_rayRange;
        float forward_RZ = transPos.z + ((cosf(targetAngleRight))) * m_rayRange;
        Vec3 endRSp(forward_RX, transPos.y, forward_RZ);

        // 左
        float targetAngleLeft = m_rotY - XMConvertToRadians(90.0f);
        float forward_LX = transPos.x + ((sinf(targetAngleLeft))) * m_rayRange;
        float forward_LZ = transPos.z + ((cosf(targetAngleLeft))) * m_rayRange;
        Vec3 endLSp(forward_LX, transPos.y, forward_LZ);
        //

        float minT = 1.0f;
        // 全てのゲームオブジェクトを探す
        for (auto& obj : stage->GetGameObjectVec())
        {
            // 自身が引数のgameObjectと等しければ次にいく
            if (obj == gameObject)
            {
                continue;
            }

            auto objDrawComp = obj->GetComponent<PNTStaticDraw>(false);
            if (objDrawComp)
            {
                Vec3 hitPoint;
                TRIANGLE tri;
                size_t index;
                if (obj->FindTag(L"Wall") && m_isRotated == false)
                {
                    float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f, len = 0.0f;

                    if (objDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endLSp, hitPoint, tri, index))
                    {
                        // 左
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, dirY, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoLeft = false;
                        }
                    }

                    if (objDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endRSp, hitPoint, tri, index))
                    {
                        // 右
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, dirY, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoRight = false;
                        }
                    }

                    if (objDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endFSp, hitPoint, tri, index))
                    {
                        // 正面
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, dirY, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoForward = false;
                        }
                    }
                }
            }
        }

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

        const float LIMIT_ANGLE = 360.0f,ZERO_ANGLE = 0.0f;
        if (m_isRotated)
        {
            // 現在の角度より目標の角度が大きい場合
            if(m_rotY < m_targetRotY)
            {
                m_rotY += App::GetApp()->GetElapsedTime() * m_rotationSpeed;

                // 目標の角度を超過してしまった場合
                if (m_rotY >= m_targetRotY)
                {
                    m_rotY = m_targetRotY;
                    m_isRotated = false;
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
        auto transRot = transComp->GetRotation();

        // 時間（乱数）
        float randTime = (float)(rand() % 3 + 1);
        float randRotation = (float)(rand() % 361);

        // ゼロ
        const float ZERO_TIME = 0.0f;

        // タイマースピード
        float timerSpeed = 1.0f;

        // 移動距離
        float distance = 3.0f,distanceArrow = 0.05f;

        // 次のポイントに行くための変数
        int pointNext = 1;

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
                m_NumPoint = static_cast<Point>((static_cast<int>(m_NumPoint) + pointNext) % Number);

                // 初期値を格納する
                m_TargetPosition = m_InitialPosition;

                switch (m_NumPoint)
                {
                case Point0: // 原点
                    m_TargetPosition = m_InitialPosition;
                    break;

                case Point1:
                    m_TargetPosition = m_Point1Position;
                    break;

                case Point2:
                    m_TargetPosition = m_Point2Position;
                    break;

                case Point3:
                    m_TargetPosition = m_Point3Position;
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

    void EnemyBase::OnCollisionEnter(shared_ptr<GameObject>& Other)
    {
        auto bubble = dynamic_pointer_cast<Bubble>(Other);
        if (bubble == nullptr)
        {
            return;
        }

        auto force = bubble->GetBubbleHP();
        bool soup = bubble->GetBubblePowerUp();
        if (Other->FindTag(L"Bubble") && soup == true)
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
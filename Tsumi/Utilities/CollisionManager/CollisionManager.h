#pragma once

#include "SphereCollider.h"
#include "OBBCollider.h"
#include "AABBCollider.h"
#include "SegmentCollider.h"
#include "ColliderConfig.h"
#include "MyMath.h"
#include "Struct.h"
#include "IsCollision/IsCollision.h"

#include <list>
using namespace std;



/* CollisionManagerクラス */
class CollisionManager {

public: // 総当たり用

	/// <summary>
	/// 登録されたすべてのコライダーに対して衝突を検出する。
	/// </summary>
	void CheckAllCollision();

    /// <summary>
    /// 各種コライダーをリストに登録するメソッド
    /// </summary>
    void AddSphereColliders(SphereCollider* collider) { sphereColliders_.push_back(collider); }
    void AddSegmentColliders(SegmentCollider* collider) { segmentColliders_.push_back(collider); }
    void AddAABBColliders(AABBCollider* collider) { aabbColliders_.push_back(collider); }
    void AddOBBColliders(OBBCollider* collider) { obbColliders_.push_back(collider); }

    /// <summary>
    /// 登録されたコライダーリストをクリアするメソッド
    /// </summary>
    void ColliderClear()	{
		sphereColliders_.clear();
        segmentColliders_.clear();
		aabbColliders_.clear();
        obbColliders_.clear();
	}

private:

    // -------------------------------------------------------------------------
    // 衝突検出メソッド
    // -------------------------------------------------------------------------
    //  各種コライダー同士のイテレーター処理と衝突判定の呼び出し
    // -------------------------------------------------------------------------
    void DetectSphere2SphereList(list<SphereCollider*>::iterator itrA);
    void DetectAABB2AABBList(list<AABBCollider*>::iterator itrA);
    void DetectAABB2SphereList(list<AABBCollider*>::iterator itrA, list<SphereCollider*>::iterator itrB);
    void DetectAABB2SegmentList(list<AABBCollider*>::iterator itrA, list<SegmentCollider*>::iterator itrB);
    void DetectOBB2SphereList(list<OBBCollider*>::iterator itrA, list<SphereCollider*>::iterator itrB);
    void DetectOBB2SegmentList(list<OBBCollider*>::iterator itrA, list<SegmentCollider*>::iterator itrB);
    void DetectOBB2OBBList(list<OBBCollider*>::iterator itrA);

    // -------------------------------------------------------------------------
    // 衝突検出と応答メソッド
    // -------------------------------------------------------------------------
    // 2つのコライダーの衝突フィルターと衝突判定
    // -------------------------------------------------------------------------
    void CheckCollisionSpherexSphere(SphereCollider* cA, SphereCollider* cB);
    void CheckCollisionAABBxAABB(AABBCollider* cA, AABBCollider* cB);
    void CheckCollisionAABBxSphere(AABBCollider* cA, SphereCollider* cB);
    void CheckCollisionAABBxSegment(AABBCollider* cA, SegmentCollider* cB);
    void CheckCollisionOBBxSphere(OBBCollider* cA, SphereCollider* cB);
    void CheckCollisionOBBxSegment(OBBCollider* cA, SegmentCollider* cB);
    void CheckCollisionOBBxOBB(OBBCollider* cA, OBBCollider* cB);

private:

	// コライダーリスト
	list<SphereCollider*> sphereColliders_;
	list<SegmentCollider*> segmentColliders_;
    list<AABBCollider*> aabbColliders_;
    list<OBBCollider*> obbColliders_;


public: // 個別用

    // -------------------------------------------------------------------------
    // 衝突検出と応答メソッド
    // -------------------------------------------------------------------------
    // 2つのコライダーの衝突判定
    // -------------------------------------------------------------------------
    static bool CheckSpherexSphere(SphereCollider* cA, SphereCollider* cB);
    static bool CheckAABBxAABB(AABBCollider* cA, AABBCollider* cB);
    static bool CheckAABBxSphere(AABBCollider* cA, SphereCollider* cB);
    static bool CheckAABBxSegment(AABBCollider* cA, SegmentCollider* cB);
    static bool CheckOBBxSphere(OBBCollider* cA, SphereCollider* cB);
    static bool CheckOBBxSegment(OBBCollider* cA, SegmentCollider* cB);
    static bool CheckOBBxAABB(OBBCollider* cA, AABB* cB);
    static bool CheckOBBxOBB(OBBCollider* cA, OBBCollider* cB);
};

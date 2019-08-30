#pragma once

#include "Mesh.h"
#include "Camera.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGameObject
{
public:
	CGameObject(CMesh *pMesh);
	CGameObject();
	virtual ~CGameObject();

public:
	bool						m_bActive = true;
	
	CMesh						*m_pMesh = NULL;
    XMFLOAT4X4					m_xmf4x4World;    

	BoundingOrientedBox			m_xmOOBB;

	CGameObject					*m_pObjectCollided = NULL;
	CGameObject					*m_pTarget = NULL;

	DWORD						m_dwColor;

	XMFLOAT3					m_xmf3MovingDirection;
	float						m_fMovingSpeed;
	float						m_fMovingRange;

	XMFLOAT3					m_xmf3RotationAxis;
	float						m_fRotationSpeed;

public:
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	XMMATRIX GetWorld();
	XMFLOAT3 GetRotationAxis() { return m_xmf3RotationAxis; }

	void MoveStrafe(float fDistance=1.0f);
	void MoveUp(float fDistance=1.0f);
	void MoveForward(float fDistance=1.0f);
	void Move(XMFLOAT3& vDirection, float fSpeed);

	void Rotate(float fPitch=10.0f, float fYaw=10.0f, float fRoll=10.0f);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

class CExplosiveObject : public CGameObject
{
public:
	CExplosiveObject(CGameObject *pTarget);
	virtual ~CExplosiveObject();

	bool						m_bBlowingUp = false;

	XMFLOAT4X4					m_pxmf4x4Transforms[EXPLOSION_DEBRISES];

	float						m_fElapsedTimes = 0.0f;
	float						m_fDuration = 0.5f;
	float						m_fExplosionSpeed = 20.0f;
	float						m_fExplosionRotation = 720.0f;


	void SetBoxParalyzed();

	

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

public:
	static CMesh				*m_pExplosionMesh;
	static XMFLOAT3				m_pxmf3SphereVectors[EXPLOSION_DEBRISES];					//폭발시 방향 벡터가 저장되는 배열

	static unsigned int			RedboxBoom;													//빨간 박스 터짐 변수
	static void PrepareExplosion();
};

class CWallsObject : public CGameObject
{
public:
	CWallsObject();
	virtual ~CWallsObject();

public:
	XMFLOAT4					m_pxmf4WallPlanes[6];
};



class CBulletObject : public CGameObject
{
public:
	CBulletObject();
	virtual ~CBulletObject();


	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

public:

	bool m_fired = false;						//총알이 발사되었는지?	
	float m_fElapsedTimes = 0.0f;			
	
	float GetTargetDistance(XMFLOAT3 &target);	//타겟과의 직선상의 거리 구하기

	void SetFire();
	
	void RotateBullet(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	virtual void Animate(float fElapsedTime,XMFLOAT3 &xmfPosition);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);


};


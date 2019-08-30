#pragma once
#include "Mesh.h"
#include "Camera.h"
class CShader;




class CGameObject
{
private:
	int m_nReferences = 0;
public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	UINT m_xmColor;

protected:

	XMFLOAT4X4	m_xmf4x4World;
	CMesh		*m_pMesh = NULL;
	CShader		*m_pShader = NULL;

	XMFLOAT3	m_xmf3RotationAxis;
	float		m_fRotationSpeed;


public:
	void ReleaseUploadBuffers();

	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);

public:
	BoundingOrientedBox m_xmOOBB;
	BoundingOrientedBox m_xmOOBBTransformed;
	CGameObject *m_pObjectCollided = NULL;

	CGameObject *m_pTarget = NULL;

	XMFLOAT3	m_xmf3MovingDirection;
	float		m_fMovingSpeed;
	float		m_fMovingRange;

	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) {
		m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); 
	}

public:
	//상수 버퍼를 생성한다.
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList);
	//상수 버퍼의 내용을 갱신한다.
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	//게임 객체의 월드 변환 행렬에서 위치 벡터와 방향(x-축, y-축, z-축) 벡터를 반환한다. 
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	//게임 객체의 위치를 설정한다.
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }
	

	CMesh* GetMesh() { return this->m_pMesh; }


	//게임 객체를 로컬 x-축, y-축, z-축 방향으로 이동한다.
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(XMFLOAT3& vDirection, float fSpeed);

	//게임 객체를 회전(x-축, y-축, z-축)한다.
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f); 
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDir);
	int Pick(XMVECTOR& xmPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

	
public:
		CGameObject();
		virtual ~CGameObject();


		UINT mPickedTriangle;
		bool m_bActive = true;
};

class CWallsObject :public CGameObject
{
public:
	CWallsObject();
	virtual ~CWallsObject();
public:
	XMFLOAT4 m_pxmf4WallPlanes[6];
};

//벽이 RotatingObject로 되어있다.
class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();
protected:
	XMFLOAT3 m_xmf3RotationAxis;
	float m_fRotationSpeed;
public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) {
		m_xmf3RotationAxis =
			xmf3RotationAxis;
	}

	float GetRotationSpeed() { return m_fRotationSpeed; }
	virtual void Animate(float fTimeElapsed);
};


//RotatingObject를 상속
class CExplosiveObject : public CRotatingObject
{
public:
	CExplosiveObject(CGameObject *Player);
	virtual ~CExplosiveObject();
	
	bool m_bBlowingUp = false;

	XMFLOAT4X4 m_pxmf4x4Transforms[EXPLOSION_DEBRISES];
	

	float m_fElapsedTimes = 0.0f;
	float m_fDuration = 0.5f;
	float m_fExplosionSpeed = 30.0f;
	float m_fExplosionRotation = 720.0f;


	void SetBoxParalyzed();


public:

	static CMesh *m_pExplosionMesh;
	static XMFLOAT3 m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
	static void PrepareExplosion(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);

	static unsigned int RedboxBoom;

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Animate(float fElapsedTime,CPlayer *player);
	virtual void Render( ID3D12GraphicsCommandList *pd3dCommandList,CCamera *pCamera);

	
};

class CBulletObject : public CRotatingObject
{
public:
	CBulletObject();
	virtual ~CBulletObject();

	XMFLOAT3	m_xmf3Position;
	XMFLOAT3	m_xmf3Right;
	XMFLOAT3	m_xmf3Up;
	XMFLOAT3	m_xmf3Look;

public:
	bool m_fired = false;

	float m_fElapsedTimes = 0.0f;

	float GetTargetDistance(XMFLOAT3 &target);		//타겟과의 직선상의 거리 구하기

	void SetFire();

	void RotateBullet(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);

	void SetUpVector(XMFLOAT3 &upVector) { this->m_xmf3Up = upVector; }
	void SetRightVector(XMFLOAT3 &RightVector) { this->m_xmf3Right = RightVector;  }

	virtual void Animate(float fElapsedTime, XMFLOAT3& xmfPosition);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
};


//CExplosiveObject 상속
class CBoss : public CExplosiveObject
{
public:
	CBoss(CGameObject* pTarget);
	virtual ~CBoss();

	CBulletObject	**m_pBulletObjects = NULL;



	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;


	XMFLOAT3					m_xmf3Velocity;
	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

public:
	static int m_countBullet;														//총알의 수
	static int m_currentBullet;														//현재 쏜 총알의 수


	static void LoadBullet();

public:
	void TakeShot(float fElapsedTime);

	virtual void Animate(float fElapsedTime,CPlayer *pPlayer);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
};
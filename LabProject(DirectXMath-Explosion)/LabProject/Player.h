#pragma once

#include "GameObject.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();


	BoundingSphere *m_sphereBox;			//반경 50m구체 충돌 박스

	CBulletObject				**m_pBulletObjects = NULL;

	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	XMFLOAT3					m_xmf3CameraOffset;
	XMFLOAT3					m_xmf3Velocity;
	float						m_fFriction = 125.0f;

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	CCamera						*m_pCamera = NULL;

	void SetPosition(float x, float y, float z);
	void Move(DWORD dwDirection, float fDistance);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);
	void Update(float fTimeElapsed=0.016f);
	void ChangeColor(DWORD color=RGB(255,0,255)) { m_dwColor = color; }


	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

public:
	static int m_countBullet;					//총알의 수
	static int m_currentBullet;					//현재 쏜 총알의 수

	static void LoadBullet(int number);
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer();
	virtual ~CAirplanePlayer();
};

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer();
	virtual ~CTerrainPlayer();
};

#pragma once
#include "GameObject.h"


class CBoss :public CExplosiveObject
{
public:
	CBoss(CGameObject *pTarget);
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
	static int m_countBullet;														//�Ѿ��� ��
	static int m_currentBullet;														//���� �� �Ѿ��� ��


	static void LoadBullet();														//����

public:

	void TakeShot(float fElapsedTime);												//�Ѿ� �߻�
	void SetPosition(float x, float y, float z);
	void SetBullet();


	virtual void Animate(float fElapsedTime,CPlayer *pPlayer);
	virtual void Render(HDC hdcFrameBuffer,CCamera *pCamera);
};


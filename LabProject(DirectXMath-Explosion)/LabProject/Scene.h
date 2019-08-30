#pragma once

#include "GameObject.h"
#include "Boss.h"
#include "Player.h"
#include "Resource.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

	CPlayer						*m_pPlayer = NULL;

	int							m_nObjects = 0;
	CExplosiveObject			**m_ppObjects = NULL;


	//vector<CExplosiveObject*> m_ppvObjects;
	int						m_ClientWidth=0;
	int						m_ClientHeight=0;
	
	//CBulletObject				**m_pBulletObjects = NULL;
	CWallsObject				*m_pWallsObject = NULL;

	CBoss						*m_pBossObject = NULL;

	float						m_fElapsedTimes = 0.0f;
	float						m_fRedElapsedTimes = 0.0f;

	

	HBITMAP hGameOverBmp;

	BITMAP GameOverBmp;

	//ºä °ø°£¿¡¼­ÀÇ ±¤¼± Á¤ÀÇ
	XMVECTOR rayOrigin = XMVectorZero();
	XMVECTOR rayDir = XMVectorZero();


	bool ComparePosition(int num);


	void SetClientRect(int x, int y);

	void GetKeyUp();
	void GenerateBoxObject(int num,CPlayer *pPlayer);
	void GenerateRedBoxObject(int num, CPlayer *pPlayer);
	void GeneratePickingRay(int sx,int sy);
	void AnimatePickingRay(int num);

	virtual void BuildObjects(HINSTANCE hInstance,CPlayer *pPlayer);
	virtual void ReleaseObjects();

	virtual void CheckObjectByObjectCollisions();
	virtual void CheckObjectByWallCollisions();
	virtual void CheckObjectByBulletCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	virtual void OnMouseDown(WPARAM btnState, int x, int y);

	

	void SetImage(HINSTANCE g_hInst);

};


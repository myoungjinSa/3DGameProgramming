#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#include "Timer.h"
#include "Scene.h"
#include "Player.h"


#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance = NULL;
	HWND						m_hWnd = NULL;

    bool						m_bActive = true;          

	CGameTimer					m_GameTimer;

	HDC							m_hDCFrameBuffer = NULL;
    HBITMAP						m_hBitmapFrameBuffer = NULL;

	CPlayer						*m_pPlayer = NULL;

	CScene						*m_pScene = NULL;

	RECT						m_rcClient;															//클라이언트 영역 정보

	POINT						m_PickingPoint= { 0 , };											//피킹 포인트

public:
	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	void BuildObjects(HINSTANCE hInstance);
	void ReleaseObjects();
	void ProcessInput();

	void GeneratePickingRay();

	void SetClientRect();


	

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);



	// 마우스 처리를 쉽게 하기위해 오버라이드 함수 정의
	virtual void OnMouseDown(WPARAM btnState, int x,int y) {};

	POINT						m_ptOldCursorPos;

	_TCHAR						m_pszFrameRate[50];
};


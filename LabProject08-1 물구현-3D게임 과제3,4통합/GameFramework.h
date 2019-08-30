#pragma once

#define FRAME_BUFFER_WIDTH		640
#define FRAME_BUFFER_HEIGHT		480

#include "Timer.h"
#include "Player.h"
#include "Scene.h"



class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	void CreateCommandQueueAndList();
	void CreateOffScreenRenderTargetViews();

	void OnResizeBackBuffers();

    void BuildObjects();
    void ReleaseObjects();

    void ProcessInput();
    void AnimateObjects();
    void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	const enum GAMESTATE { INTRO = 0, HELP,PLAY, END };
private:
	
	int gamestate = INTRO;
	HINSTANCE					m_hInstance;
	HWND						m_hWnd; 


	bool						m_bCartoon = false;
	bool						m_bBlurring = false;
	int							m_bSobel = 0;
	int							m_nWndClientWidth;
	int							m_nWndClientHeight;
        
	IDXGIFactory4				*m_pdxgiFactory = NULL;
	IDXGISwapChain3				*m_pdxgiSwapChain = NULL;
	ID3D12Device				*m_pd3dDevice = NULL;

	bool						m_bMsaa4xEnable = false;
	UINT						m_nMsaa4xQualityLevels = 0;

	static const UINT			m_nSwapChainBuffers = 2;
	UINT						m_nSwapChainBufferIndex;

	ID3D12Resource				*m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap		*m_pd3dRtvDescriptorHeap = NULL;
	UINT						m_nRtvDescriptorIncrementSize;
	D3D12_CPU_DESCRIPTOR_HANDLE m_pd3dRtvSwapChainBackBufferCPUHandles[m_nSwapChainBuffers];



	ID3D12Resource				*m_pd3dDepthStencilBuffer = NULL;
	ID3D12DescriptorHeap		*m_pd3dDsvDescriptorHeap = NULL;
	UINT						m_nDsvDescriptorIncrementSize;
	D3D12_CPU_DESCRIPTOR_HANDLE m_d3dDsvDepthStencilBufferCPUHandle;


	ID3D12CommandAllocator		*m_pd3dCommandAllocator = NULL;
	ID3D12CommandAllocator		*m_pd3dCommandAllocator2 = NULL;
	ID3D12CommandAllocator		*m_pd3dCommandAllocator3 = NULL;

	ID3D12CommandQueue			*m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList = NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList2 = NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList3 = NULL;


	ID3D12Fence					*m_pd3dFence = NULL;
	UINT64						m_nFenceValues[m_nSwapChainBuffers];
	HANDLE						m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug					*m_pd3dDebugController;
#endif

	CGameTimer					m_GameTimer;

	CScene						*m_pScene = NULL;
	CTexturedUIShader			*m_pUIShader = NULL;
	CTexturedMenuUIShader		*m_pMenuShader = NULL;
	CTexturedTargetUIShader		*m_pTargetUIShader = NULL;
	CPlayer						*m_pPlayer = NULL;
	CCamera						*m_pCamera = NULL;
	
	CCamera						*m_pBackMirrorCamera = NULL;

	POINT						m_ptOldCursorPos;

	static const UINT			m_nOffScreenRenderTargetBuffers = 1;
	ID3D12Resource				*m_ppd3dOffScreenRenderTargetBuffers[m_nOffScreenRenderTargetBuffers];
	D3D12_CPU_DESCRIPTOR_HANDLE	m_pd3dOffScreenRenderTargetBufferCPUHandles[m_nOffScreenRenderTargetBuffers];


	static const UINT			m_nBlurScreenRenderTargetBuffers = 2;
	ID3D12Resource				*m_ppd3dScreenBlur[m_nBlurScreenRenderTargetBuffers];


	static const UINT			m_nCartoonScreenRenderTargetBuffers =1;
	ID3D12Resource				*m_ppd3dCartoonScreenRenderTargetBuffers[m_nCartoonScreenRenderTargetBuffers];
	D3D12_CPU_DESCRIPTOR_HANDLE	m_pd3dCartoonScreenRenderTargetBufferCPUHandles[m_nOffScreenRenderTargetBuffers];


	static const UINT			m_nSobelScreenRenderTargetBuffers = 2;
	ID3D12Resource				*m_ppd3dSobelScreen[m_nBlurScreenRenderTargetBuffers];




	CPostProcessingShader		*m_pPostProcessingShader = NULL;
	//CCartoonShaderByLaplacianShader				*m_pCartoonShader = NULL;

	CCartoonShader				*m_pCartoonShader = NULL;
	CComputeShader				*m_pBlurShader = NULL;
	CSobelComputeShader			*m_pSobelShader = NULL;

	_TCHAR						m_pszFrameRate[50];
};


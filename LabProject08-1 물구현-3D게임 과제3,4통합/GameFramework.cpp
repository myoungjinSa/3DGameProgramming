//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"


CGameFramework::CGameFramework()
{
	m_pdxgiFactory = NULL;
	m_pdxgiSwapChain = NULL;
	m_pd3dDevice = NULL;

	for (int i = 0; i < m_nSwapChainBuffers; i++) m_ppd3dSwapChainBackBuffers[i] = NULL;
	m_nSwapChainBufferIndex = 0;

	m_pd3dCommandAllocator = NULL;
	m_pd3dCommandQueue = NULL;
	m_pd3dCommandList = NULL;

	m_pd3dRtvDescriptorHeap = NULL;
	m_pd3dDsvDescriptorHeap = NULL;

	m_nRtvDescriptorIncrementSize = 0;
	m_nDsvDescriptorIncrementSize = 0;

	m_hFenceEvent = NULL;
	m_pd3dFence = NULL;
	for (int i = 0; i < m_nSwapChainBuffers; i++) m_nFenceValues[i] = 0;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;
	m_pPlayer = NULL;

	

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

CGameFramework::~CGameFramework()
{
	if(m_pUIShader) delete m_pUIShader;
	if(m_pTargetUIShader) delete m_pTargetUIShader;

	if (m_pPostProcessingShader)
	{
		m_pPostProcessingShader->ReleaseShaderVariables();
		//m_pPostProcessingShader->ReleaseObjects();
		m_pPostProcessingShader->Release();
	}
	if (m_pBlurShader)
	{
		
		//m_pPostProcessingShader->ReleaseObjects();
		m_pBlurShader->Release();
	}



}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();

	BuildObjects();

	CreateOffScreenRenderTargetViews();

	return(true);
}

//#define _WITH_SWAPCHAIN

void CGameFramework::CreateSwapChain()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

#ifdef _WITH_CREATE_SWAPCHAIN_FOR_HWND
	DXGI_SWAP_CHAIN_DESC1 dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	dxgiSwapChainDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.Scaling = DXGI_SCALING_NONE;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxgiSwapChainFullScreenDesc;
	::ZeroMemory(&dxgiSwapChainFullScreenDesc, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
	dxgiSwapChainFullScreenDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainFullScreenDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainFullScreenDesc.Windowed = TRUE;

	HRESULT hResult = m_pdxgiFactory->CreateSwapChainForHwnd(m_pd3dCommandQueue, m_hWnd, &dxgiSwapChainDesc, &dxgiSwapChainFullScreenDesc, NULL, (IDXGISwapChain1 **)&m_pdxgiSwapChain);
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	dxgiSwapChainDesc.Windowed = TRUE;
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	dxgiSwapChainDesc.Flags = 0;
#else
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	HRESULT hResult = m_pdxgiFactory->CreateSwapChain(m_pd3dCommandQueue, &dxgiSwapChainDesc, (IDXGISwapChain **)&m_pdxgiSwapChain);
#endif

	if (!m_pdxgiSwapChain)
	{
		MessageBox(NULL, L"Swap Chain Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	hResult = m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
}

void CGameFramework::CreateDirect3DDevice()
{
	HRESULT hResult;

	UINT nDXGIFactoryFlags = 0;
#if defined(_DEBUG)
	ID3D12Debug *pd3dDebugController = NULL;
	hResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void **)&pd3dDebugController);
	if (pd3dDebugController)
	{
		pd3dDebugController->EnableDebugLayer();
		pd3dDebugController->Release();
	}
	nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hResult = ::CreateDXGIFactory2(nDXGIFactoryFlags, __uuidof(IDXGIFactory4), (void **)&m_pdxgiFactory);

	IDXGIAdapter1 *pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void **)&m_pd3dDevice))) break;
	}

	if (!m_pd3dDevice)
	{
		hResult = m_pdxgiFactory->EnumWarpAdapter(_uuidof(IDXGIAdapter1), (void **)&pd3dAdapter);
		hResult = D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void **)&m_pd3dDevice);
	}

	if (!m_pd3dDevice)
	{
		MessageBox(NULL, L"Direct3D 12 Device Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	hResult = m_pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;

	hResult = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void **)&m_pd3dFence);
	for (UINT i = 0; i < m_nSwapChainBuffers; i++) m_nFenceValues[i] = 1;
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	::gnCbvSrvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	if (pd3dAdapter)
	{
		pd3dAdapter->Release();
	}
}

void CGameFramework::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hResult = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, _uuidof(ID3D12CommandQueue), (void **)&m_pd3dCommandQueue);

	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&m_pd3dCommandAllocator);
	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&m_pd3dCommandAllocator2);
	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&m_pd3dCommandAllocator3);

	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&m_pd3dCommandList);
	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator2, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&m_pd3dCommandList2);
	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator3, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&m_pd3dCommandList3);


	hResult = m_pd3dCommandList->Close();
	hResult = m_pd3dCommandList2->Close();
	hResult = m_pd3dCommandList3->Close();
}

void CGameFramework::CreateOffScreenRenderTargetViews()
{
	CTexture *pTextureForPostProcessing = new CTexture(m_nOffScreenRenderTargetBuffers, RESOURCE_TEXTURE2D_ARRAY, 0);
	
	CTexture *pTextureForBlurProcessing = new CTexture(m_nBlurScreenRenderTargetBuffers, RESOURCE_TEXTURE2D_ARRAY, 0);

	CTexture *pTextureForCartoonProcessing = new CTexture(m_nCartoonScreenRenderTargetBuffers, RESOURCE_TEXTURE2D_ARRAY, 0);

	CTexture *pTextureForSobelProcessing = new CTexture(m_nSobelScreenRenderTargetBuffers, RESOURCE_TEXTURE2D_ARRAY, 0);


	D3D12_CLEAR_VALUE d3dClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM, { 0.0f, 0.0f, 0.0f, 1.0f } };
	for (UINT i = 0; i < m_nOffScreenRenderTargetBuffers; i++)
	{
		m_ppd3dOffScreenRenderTargetBuffers[i] = pTextureForPostProcessing->CreateTexture(m_pd3dDevice, m_pd3dCommandList, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &d3dClearValue, i);
		m_ppd3dOffScreenRenderTargetBuffers[i]->AddRef();			//AddRef()는 인터페이스 함수
	}

	for (UINT i = 0; i < m_nBlurScreenRenderTargetBuffers; i++)
	{
		
		m_ppd3dScreenBlur[i] = pTextureForBlurProcessing->CreateTexture(m_pd3dDevice, m_pd3dCommandList, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON, nullptr, i);
		m_ppd3dScreenBlur[i]->AddRef();
	}
	
	for (UINT i = 0; i < m_nSobelScreenRenderTargetBuffers; i++)
	{
		m_ppd3dSobelScreen[i] = pTextureForSobelProcessing->CreateTexture(m_pd3dDevice, m_pd3dCommandList, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, i);
		m_ppd3dSobelScreen[i]->AddRef();
	}

	for (UINT i = 0; i < m_nCartoonScreenRenderTargetBuffers; i++)
	{

		m_ppd3dCartoonScreenRenderTargetBuffers[i] = pTextureForCartoonProcessing->CreateTexture(m_pd3dDevice, m_pd3dCommandList, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &d3dClearValue, i);
		m_ppd3dCartoonScreenRenderTargetBuffers[i]->AddRef();
	}


	// RtvCPU서술자 증가 크기가 스왑체인의 크기 배수로 증가하는 이유?
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRtvCPUDescriptorHandle.ptr += (m_nSwapChainBuffers * m_nRtvDescriptorIncrementSize);

	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;


	for (UINT i = 0; i < m_nOffScreenRenderTargetBuffers; i++)
	{
		m_pd3dOffScreenRenderTargetBufferCPUHandles[i] = d3dRtvCPUDescriptorHandle;
		m_pd3dDevice->CreateRenderTargetView(pTextureForPostProcessing->GetTexture(i), &d3dRenderTargetViewDesc, m_pd3dOffScreenRenderTargetBufferCPUHandles[i]);
		d3dRtvCPUDescriptorHandle.ptr += m_nRtvDescriptorIncrementSize;
	}
	


	m_pPostProcessingShader = new CPostProcessingShader();
	m_pPostProcessingShader->CreateGraphicsRootSignature(m_pd3dDevice);
	m_pPostProcessingShader->CreateShader(m_pd3dDevice, m_pPostProcessingShader->GetGraphicsRootSignature(),1);
	m_pPostProcessingShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList, pTextureForPostProcessing);



	//m_pCartoonShader = new CCartoonShader();
	//m_pCartoonShader->CreateGraphicsRootSignature(m_pd3dDevice);
	//m_pCartoonShader->CreateShader(m_pd3dDevice, m_pCartoonShader->GetGraphicsRootSignature(),2);
	//m_pCartoonShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList, pTextureForCartoonProcessing);


	m_pSobelShader = new CSobelComputeShader();
	m_pSobelShader->CreateGraphicsRootSignature(m_pd3dDevice);
	m_pSobelShader->CreateShader(m_pd3dDevice, m_pSobelShader->GetGraphicsRootSignature(), 1);
	m_pSobelShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList, pTextureForSobelProcessing);

	//m_pCartoonShader = new CCartoonShaderByLaplacianShader();
	//m_pCartoonShader->CreateGraphicsRootSignature(m_pd3dDevice);
	//m_pCartoonShader->CreateShader(m_pd3dDevice, m_pCartoonShader->GetGraphicsRootSignature(), 2);
	//m_pCartoonShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList, pTextureForCartoonProcessing);


	m_pBlurShader = new CComputeShader();
	m_pBlurShader->CreateGraphicsRootSignature(m_pd3dDevice);
	//m_pBlurShader->CreateComputeRootSignature(m_pd3dDevice);
	m_pBlurShader->CreateShader(m_pd3dDevice, m_pBlurShader->GetGraphicsRootSignature(), 1);
	m_pBlurShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList, pTextureForBlurProcessing);




}

void CGameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	d3dDescriptorHeapDesc.NumDescriptors = m_nSwapChainBuffers + m_nOffScreenRenderTargetBuffers+m_nCartoonScreenRenderTargetBuffers;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dRtvDescriptorHeap);
	m_nRtvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dDsvDescriptorHeap);
	m_nDsvDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void CGameFramework::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < m_nSwapChainBuffers; i++)
	{
		m_pdxgiSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void **)&m_ppd3dSwapChainBackBuffers[i]);
		m_pd3dDevice->CreateRenderTargetView(m_ppd3dSwapChainBackBuffers[i], NULL, d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += m_nRtvDescriptorIncrementSize;
	}
}

void CGameFramework::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc;
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = m_nWndClientWidth;
	d3dResourceDesc.Height = m_nWndClientHeight;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
	d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapProperties.CreationNodeMask = 1;
	d3dHeapProperties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue, __uuidof(ID3D12Resource), (void **)&m_pd3dDepthStencilBuffer);

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, d3dDsvCPUDescriptorHandle);
	//	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);
}

void CGameFramework::OnResizeBackBuffers()
{
	WaitForGpuComplete();

	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	for (int i = 0; i < m_nSwapChainBuffers; i++) if (m_ppd3dSwapChainBackBuffers[i]) m_ppd3dSwapChainBackBuffers[i]->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	m_nSwapChainBufferIndex = 0;
#else
	//m_pdxgiSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(m_nSwapChainBuffers, m_nWndClientWidth, m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
	m_nSwapChainBufferIndex = 0;
#endif
	CreateRenderTargetViews();
	CreateDepthStencilView();

	m_pd3dCommandList->Close();

	ID3D12CommandList *ppd3dCommandLists[] ={ m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			if (gamestate == INTRO)
			{
				gamestate = HELP;
			}
			else if (gamestate == HELP)
			{
				gamestate = PLAY;
			}
			break;
		case VK_F1:
		case VK_F2:
		case VK_F3:
			m_pCamera = m_pPlayer->ChangeCamera((DWORD)(wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
			
		
								//m_pCamera->CreateShaderVariables(m_pd3dDevice, m_pd3dCommandList);
			break;
		case VK_F4:
			m_pCamera = m_pPlayer->ChangeCamera(SPACESHIP_CAMERA, m_GameTimer.GetTimeElapsed());
			break;
		case VK_F9:
		{
			BOOL bFullScreenState = FALSE;
			m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);
			m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);

			DXGI_MODE_DESC dxgiTargetParameters;
			dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxgiTargetParameters.Width = m_nWndClientWidth;
			dxgiTargetParameters.Height = m_nWndClientHeight;
			dxgiTargetParameters.RefreshRate.Numerator = 60;
			dxgiTargetParameters.RefreshRate.Denominator = 1;
			dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);

			OnResizeBackBuffers();

			break;
		}
		case VK_F10:
			break;
		case 'G':
			if (m_pScene->m_bTerrainWireFrame == false)
			{
				m_pScene->m_bTerrainWireFrame = true;
			}
			else {
				m_pScene->m_bTerrainWireFrame = false;
			}

			break;
		case 'W':
			if (m_pScene->m_bWaterWireFrame == false)
			{
				m_pScene->m_bWaterWireFrame = true;
			}
			else {
				m_pScene->m_bWaterWireFrame = false;
			}
			break;
		case 'T':
			if (m_bSobel == 0)
			{
				m_bSobel = 1;
			}
			else if(m_bSobel == 1){
				m_bSobel = 2;
			}
			else if (m_bSobel == 2)
			{
				m_bSobel = 0;
			}
			break;
		case 'F':
			if (m_bBlurring == false) {
				m_bBlurring = true;
			}
			else {
				m_bBlurring = false;
			}
			break;

		case 'Z':
		{

			if (CPlayer::m_currentMissile < CPlayer::m_countMissile - 1)
			{
				CMissileShader::ppMissile[CPlayer::m_currentMissile]->SetFire();
				CMissileShader::ppMissile[CPlayer::m_currentMissile]->SetPosition(m_pPlayer->GetPosition());
				CMissileShader::ppMissile[CPlayer::m_currentMissile]->SetLook(m_pPlayer->GetLookVector());
				CMissileShader::ppMissile[CPlayer::m_currentMissile]->SetUp(m_pPlayer->GetUpVector());
				CMissileShader::ppMissile[CPlayer::m_currentMissile]->SetRight(m_pPlayer->GetRightVector());
				CMissileShader::ppMissile[CPlayer::m_currentMissile]->SetMovingDirection(m_pPlayer->GetLookVector());

			}
			if (CPlayer::m_currentMissile < CPlayer::m_countMissile - 1)
			{
				CPlayer::m_currentMissile++;
			}

		}
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_SIZE:
	{
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		OnResizeBackBuffers();
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	::CloseHandle(m_hFenceEvent);

#if defined(_DEBUG)
	//IDXGIDebug1 *pdxgiDebug = NULL;
	//DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void **)&pdxgiDebug);
	//HRESULT hResult = pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	//pdxgiDebug->Release();
	if (m_pd3dDebugController)
	{
		m_pd3dDebugController->Release();
	}
	
#endif

	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDsvDescriptorHeap) m_pd3dDsvDescriptorHeap->Release();

	for (int i = 0; i < m_nSwapChainBuffers; i++) if (m_ppd3dSwapChainBackBuffers[i]) m_ppd3dSwapChainBackBuffers[i]->Release();
	if (m_pd3dRtvDescriptorHeap) m_pd3dRtvDescriptorHeap->Release();

	if (m_pd3dCommandAllocator) m_pd3dCommandAllocator->Release();
	if (m_pd3dCommandAllocator2) m_pd3dCommandAllocator2->Release();

	if (m_pd3dCommandQueue) m_pd3dCommandQueue->Release();
	if (m_pd3dCommandList)
	{
		m_pd3dCommandList->Release();
	
	}
	if (m_pd3dCommandList2)
	{
		m_pd3dCommandList2->Release();

	}

	if (m_pd3dFence) m_pd3dFence->Release();

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiFactory) m_pdxgiFactory->Release();
}

void CGameFramework::BuildObjects()
{
	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);				//명령 리스트를 초기 상태로 만듬. 명령 큐의 명령은 영향 없음

	m_pScene = new CScene();
	m_pScene->BuildObjects(m_pd3dDevice, m_pd3dCommandList);

	//m_pScene->m_pPlayer = m_pPlayer = new CTerrainPlayer(m_pd3dDevice, m_pd3dCommandList, m_pScene->GetGraphicsRootSignature(), m_pScene->GetTerrain());
	//m_pCamera = m_pPlayer->GetCamera();

	//m_pPlayer->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	CAirplanePlayer *pAirplanePlayer = new CAirplanePlayer(m_pd3dDevice, m_pd3dCommandList, m_pScene->GetGraphicsRootSignature());
	pAirplanePlayer->SetPosition(XMFLOAT3(500.0f, 300.0f, 200.0f));
	m_pScene->m_pPlayer = m_pPlayer = pAirplanePlayer;
	m_pCamera = m_pPlayer->GetCamera();



	
	
	m_pUIShader = new CTexturedUIShader;
	m_pUIShader->CreateGraphicsRootSignature(m_pd3dDevice);
	m_pUIShader->CreateShader(m_pd3dDevice, m_pUIShader->GetGraphicsRootSignature());
	m_pUIShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList);
	


	m_pTargetUIShader = new CTexturedTargetUIShader;
	m_pTargetUIShader->CreateGraphicsRootSignature(m_pd3dDevice);
	m_pTargetUIShader->CreateShader(m_pd3dDevice, m_pTargetUIShader->GetGraphicsRootSignature());
	m_pTargetUIShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList);

	m_pMenuShader = new CTexturedMenuUIShader;
	m_pMenuShader->CreateGraphicsRootSignature(m_pd3dDevice);
	m_pMenuShader->CreateShader(m_pd3dDevice, m_pMenuShader->GetGraphicsRootSignature());
	m_pMenuShader->BuildObjects(m_pd3dDevice, m_pd3dCommandList);


	m_pd3dCommandList->Close();	//명령 리스트를 닫힌 상태로 만듬. Reset()호출 전의 실행
	ID3D12CommandList *ppd3dCommandLists[] ={ m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();
	if (m_pScene) m_pScene->ReleaseUploadBuffers();
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();

	m_GameTimer.Reset();
}

void CGameFramework::ReleaseObjects()
{
	if (m_pPlayer) delete m_pPlayer;

	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;
	
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	if (GetKeyboardState(pKeysBuffer) && m_pScene) bProcessedByScene = m_pScene->ProcessInput(pKeysBuffer);
	if (!bProcessedByScene)
	{
		DWORD dwDirection = 0;
		if (pKeysBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeysBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeysBuffer[VK_CONTROL] & 0xF0) dwDirection |= DIR_UP;
		if (pKeysBuffer[VK_SPACE] & 0xF0) dwDirection |= DIR_DOWN;

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		if (GetCapture() == m_hWnd)
		{
			SetCursor(NULL);
			GetCursorPos(&ptCursorPos);
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}

		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeysBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
			if (dwDirection) m_pPlayer->Move(dwDirection, 1.25f, true);
		}
	}
	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::AnimateObjects()
{
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();

	if (m_pScene) m_pScene->AnimateObjects(fTimeElapsed);

	m_pPlayer->Animate(fTimeElapsed, NULL);
}


void CGameFramework::WaitForGpuComplete()
{
	const UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}


void CGameFramework::MoveToNextFrame()
{
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	//m_nSwapChainBufferIndex = (m_nSwapChainBufferIndex + 1) % m_nSwapChainBuffers;

	UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

#define _WITH_PLAYER_TOP

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(0.0f);
	ProcessInput();
	AnimateObjects();
	float pfClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//DWORD nCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;

	HRESULT hResult = m_pd3dCommandAllocator2->Reset();
	hResult = m_pd3dCommandList2->Reset(m_pd3dCommandAllocator2, NULL);
		
	// 전에는 읽기 상태였는데 그리는걸로 상태를 바꾸겠다. 스크린에 안그리는 렌더타겟버퍼들을.
	for (int i = 0; i < m_nOffScreenRenderTargetBuffers; i++) {
		::SynchronizeResourceTransition(m_pd3dCommandList2, m_ppd3dOffScreenRenderTargetBuffers[i], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

		// 논스크린 렌더타겟버퍼들을 클리어해줌. 뎁스스텐실버퍼도 초기화
	for (int i = 0; i < m_nOffScreenRenderTargetBuffers; i++) {
		m_pd3dCommandList2->ClearRenderTargetView(m_pd3dOffScreenRenderTargetBufferCPUHandles[i], pfClearColor, 0, NULL);
	}

	m_d3dDsvDepthStencilBufferCPUHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();


	m_pd3dCommandList2->ClearDepthStencilView(m_d3dDsvDepthStencilBufferCPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	m_pd3dCommandList2->OMSetRenderTargets(m_nOffScreenRenderTargetBuffers, m_pd3dOffScreenRenderTargetBufferCPUHandles, TRUE, &m_d3dDsvDepthStencilBufferCPUHandle);

	if (gamestate == PLAY) {
		m_pCamera->SetLookVector(XMFLOAT3(-m_pCamera->GetLookVector().x, -m_pCamera->GetLookVector().y, -m_pCamera->GetLookVector().z));
		m_pCamera->RegenerateViewMatrix();
		m_pScene->Render(m_pd3dCommandList2, m_pCamera);

	}


	for (int i = 0; i < m_nOffScreenRenderTargetBuffers; i++) {
		::SynchronizeResourceTransition(m_pd3dCommandList2, m_ppd3dOffScreenRenderTargetBuffers[i], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	}


	hResult = m_pd3dCommandList2->Close();
	ID3D12CommandList *ppd3dPostCommandLists[] = { m_pd3dCommandList2 };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dPostCommandLists);
	WaitForGpuComplete();
		

	hResult = m_pd3dCommandAllocator->Reset();
	hResult = m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	///////////////////////////////////////////////////////////////////////////
	// 후면버퍼
	::SynchronizeResourceTransition(m_pd3dCommandList,m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex],D3D12_RESOURCE_STATE_PRESENT,D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_pd3dRtvSwapChainBackBufferCPUHandles[m_nSwapChainBufferIndex] = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dRtvSwapChainBackBufferCPUHandles[m_nSwapChainBufferIndex].ptr += (m_nSwapChainBufferIndex * m_nRtvDescriptorIncrementSize);

	m_pd3dCommandList->ClearRenderTargetView(m_pd3dRtvSwapChainBackBufferCPUHandles[m_nSwapChainBufferIndex],pfClearColor, 0, NULL);

	

	m_pd3dCommandList->ClearDepthStencilView(m_d3dDsvDepthStencilBufferCPUHandle,D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,1.0f, 0, 0, NULL);
	m_pd3dCommandList->OMSetRenderTargets(1,&m_pd3dRtvSwapChainBackBufferCPUHandles[m_nSwapChainBufferIndex],TRUE, &m_d3dDsvDepthStencilBufferCPUHandle);

	if (gamestate == INTRO || gamestate == HELP) {
		m_pMenuShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer, gamestate);
	}
	if (gamestate == PLAY) {
		//m_pCamera->SetLookVector(XMFLOAT3(-m_pCamera->GetLookVector().x, -m_pCamera->GetLookVector().y, -m_pCamera->GetLookVector().z));
		//m_pCamera->RegenerateViewMatrix();

		//TexturedUIShader를 제외한 Shader들을 렌더링한다.
		m_pCamera->SetLookVector(XMFLOAT3(-m_pCamera->GetLookVector().x, -m_pCamera->GetLookVector().y, -m_pCamera->GetLookVector().z));
		m_pCamera->RegenerateViewMatrix();
		m_pScene->Render(m_pd3dCommandList, m_pCamera);
	
		


		DWORD nCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
		if (nCameraMode == THIRD_PERSON_CAMERA) {

			m_pPlayer->Render(m_pd3dCommandList, m_pCamera);

		}
		

		if (m_bSobel == 1 || m_bSobel ==2)
		{
			m_pSobelShader->SobelFilter(m_pd3dCommandList, m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex], m_ppd3dSobelScreen, m_pCamera);
	
			if (nCameraMode == SPACESHIP_CAMERA)
			{
				m_pTargetUIShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer);
				m_pUIShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer);
				//m_pPostProcessingShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer);

			}
			if (m_bSobel == 2)
			{
				m_pSobelShader->Render(m_pd3dCommandList, m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex], m_ppd3dSobelScreen, m_pCamera);
			}
		}


		if (m_bBlurring)
		{
			m_pBlurShader->Render(m_pd3dCommandList, m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex], m_ppd3dScreenBlur, m_pCamera, 4);
		}


		if (nCameraMode == SPACESHIP_CAMERA)
		{
			m_pTargetUIShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer);
			m_pUIShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer);
			m_pPostProcessingShader->Render(m_pd3dCommandList, m_pCamera, m_pPlayer);

		}


	}

	::SynchronizeResourceTransition(m_pd3dCommandList,m_ppd3dSwapChainBackBuffers[m_nSwapChainBufferIndex],D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PRESENT);



	// 렌더타겟
	//hResult = m_pd3dCommandList2->Close();
	hResult = m_pd3dCommandList->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	
#ifdef _WITH_PRESENT_PARAMETERS
	DXGI_PRESENT_PARAMETERS dxgiPresentParameters;
	dxgiPresentParameters.DirtyRectsCount = 0;
	dxgiPresentParameters.pDirtyRects = NULL;
	dxgiPresentParameters.pScrollRect = NULL;
	dxgiPresentParameters.pScrollOffset = NULL;
	m_pdxgiSwapChain->Present1(1, 0, &dxgiPresentParameters);
#else
#ifdef _WITH_SYNCH_SWAPCHAIN
	m_pdxgiSwapChain->Present(1, 0);
#else
	m_pdxgiSwapChain->Present(0, 0);
#endif
#endif
	//m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	MoveToNextFrame();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);

	
	::SetWindowText(m_hWnd, m_pszFrameRate);

}




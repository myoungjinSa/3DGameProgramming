//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once
#include "Shader.h"



struct FOGFACTORS
{
	float gFogMode;
	float gFogStart;
	float gFogEnd;
	float gFogDensity;
};
struct FOG
{
	XMFLOAT4 FogColor;
	FOGFACTORS FogFactor;
};

struct LIGHT
{
	XMFLOAT4		m_xmf4Ambient;
	XMFLOAT4		m_xmf4Diffuse;
	XMFLOAT4		m_xmf4Specular;
	XMFLOAT3		m_xmf3Position;
	float			m_fFalloff;
	XMFLOAT3		m_xmf3Direction;
	float			m_fTheta;		//cos(m_fTheta)
	XMFLOAT3		m_xmf3Attenuation;
	float			m_fPhi;			//cos(m_fphi)
	bool			m_bEnable;
	int				m_nType;
	float			m_fRange;
	float			padding;
};

struct LIGHTS
{
	LIGHT			m_pLights[MAX_LIGHTS];
	XMFLOAT4		m_xmf4GlobalAmbient;
};

struct MATERIAL
{
	XMFLOAT4		m_xmf4Amibent;
	XMFLOAT4		m_xmf4Diffuse;
	XMFLOAT4		m_xmf4Specular;			//(r,g,b,a=power)
	XMFLOAT4		m_xmf4Emissive;
};

struct MATERIALS
{
	MATERIAL		m_pReflections[MAX_MATERIALS];
};

class CPlayer;

class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildLightsAndMaterials();
	void BuildFog();
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }
	void SetGraphicsRootSignature(ID3D12GraphicsCommandList *pd3dCommandList) { pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature); }	

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	bool ProcessInput(UCHAR *pKeysBuffer);
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList *pd3dCommandList,bool bFillmode, CCamera *pCamera=NULL);

	void ReleaseUploadBuffers();


	CHeightMapTerrain *GetPlaneTerrain() { return(m_pPlaneTerrain); }
	CHeightMapTerrain *GetTerrain() { return(m_pTerrain); }


	CPlayer						*m_pPlayer = NULL;

	static UINT					keyCallCount;		//사용자의 키 입력 횟수 GameFrameWork에서 호출 예정
protected:
	CFrameObject				**m_ppFrameObjects = NULL;
	int							m_nFrameObjects = 0;

	
	
	CShader						**m_ppShaders = NULL;
	int							m_nShaders = 0;

	
	CHeightMapTerrain			*m_pTerrain = NULL;
	CHeightMapTerrain			*m_pPlaneTerrain = NULL;
	CSkyBox						*m_pSkyBox = NULL;

	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;	//루트 파라미터의 배열이 루트 시그니쳐이다.

	FOG							*m_pFog = NULL;

	ID3D12Resource				*m_pd3dcbFog = NULL;
	FOG							*m_pcbMappedFog = NULL;

	LIGHTS						*m_pLights = NULL;
	//UINT						m_NumLights = 0;

	ID3D12Resource				*m_pd3dcbLights = NULL;
	LIGHTS						*m_pcbMappedLights = NULL;

	MATERIALS					*m_pMaterials = NULL;

	ID3D12Resource				*m_pd3dcbMaterials = NULL;
	MATERIAL					*m_pcbMappedMaterials = NULL;

};

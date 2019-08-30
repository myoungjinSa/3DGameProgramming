//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once
#include "Shader.h"


class CPlayer;

class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

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
    void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	void ReleaseUploadBuffers();


	CHeightMapTerrain *GetPlaneTerrain() { return(m_pPlaneTerrain); }
	CHeightMapTerrain *GetTerrain() { return(m_pTerrain); }


	CPlayer						*m_pPlayer = NULL;

	
protected:
	CShader						**m_ppShaders = NULL;
	int							m_nShaders = 0;

	
	CHeightMapTerrain			*m_pTerrain = NULL;
	CHeightMapTerrain			*m_pPlaneTerrain = NULL;
	CSkyBox						*m_pSkyBox = NULL;

	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;	//��Ʈ �Ķ������ �迭�� ��Ʈ �ñ״����̴�.
};

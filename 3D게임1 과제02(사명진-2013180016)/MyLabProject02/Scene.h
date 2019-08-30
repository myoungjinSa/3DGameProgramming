#pragma once
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"

class CScene
{
public:
	CScene(ID3D12Device *pd3dDevice);
	~CScene();
	//씬에서 마우스와 키보드 메시지를 처리한다.
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam,CPlayer* player);
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
		*pd3dCommandList,CPlayer* Player);
	void ReleaseObjects();
	bool ProcessInput(UCHAR *pKeysBuffer);
	void AnimateObjects(float fTimeElapsed, CPlayer* player);
	void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	void ReleaseUploadBuffers();


	//virtual void CheckObjectByObjectCollisions();

	//그래픽 루트 시그너쳐를 생성한다.
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature();



	CGameObject* CScene::PickObjectByCursor(int xClient, int yClient, CCamera *pCamera);


protected:
	//씬은 게임 객체들의 집합이다. 게임 객체는 셰이더를 포함한다.
	ID3D12RootSignature *m_pd3dGraphicsRootSignature = NULL;

	//배치(Batch) 처리를 하기 위하여 씬을 셰이더들의 리스트로 표현한다.
	CObjectsShader *m_pShaders = NULL;
	int m_nShaders = 0;

};
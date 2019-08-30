//-----------------------------------------------------------------------------
// File: Shader.h
//-----------------------------------------------------------------------------

#pragma once

#include "FrameObject.h"
#include "Object.h"
#include "Camera.h"
#include "BillboardObject.h"
#include "Player.h"



class CShader
{
public:
	CShader();
	virtual ~CShader();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	void CreateCbvAndSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);
	void CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	void CreateShaderResourceViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL) { }
	virtual void AnimateObjects(float fTimeElapsed) {};
	virtual void ReleaseObjects() { }

	virtual void ReleaseUploadBuffers();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum) ;
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum ,CCamera *pCamera=NULL);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
	//virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum, CCamera *pCamera);


	//cpu,gpu 디스크립터 핸들을 반환해주는 함수가 각각 필요 
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return(m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() { return(m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart()); }

	//상수 버퍼용 디스크립터 핸들과 쉐이더 리소스용 디스크립터 핸들 필요
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() { return(m_d3dCbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() { return(m_d3dCbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() { return(m_d3dSrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() { return(m_d3dSrvGPUDescriptorStartHandle); }

protected:
	//ID3D12PipelineState				*m_pd3dPipelineState = NULL;				//draw함수를 호출할때 어떤 리소스를 어떻게 읽는지를 정한다.

	ID3D12PipelineState  **m_ppd3dPipelineStates = NULL;
	int m_nPipelineStates = 0;
	//서술자 힙 cbv,srv용 
	ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap=NULL;			//cbv,srv의 서술자 힙

	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dCbvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDescriptorStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dSrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGPUDescriptorStartHandle;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CPlayerShader : public CShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTexturedShader : public CShader
{
public:
	CTexturedShader();
	virtual ~CTexturedShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _WITH_BATCH_MATERIAL
class CTerrainShader : public CTexturedShader
{
public:
	CTerrainShader();
	virtual ~CTerrainShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(bool bFillmode);
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometeryShader(ID3DBlob **ppd3dShaderBlob);
	
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,UINT nParts);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum, CCamera *pCamera=NULL);
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum);
	//virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob **ppd3dShaderBlob);

	//virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

public:

	int m_nParts = 0;
	enum PIPELINESTATES  {SOLID=0,WIREFRAME,GEOMETRYSHADER,GEOMETRYSOLID};

//public:
//
//	ID3D12Resource					* m_pd3dcbTerrainParts = NULL;
//	CB_T				*m_pcbMappedTerrainPartition = NULL;
//
//protected:
//	ID3D12DescriptorHeap * m_pd3dCbvSrvDetailDescriptorHeap = NULL;							//디테일 텍스쳐 저장 힙
//
//
//	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dCbvCPUDetailDescriptorStartHandle;					//cpu와 gpu가 접근가능한 핸들
//	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDetailDescriptorStartHandle;
//	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dSrvCPUDetailDescriptorStartHandle;
//	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGPUDetailDescriptorStartHandle;
};

///////////////////////////////////////////////////////////////////////////////////
//CIlluminatedShader

class CIlluminatedShader : public CPlayerShader
{
public:
	CIlluminatedShader();
	virtual ~CIlluminatedShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(bool bFillmode);
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int nPipelineState = 0);


};


/////////////////////////////////////////////////////////////////////////////
//InstancingShader


class CInstancingShader : public CTexturedShader
{
public:
	CInstancingShader();
	virtual ~CInstancingShader();

	virtual void AnimateObjects(float fTimeElapsed, CCamera *pCamera);
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,void* pContext =NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum,CCamera *pCamera);


	//void RenderInstancing(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT nInstances);
protected:
	CGameObject **m_ppObjects = 0;
	int								m_nObjects = 0;
	//인스턴스 정점 버퍼와 정점 버퍼 뷰이다.
	ID3D12Resource * m_pd3dcbGameObjects = NULL;
	VS_VB_INSTANCE  *m_pcbMappedGameObjects = NULL;

	D3D12_VERTEX_BUFFER_VIEW  m_d3dInstancingBufferView;


#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_pMaterial = NULL;
#endif

};
/////////////////////////////////////////////////////////////////////////////
//billboardShader


class CBillboardShader :public CTexturedShader
{
public:
	CBillboardShader();
	virtual ~CBillboardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed,CCamera *pCamera);
	virtual void ReleaseObjects();


	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum, CCamera *pCamera);

protected:
	CGameObject **m_ppObjects = 0;
	int								m_nObjects = 0;

	ID3D12Resource					*m_pd3dcbBillboardObjects = NULL;
	CB_GAMEOBJECT_INFO				*m_pcbMappedBillboardObjects = NULL;

#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_pMaterial = NULL;
#endif

};

//////////////////////////////////////////
//CGrassBillboardShader

//좀 더 직관적인 클래스 구조를 위해 Tree, grass 빌보드를 동일 계층으로 두었다.
// CTexturedShader-> CBillboardShader , CTexturedShader -> CGrassBillboardShader

class CGrassBillboardShader : public CTexturedShader
{
public:
	CGrassBillboardShader();
	virtual ~CGrassBillboardShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed, CCamera *pCamera,CPlayer *pPlayer);
	virtual void ReleaseObjects();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum,CCamera *pCamera);


	
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
protected:
	CGameObject **m_ppObjects = 0;
	int								m_nObjects = 0;

	ID3D12Resource					*m_pd3dcbBillboardObjects = NULL;
	CB_GAMEOBJECT_INFO				*m_pcbMappedBillboardObjects = NULL;


	//ID3D12Resource					*m_pd3dcbTextureTransform = NULL;
	//CB_TEXTURE_MATRIX				*m_pcbMappedTextureTransform = NULL;
	//////텍스쳐 좌표 변환 행렬
	//ID3D12Resource					* m_pd3dcbTexcoord = NULL;
	//CB_TEXTURE_COORD				*m_pcbTransformTexcoord = NULL;


#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_pMaterial = NULL;
#endif

};

//////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGeometeryBillboardShader : public CTexturedShader
{
public :
	CGeometeryBillboardShader();
	virtual ~CGeometeryBillboardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometeryShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();
	//virtual void ReleaseObjects();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dDevice,UINT rasterNum,CCamera* pCamera);
protected:

	//CGameObject **m_ppObjects = 0;
	//int								m_nObjects = 0;
	////인스턴스 정점 버퍼와 정점 버퍼 뷰이다.
	//ID3D12Resource * m_pd3dcbGameObjects = NULL;
	//CTreeVertex  *m_pcbMappedGameObjects = NULL;
	
	int m_nStride = 0;
	int m_nVertices = 0;

	/*Edit*///CTreeVertex * pTreeVertices = NULL;

	ID3D12Resource* m_pd3dVertexBuffer;
	ID3D12Resource* m_pd3dVertexUploadBuffer;

	////인스턴스 정점 버퍼와 정점 버퍼 뷰이다.
	//ID3D12Resource * m_pd3dcbGameObjects = NULL;
	//VS_VB_INSTANCE  *m_pcbMappedGameObjects = NULL;

	D3D12_VERTEX_BUFFER_VIEW m_pd3dVertexBufferView;

#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_ppMaterial = NULL;
#endif
};



/////////////////////////////////////////////////////////

class CSkyBoxShader :public CTexturedShader
{
public:
	CSkyBoxShader();
	virtual ~CSkyBoxShader();

	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

};




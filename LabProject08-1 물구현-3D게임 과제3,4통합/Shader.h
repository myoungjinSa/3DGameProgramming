//-----------------------------------------------------------------------------
// File: Shader.h
//-----------------------------------------------------------------------------

#pragma once

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

	virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature,UINT nRenderTarget){}

	void CreateCbvAndSrvAndUavDesciptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAccessViews);
	void CreateCbvAndSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);
	void CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
	void CreateShaderResourceViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement);
	void CreateSrvAndUavViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pTexture1, UINT nRootParameterStartIndex1, UINT nRootParameterStartIndex2);

	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice){}

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL) { }
	//virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera){}

	virtual void AnimateObjects(float fTimeElapsed, CCamera *pCamera) {}
	virtual void AnimateObjects(float fTimeElapsed, CPlayer *pPlayer,CCamera *pCamera =NULL){}
	virtual void ReleaseObjects() { }

	virtual void ReleaseUploadBuffers();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,bool bSolid){}
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList, int index){}
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum);

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,  CCamera *pCamera);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,CCamera *pCamera,CPlayer *pPlayer);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum, CCamera *pCamera = NULL);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, D3D12_CPU_DESCRIPTOR_HANDLE& DepthStencilBufferCPUHandle){}
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, bool bSolid);


	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	//cpu,gpu 디스크립터 핸들을 반환해주는 함수가 각각 필요 
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return(m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() { return(m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart()); }

	//상수 버퍼용 디스크립터 핸들과 쉐이더 리소스용 디스크립터 핸들 필요
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvDescriptorStartHandle() { return(m_d3dCbvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvDescriptorStartHandle() { return(m_d3dCbvGPUDescriptorStartHandle); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSrvDescriptorStartHandle() { return(m_d3dSrvCPUDescriptorStartHandle); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSrvDescriptorStartHandle() { return(m_d3dSrvGPUDescriptorStartHandle); }
	
	
protected:
				//draw함수를 호출할때 어떤 리소스를 어떻게 읽는지를 정한다.

	ID3D12PipelineState  **m_ppd3dPipelineStates = NULL;
	int m_nPipelineStates = 0;
	//int									m_nPipelineStates = 0;
	//ID3D12PipelineState					*m_pd3dPipelineState = NULL;
	//서술자 힙 cbv,srv용 
	ID3D12DescriptorHeap			*m_pd3dCbvSrvDescriptorHeap = NULL;			//cbv,srv의 서술자 힙
	ID3D12DescriptorHeap			*m_pd3dCbvSrvUavDescriptorHeap = NULL;

	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dCbvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDescriptorStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dSrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGPUDescriptorStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE		m_d3dUavCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dUavGPUDescriptorStartHandle;

	ID3D12RootSignature				*m_pd3dGraphicsRootSignature = NULL;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CPlayerShader : public CShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

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
#define _WITH_BATCH_MATERIAL

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CIlluminatedTexturedShader : public CTexturedShader
{
public:
	CIlluminatedTexturedShader();
	virtual ~CIlluminatedTexturedShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTerrainShader : public CTexturedShader
{
public:
	CTerrainShader();
	virtual ~CTerrainShader();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,bool bSolid);
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(bool bSolid);


};


class CWaterShader : public CTexturedShader
{
public:
	CWaterShader();
	virtual ~CWaterShader();


	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob **ppd3dShaderBlob);



	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(bool bSolid);


	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,bool bWireFrame);

};
//class CTerrainTessellationShader : public CTerrainShader
//{
//public:
//	CTerrainTessellationShader();
//	virtual ~CTerrainTessellationShader();
//
//
//	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
//	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
//	virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob **ppd3dShaderBlob);
//	virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob **ppd3dShaderBlob);
//
//	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
//};


//////////////////////////////////////////////////////////////////////////////////////
class CGeometryBillboardShader : public CTexturedShader
{
public:
	CGeometryBillboardShader();
	virtual ~CGeometryBillboardShader();

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
	virtual void Render(ID3D12GraphicsCommandList *pd3dDevice,CCamera* pCamera, CPlayer *pPlayer );
protected:

	

	int m_nStride = 0;
	int m_nVertices = 0;


	ID3D12Resource* m_pd3dVertexBuffer;
	ID3D12Resource* m_pd3dVertexUploadBuffer;



	D3D12_VERTEX_BUFFER_VIEW m_pd3dVertexBufferView;

#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_ppMaterial = NULL;
#endif
};



//////////////////////////////////////////////////////////////////////////////////////

class CMissileShader : public CTexturedShader
{
public:
	CMissileShader();
	virtual ~CMissileShader();

	//virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList, int index);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	//virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int index);

	virtual void ReleaseShaderVariables();

	virtual void AnimateObjects(float fTimeElapsed,CPlayer *pPlayer, CCamera *pCamera = NULL);
	virtual void ReleaseObjects();
	virtual void ReleaseUploadBuffers();


	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,  CCamera *pCamera, CPlayer *pPlayer );

	//virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob, int bExplosion);
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_BLEND_DESC CreateBlendState();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);

	//CMissileObject** GetppMissileArray();
public:
	int status = CMissileObject::eStatus::SHOT;

	static CMissileObject **ppMissile;
	int m_nMissile = 0;
	float m_fExpolsiveTime;
	//std::map<CMissileObject**, CExplosionBillboardObject**> m_MappedObject;
	int m_nExplosion = 0;
	CExplosionBillboardObject					**m_ppExplosion = NULL;
	CHeightMapTerrain							*m_pTerrain = NULL;
protected:


	//ID3D12Resource					*m_pd3dcbExplosionObjects = NULL;
	//CB_GAMEOBJECT_INFO				*m_pcbMappedExplosionObjects = NULL;


	ID3D12Resource					*m_pd3dcbMissileObjects = NULL;
	CB_GAMEOBJECT_INFO				*m_pcbMappedMissileObjects = NULL;

#ifdef _WITH_BATCH_MATERIAL
	CMaterial						*m_pMaterial = NULL;
#endif
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
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,void* pContext =NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);


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
#define _WITH_BATCH_MATERIAL

class CBillboardShader :public CTexturedShader
{
public:
	CBillboardShader();
	virtual ~CBillboardShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed,CCamera *pCamera);
	virtual void ReleaseObjects();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

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

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);


	
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
/////////////////////////////////////////////////////////////////////////////
class CTexturedUIShader : public CTexturedShader
{
public:
	CTexturedUIShader();
	virtual ~CTexturedUIShader();

	//virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,void *pContext =NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer);

	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual D3D12_BLEND_DESC CreateBlendState();

	//virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);


protected:
	CTexture						**m_pTexture = NULL;
	int								m_nTexture = 0;
};

class CTexturedMenuUIShader : public CTexturedUIShader
{
public:
	CTexturedMenuUIShader();
	virtual ~CTexturedMenuUIShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer,int gamestate);

	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual D3D12_BLEND_DESC CreateBlendState();

	//virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
protected:
	CTexture						**m_pTexture = NULL;
	int								m_nTexture = 0;
};

class CTexturedTargetUIShader : public CTexturedUIShader
{
public:
	CTexturedTargetUIShader();
	virtual ~CTexturedTargetUIShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer);
	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual D3D12_BLEND_DESC CreateBlendState();

	//virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);


protected:
	CTexture						**m_pTexture = NULL;
	int								m_nTexture = 0;

};


/////////////////////////////////////////////////////////

class CSkyBoxShader :public CTexturedShader
{
public:
	CSkyBoxShader();
	virtual ~CSkyBoxShader();

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);


	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
	//virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	
};


////////////////////////////////////////////////////////////////////////////////////////////

class CStandardShader : public CShader
{
public:
	CStandardShader();
	virtual ~CStandardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
};

class CFrameShader : public CStandardShader
{
public:
	CFrameShader();
	virtual ~CFrameShader();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext = NULL);
	virtual void AnimateObjects(float fTimeElapsed,CPlayer*pPlayer,CCamera *pCamera =NULL);
	virtual void ReleaseObjects();

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer);

	CFrameObject*	GetObjects(int index)
	{
		if (m_ppObjects[index])
		{
			return m_ppObjects[index];
		}
		else {
			return nullptr;
		}
	}

	int GetObjectCount() { return m_nObjects; }
protected:
	CFrameObject					**m_ppObjects = 0;
	int								m_nObjects = 0;
};

/////////////////////////////////////////////////////////////////
class CPostProcessingShader : public CShader
{
public:
	CPostProcessingShader();
	virtual ~CPostProcessingShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets);

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void ReleaseObjects();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera,CPlayer *pPlayer);
protected:
	CTexture						**m_pTexture = NULL;
	int								m_nTexture = 0;
};




class CComputeShader : public CPostProcessingShader
{
public:

	CComputeShader();
	virtual ~CComputeShader();


	std::vector<float> CalcGaussWeights(float sigma);
	//virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	//void CreateBlurTargetSrvUavView(ID3D12Device *pd3dDevice,ID3D12GraphicsCommandList *pd3dCommandList);
	//void CreateSrvUavDescriptorHeaps(ID3D12Device *pd3dDevice);
	//virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets);

	virtual D3D12_SHADER_BYTECODE CreateComputeShader(ID3DBlob **ppd3dShaderBlob, bool bHorizon);

	//void BuildDescriptors(ID3D12Device *pd3dDevice);
	//void BuildResources(ID3D12Device *pd3dDevice, float wndClientWidth, float wndClientHeight);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pContext);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pRenderTexture, ID3D12Resource **pDestTexture, CCamera* pCamera, int blurCount = 4);

protected:
	float m_fGaussianWeights[11];


	CTexture						**m_pTexture = NULL;
	int								m_nTexture = 0;

	UINT m_width = 0;
	UINT m_height = 0;

	const int MaxBlurRadius = 5;


	
	UINT						m_nSrvUavDescriptorIncrementSize;

	ID3D12RootSignature				*m_pd3dComputeRootSignature = NULL;
	


};

////////////////////////////////////////////////////////////////////////////
class CSobelComputeShader : public CShader
{
public:
	CSobelComputeShader();
	virtual ~CSobelComputeShader();

	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets);
	virtual D3D12_SHADER_BYTECODE CreateComputeShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **pd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **pd3dShaderBlob);


	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pContext);
	void SobelFilter(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pRenderTexture, ID3D12Resource **pDestTexture, CCamera* pCamera);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pRenderTexture, ID3D12Resource **pDestTexture, CCamera* pCamera);


protected:
	ID3D12RootSignature				*m_pd3dComputeRootSignature = NULL;

	UINT							m_nSrvUavDescriptorIncrementSize;

	CTexture						**m_pTexture = NULL;
	int								m_nTexture = 0;

	UINT m_width = 0;
	UINT m_height = 0;
};



////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CartoonShading
class CCartoonShader : public CPostProcessingShader
{
public:
	CCartoonShader();
	virtual ~CCartoonShader();

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature ,UINT nRenderTargets);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext=NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer);


};


class CCartoonShaderByLaplacianShader : public CCartoonShader
{
public:
	CCartoonShaderByLaplacianShader();
	virtual ~CCartoonShaderByLaplacianShader();


	

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
};

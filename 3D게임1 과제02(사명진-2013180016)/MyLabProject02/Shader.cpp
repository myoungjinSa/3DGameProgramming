#include "stdafx.h"
#include "Shader.h"
#include "Player.h"




//============================================================================

CShader::CShader()
{
}

//래스터라이저 상태를 설정하기 위한 구조체를 반환한다.
D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;		//  컬링 됨.
	//d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;	//	컬링 안됨.

	d3dRasterizerDesc.FrontCounterClockwise = FALSE;		//	전면이 반시계 방향
	d3dRasterizerDesc.DepthBias = 0;						//	픽셀의 깊이값에 더해질 깊이 바이어스 값(0)
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;				//	픽셀의 깊이값에 더해질 최대값
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;			//	픽셀의 기울기에 따라 바이어스를 조절할 값
	d3dRasterizerDesc.DepthClipEnable = TRUE;				//	픽셀의 깊이값을 사용한 클리핑(TRUE)
	d3dRasterizerDesc.MultisampleEnable = FALSE;			//	다중 샘플링(MSAA)사용(FALSE)
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;				//	UAV렌더링에 사용할 샘플의 개수(0,1,2,4,8,16)
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;		//보수적 래스터라이제이션 사용(OFF)
	return(d3dRasterizerDesc);
}

//깊이-스텐실 검사를 위한 상태를 설정하기 위한 구조체를 반환한다.
D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	return(d3dDepthStencilDesc);
}

//블렌딩 상태를 설정하기 위한 구조체를 반환한다.
D3D12_BLEND_DESC CShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //모든 요소가 데이터로 저장되는 것을 허용
	return(d3dBlendDesc);
}

//입력 조립기에게 정점 버퍼의 구조를 알려주기 위한 구조체를 반환한다.
D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = NULL;		//입력 원소들의 배열
	d3dInputLayoutDesc.NumElements = 0;					//입력 원소의 개수
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;	//pd3dVertexShaderBlob->GetBufferPointer();
	d3dShaderByteCode.pShaderBytecode = NULL;	//pd3dVertexShaderBlob->GetBufferSize();
	return(d3dShaderByteCode);
}
D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;	//pd3dPixelShader->GetBufferPointer();
	d3dShaderByteCode.pShaderBytecode = NULL;	//pd3dPixelShader->GetBufferSize();
	return(d3dShaderByteCode);
}

//셰이더 소스 코드를 컴파일하여 바이트 코드 구조체를 반환한다.
D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR
	pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile,
		nCompileFlags, 0, ppd3dShaderBlob, NULL);
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
	return(d3dShaderByteCode);
}

//그래픽스 파이프라인 상태 객체를 생성한다.
void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature)
{

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;										//렌더 타겟 수
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		__uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	
	//메모리가 있으면 메모리 해제
	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
	// 입력 버퍼 해제 
	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다.
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList);
}

void CShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList)
{
}
void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}
//상수버퍼에 올라갈 내용 업데이트
void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4
	*pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);


}
void CShader::ReleaseShaderVariables()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

CShader::~CShader()
{
	if (m_ppd3dPipelineStates)
	{
		for (int i = 0; i < m_nPipelineStates; i++) if (m_ppd3dPipelineStates[i])
			m_ppd3dPipelineStates[i]->Release();
		delete[] m_ppd3dPipelineStates;
	}
}

CPlayerShader::CPlayerShader()
{
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));
}

void CPlayerShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

CPlayerShader::~CPlayerShader()
{
}

CObjectsShader::CObjectsShader()
{
}
CObjectsShader::~CObjectsShader()
{
}

D3D12_INPUT_LAYOUT_DESC CObjectsShader::CreateInputLayout()
{
	//입력 조립기 단계에 연결되는 입력 버퍼의 구조를 표현
	// 정점 데이터의 구조 또는 인스턴스 데이터의 구조를 표현
	
	//정점 쉐이더의 입력 시그니쳐와 일치해야한다.
	//입력 버퍼(배열)의 모든 원소들은 같은 자료 구조(자료형)을 가져야 함.

	//입력 데이터는 하나 또는 여러 개의 버퍼로 표현되고 조립될 수 있음.

	//최대 16개의 정점 버퍼를 연결할 수 있다.

	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CObjectsShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",
		ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CObjectsShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1",
		ppd3dShaderBlob));
}


void CObjectsShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature
	*pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;			//파이프라인 상태 객체의 수
	//파이프라인 상태객체의 포인터를 갖고있는 배열
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}





void CObjectsShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList,CPlayer* Player)
{
	float fHalfWidth = 90.0f, fHalfHeight = 90.0f, fHalfDepth = 500.0f;
	
	CExplosiveObject::PrepareExplosion(pd3dDevice, pd3dCommandList);
	CCubeMeshDiffused *pCubeMesh= new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 4.0f, 4.0f, 4.0f);
	
	
	m_nObjects = MAX_BOX_COUNT;
	m_ppObjects = new CGameObject*[MAX_BOX_COUNT];

	m_nObjects = 1;

	CExplosiveObject *pExplosiveObject = NULL;
	

	pExplosiveObject = new CExplosiveObject(Player);

	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	pExplosiveObject->SetPosition(RandF(-fHalfWidth, fHalfWidth), RandF(-fHalfHeight, fHalfHeight), RandF(0, fHalfDepth*2.0f));
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(90.0f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));


	m_ppObjects[0] = pExplosiveObject;

	for (int i = 1; i < MAX_BOX_COUNT; i++) {
		pExplosiveObject = new CExplosiveObject(Player);
		pExplosiveObject->SetMesh(pCubeMesh);
		pExplosiveObject->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		pExplosiveObject->SetRotationSpeed(90.0f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));
		m_ppObjects[i] = pExplosiveObject;
	}
	


	// 객체 생성 구역
	// 벽그리기

	

	CWallMeshDiffused *pWwallMesh = new CWallMeshDiffused(pd3dDevice, pd3dCommandList,
		fHalfWidth *2.0f, fHalfHeight *2.0f, fHalfDepth *2.0f);
	//m_ppWall = new CGameObject*[20];
	

	
	pWallObject = new CWallsObject();
	pWallObject->SetMesh(pWwallMesh);
	//각 정육면체 객체의 위치를 설정한다. 

	// 벽의 z방향 깊이는 많이 깊어야 하므로 
	pWallObject->SetOOBB(XMFLOAT3(0.0f, 0.0f, 500.0f), XMFLOAT3(fHalfWidth,fHalfHeight,fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	pWallObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	pWallObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	pWallObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	pWallObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	pWallObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	pWallObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);
	//pRotatingWallObject->SetPosition(0.0f, 0.0f, 0.0f);

	//cout << m_ppWall->GetPosition().x <<","<< m_ppWall->GetPosition().y <<","<< m_ppWall->GetPosition().z<<","<<endl;
	

	pBullet = new CBulletObject*[CPlayer::m_countBullet];
	pBullet = Player->m_pBulletObjects;

	


	//Boss
	CCubeMeshDiffused *pBossCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 15.0f, 15.0f, 15.0f);

	m_pBossObject = new CBoss(Player);
	m_pBossObject->SetMesh(pBossCubeMesh);
	m_pBossObject->SetOOBB(XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(7.5f,7.5f,7.5f),XMFLOAT4(0.0f,0.0f,0.0f,1.0f));
	m_pBossObject->SetPosition(0.0f, 0.0f, 800.0f);
	m_pBossObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pBossObject->SetRotationSpeed(120.0f);
	m_pBossObject->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 0.0f));

	m_pBossObject->m_pBulletObjects = new CBulletObject*[CBoss::m_countBullet];

	CBulletMeshDiffused* pBulletObject= new CBulletMeshDiffused(pd3dDevice, pd3dCommandList, 1.0f, 1.0f, 5.0f);

	for (int i = 0; i < CBoss::m_countBullet; i++) {
		m_pBossObject->m_pBulletObjects[i] = new CBulletObject();
		m_pBossObject->m_pBulletObjects[i]->SetMesh(pBulletObject);
		//m_pBossObject->m_pBulletObjects[i]->SetOOBB(m_pBossObject->GetPosition(), XMFLOAT3(0.5f, 0.5f, 2.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pBossObject->m_pBulletObjects[i]->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
		m_pBossObject->m_pBulletObjects[i]->SetRotationSpeed(5000.0f);
		m_pBossObject->m_pBulletObjects[i]->SetMovingSpeed(120.0f);

	}
	

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

}

void CObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++)
		{
			if (m_ppObjects[j]) delete m_ppObjects[j];
		}
		delete[] m_ppObjects;
	}
	if (pWallObject) {
		delete pWallObject;
		pWallObject = NULL;
	}
	if (m_pBossObject) {
		if (m_pBossObject->m_pBulletObjects) {
			for (int i = 0; i < CBoss::m_countBullet; i++) {
				delete m_pBossObject->m_pBulletObjects[i];
				m_pBossObject->m_pBulletObjects[i] = NULL;
			}
		}
		delete m_pBossObject;
	}
}

void CObjectsShader::CheckObjectByWallCollisions(CPlayer *player)
{

	for (int i = 0; i < m_nObjects; i++) {
		ContainmentType containType = pWallObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType) 
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++) {
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&pWallObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK) {
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(pWallObject->m_pxmf4WallPlanes[nPlaneIndex].x, pWallObject->m_pxmf4WallPlanes[nPlaneIndex].y, pWallObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&pWallObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(pWallObject->m_pxmf4WallPlanes[nPlaneIndex].x, pWallObject->m_pxmf4WallPlanes[nPlaneIndex].y, pWallObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
			break;
		case CONTAINS:
			//cout << "포함" << endl;

			break;
		}
	}

	ContainmentType containType = pWallObject->m_xmOOBB.Contains(player->m_xmOOBB);
	switch (containType) {
	case DISJOINT:
		
		break;
	case INTERSECTS:
		
		InitAll(player);
		break;
	case CONTAINS:
		break;
	}

}

void CObjectsShader::CheckObjectByObjectCollisions() {
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;

	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}

	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}



}

void CObjectsShader::CheckObjectByPlayerCollisions(CPlayer *player) {
	for (int i = 0; i < m_nObjects; i++) {
		ContainmentType containType = m_ppObjects[i]->m_xmOOBB.Contains(player->m_xmOOBB);
		switch (containType) {
		case DISJOINT:
			
			break;

		case INTERSECTS:
			InitAll(player);
			break;
		case CONTAINS:
			break;

		}
	}
}

void CObjectsShader::CheckObjectByBulletCollisions(CPlayer *player) {
	
	for (int j = 0; j < CPlayer::m_currentBullet; j++) {
		//ContainmentType containType = m_ppObjects[i]->m_xmOOBB.Contains(player->m_pBulletObjects[j]->m_xmOOBB);
		for (int i = 0; i < m_nObjects; i++) {
			if (m_ppObjects[i]->m_xmOOBB.Intersects(player->m_pBulletObjects[j]->m_xmOOBB))
			{
				dynamic_cast<CExplosiveObject*>(m_ppObjects[i])->m_bBlowingUp = true;
			}
		}
		//보스와 충돌 처리
		if (m_pBossObject->m_xmOOBB.Intersects(player->m_pBulletObjects[j]->m_xmOOBB)) {
			m_pBossObject->m_bBlowingUp = true;
		}
	}


	for (int i = 0; i < CBoss::m_currentBullet; i++) {

		ContainmentType containType = m_pBossObject->m_pBulletObjects[i]->m_xmOOBB.Contains(player->m_xmOOBB);
		switch (containType) {
		case DISJOINT:
			//cout << "안만남" << endl;
			break;
		case INTERSECTS:
			InitAll(player);
		
			break;
		case CONTAINS:
			//cout << "포함" << endl;
			break;

		}

	}
	
}
void CObjectsShader::GenerateBoxObject(int num,CPlayer *player) {
	
	if (m_nObjects < MAX_BOX_COUNT - 1) {
		
		m_nObjects += 1;

		m_ppObjects[num]->SetPosition(RandF(-100.0f,100.0f), RandF(-100.0f,100.0f), RandF(player->GetPosition().z,1000));
	}
}
void CObjectsShader::AnimateObjects(float fTimeElapsed, CPlayer* player)
{


	m_fElapsedTimes += fTimeElapsed;

	
	if (m_fElapsedTimes > 1.0f) {
		GenerateBoxObject(m_nObjects, player);
		m_fElapsedTimes = 0.0f;
		
	}

	// 이거 충돌체크 해주는 부분
	player->Animate(fTimeElapsed);
	for (int i = 0; i < CPlayer::m_currentBullet; i++) {
		player->m_pBulletObjects[i]->Animate(fTimeElapsed, player->GetPosition());
	}
	
	for (int i = 0; i < m_nObjects; ++i) {
		dynamic_cast<CExplosiveObject*>(m_ppObjects[i])->Animate(fTimeElapsed,player);
		//m_ppObjects[i]->Animate(fTimeElapsed,player);
	}
	//cout << m_ppObjects[0]->GetPosition().x <<','<< m_ppObjects[0]->GetPosition().y <<','<< m_ppObjects[0]->GetPosition().z <<","<< endl;
	//cout << m_ppObjects[1]->GetPosition().x << m_ppObjects[1]->GetPosition().y << m_ppObjects[1]->GetPosition().z << endl;
	
	
	pWallObject->Animate(fTimeElapsed);
	
	
	

	m_pBossObject->Animate(fTimeElapsed, player);
	
	for (int i = 0; i < CBoss::m_currentBullet; i++) {
		m_pBossObject->m_pBulletObjects[i]->Animate(fTimeElapsed, m_pBossObject->GetPosition());
		
	}
	for (int i = 0; i < CPlayer::m_currentBullet; i++) {
		pBullet[i]->Animate(fTimeElapsed, player->GetPosition());
	}

	// 벽과 플레이어 충돌체크
	CheckObjectByWallCollisions(player);
	// 박스간의 충돌 체크
	CheckObjectByObjectCollisions();
	// 총알과 적 충돌 체크
	CheckObjectByBulletCollisions(player);

	// 플레이어와 적 충돌 체크
	CheckObjectByPlayerCollisions(player);


}

void CObjectsShader::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) m_ppObjects[j]->ReleaseUploadBuffers();
	}

	
	pWallObject->ReleaseUploadBuffers();
	
}

void CObjectsShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CShader::Render(pd3dCommandList, pCamera);
	

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			if (m_ppObjects[j]->m_bActive) {
				dynamic_cast<CExplosiveObject*>(m_ppObjects[j])->Render(pd3dCommandList, pCamera);
			}
		}
	}
	
	
	pWallObject->Render(pd3dCommandList, pCamera);


	for (int i = 0; i < CPlayer::m_currentBullet; i++) {
		if (pBullet[i]) {
			if (pBullet[i]->m_fired) {
				pBullet[i]->Render(pd3dCommandList, pCamera);
			}
		}
	}

	if (m_pBossObject) {
		m_pBossObject->Render(pd3dCommandList, pCamera);
	}

	for (int i = 0; i < CBoss::m_currentBullet; i++) {
		m_pBossObject->m_pBulletObjects[i]->Render(pd3dCommandList, pCamera);
	}

}


void CObjectsShader::InitAll(CPlayer *player) {
	player->SetPosition(XMFLOAT3(0.0f, 0.0f, 100.0f));
	player->SetRightVector(XMFLOAT3(1.0f, 0.0f, 0.0f));
	player->SetUpVector( XMFLOAT3(0.0f, 1.0f, 0.0f));
	player->SetLookVector(XMFLOAT3(0.0f, 0.0f, 1.0f));
	this->m_nObjects = 1;
	m_fElapsedTimes = 0.0f;
	
	
	if (m_pBossObject->m_bActive == FALSE) {
		m_pBossObject->m_bActive = TRUE;
	}
}
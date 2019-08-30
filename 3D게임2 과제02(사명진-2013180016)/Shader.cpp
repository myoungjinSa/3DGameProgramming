//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "DDSTextureLoader12.h"


#define TEXTURES 4

CShader::CShader()
{
	m_d3dSrvCPUDescriptorStartHandle.ptr = NULL;
	m_d3dSrvGPUDescriptorStartHandle.ptr = NULL;


}

CShader::~CShader()
{
	if (m_ppd3dPipelineStates)
	{
		for (int i = 0; i < m_nPipelineStates; i++) if (m_ppd3dPipelineStates[i]) m_ppd3dPipelineStates[i]->Release();
		delete[] m_ppd3dPipelineStates;
	}
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBlob = nullptr;

	::D3DCompileFromFile(pszFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, &errorBlob);

	if (errorBlob)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}


	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = NULL;
	d3dInputLayoutDesc.NumElements = 0;

	return(d3dInputLayoutDesc);
}

D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

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
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::CreateCbvAndSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews)
{
	//리소스를 사용하려면 뷰를 만들어야한다. 
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	//서술자 힙: 리소스를 서술하는 서술자들을 저장하는 연속적인 메모리 영역(배열)
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	//디바이스(어댑터)마다 서술자 유형별 메모리 크기가다름(32~64바이트)
	// 서술자 힙을 사용하려면 서술자 배열의 원소의 크기를 알아야 함.
	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);

}

void CShader::CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride)
{
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCPUDescriptorHandle;
		d3dCbvCPUDescriptorHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * j);
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorHandle);
	}
}

D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(D3D12_RESOURCE_DESC d3dResourceDesc, UINT nTextureType)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;		//뷰가 리소스를 보는 형식
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;	//텍스쳐의 rgba를 읽는 순서를 정해준다.
	switch (nTextureType)
	{
	case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case RESOURCE_TEXTURE2D_ARRAY:
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		break;
	case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels = -1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements = 0;
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	}
	return(d3dShaderResourceViewDesc);
}

void CShader::CreateShaderResourceViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement)
{
	/*D3D12_CPU_DESCRIPTOR_HANDLE d3dSrvCPUDescriptorHandle = m_d3dSrvCPUDescriptorStartHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGPUDescriptorHandle = m_d3dSrvGPUDescriptorStartHandle;*/
	int nTextures = pTexture->GetTextures();
	int nTextureType = pTexture->GetTextureType();
	for (int i = 0; i < nTextures; i++)
	{
		ID3D12Resource *pShaderResource = pTexture->GetTexture(i);				//텍스처 리소스를 하나 얻어온다.
		D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();		//그 리소스들에 대한 정보를 얻어온다.
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);  //텍스처에 대한 서술자 뷰를 설정한다.
		pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorStartHandle);		//cpu가 접근할수있는 srv뷰를 만든다.
		m_d3dSrvCPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

		//srv뷰를 
		pTexture->SetRootArgument(i, (bAutoIncrement) ? (nRootParameterStartIndex + i) : nRootParameterStartIndex, m_d3dSrvGPUDescriptorStartHandle);
		m_d3dSrvGPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
	}
}

void CShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
}

void CShader::ReleaseShaderVariables()
{
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();
}

void CShader::ReleaseUploadBuffers()
{
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList)
{
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작
}
void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum)
{
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}

//void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, bool bFillmode,CCamera *pCamera)
//{
//	OnPrepareRender(pd3dCommandList,bFillmode);
//}
void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList,rasterNum);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs =2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	//pd3dInputElementDescs[1] ={ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}
D3D12_DEPTH_STENCIL_DESC CPlayerShader::CreateDepthStencilState()
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
D3D12_RASTERIZER_DESC CPlayerShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}
D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSPlayer", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSPlayer", "ps_5_1", ppd3dShaderBlob));
}

void CPlayerShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTexturedShader::CTexturedShader()
{
}

CTexturedShader::~CTexturedShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTexturedShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];


	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTexturedShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTextured", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTexturedShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTextured", "ps_5_1", ppd3dShaderBlob));
}

void CTexturedShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIlluminatedShader::CIlluminatedShader()
{
}

CIlluminatedShader::~CIlluminatedShader()
{
}

D3D12_INPUT_LAYOUT_DESC CIlluminatedShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CIlluminatedShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSLighting", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CIlluminatedShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSLighting", "ps_5_1", ppd3dShaderBlob));
}

void CIlluminatedShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, int nPipelineState)
{
	OnPrepareRender(pd3dCommandList, nPipelineState);
}

D3D12_RASTERIZER_DESC CIlluminatedShader::CreateRasterizerState(bool bfillmode)
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	if (bfillmode) {
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	}
	else {
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	}

	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

void CIlluminatedShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc[2];
	::ZeroMemory(d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC) * 2);
	d3dPipelineStateDesc[0].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[0].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[0].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[0].RasterizerState = CreateRasterizerState(true);
	d3dPipelineStateDesc[0].BlendState = CreateBlendState();
	d3dPipelineStateDesc[0].DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc[0].InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc[0].SampleMask = UINT_MAX;
	d3dPipelineStateDesc[0].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc[0].NumRenderTargets = 1;
	d3dPipelineStateDesc[0].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc[0].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc[0].SampleDesc.Count = 1;
	d3dPipelineStateDesc[0].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


	d3dPipelineStateDesc[1].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[1].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[1].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[1].RasterizerState = CreateRasterizerState(false);
	d3dPipelineStateDesc[1].BlendState = CreateBlendState();
	d3dPipelineStateDesc[1].DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc[1].InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc[1].SampleMask = UINT_MAX;
	d3dPipelineStateDesc[1].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc[1].NumRenderTargets = 1;
	d3dPipelineStateDesc[1].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc[1].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc[1].SampleDesc.Count = 1;
	d3dPipelineStateDesc[1].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[0], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);
	if (hResult == E_FAIL) {
		return;
	}

	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[1], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[1]);
	if (hResult == E_FAIL) {
		return;
	}


	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc[0].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[0].InputLayout.pInputElementDescs;
	if (d3dPipelineStateDesc[1].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[1].InputLayout.pInputElementDescs;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}
void CTerrainShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 4;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL, *pd3dGeoShaderBlob= NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc[4];
	::ZeroMemory(d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC) * 4);
	d3dPipelineStateDesc[0].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[0].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[0].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[0].RasterizerState = CreateRasterizerState(true);
	d3dPipelineStateDesc[0].BlendState = CreateBlendState();
	d3dPipelineStateDesc[0].DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc[0].InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc[0].SampleMask = UINT_MAX;
	d3dPipelineStateDesc[0].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc[0].NumRenderTargets = 1;
	d3dPipelineStateDesc[0].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc[0].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc[0].SampleDesc.Count = 1;
	d3dPipelineStateDesc[0].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


	d3dPipelineStateDesc[1].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[1].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[1].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[1].RasterizerState = CreateRasterizerState(false);
	d3dPipelineStateDesc[1].BlendState = CreateBlendState();
	d3dPipelineStateDesc[1].DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc[1].InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc[1].SampleMask = UINT_MAX;
	d3dPipelineStateDesc[1].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc[1].NumRenderTargets = 1;
	d3dPipelineStateDesc[1].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc[1].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc[1].SampleDesc.Count = 1;
	d3dPipelineStateDesc[1].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;



	d3dPipelineStateDesc[2].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[2].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[2].GS = CreateGeometeryShader(&pd3dGeoShaderBlob);
	d3dPipelineStateDesc[2].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[2].RasterizerState = CreateRasterizerState(false);
	d3dPipelineStateDesc[2].BlendState = CreateBlendState();
	d3dPipelineStateDesc[2].DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc[2].InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc[2].SampleMask = UINT_MAX;
	d3dPipelineStateDesc[2].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc[2].NumRenderTargets = 1;
	d3dPipelineStateDesc[2].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc[2].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc[2].SampleDesc.Count = 1;
	d3dPipelineStateDesc[2].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	d3dPipelineStateDesc[3].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[3].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[3].GS = CreateGeometeryShader(&pd3dGeoShaderBlob);
	d3dPipelineStateDesc[3].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[3].RasterizerState = CreateRasterizerState(true);
	d3dPipelineStateDesc[3].BlendState = CreateBlendState();
	d3dPipelineStateDesc[3].DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc[3].InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc[3].SampleMask = UINT_MAX;
	d3dPipelineStateDesc[3].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc[3].NumRenderTargets = 1;
	d3dPipelineStateDesc[3].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc[3].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc[3].SampleDesc.Count = 1;
	d3dPipelineStateDesc[3].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[0], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);
	if (hResult == E_FAIL) {
		return;
	}

	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[1], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[1]);
	if (hResult == E_FAIL) {
		return;
	}

	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[2], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[2]);
	if (hResult == E_FAIL) {
		return;
	}
	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc[3], __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[3]);
	if (hResult == E_FAIL) {
		return;
	}

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
	if (pd3dGeoShaderBlob) pd3dGeoShaderBlob->Release();

	if (d3dPipelineStateDesc[0].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[0].InputLayout.pInputElementDescs;
	if (d3dPipelineStateDesc[1].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[1].InputLayout.pInputElementDescs;
}


void CTerrainShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,UINT nParts)
{



}

void CTerrainShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	//for (int j = 0; j < m_nParts; j++)
	//{

	//	m_pcbMappedTerrainPartition->m_TerrainPartition = j;
	//	//XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
	//}
	//pd3dCommandList->SetGraphicsRootShaderResourceView(10, m_pd3dcbGameObjects->GetGPUVirtualAddress());
	//pd3dCommandList->SetComputeRootConstantBufferView(20, m_pd3dcbTerrainParts->GetGPUVirtualAddress());
}
void CTerrainShader::ReleaseShaderVariables()
{
	/*if (m_pd3dcbTerrainParts)
	{
		m_pd3dcbTerrainParts->Unmap(0, NULL);
		m_pd3dcbTerrainParts->Release();
	}*/

}


void CTerrainShader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList,UINT rasterNum)
{

	if (rasterNum == SOLID)
	{
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[SOLID]);	//그리기 시작
	}
	else if(rasterNum == WIREFRAME){
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[WIREFRAME]);	//그리기 시작
	}
	else if (rasterNum == GEOMETRYSHADER)
	{
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[GEOMETRYSHADER]);
	}
	else if (rasterNum == GEOMETRYSOLID)
	{
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[GEOMETRYSOLID]);
	}

	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}
D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	//pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] ={ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TEXCOORD" ,1, DXGI_FORMAT_R32G32_FLOAT,0,36,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	pd3dInputElementDescs[4] = { "NORMAL" ,0,DXGI_FORMAT_R32G32B32_FLOAT,0,44,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	//pd3dInputElementDescs[5] = { "PART" ,0,DXGI_FORMAT_R32_UINT,0,56,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	//pd3dInputElementDescs[5] = { "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,56,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	//pd3dInputElementDescs[6] = { "BITANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,68,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };


	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_BLEND_DESC CTerrainShader::CreateBlendState()
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
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_DEPTH_STENCIL_DESC CTerrainShader::CreateDepthStencilState()
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
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);

}
D3D12_RASTERIZER_DESC CTerrainShader::CreateRasterizerState(bool bfillmode)
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	if (bfillmode) {
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	}
	else {
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	}
	
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

void CTerrainShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList, rasterNum);
	

}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTerrain", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTerrain", "ps_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CTerrainShader::CreateGeometeryShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "GSTerrain", "gs_5_1", ppd3dShaderBlob));
}

//D3D12_SHADER_BYTECODE CTerrainShader::CreateGeometryShader(ID3DBlob **ppd3dShaderBlob)
//{
//	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "GSTerrain", "gs_5_1", ppd3dShaderBlob));
//}

//
//void CTerrainShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
//{
//	//CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
//
//	
//
//}

///////////////////////////////////////////////////////////////////////////////////////
CInstancingShader::CInstancingShader()
{

}
CInstancingShader::~CInstancingShader() {}


D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	pd3dInputElementDescs[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };


	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);

}

D3D12_RASTERIZER_DESC CInstancingShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);

}

D3D12_SHADER_BYTECODE CInstancingShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	
	return (CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSInstancing", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CInstancingShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return (CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSInstancing", "ps_5_1", ppd3dShaderBlob));
}


void CInstancingShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

}

void CInstancingShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255); //256의 배수

	
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes*m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);



	////////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbGameObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nObjects;
}

void CInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects) {
		m_pd3dcbGameObjects->Unmap(0, NULL);
	}
	if (m_pd3dcbGameObjects) {
		m_pd3dcbGameObjects->Release();
	}
}

//인스턴싱 정보 (객체의 월드 변환 행렬과 색생)를 정점 버퍼에 복사한다.
void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{

		//pd3dCommandList->SetGraphicsRootShaderResourceView(9,m_pd3dcbGameObjects->GetGPUVirtualAddress());
	for (int j = 0; j < m_nObjects; j++)
	{
	
		XMStoreFloat4x4(&m_pcbMappedGameObjects[j].m_xmf4x4Transform, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
	}
	pd3dCommandList->SetGraphicsRootShaderResourceView(10, m_pd3dcbGameObjects->GetGPUVirtualAddress());
}

void CInstancingShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,void *pContext)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain*)pContext;

	float fxPitch = 10.0f;
	float fyPitch = 1;
	float fzPitch = 10.0f;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();
	
	int xObjects = int(fTerrainWidth/fxPitch);
	int yObjects = 1;
	int zObjects = int(fTerrainLength/fzPitch);

	CTexture *pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Flower02.dds", 0);


	m_nObjects = (xObjects*yObjects*zObjects);

	//UINT ncbElementBytes

	UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255);

	//CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);				//m_nObjects만큼의 상수 버퍼뷰와 1개의 srv
	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList,0, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 0, m_pd3dcbGameObjects, ncbElementBytes); //오브젝트를 상수 버퍼뷰 생성
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture,11 , false);


#ifdef _WITH_BATCH_MATERIAL
	m_pMaterial = new CMaterial();
	m_pMaterial->SetTexture(pTexture);
#else
	CMaterial *pCubeMaterial = new CMaterial();
	pCubeMaterial->SetTexture(pTexture);
#endif

	CTexturedRectMesh *pBillboardMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 15.0f, 15.0f, 0.0f, 0.0f, 0.0f, -10.0f);


	m_ppObjects = new CGameObject*[m_nObjects];

	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
	CFlowerBillboardObject *pFlowerBillboardObject = NULL;
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		for (int z = 0; z< zObjects; z++)
		{
			for (int y = 0; y < yObjects; y++) {
				pFlowerBillboardObject = new CFlowerBillboardObject();
				pFlowerBillboardObject->SetMesh(0, pBillboardMesh);
#ifndef _WITH_BATCH_MATERIAL
				pBillboardObject->SetMaterial(pCubeMaterial);
#endif
				float xPosition = x * fxPitch;
				float zPosition = z * fzPitch;
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);

				pFlowerBillboardObject->SetPosition(xPosition, fHeight + (y * 3.0f * fyPitch) + 8.0f, zPosition);
				//std::cout << pBillboardObject->GetPosition().x << pBillboardObject->GetPosition().y << pBillboardObject->GetPosition().z << std::endl;
				if (y == 0) {
					xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
					xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal);
					if (Vector3::IsZero(xmf3RotateAxis))
					{
						xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
					}

					float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal));
					pFlowerBillboardObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
				}

				pFlowerBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize*i));
				m_ppObjects[i++] = pFlowerBillboardObject;
			}
		}
	}


}

void CInstancingShader::Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum, CCamera *pCamera)
{

	CTexturedShader::Render(pd3dCommandList, rasterNum,pCamera);
#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);
#endif

		m_ppObjects[0]->RenderInstancing(pd3dCommandList, pCamera, m_nObjects, m_d3dInstancingBufferView);
	
}
void CInstancingShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) delete m_pMaterial;
#endif
}

void CInstancingShader::AnimateObjects(float fTimeElapsed, CCamera *pCamera)
{

	for (int j = 0; j < m_nObjects; j++)
	{

		dynamic_cast<CFlowerBillboardObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//billboardShader
CBillboardShader::CBillboardShader()
{

}

CBillboardShader::~CBillboardShader()
{

}

D3D12_INPUT_LAYOUT_DESC CBillboardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	pd3dInputElementDescs[1] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };


	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CBillboardShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수

																		//리소스와 내부적인 힙을 생성 
																		//D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> 정점 버퍼 또는 상수 버퍼로 사용될때 초기 STATE

																		//업로드힙
	m_pd3dcbBillboardObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	//리소스에 대한 CPU포인터를 반환 (다중 - 쓰레드 안전)
	m_pd3dcbBillboardObjects->Map(0, NULL, (void **)&m_pcbMappedBillboardObjects);	//m_pcbMappedGameObjects 는 영구적으로 cpu가 접근 가능한 주소가 된다. 이 리소스에 대한 포인터는 영구적으로 위치를 안바꾼다.
																			//왜냐하면 cpu가 자주 접근하는 리소스이기 때문에
																			//m_pcbMappedGameObjects는 읽기를 하지 말아야한다.(성능상 문제가 발생)

}

void CBillboardShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	for (int j = 0; j < m_nObjects; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedBillboardObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));
//#ifdef 	_WITH_BATCH_MATERIAL
//		if (m_pMaterial) pbMappedcbGameObject->m_nObjectID = j;
//#endif
	}

}

void CBillboardShader::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->ReleaseUploadBuffers();
#endif
}

void CBillboardShader::ReleaseShaderVariables()
{
	if (m_pd3dcbBillboardObjects)
	{
		m_pd3dcbBillboardObjects->Unmap(0, NULL);
		m_pd3dcbBillboardObjects->Release();
	}

	CTexturedShader::ReleaseShaderVariables();
}

void CBillboardShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) delete m_pMaterial;
#endif
}

void CBillboardShader::AnimateObjects(float fTimeElapsed,CCamera *pCamera)
{
	
	for (int j = 0; j < m_nObjects; j++)
	{

		dynamic_cast<CBillboardObject*>(m_ppObjects[j])->Animate(fTimeElapsed,pCamera);
	}
}

void CBillboardShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;

	float fxPitch = 50.0f;				//이 값이 커질수록 오브젝트의 수가 적어진다. fxPitch 지형의 x좌표 변화량
	float fyPitch = 1.0f;
	float fzPitch = 60.0f;				//

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = int(fTerrainWidth / fxPitch);
	int yObjects = 1;
	int zObjects = int(fTerrainLength / fzPitch);
	//CTexture *pTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Tree01s.dds", 0);





	CTexture *ppTextures[TEXTURES];
	ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree01s.dds", 0);
	ppTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree02S.dds", 0);
	ppTextures[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree35S.dds", 0);
	ppTextures[3] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree4.dds", 0);



	/*CTexture *pTextureArray = new CTexture(1, RESOURCE_TEXTURE2DARRAY, 0);
	pTextureArray->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/treearray.dds", 0);*/


	m_nObjects = (xObjects * yObjects * zObjects);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, TEXTURES);				//m_nObjects만큼의 상수 버퍼뷰와 1개의 srv
	CreateShaderVariables(pd3dDevice, pd3dCommandList);										//업로드힙에 리소스 생성
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbBillboardObjects, ncbElementBytes); //오브젝트를 상수 버퍼뷰 생성
	
	for (int i = 0; i < TEXTURES; i++) {
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTextures[i], 8, false);				//SRV 버퍼 뷰 생성
	}
	

	CMaterial *ppMaterials[TEXTURES];
	for (int i = 0; i < TEXTURES; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTextures[i]);
	}



//#ifdef _WITH_BATCH_MATERIAL
//	m_pMaterial = new CMaterial();
//	m_pMaterial->SetTexture(pTextureArray);
//#else
//	CMaterial *pCubeMaterial = new CMaterial();
//	pCubeMaterial->SetTexture(pTexture);
//#endif


	//CCubeMeshTextured *pCubeMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 12.0f, 12.0f, 12.0f);
	CTexturedRectMesh* pBillboardMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 50.0f, 50.0f, 0.0f,0.0f,0.0f,-10.0f );

	m_ppObjects = new CGameObject*[m_nObjects];

	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
	CBillboardObject* pBillboardObject = NULL;

	for (int i = 0, x = 0; x < xObjects; x++)
	{
		for (int z = 0; z< zObjects; z++)
		{

				
				//////////////////////////////////////////////////////////////////
				//이 구간을 지형의 기울기를 읽어와서 
				//지형이 평평하다고 생각되면
				//빌보드를 셋할 예정
				pBillboardObject = new CBillboardObject();
				pBillboardObject->SetMesh(0, pBillboardMesh);
//#ifndef _WITH_BATCH_MATERIAL
//				pBillboardObject->SetMaterial(pCubeMaterial);
//#endif
				pBillboardObject->SetMaterial(ppMaterials[i % TEXTURES]);
				float xPosition = x * fxPitch;
				float zPosition = z * fzPitch;
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
				if (fHeight > 100.0f) {
					pBillboardObject->SetPosition(xPosition, fHeight + 18.0f, zPosition);
				}
				//////////////////////////////////////////////////////////
				//여기아래는 로직이 같으므로 건드릴필요 없어보임

		
				xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
				xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal);
				if (Vector3::IsZero(xmf3RotateAxis))
				{
					xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
				}

				float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal));
				pBillboardObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
			

				pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize*i));
				m_ppObjects[i++] = pBillboardObject;
			
			
		}
	}

}

void CBillboardShader::Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum ,CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, rasterNum,pCamera);

//#ifdef _WITH_BATCH_MATERIAL
//	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);
//#endif

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera,j);
	}
}


/////////////////////////////////////////////////////////////////////////////////////
//CGrassBillboardShader 
//
CGrassBillboardShader::CGrassBillboardShader()
{

}

CGrassBillboardShader::~CGrassBillboardShader()
{}

D3D12_SHADER_BYTECODE CGrassBillboardShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSGrassTextured", "ps_5_1", ppd3dShaderBlob));
}

void CGrassBillboardShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수

																		//리소스와 내부적인 힙을 생성 
																		//D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> 정점 버퍼 또는 상수 버퍼로 사용될때 초기 STATE
																		//업로드힙
	m_pd3dcbBillboardObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	//리소스에 대한 CPU포인터를 반환 (다중 - 쓰레드 안전)
	m_pd3dcbBillboardObjects->Map(0, NULL, (void **)&m_pcbMappedBillboardObjects);	//m_pcbMappedGameObjects 는 영구적으로 cpu가 접근 가능한 주소가 된다. 이 리소스에 대한 포인터는 영구적으로 위치를 안바꾼다.
																					//왜냐하면 cpu가 자주 접근하는 리소스이기 때문에


	//UINT ncbTextureElementBytes = ((sizeof(CB_TEXTURE_MATRIX) + 255)& ~255);

	//m_pd3dcbTextureTransform = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbTextureElementBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);


	//m_pd3dcbTextureTransform->Map(0, NULL, (void**)&m_pcbMappedTextureTransform);	


	//////////////////////////////////////////////////////////////////////
	////텍스쳐 좌표 변환 상수버퍼 생성
	//UINT ncbUVCoordinateBytes = ((sizeof(CB_TEXTURE_COORD) + 255)&~255);

	//m_pd3dcbTexcoord = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbUVCoordinateBytes * m_nObjects, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	//m_pd3dcbTexcoord->Map(0, NULL, (void**)&m_pcbTransformTexcoord);

}

void CGrassBillboardShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	

	for (int j = 0; j < m_nObjects; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedBillboardObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));



//#ifdef 	_WITH_BATCH_MATERIAL
//		if (m_pMaterial) pbMappedcbGameObject->m_nObjectID = j;
//#endif
		/*CB_TEXTURE_COORD *pbMappedcbTextureUV = (CB_TEXTURE_COORD *)((UINT8*)m_pcbTransformTexcoord + (j*ncbUVCoordinateBytes));
		XMStoreFloat4x4(&pbMappedcbTextureUV->m_xmf4x4Texcoord, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4Texcoord)));
*/

	}


}


void CGrassBillboardShader::ReleaseShaderVariables()
{
	if (m_pd3dcbBillboardObjects)
	{
		m_pd3dcbBillboardObjects->Unmap(0, NULL);
		m_pd3dcbBillboardObjects->Release();
	}

	//if (m_pd3dcbTextureTransform)
	//{
	//	m_pd3dcbTextureTransform->Unmap(0, NULL);
	//	m_pd3dcbTextureTransform->Release();
	//}

	CTexturedShader::ReleaseShaderVariables();
}

void CGrassBillboardShader::ReleaseUploadBuffers()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->ReleaseUploadBuffers();
#endif

}

void CGrassBillboardShader::Render(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum,CCamera* pCamera = NULL)
{
	CTexturedShader::Render(pd3dCommandList,rasterNum,pCamera);

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);
#endif
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera,j);
	}
}

void CGrassBillboardShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) delete m_pMaterial;
#endif
}

void CGrassBillboardShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;



	float fxPitch = 15.0f;				//이 값이 커질수록 오브젝트의 수가 적어진다. fxPitch 지형의 x좌표 변화량
	float fyPitch = 1;
	float fzPitch = 10.0f;				//

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = int(fTerrainWidth / fxPitch);
	int yObjects = 1;
	int zObjects = int(fTerrainLength / fzPitch);


	CTexture *ppTexture[3];
	ppTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/grass01.dds", 0);
	ppTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Grass02.dds", 0);
	ppTexture[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTexture[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/grass04.dds", 0);


	m_nObjects = (xObjects * yObjects * zObjects);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 3);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);										//업로드힙에 리소스 생성
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbBillboardObjects, ncbElementBytes); //오브젝트를 상수 버퍼뷰 생성
	for (int i = 0; i < 3; i++) {
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTexture[i], 7, false);				//SRV 버퍼 뷰 생성
	}
//#ifdef _WITH_BATCH_MATERIAL
//	m_pMaterial = new CMaterial();
//	m_pMaterial->SetTexture(pTexture);
//#else
//	CMaterial *pCubeMaterial = new CMaterial();
//	pCubeMaterial->SetTexture(pTexture);
//#endif
	CMaterial *ppMaterials[3];
	for (int i = 0; i < 3; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTexture[i]);
	}



	CTexturedRectMesh* pBillboardMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 15.0f, 15.0f, 0.0f, 0.0f, 0.0f, -10.0f);

	m_ppObjects = new CGameObject*[m_nObjects];

	XMFLOAT3 xmf3RotateAxis, xmf3SurfaceNormal;
	CGrassBillboardObject* pBillboardObject = NULL;
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		for (int z = 0; z< zObjects; z++)
		{
			for (int y = 0; y < yObjects; y++) {
				pBillboardObject = new CTexturedTransformObject();
				pBillboardObject->SetMesh(0, pBillboardMesh);
//#ifndef _WITH_BATCH_MATERIAL
//				pBillboardObject->SetMaterial(pCubeMaterial);
//#endif

				//pBillboardObject->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 1.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
				//pBillboardObject->SetMaterial(ppMaterials[i % 3]);
				float xPosition = x * fxPitch + (rand()%10);
				float zPosition = z * fzPitch + (rand()%11); 
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);
				
					pBillboardObject->SetMaterial(ppMaterials[i % 3]);
					pBillboardObject->SetPosition(xPosition, fHeight + (y * 3.0f * fyPitch) + 8.0f, zPosition);
					pBillboardObject->SetOOBB(XMFLOAT3(xPosition, fHeight, zPosition), XMFLOAT3(2.0f, 1.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
					//std::cout << pBillboardObject->GetPosition().x << pBillboardObject->GetPosition().y << pBillboardObject->GetPosition().z << std::endl;
					if (y == 0) {
						xmf3SurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
						xmf3RotateAxis = Vector3::CrossProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal);
						if (Vector3::IsZero(xmf3RotateAxis))
						{
							xmf3RotateAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
						}

						float fAngle = acos(Vector3::DotProduct(XMFLOAT3(0.0f, 1.0f, 0.0f), xmf3SurfaceNormal));
						pBillboardObject->Rotate(&xmf3RotateAxis, XMConvertToDegrees(fAngle));
					}

					pBillboardObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize*i));
					m_ppObjects[i++] = pBillboardObject;
				
			}
		}


	}
}

void CGrassBillboardShader::AnimateObjects(float fTimeElapsed, CCamera *pCamera,CPlayer *pPlayer)
{
	ContainmentType containType = DISJOINT;
	for (int j = 0; j < m_nObjects; j++)
	{
		containType=pPlayer->m_sphereBox->Contains(m_ppObjects[j]->m_xmOOBB);

		if (containType == CONTAINS)
		{
			dynamic_cast<CTexturedTransformObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera, true);
		}
		else {
			dynamic_cast<CTexturedTransformObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera, false);
		}
		/*if (m_ppObjects[j]->m_xmOOBB.Intersects(pPlayer->m_sphereBox)) {
			dynamic_cast<CTexturedTransformObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera, true);
		}
		else {
			dynamic_cast<CTexturedTransformObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera, false);
		}*/
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
CGeometeryBillboardShader::CGeometeryBillboardShader(){}

CGeometeryBillboardShader::~CGeometeryBillboardShader(){
	ReleaseShaderVariables();
	ReleaseUploadBuffers();
}

D3D12_INPUT_LAYOUT_DESC CGeometeryBillboardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	pd3dInputElementDescs[1] = { "SIZE",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };


	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}


D3D12_SHADER_BYTECODE CGeometeryBillboardShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSBillboard", "vs_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CGeometeryBillboardShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSBillboard", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CGeometeryBillboardShader::CreateGeometeryShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "GSBillboard", "gs_5_1", ppd3dShaderBlob));
}

void CGeometeryBillboardShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;
	ID3DBlob *pd3dGeometeryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc.GS = CreateGeometeryShader(&pd3dGeometeryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();							//계층구조상에서 오버이딩 가능
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
	if (pd3dGeometeryShaderBlob) pd3dGeometeryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;

}



D3D12_RASTERIZER_DESC CGeometeryBillboardShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	/*Edit*///	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	/*Edit*/	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3dRasterizerDesc);
}

D3D12_BLEND_DESC CGeometeryBillboardShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
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
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);

}
D3D12_DEPTH_STENCIL_DESC CGeometeryBillboardShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

void CGeometeryBillboardShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

	//UINT ncbElementBytes = ((sizeof(VS_VB_INSTANCE) + 255) & ~255); //256의 배수


	//																//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	//m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes*m_nVertices, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	////정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	//m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);



	//////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_pd3dVertexBufferView.BufferLocation = m_pd3dcbGameObjects->GetGPUVirtualAddress();
	//m_pd3dVertexBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_pd3dVertexBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nVertices;
	//
}
void CGeometeryBillboardShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,void *pContext)
{
	

	CTexture *pTexture = new CTexture(4, RESOURCE_TEXTURE2DARRAY, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/treeArray2.dds", 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree01Normal.dds", 1);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree02Normal.dds", 2);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree03Normal.dds", 3);
	
	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 4);				//m_nObjects만큼의 상수 버퍼뷰와 1개의 srv						
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 16, true);


	//CTexture *pTexture[2];

	//pTexture[0] = new CTexture(1, RESOURCE_TEXTURE2DARRAY, 0);
	//pTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/treeArray2.dds", 0);


	//CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);				//m_nObjects만큼의 상수 버퍼뷰와 1개의 srv						
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture[0], 13, false);



	//pTexture[1] = new CTexture(3, RESOURCE_TEXTURE2D, 0);
	//pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree01Normal.dds", 0);
	//pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree02Normal.dds", 1);
	//pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree03Normal.dds", 2);


	//CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 3);				//m_nObjects만큼의 상수 버퍼뷰와 1개의 srv						
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture[1], 17, false);


/*
	CTexture *ppTextures[TEXTURES];
	ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree01s.dds", 0);
	ppTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree02S.dds", 0);
	ppTextures[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree35S.dds", 0);
	ppTextures[3] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	ppTextures[3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/tree4.dds", 0);


	for (int i = 0; i < TEXTURES; i++) {
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, ppTextures[i], 8, false);				//SRV 버퍼 뷰 생성
	}

	CMaterial *ppMaterials[TEXTURES];
	for (int i = 0; i < TEXTURES; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTextures[i]);
	}
*/

//	int nTrees = 100;
	/*Edit*/	
/*Why?*///	m_nStride = sizeof(CTreeVertex);
#ifdef _WITH_BATCH_MATERIAL
	
		m_ppMaterial = new CMaterial();
		m_ppMaterial->SetTexture(pTexture);
	
#else
	CMaterial *pCubeMaterial = new CMaterial();
	pCubeMaterial->SetTexture(pTexture);
#endif


	/*Edit*/
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	int cxTerrain = pTerrain->GetWidth();
	int czTerrain = pTerrain->GetLength();

	int fxTerrain = cxTerrain / 256;
	int fzTerrain = cxTerrain / 256;

	int pxTerrain = cxTerrain / fxTerrain; //하나의 간격
	int pzTerrain = czTerrain / fzTerrain;

	//const size_t size = 257 * 257;
	//std::ifstream file;
	//file.open("Models/TerrainInfo.txt", std::ifstream::in);

	//char *arr = new char[size];

	//for (int i = 0; i < size; i++)
	//{
	//	file >> arr[i];
	//}
	//
	//int treeCount = 0;
	//std::vector<int> v;
	//for (size_t i = 0; i < size; i++)
	//{
	//	
	//	if ((UINT)arr[i] == 49) {
	//		treeCount++;
	//		v.push_back(i);
	//	}

	//}

	m_nStride = sizeof(CTexturedVertex);
//	m_nStride = sizeof(XMFLOAT3) + sizeof(XMFLOAT2);
	m_nVertices = 1000;
	int treeCount = 0;
	/*Edit*///CTreeVertex* pTreeVertices = new CTreeVertex[m_nVertices];
	CTexturedVertex *pTreeVertices = new CTexturedVertex[m_nVertices];
	//std::vector<CTexturedVertex*> pTreeVertices;

	float** ppTerrainHeight = new float*[257];

	for (int i = 0; i < 257; i++)
	{
		ppTerrainHeight[i] = new float[257];
	}

	for (int i = 0; i < 257; i++)
	{
		for (int j = 0; j < 257; j++)
		{
			ppTerrainHeight[i][j] = CHeightMapGridMesh::m_pixelHeight[i][j];
		}
	}

	XMFLOAT3 xmf3Position;
	
	//int i = 0;
	//while (i<m_nVertices) {


	
	
	
	while (treeCount < m_nVertices) {
		xmf3Position.x = rand() % cxTerrain;
		xmf3Position.z = rand() % czTerrain;
		xmf3Position.y = pTerrain->GetHeight(xmf3Position.x, xmf3Position.z, false) + 10.0f;
		if (xmf3Position.y > 245.0f) {
			//if (treeCount < m_nVertices) {
				pTreeVertices[treeCount].m_xmf3Position = xmf3Position;
					//pTreeVertices[i].m_xmf2Size = XMFLOAT2(5.0f, 5.0f);
				pTreeVertices[treeCount++].m_xmf2TexCoord = XMFLOAT2(20.0f, 20.0f);
				//}
		}
	}
		
	
	
	

		//pTreeVertices[i].m_xmf3Position = xmf3Position;
		//	//pTreeVertices[i].m_xmf2Size = XMFLOAT2(5.0f, 5.0f);
		//pTreeVertices[i].m_xmf2TexCoord = XMFLOAT2(20.0f, 20.0f);
			//treeCount++;


		/*xmf3Position.x = pTerrain->vecTreePosition[i%pTerrain->vecTreePosition.size()].x * fxTerrain;
		xmf3Position.z = pTerrain->vecTreePosition[i%pTerrain->vecTreePosition.size()].z * fzTerrain;*/
		//xmf3Position.y = pTerrain->GetHeight(xmf3Position.x, xmf3Position.z, false) + 10.0f;
		//
		//
		//pTreeVertices[i].m_xmf3Position = xmf3Position;
		////pTreeVertices[i].m_xmf2Size = XMFLOAT2(5.0f, 5.0f);
		//pTreeVertices[i].m_xmf2TexCoord = XMFLOAT2(20.0f, 20.0f);
		//std::cout << pTreeVertices[i].m_xmf3Position.x <<","<<pTreeVertices[i].m_xmf3Position. y<<","<< pTreeVertices[i].m_xmf3Position.z<< std::endl;
	//}

	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pTreeVertices, m_nStride*treeCount, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

	m_pd3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferView.StrideInBytes = m_nStride;
	m_pd3dVertexBufferView.SizeInBytes = m_nStride*m_nVertices;

	if (pTreeVertices) delete[] pTreeVertices;
}


void CGeometeryBillboardShader::ReleaseUploadBuffers()
{
/*Edit*///	if (pTreeVertices) delete[] pTreeVertices;
/*
	{
		for (int i = 0; i < this->m_nVertices; i++)
		{
			delete[i] pTreeVertices;
			memset(&pTreeVertices[i],NULL,sizeof(CTreeVertex));
		}
	}
*/
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
}

void CGeometeryBillboardShader::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
	
}

void CGeometeryBillboardShader::ReleaseShaderVariables()
{
	if (m_pd3dVertexBuffer)
	{
		/*Edit*///		m_pd3dVertexBuffer->Unmap(0, NULL);
		m_pd3dVertexBuffer->Release();
	}
}

void CGeometeryBillboardShader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	/*Edit*/	
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}

void CGeometeryBillboardShader::Render(ID3D12GraphicsCommandList* pd3dCommandList,UINT rasterNum ,CCamera* pCamera)
{
	/*Edit*/

	OnPrepareRender(pd3dCommandList);


		if (m_ppMaterial)
	{
		if (m_ppMaterial->m_pShader)
		{
			m_ppMaterial->m_pShader->Render(pd3dCommandList, rasterNum, pCamera);
			m_ppMaterial->m_pShader->UpdateShaderVariables(pd3dCommandList);

			UpdateShaderVariables(pd3dCommandList);
		}
		if (m_ppMaterial->m_pTexture)
		{
			m_ppMaterial->m_pTexture->UpdateShaderVariables(pd3dCommandList);
		}
	}

		pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		pd3dCommandList->IASetVertexBuffers(0, 1, &m_pd3dVertexBufferView);

		pd3dCommandList->DrawInstanced(m_nVertices, 1, 0, 0);
	
	

}




////////////////////////////////////////////////////////////////////////////////////////////
CSkyBoxShader::CSkyBoxShader()
{

}

CSkyBoxShader::~CSkyBoxShader()
{

}

D3D12_BLEND_DESC CSkyBoxShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_DEPTH_STENCIL_DESC CSkyBoxShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

D3D12_SHADER_BYTECODE CSkyBoxShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSSkyBox", "ps_5_1", ppd3dShaderBlob));
}

void CSkyBoxShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}



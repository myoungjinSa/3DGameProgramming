//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "DDSTextureLoader12.h"
#include "Scene.h"


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
	//if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
}

D3D12_SHADER_BYTECODE CShader::CreateHullShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreateDomainShader(ID3DBlob **ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;

	return(d3dShaderByteCode);
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

void CShader::CreateCbvAndSrvAndUavDesciptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews, int nUnorderedAccessViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews + nUnorderedAccessViews; //CBVs + SRVs + UAVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;

	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvUavDescriptorHeap);

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);

	m_d3dUavCPUDescriptorStartHandle.ptr = m_d3dSrvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);
	m_d3dUavGPUDescriptorStartHandle.ptr = m_d3dSrvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nShaderResourceViews);

}
D3D12_UNORDERED_ACCESS_VIEW_DESC GetUnorderedAccessViewDesc(D3D12_RESOURCE_DESC d3dResourceDesc)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC d3dUnorderedAccessViewDesc;
	d3dUnorderedAccessViewDesc.Format = d3dResourceDesc.Format;
	d3dUnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	d3dUnorderedAccessViewDesc.Texture2D.PlaneSlice = 0;
	d3dUnorderedAccessViewDesc.Texture2D.MipSlice = 0;

	return(d3dUnorderedAccessViewDesc);

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

void CShader::CreateSrvAndUavViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pTexture, UINT nRootParameterStartIndex1, UINT nRootParameterStartIndex2)
{
	int nTextures = pTexture->GetTextures();
	int nTextureType = pTexture->GetTextureType();
	for (int i = 0; i < nTextures; i++)
	{

		//srv뷰를 

		ID3D12Resource *pShaderResource = pTexture->GetTexture(i);				//텍스처 리소스를 하나 얻어온다.
		D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();		//그 리소스들에 대한 정보를 얻어온다.
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = GetShaderResourceViewDesc(d3dResourceDesc, nTextureType);  //텍스처에 대한 서술자 뷰를 설정한다.

		ID3D12Resource *pUnorderedResource = pTexture->GetTexture(i);
		D3D12_RESOURCE_DESC d3dUavResourceDesc = pUnorderedResource->GetDesc();
		D3D12_UNORDERED_ACCESS_VIEW_DESC d3dUnorderedAccessViewDesc = GetUnorderedAccessViewDesc(d3dUavResourceDesc);


		if (i == 0) {
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorStartHandle);		//cpu가 접근할수있는 srv뷰를 만든다.
			m_d3dSrvCPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, nRootParameterStartIndex1, m_d3dSrvGPUDescriptorStartHandle);
			m_d3dSrvGPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pd3dDevice->CreateUnorderedAccessView(pUnorderedResource, nullptr, &d3dUnorderedAccessViewDesc, m_d3dUavCPUDescriptorStartHandle);		//cpu가 접근할수있는 srv뷰를 만든다.
			m_d3dUavCPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;


			pTexture->SetRootArgumentUav(i, nRootParameterStartIndex2, m_d3dUavGPUDescriptorStartHandle);
			m_d3dUavGPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		}
		else {
			pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorStartHandle);		//cpu가 접근할수있는 srv뷰를 만든다.
			m_d3dSrvCPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgument(i, nRootParameterStartIndex1, m_d3dSrvGPUDescriptorStartHandle);
			m_d3dSrvGPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pd3dDevice->CreateUnorderedAccessView(pUnorderedResource, nullptr, &d3dUnorderedAccessViewDesc, m_d3dUavCPUDescriptorStartHandle);		//cpu가 접근할수있는 srv뷰를 만든다.
			m_d3dUavCPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;

			pTexture->SetRootArgumentUav(i, nRootParameterStartIndex2, m_d3dUavGPUDescriptorStartHandle);
			m_d3dUavGPUDescriptorStartHandle.ptr += ::gnCbvSrvDescriptorIncrementSize;
		}

	}

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
	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,UINT rasterNum)
{
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}
void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT rasterNum, CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList, rasterNum);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList,  CCamera *pCamera)
{
	OnPrepareRender(pd3dCommandList);
}
void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera,CPlayer *pPlayer)
{
	OnPrepareRender(pd3dCommandList);
}

void CShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, bool bSolid)
{
	OnPrepareRender(pd3dCommandList,bSolid);
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
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

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

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSStandard", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSStandard", "ps_5_1", ppd3dShaderBlob));
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIlluminatedTexturedShader::CIlluminatedTexturedShader()
{
}

CIlluminatedTexturedShader::~CIlluminatedTexturedShader()
{
}
D3D12_INPUT_LAYOUT_DESC CIlluminatedTexturedShader::CreateInputLayout()
{
	UINT nInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CIlluminatedTexturedShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTexturedLighting", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CIlluminatedTexturedShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTexturedLighting", "ps_5_1", ppd3dShaderBlob));
}

void CIlluminatedTexturedShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
CGeometryBillboardShader::CGeometryBillboardShader() {}

CGeometryBillboardShader::~CGeometryBillboardShader() {
	ReleaseShaderVariables();
	ReleaseUploadBuffers();
}

D3D12_INPUT_LAYOUT_DESC CGeometryBillboardShader::CreateInputLayout()
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


D3D12_SHADER_BYTECODE CGeometryBillboardShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSBillboard", "vs_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CGeometryBillboardShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSBillboard", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CGeometryBillboardShader::CreateGeometeryShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "GSBillboard", "gs_5_1", ppd3dShaderBlob));
}

void CGeometryBillboardShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
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



D3D12_RASTERIZER_DESC CGeometryBillboardShader::CreateRasterizerState()
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

D3D12_BLEND_DESC CGeometryBillboardShader::CreateBlendState()
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
D3D12_DEPTH_STENCIL_DESC CGeometryBillboardShader::CreateDepthStencilState()
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

void CGeometryBillboardShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
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
void CGeometryBillboardShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{


	CTexture *pTexture = new CTexture(1, RESOURCE_TEXTURE2DARRAY, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/treeArray2.dds", 0);
	//pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/ExplosionParticle.dds", 0);
	

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);				//m_nObjects만큼의 상수 버퍼뷰와 1개의 srv						
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 22, false);


	
#ifdef _WITH_BATCH_MATERIAL

	m_ppMaterial = new CMaterial();
	m_ppMaterial->SetTexture(pTexture);

#else
	CMaterial *pCubeMaterial = new CMaterial();
	pCubeMaterial->SetTexture(pTexture);
#endif

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	int cxTerrain = pTerrain->GetWidth();
	int czTerrain = pTerrain->GetLength();

	int fxTerrain = cxTerrain / 256;
	int fzTerrain = cxTerrain / 256;

	int pxTerrain = cxTerrain / fxTerrain; //하나의 간격
	int pzTerrain = czTerrain / fzTerrain;

	

	m_nStride = sizeof(CTexturedVertex);
	//	m_nStride = sizeof(XMFLOAT3) + sizeof(XMFLOAT2);
	m_nVertices = 10000;
	int treeCount = 0;
	/*Edit*///CTreeVertex* pTreeVertices = new CTreeVertex[m_nVertices];
	CTexturedVertex *pTreeVertices = new CTexturedVertex[m_nVertices];
	//std::vector<CTexturedVertex*> pTreeVertices;


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


	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pTreeVertices, m_nStride*treeCount, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

	m_pd3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferView.StrideInBytes = m_nStride;
	m_pd3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	if (pTreeVertices) delete[] pTreeVertices;
	


}


void CGeometryBillboardShader::ReleaseUploadBuffers()
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

void CGeometryBillboardShader::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{

}

void CGeometryBillboardShader::ReleaseShaderVariables()
{
	if (m_pd3dVertexBuffer)
	{
		/*Edit*///		m_pd3dVertexBuffer->Unmap(0, NULL);
		m_pd3dVertexBuffer->Release();
	}
}

void CGeometryBillboardShader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);

}

void CGeometryBillboardShader::Render(ID3D12GraphicsCommandList* pd3dCommandList,CCamera* pCamera,CPlayer *pPlayer )
{

	OnPrepareRender(pd3dCommandList);


	if (m_ppMaterial)
	{
		if (m_ppMaterial->m_pShader)
		{
			m_ppMaterial->m_pShader->Render(pd3dCommandList, pCamera);
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




///////////////////////////////////////////////////////////////////////////////////////////////////////////
CMissileShader::CMissileShader()

{

}

CMissileShader::~CMissileShader()
{

}

void CMissileShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, int index)
{
	if (ppMissile[index]->status == CMissileObject::eStatus::SHOT)
	{
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작
	}
	else if (ppMissile[index]->status == CMissileObject::eStatus::CRASH) {
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);	//그리기 시작
	}
	else if (ppMissile[index]->status == CMissileObject::eStatus::SMOKE)
	{
		if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[2]);	//그리기 시작
	}

	pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList, index);
}
void CMissileShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	//0은 미사일 나가는 상태 1은 폭발 상태
	m_nPipelineStates = 3;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc[3];
	::ZeroMemory(d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC) * 3);
	d3dPipelineStateDesc[0].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[0].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[0].PS = CreatePixelShader(&pd3dPixelShaderBlob, CMissileObject::SHOT);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[0].RasterizerState = CreateRasterizerState();
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
	d3dPipelineStateDesc[1].PS = CreatePixelShader(&pd3dPixelShaderBlob, CMissileObject::CRASH);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[1].RasterizerState = CreateRasterizerState();
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
	d3dPipelineStateDesc[2].PS = CreatePixelShader(&pd3dPixelShaderBlob, CMissileObject::SMOKE);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[2].RasterizerState = CreateRasterizerState();
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

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc[0].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[0].InputLayout.pInputElementDescs;
	if (d3dPipelineStateDesc[1].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[1].InputLayout.pInputElementDescs;
	if (d3dPipelineStateDesc[2].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[2].InputLayout.pInputElementDescs;
	//CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

}

D3D12_DEPTH_STENCIL_DESC CMissileShader::CreateDepthStencilState()
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

D3D12_BLEND_DESC CMissileShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;					//알파커버리지
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



D3D12_SHADER_BYTECODE CMissileShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob, int bExplosion)
{
	D3D12_SHADER_BYTECODE shader;
	if (bExplosion == CMissileObject::CRASH)
	{
		shader = CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSExplosion", "ps_5_1", ppd3dShaderBlob);
	}
	else if(bExplosion == CMissileObject::SHOT){
		shader = CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTexturedMissile", "ps_5_1", ppd3dShaderBlob);
	}
	else if (bExplosion == CMissileObject::SMOKE)
	{
		shader = CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSSmoke", "ps_5_1", ppd3dShaderBlob);
	}

	return shader;

}

void CMissileShader::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255)& ~255);

	m_pd3dcbMissileObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nMissile, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMissileObjects->Map(0, NULL, (void**)&m_pcbMappedMissileObjects);


}


void CMissileShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList, int index)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedMissileObjects + (index*ncbElementBytes));
	XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&ppMissile[index]->m_xmf4x4World)));

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) pbMappedcbGameObject->m_nMaterial = m_pMaterial->m_nReflection;
#endif
	//std::cout <<index<<","<<ppMissile[index]->m_fExplosionTimes<<std::endl;
	pbMappedcbGameObject->m_nTimeElapsed = ppMissile[index]->m_fExplosionTimes;//ppMissile[j]->pExplosiveObject->GetExplosionTime();


}

void CMissileShader::ReleaseShaderVariables()
{
	if (m_pd3dcbMissileObjects)
	{
		m_pd3dcbMissileObjects->Unmap(0, NULL);
		m_pd3dcbMissileObjects->Release();
	}


	CTexturedShader::ReleaseShaderVariables();
}

void CMissileShader::ReleaseObjects()
{
	if (ppMissile)
	{
		for (int j = 0; j < m_nMissile; j++)
		{
			if (ppMissile[j]) delete ppMissile[j];
		}
		delete[] ppMissile;
	}

}

CMissileObject** CMissileShader::ppMissile = NULL;
void CMissileShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void *pContext)
{
	m_pTerrain = (CHeightMapTerrain*)pContext;

	const int nMissile = CPlayer::m_countMissile;

	m_nMissile = nMissile;

	ppMissile = new CMissileObject*[nMissile];

	CTexture *pTexture = new CTexture(3, RESOURCE_TEXTURE2D, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/BlueMissile.dds", 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/fire.dds", 1);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Smoke.dds", 2);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255)&~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nMissile, 3);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nMissile, m_pd3dcbMissileObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 23, true);



#ifdef _WITH_BATCH_MATERIAL
	m_pMaterial = new CMaterial();
	m_pMaterial->SetTexture(pTexture);
	m_pMaterial->SetReflection(1);

#else
	CMaterial *pCubeMaterial = new CMaterial();
	pCubeMaterial->SetTexture(pTexture);
#endif

	CCubeMeshTextured *pCubeMesh = new CCubeMeshTextured(pd3dDevice, pd3dCommandList, 2.0f, 2.0f, 10.0f);

	CTexturedRectMesh* pBillboardMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 10.0f);
	CTexturedRectMesh* pSmokeBillboardMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 60.0f, 60.0f, 0.0f, 0.0f, 0.0f, 10.0f);

	for (int i = 0; i < m_nMissile; i++)
	{
		ppMissile[i] = new CMissileObject(3);
		ppMissile[i]->SetMesh(0, pCubeMesh);
		ppMissile[i]->SetMesh(1, pBillboardMesh);
		ppMissile[i]->SetMesh(2, pSmokeBillboardMesh);
		ppMissile[i]->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
		ppMissile[i]->SetRotationSpeed(100.0f);
		ppMissile[i]->SetMovingSpeed(300.0f);
		ppMissile[i]->SetMaterial(m_pMaterial);
		ppMissile[i]->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
	}

}

void CMissileShader::AnimateObjects(float fTimeElapsed, CPlayer *pPlayer,CCamera *pCamera)
{
	for (int i = 0; i < CPlayer::m_currentMissile; i++)
	{
		if (ppMissile[i])
		{
			if (ppMissile[i]->status == CMissileObject::eStatus::SHOT) {
				float fHeight = m_pTerrain->GetHeight(ppMissile[i]->GetPosition().x, ppMissile[i]->GetPosition().z);

				float fBulletHeight = ppMissile[i]->GetPosition().y;

				if (fHeight >= fBulletHeight-20.0f)
				{
					ppMissile[i]->status = CMissileObject::eStatus::CRASH;
				}
				
				
				
			}

			ppMissile[i]->Animate(fTimeElapsed, pCamera);
		}
	

	}


}

void CMissileShader::ReleaseUploadBuffers()
{
	if (ppMissile)
	{
		for (int i = 0; i < m_nMissile; i++)
		{
			if (ppMissile[i])
			{
				ppMissile[i]->ReleaseUploadBuffers();
			}
		}
	}

#ifdef _WITH_BATCH_MATERIAL
	if (m_pMaterial) m_pMaterial->ReleaseUploadBuffers();
#endif
}


void CMissileShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer)
{


	for (int i = 0; i < CPlayer::m_currentMissile; i++)
	{
		OnPrepareRender(pd3dCommandList, i);
		if (ppMissile[i])
		{
			if (ppMissile[i]->status != CMissileObject::eStatus::IDLE) {

				ppMissile[i]->Render(pd3dCommandList, pCamera, i);

			}

		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] ={ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] ={ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] ={ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TEXCOORD" ,1, DXGI_FORMAT_R32G32_FLOAT,0,36,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	pd3dInputElementDescs[4] = { "NORMAL" ,0,DXGI_FORMAT_R32G32B32_FLOAT,0,44,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };


	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSIlluminatedTerrain", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSIlluminatedTerrain", "ps_5_1", ppd3dShaderBlob));
}

void CTerrainShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	//CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc[2];
	::ZeroMemory(d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC) * 2);
	d3dPipelineStateDesc[0].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[0].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[0].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[0].RasterizerState = CreateRasterizerState(true);				//SOLID
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
	d3dPipelineStateDesc[1].RasterizerState = CreateRasterizerState(false);				//WIREFRAME
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


	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();


	if (d3dPipelineStateDesc[0].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[0].InputLayout.pInputElementDescs;
	if (d3dPipelineStateDesc[1].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[1].InputLayout.pInputElementDescs;


}
void CTerrainShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList,bool bWireFrame) {
	
	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (bWireFrame == false) {
		if (m_ppd3dPipelineStates[0]) {
			pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작
		}
	}
	else {
		if (m_ppd3dPipelineStates[1]) {
			pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);	//그리기 시작
		}
	}
	
	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}
D3D12_RASTERIZER_DESC CTerrainShader::CreateRasterizerState(bool bSolid)
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	if (bSolid) {
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

//////////////////////////////////////////////////////////////////////////////////////

CWaterShader::CWaterShader()
{

}

CWaterShader::~CWaterShader()
{

}


D3D12_INPUT_LAYOUT_DESC CWaterShader::CreateInputLayout()
{
	UINT nInputElementDescs = 4;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "NORMAL" ,0,DXGI_FORMAT_R32G32B32_FLOAT,0,36,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };


	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CWaterShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSIlluminatedWater", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CWaterShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSIlluminatedWater", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CWaterShader::CreateHullShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "HSIlluminatedWater", "hs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CWaterShader::CreateDomainShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "DSIlluminatedWater", "ds_5_1", ppd3dShaderBlob));
}


void CWaterShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];
	
	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc[2];
	::ZeroMemory(d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC) * 2);
	d3dPipelineStateDesc[0].pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc[0].VS = CreateVertexShader(&pd3dVertexShaderBlob);			//계층 구조 상에서 오버라이딩 가능 
	d3dPipelineStateDesc[0].PS = CreatePixelShader(&pd3dPixelShaderBlob);				//계층 구조 상에서 오버라이딩 가능
	d3dPipelineStateDesc[0].RasterizerState = CreateRasterizerState(true);				//SOLID
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
	d3dPipelineStateDesc[1].RasterizerState = CreateRasterizerState(false);				//WIREFRAME
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


	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();


	if (d3dPipelineStateDesc[0].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[0].InputLayout.pInputElementDescs;
	if (d3dPipelineStateDesc[1].InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc[1].InputLayout.pInputElementDescs;


}


void CWaterShader::OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, bool bWireFrame) {

	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (bWireFrame == false) {
		if (m_ppd3dPipelineStates[0]) {
			pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작
		}
	}
	else {
		if (m_ppd3dPipelineStates[1]) {
			pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);	//그리기 시작
		}
	}

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
}

D3D12_BLEND_DESC CWaterShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_COLOR;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);

}


D3D12_RASTERIZER_DESC CWaterShader::CreateRasterizerState(bool bSolid)
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	if (bSolid) {
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


}
	//////뷰?를 왜 생성하지 (추후에 확인 다시 해볼 것)
	//m_d3dInstancingBufferView.BufferLocation = m_pd3dcbGameObjects->GetGPUVirtualAddress();
	//m_d3dInstancingBufferView.StrideInBytes = sizeof(VS_VB_INSTANCE);
	//m_d3dInstancingBufferView.SizeInBytes = sizeof(VS_VB_INSTANCE)*m_nObjects;


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

void CInstancingShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{

	CTexturedShader::Render(pd3dCommandList, pCamera);
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

void CBillboardShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);

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



}

void CGrassBillboardShader::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	

	for (int j = 0; j < m_nObjects; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedBillboardObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppObjects[j]->m_xmf4x4World)));


	}


}


void CGrassBillboardShader::ReleaseShaderVariables()
{
	if (m_pd3dcbBillboardObjects)
	{
		m_pd3dcbBillboardObjects->Unmap(0, NULL);
		m_pd3dcbBillboardObjects->Release();
	}


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

void CGrassBillboardShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera* pCamera = NULL)
{
	CTexturedShader::Render(pd3dCommandList, pCamera);

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
				pBillboardObject = new CGrassBillboardObject();
				pBillboardObject->SetMesh(0, pBillboardMesh);
//#ifndef _WITH_BATCH_MATERIAL
//				pBillboardObject->SetMaterial(pCubeMaterial);
//#endif

				//pBillboardObject->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 1.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
				pBillboardObject->SetMaterial(ppMaterials[i % 3]);
				float xPosition = x * fxPitch + (rand()%10);
				float zPosition = z * fzPitch + (rand()%11); 
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);

				pBillboardObject->SetPosition(xPosition, fHeight + (y * 3.0f * fyPitch)+8.0f , zPosition);
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
			dynamic_cast<CGrassBillboardObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera, true);
		}
		else {
			dynamic_cast<CGrassBillboardObject*>(m_ppObjects[j])->Animate(fTimeElapsed, pCamera, false);
		}
	
	}
}


//////////////////////////////////////////////////////////////////
CSkyBoxShader::CSkyBoxShader()
{

}

CSkyBoxShader::~CSkyBoxShader()
{

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
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CStandardShader::CStandardShader()
{
}

CStandardShader::~CStandardShader()
{
}

D3D12_INPUT_LAYOUT_DESC CStandardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 5;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CStandardShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(const_cast<wchar_t*>(L"Shaders.hlsl"), "VSStandard", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CStandardShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(const_cast<wchar_t*>(L"Shaders.hlsl"), "PSStandard", "ps_5_1", ppd3dShaderBlob));
}

void CStandardShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	CShader::CreateShader(pd3dDevice,  pd3dGraphicsRootSignature);


}


///////////////////////////////////////////////////////////////////
CFrameShader::CFrameShader()
{}

CFrameShader::~CFrameShader()
{}

float Random(float fMin, float fMax)
{
	float fRandomValue = (float)rand();
	if (fRandomValue < fMin) fRandomValue = fMin;
	if (fRandomValue > fMax) fRandomValue = fMax;
	return(fRandomValue);
}

float Random()
{
	return(rand() / float(RAND_MAX));
}
XMFLOAT3 RandomPositionInSphere(XMFLOAT3 xmf3Center, float fRadius, int nColumn, int nColumnSpace)
{
	float fAngle = Random() * 360.0f * (2.0f * 3.14159f / 360.0f);

	XMFLOAT3 xmf3Position;
	xmf3Position.x = xmf3Center.x + fRadius * sin(fAngle);
	xmf3Position.y = xmf3Center.y - (nColumn * float(nColumnSpace) / 2.0f) + (nColumn * nColumnSpace) + Random();
	xmf3Position.z = xmf3Center.z + fRadius * cos(fAngle);

	return(xmf3Position);
}

void CFrameShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext)
{
	m_nObjects = 50;
	m_ppObjects = new CFrameObject*[m_nObjects];

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 17 + 2); //SuperCobra(17), Gunship(2)

	CFrameObject *pSuperCobraModel = CFrameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, const_cast<char*>("Model/SuperCobra.bin"), this);
	CFrameObject *pGunshipModel = CFrameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, const_cast<char*>("Model/Gunship.bin"), this);

	int nObjects = 0;

	for (int i = 0; i < m_nObjects/2; i++) {
		m_ppObjects[i] = new CSuperCobraObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
		m_ppObjects[i]->SetChild(pSuperCobraModel);
		pSuperCobraModel->AddRef();
		
		m_ppObjects[i]->SetRotationSpeed(0.2f);
		m_ppObjects[i]->SetSpeed(1.0f);
		m_ppObjects[i]->SetPosition(XMFLOAT3(500.0f+(50*i),290.0f,500.0f+(50*i)));
		m_ppObjects[i]->Rotate(0.0f, 90.0f, 0.0f);
		m_ppObjects[i]->PrepareAnimate();

	}
	
	for (int j = m_nObjects/2; j < m_nObjects; j++)
	{
		m_ppObjects[j] = new CGunshipObject(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
		m_ppObjects[j]->SetChild(pGunshipModel);
		pGunshipModel->AddRef();

		
		m_ppObjects[j]->SetRotationSpeed(0.3f);
		m_ppObjects[j]->SetSpeed(1.0f);
		m_ppObjects[j]->SetPosition(XMFLOAT3(500.0f + (50 * j), 300.0f, 500.0f + (50 * j)));
		m_ppObjects[j]->Rotate(0.0f, 90.0f, 0.0f);
		m_ppObjects[j]->PrepareAnimate();
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

}


void CFrameShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void CFrameShader::AnimateObjects(float fTimeElapsed,CPlayer* pPlayer,CCamera *pCamera)
{

	for (int i = 0; i < m_nObjects; i++)
	{


		m_ppObjects[i]->Animate(fTimeElapsed, pPlayer, NULL);
		m_ppObjects[i]->UpdateTransform(NULL);
	};

}

void CFrameShader::ReleaseUploadBuffers()
{
	for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
}

void CFrameShader::Render(ID3D12GraphicsCommandList *pd3dCommandList,CCamera *pCamera,CPlayer *pPlayer)
{
	CShader::Render(pd3dCommandList, pCamera);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			if (m_ppObjects[j]->status == CFrameObject::LIVE) {
				m_ppObjects[j]->Animate(0.16, pPlayer, NULL);
				m_ppObjects[j]->UpdateTransform(NULL);
				m_ppObjects[j]->Render(pd3dCommandList, pCamera, pPlayer);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////
//
CPostProcessingShader::CPostProcessingShader()
{

}

CPostProcessingShader::~CPostProcessingShader()
{
	ReleaseShaderVariables();
	ReleaseObjects();
}
D3D12_DEPTH_STENCIL_DESC CPostProcessingShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = FALSE;							//블렌딩 활성화 x
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
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
D3D12_INPUT_LAYOUT_DESC CPostProcessingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];


	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}
//D3D12_INPUT_LAYOUT_DESC CPostProcessingShader::CreateInputLayout()
//{
//	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
//	d3dInputLayoutDesc.pInputElementDescs = NULL;
//	d3dInputLayoutDesc.NumElements = 0;
//
//	return(d3dInputLayoutDesc);
//}



D3D12_SHADER_BYTECODE CPostProcessingShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSPostProcessing", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPostProcessingShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSPostProcessing", "ps_5_1", ppd3dShaderBlob));
}

void CPostProcessingShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[1];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors =1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 20; //t20 : gtxtScene
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;

	//pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//pd3dDescriptorRanges[1].NumDescriptors = 1;
	//pd3dDescriptorRanges[1].BaseShaderRegister = 24; //t24 : gtxtSceneColor
	//pd3dDescriptorRanges[1].RegisterSpace = 0;
	//pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = 0;
	
	D3D12_ROOT_PARAMETER pd3dRootParameters[1];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t20 : gtxtScene
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	//pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; //t24 : gtxtSceneColor
	//pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;


	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
}


void CPostProcessingShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

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
	d3dPipelineStateDesc.NumRenderTargets = nRenderTargets;
	for (UINT i = 0; i < nRenderTargets; i++) d3dPipelineStateDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CPostProcessingShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	m_nTexture = 1;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = (CTexture *)pContext;
	//m_pTexture[1] =  new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//m_pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Pilot.dds", 0);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, m_pTexture[0]->GetTextures());
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, false);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[1], 1, false);


	//m_pTexture[1] = (CTexture *)pContext2;
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[1], 26, false);

}
void CPostProcessingShader::ReleaseObjects()
{
	if (m_pTexture)
	{
		for (int i = 0; i < m_nTexture; i++) {
			m_pTexture[i]->Release();
		}
		
	}
}

void CPostProcessingShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera,CPlayer *pPlayer)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	CShader::Render(pd3dCommandList, pCamera);

	if (m_pTexture)
	{
		for (int i = 0; i <m_nTexture; i++) {
			//CShader::Render(pd3dCommandList, pCamera);
			m_pTexture[i]->UpdateShaderVariables(pd3dCommandList);
			pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pd3dCommandList->DrawInstanced(6, 1, 0, 0);
		}
		
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CTexturedUIShader::CTexturedUIShader()
{}

CTexturedUIShader::~CTexturedUIShader()
{}


//void CTexturedUIShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
//{
//
//}
void CTexturedUIShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[2];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 19; //t19 : gtxtPilot
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;


	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 22; //t22 : gtxtPilot
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = 0;

	

	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t19: gtxtPilot
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;



	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; //t22: gtxtTarget
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

}
D3D12_SHADER_BYTECODE CTexturedUIShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTexturedUI", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTexturedUIShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{

	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTexturedUI", "ps_5_1", ppd3dShaderBlob));
}


void CTexturedUIShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	CTexturedShader::BuildObjects(pd3dDevice, pd3dCommandList, pContext);
	
	m_nTexture = 1;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Gunshit.dds", 0);
	//m_pTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//m_pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/GameMenu.dds", 0);


	//m_nObject = 1;

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, false);
	//CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[1], 2, false);



}
void CTexturedUIShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer)
{
	//릴리즈 실행시 FrameWork의 카메라를 인자로 전달받는 도중, pOstProcessing의 Render와 같은 카메라이다.
	// SetViewPortsAnd,,이 함수를 거의 동시에 2번 호출해서 문제가 되는듯 하다.
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera


	if (m_pTexture) m_pTexture[0]->UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCommandList->DrawInstanced(6, 1, 0, 0);

	

}
void CTexturedUIShader::ReleaseObjects()
{
	if (m_pTexture)
	{
		for (int i = 0; i < m_nTexture; i++)
		{
			m_pTexture[i]->Release();
		}
	
	}
}


D3D12_BLEND_DESC CTexturedUIShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;					//알파커버리지
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

	return(d3dBlendDesc);
}
////////////////////////////////////////////////////////////////////////////////////////
//
CTexturedMenuUIShader::CTexturedMenuUIShader()
{

}

CTexturedMenuUIShader::~CTexturedMenuUIShader()
{

}

void CTexturedMenuUIShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	CTexturedShader::BuildObjects(pd3dDevice, pd3dCommandList, pContext);

	m_nTexture = 2;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/GameStart.dds", 0);
	m_pTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Keyboard.dds", 0);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, false);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[1], 0, false);

}

void CTexturedMenuUIShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer,int gamestate)
{
	//릴리즈 실행시 FrameWork의 카메라를 인자로 전달받는 도중, pOstProcessing의 Render와 같은 카메라이다.
	// SetViewPortsAnd,,이 함수를 거의 동시에 2번 호출해서 문제가 되는듯 하다.
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	UpdateShaderVariables(pd3dCommandList);
	//CShader::Render(pd3dCommandList, pCamera);
	if (gamestate == 0) {
		if (m_pTexture) m_pTexture[0]->UpdateShaderVariables(pd3dCommandList);
		pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pd3dCommandList->DrawInstanced(6, 1, 0, 0);
	}
	if (gamestate == 1)
	{
		if (m_pTexture) m_pTexture[1]->UpdateShaderVariables(pd3dCommandList);
		pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pd3dCommandList->DrawInstanced(6, 1, 0, 0);
	}
	



}
void CTexturedMenuUIShader::ReleaseObjects()
{
	if (m_pTexture)
	{
		for (int i = 0; i < m_nTexture; i++)
		{
			m_pTexture[i]->Release();
		}

	}
}

void CTexturedMenuUIShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[1];


	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 23; //t23 : gtxtGameMenu
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;


	D3D12_ROOT_PARAMETER pd3dRootParameters[1];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t23: gtxtGameMenu
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;




	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

}

D3D12_BLEND_DESC CTexturedMenuUIShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;					//알파커버리지
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

	return(d3dBlendDesc);
}
//////////////////////////////////////////////////////////////////////////////////
CTexturedTargetUIShader::CTexturedTargetUIShader()
{

}

CTexturedTargetUIShader::~CTexturedTargetUIShader()
{

}

void CTexturedTargetUIShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	
	m_pTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Target.dds", 0);

	//m_nObject = 1;

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, true);
}

void CTexturedTargetUIShader::ReleaseObjects()
{
	if (m_pTexture)
	{
		for (int i = 0; i < m_nTexture; i++)
		{
			m_pTexture[i]->Release();
		}

	}
}

void CTexturedTargetUIShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer)
{
	//릴리즈 실행시 FrameWork의 카메라를 인자로 전달받는 도중, pOstProcessing의 Render와 같은 카메라이다.
	// SetViewPortsAnd,,이 함수를 거의 동시에 2번 호출해서 문제가 되는듯 하다.
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	if (m_pd3dGraphicsRootSignature) pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	if (m_ppd3dPipelineStates) pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);	//그리기 시작

	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);				//camera

	//UpdateShaderVariables(pd3dCommandList);
	//CShader::Render(pd3dCommandList, pCamera);
	if (m_pTexture) m_pTexture[0]->UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCommandList->DrawInstanced(6, 1, 0, 0);



}

D3D12_SHADER_BYTECODE CTexturedTargetUIShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTexturedTargetUI", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTexturedTargetUIShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTexturedTargetUI", "ps_5_1", ppd3dShaderBlob));
}

D3D12_BLEND_DESC CTexturedTargetUIShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;					//알파커버리지
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

	return(d3dBlendDesc);
}

void CTexturedTargetUIShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[1];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 22; //t22 : gtxtTarget
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;



	D3D12_ROOT_PARAMETER pd3dRootParameters[1];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t22 : gtxtTarget
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;




	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

}

D3D12_SHADER_BYTECODE CTexturedMenuUIShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSTexturedMenuUI", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTexturedMenuUIShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{

	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSTexturedMenuUI", "ps_5_1", ppd3dShaderBlob));
}


/////////////////////////////////////////////////////////////////
//
CComputeShader::CComputeShader()
{

}

CComputeShader::~CComputeShader()
{

}

void CComputeShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[2];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 23; //t23 : gtxtBlurScene
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 0; //u0 : RWgtxtBlurScene
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[0].Constants.Num32BitValues = 12;
	pd3dRootParameters[0].Constants.ShaderRegister = 0; //b6 : gtxtSetting
	pd3dRootParameters[0].Constants.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; // t22: gtxtBlurScene
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[2].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; // u0: RWgtxtBlurScene
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;



	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;



	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	if (pd3dErrorBlob != nullptr)
	{
		::OutputDebugStringA((char*)pd3dErrorBlob->GetBufferPointer());
	}

	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dComputeRootSignature);

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
}

void CComputeShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets)
{

	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dComputeShaderBlob = NULL;
	

	D3D12_COMPUTE_PIPELINE_STATE_DESC d3dComputePipelineStateDesc[2];
	::ZeroMemory(&d3dComputePipelineStateDesc, sizeof(D3D12_COMPUTE_PIPELINE_STATE_DESC) * 2);
	d3dComputePipelineStateDesc[0].pRootSignature = m_pd3dComputeRootSignature;
	d3dComputePipelineStateDesc[0].CS = CreateComputeShader(&pd3dComputeShaderBlob, true);		//horz
	d3dComputePipelineStateDesc[0].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateComputePipelineState(&d3dComputePipelineStateDesc[0], __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);
	if (hResult == E_FAIL) {
		return;
	}

	d3dComputePipelineStateDesc[1].pRootSignature = m_pd3dComputeRootSignature;
	d3dComputePipelineStateDesc[1].CS = CreateComputeShader(&pd3dComputeShaderBlob, false);		//false
	d3dComputePipelineStateDesc[1].Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	hResult = pd3dDevice->CreateComputePipelineState(&d3dComputePipelineStateDesc[1], __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[1]);
	if (hResult == E_FAIL) {
		return;
	}


	if (pd3dComputeShaderBlob) pd3dComputeShaderBlob->Release();

}

std::vector<float> CComputeShader::CalcGaussWeights(float sigma)
{

	//가우스 함수 G(x) = exp(-x^2/2*sigma*sigma)
	float twoSigma2 = 2.0f*sigma*sigma;

	// Estimate the blur radius based on sigma since sigma controls the "width" of the bell curve.
	// For example, for sigma = 3, the width of the bell curve is 
	// 시그마는 벨 커브의 "너비"를 제어하기 때문에 시그마를 기반으로 흐림 반경을 계산하십시오.
	// 예를들면 , 시그마가 3일때 벨커브의 너비가 된다.
	int blurRadius = (int)ceil(2.0f * sigma);

	assert(blurRadius <= MaxBlurRadius);

	std::vector<float> weights;
	weights.resize(2 * blurRadius + 1);

	float weightSum = 0.0f;

	for (int i = -blurRadius; i <= blurRadius; ++i)
	{
		float x = (float)i;

		weights[i + blurRadius] = expf(-x * x / twoSigma2);		//e^(-x^2/2.0f*sigma*sigma)

		weightSum += weights[i + blurRadius];
	}

	// Divide by the sum so all the weights add up to 1.0.
	// 합계로 나누어서 모든 가중치가 1.0이 된다.
	// 가중치들의 합이 1보다 작으면 가중 평균에 의해 색상 성분들의 값이 작아지므로, 흐려진 이미지가 원본 이미지보다 어두워진다.
	// 반대로 가중치들의 합이 1보다 크면 색상 성분들의 값이 커지므로 흐려진 이미지가 더 밝아진다.
	for (int i = 0; i < weights.size(); ++i)
	{
		weights[i] /= weightSum;
	}

	return weights;
}
void CComputeShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pRenderTexture, ID3D12Resource **pDestTexture, CCamera *pCamera, int blurCount)
{


	auto weights = CalcGaussWeights(2.5f);
	int blurRadius = (int)weights.size() / 2;


	pd3dCommandList->SetComputeRootSignature(m_pd3dComputeRootSignature);
	pd3dCommandList->SetComputeRoot32BitConstants(0, 1, &blurRadius, 0);
	pd3dCommandList->SetComputeRoot32BitConstants(0, (UINT)weights.size(), weights.data(), 1);

	//렌더 타겟 자료를 읽기 버퍼에 복사하는 명령을 등록한다.



	D3D12_RESOURCE_BARRIER d3dResourceBarrier[12];
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER) * 12);
	d3dResourceBarrier[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[0].Transition.pResource = pRenderTexture;				//이부분 주의
	d3dResourceBarrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	d3dResourceBarrier[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[0]);

	d3dResourceBarrier[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[1].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];// pDestTexture[0];				//이부분 주의
	d3dResourceBarrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	d3dResourceBarrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[1]);


	pd3dCommandList->CopyResource(m_pTexture[0]->m_ppd3dTextures[0], pRenderTexture);



	d3dResourceBarrier[2].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[2].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[2].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];				//이부분 주의
	d3dResourceBarrier[2].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[2].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[2].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[2]);

	d3dResourceBarrier[3].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[3].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[3].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의
	d3dResourceBarrier[3].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	d3dResourceBarrier[3].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	d3dResourceBarrier[3].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[3]);

	for (int i = 0; i < blurCount; ++i)
	{
		//수평 흐리기
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);
		if (m_pd3dCbvSrvUavDescriptorHeap) {
			pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvUavDescriptorHeap);
		}

		pd3dCommandList->SetComputeRootDescriptorTable(1, m_pTexture[0]->m_pRootArgumentInfos[0].m_d3dSrvGpuDescriptorHandle);
		pd3dCommandList->SetComputeRootDescriptorTable(2, m_pTexture[0]->m_pRootArgumentInfos[1].m_d3dUavGpuDescriptorHandle);


		UINT numGroupsX = (UINT)ceilf(640 / 256.0f);
		pd3dCommandList->Dispatch(numGroupsX,480, 1);

		d3dResourceBarrier[4].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier[4].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier[4].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];				//이부분 주의
		d3dResourceBarrier[4].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
		d3dResourceBarrier[4].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		d3dResourceBarrier[4].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[4]);

		d3dResourceBarrier[5].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier[5].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier[5].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의		1
		d3dResourceBarrier[5].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		d3dResourceBarrier[5].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
		d3dResourceBarrier[5].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[5]);


		////수직 흐리기
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);

		pd3dCommandList->SetComputeRootDescriptorTable(1, m_pTexture[0]->m_pRootArgumentInfos[1].m_d3dSrvGpuDescriptorHandle);
		pd3dCommandList->SetComputeRootDescriptorTable(2, m_pTexture[0]->m_pRootArgumentInfos[0].m_d3dUavGpuDescriptorHandle);


		UINT numGroupsY = (UINT)ceilf(480 / 256.0f);
		pd3dCommandList->Dispatch(640, numGroupsY, 1);

		d3dResourceBarrier[6].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier[6].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier[6].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];				//이부분 주의
		d3dResourceBarrier[6].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		d3dResourceBarrier[6].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
		d3dResourceBarrier[6].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[6]);


		d3dResourceBarrier[7].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier[7].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier[7].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의	1
		d3dResourceBarrier[7].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
		d3dResourceBarrier[7].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		d3dResourceBarrier[7].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[7]);

	}


	d3dResourceBarrier[8].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[8].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[8].Transition.pResource = pRenderTexture;				//이부분 주의
	d3dResourceBarrier[8].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	d3dResourceBarrier[8].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[8].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[8]);


	pd3dCommandList->CopyResource(pRenderTexture, m_pTexture[0]->m_ppd3dTextures[0]);


	d3dResourceBarrier[9].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[9].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[9].Transition.pResource = pRenderTexture;				//이부분 주의
	d3dResourceBarrier[9].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[9].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[9].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[9]);



	d3dResourceBarrier[10].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[10].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[10].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];				//이부분 주의
	d3dResourceBarrier[10].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[10].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
	d3dResourceBarrier[10].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[10]);

	d3dResourceBarrier[11].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[11].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[11].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의
	d3dResourceBarrier[11].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	d3dResourceBarrier[11].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
	d3dResourceBarrier[11].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[11]);


}

D3D12_SHADER_BYTECODE CComputeShader::CreateComputeShader(ID3DBlob **ppd3dShaderBlob, bool bHorizon)
{
	D3D12_SHADER_BYTECODE shader;
	if (bHorizon)
	{
		shader = CShader::CompileShaderFromFile(L"Blur.hlsl", "HorzBlurCS", "cs_5_1", ppd3dShaderBlob);
	}
	else {
		shader = CShader::CompileShaderFromFile(L"Blur.hlsl", "VertBlurCS", "cs_5_1", ppd3dShaderBlob);
	}
	return shader;
}
void CComputeShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture* pContext)
{

	/*m_nSrvTexture = 1;
	m_pSrvTexture = new CTexture*[m_nSrvTexture];
	m_pSrvTexture[0] = (CTexture *)pContext1;*/

	m_nTexture = 1;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = (CTexture *)pContext;


	CreateCbvAndSrvAndUavDesciptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 2);


	CreateSrvAndUavViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 1, 2);


}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//CSobelComputeShader
CSobelComputeShader::CSobelComputeShader()
{
}

CSobelComputeShader::~CSobelComputeShader()
{

}

D3D12_SHADER_BYTECODE CSobelComputeShader::CreateComputeShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Sobel.hlsl", "SobelCS", "cs_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CSobelComputeShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSToonShader", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CSobelComputeShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSToonShader", "ps_5_1", ppd3dShaderBlob));
}

void CSobelComputeShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets)
{
	m_nPipelineStates = 2;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dPixelShaderBlob = NULL;
	ID3DBlob *pd3dComputeShaderBlob = NULL;


	D3D12_COMPUTE_PIPELINE_STATE_DESC d3dComputePipelineStateDesc;
	::ZeroMemory(&d3dComputePipelineStateDesc, sizeof(D3D12_COMPUTE_PIPELINE_STATE_DESC));
	d3dComputePipelineStateDesc.pRootSignature = m_pd3dComputeRootSignature;
	d3dComputePipelineStateDesc.CS = CreateComputeShader(&pd3dComputeShaderBlob);		//horz
	d3dComputePipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateComputePipelineState(&d3dComputePipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);
	if (hResult == E_FAIL) {
		return;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = m_pd3dComputeRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = nRenderTargets;
	for (UINT i = 0; i < nRenderTargets; i++) d3dPipelineStateDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[1]);
	if (hResult == E_FAIL) {
		return;
	}

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();
	if (pd3dComputeShaderBlob) pd3dComputeShaderBlob->Release();
}

void CSobelComputeShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[3];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 0; //t0 : gInput
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 1; //t1 : gEdgeMap
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 0; //u0 : gOutput
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER pd3dRootParameters[3];




	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; // t0: gInput
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; // t1: gEdgeMap
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[2].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[2]; // u0: gOutput
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;




	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;


	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	if (pd3dErrorBlob != nullptr)
	{
		::OutputDebugStringA((char*)pd3dErrorBlob->GetBufferPointer());
	}

	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dComputeRootSignature);

	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
}

void CSobelComputeShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CTexture *pContext)
{
	m_nTexture = 1;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = (CTexture *)pContext;


	CreateCbvAndSrvAndUavDesciptorHeaps(pd3dDevice, pd3dCommandList, 0, 2, 2);


	CreateSrvAndUavViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, 2);

}

void CSobelComputeShader::SobelFilter(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pRenderTexture, ID3D12Resource **pDestTexture, CCamera* pCamera)
{

	D3D12_RESOURCE_BARRIER d3dResourceBarrier[9];
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER) * 9);
	d3dResourceBarrier[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[0].Transition.pResource = pRenderTexture;				//이부분 주의
	d3dResourceBarrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	d3dResourceBarrier[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[0]);

	d3dResourceBarrier[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[1].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];// pDestTexture[0];				//이부분 주의
	d3dResourceBarrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[1]);


	pd3dCommandList->CopyResource(m_pTexture[0]->m_ppd3dTextures[0], pRenderTexture);


	d3dResourceBarrier[2].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[2].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[2].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[0];				//이부분 주의
	d3dResourceBarrier[2].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[2].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[2].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;



	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[2]);


	//Se
	pd3dCommandList->SetComputeRootSignature(m_pd3dComputeRootSignature);
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[0]);

	if (m_pd3dCbvSrvUavDescriptorHeap) {
		pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvUavDescriptorHeap);
	}


	pd3dCommandList->SetComputeRootDescriptorTable(0, m_pTexture[0]->m_pRootArgumentInfos[0].m_d3dSrvGpuDescriptorHandle);
	pd3dCommandList->SetComputeRootDescriptorTable(2, m_pTexture[0]->m_pRootArgumentInfos[1].m_d3dUavGpuDescriptorHandle);


	d3dResourceBarrier[3].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[3].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[3].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의
	d3dResourceBarrier[3].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[3].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	d3dResourceBarrier[3].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[3]);

	UINT numGroupsX = (UINT)ceilf(640 / 16.0f);
	UINT numGroupsY = (UINT)ceilf(480 / 16.0f);
	pd3dCommandList->Dispatch(numGroupsX, numGroupsY, 1);

	d3dResourceBarrier[4].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[4].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[4].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의
	d3dResourceBarrier[4].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	d3dResourceBarrier[4].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[4].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[4]);



	d3dResourceBarrier[5].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[5].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[5].Transition.pResource = pRenderTexture;				//이부분 주의
	d3dResourceBarrier[5].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	d3dResourceBarrier[5].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[5].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[5]);

	d3dResourceBarrier[6].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[6].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[6].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의
	d3dResourceBarrier[6].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[6].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	d3dResourceBarrier[6].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[6]);


	pd3dCommandList->CopyResource( pRenderTexture,m_pTexture[0]->m_ppd3dTextures[1]);


	d3dResourceBarrier[7].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[7].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[7].Transition.pResource = m_pTexture[0]->m_ppd3dTextures[1];				//이부분 주의
	d3dResourceBarrier[7].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	d3dResourceBarrier[7].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	d3dResourceBarrier[7].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[7]);


	d3dResourceBarrier[8].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier[8].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier[8].Transition.pResource = pRenderTexture;				//이부분 주의
	d3dResourceBarrier[8].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier[8].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier[8].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier[8]);

	
}

void CSobelComputeShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, ID3D12Resource* pRenderTexture, ID3D12Resource **pDestTexture, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dComputeRootSignature);
	pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[1]);
	
	if (m_pd3dCbvSrvUavDescriptorHeap) {
		pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvUavDescriptorHeap);
	}
	
	pd3dCommandList->SetGraphicsRootDescriptorTable(0, m_pTexture[0]->m_pRootArgumentInfos[0].m_d3dSrvGpuDescriptorHandle);
	pd3dCommandList->SetGraphicsRootDescriptorTable(1, m_pTexture[0]->m_pRootArgumentInfos[1].m_d3dSrvGpuDescriptorHandle);
	pd3dCommandList->SetGraphicsRootDescriptorTable(2, m_pTexture[0]->m_pRootArgumentInfos[1].m_d3dUavGpuDescriptorHandle);


	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCommandList->DrawInstanced(6, 1, 0, 0);

}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
CCartoonShader::CCartoonShader()
{

}

CCartoonShader::~CCartoonShader()
{

}

void CCartoonShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature, UINT nRenderTargets)
{
	m_nPipelineStates = 1;
	m_ppd3dPipelineStates = new ID3D12PipelineState*[m_nPipelineStates];

	//CPostProcessingShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature, nRenderTargets);

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
	d3dPipelineStateDesc.NumRenderTargets = nRenderTargets;
	for (UINT i = 0; i < nRenderTargets; i++) d3dPipelineStateDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void **)&m_ppd3dPipelineStates[0]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

D3D12_SHADER_BYTECODE CCartoonShader::CreateVertexShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "VSToonShader", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CCartoonShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSToonShader", "ps_5_1", ppd3dShaderBlob));
}

void CCartoonShader::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[2];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 24; //t24 : gtxtSceneColor
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 25; //t25 : gtxtNormal
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = 0;


	D3D12_ROOT_PARAMETER pd3dRootParameters[2];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[0].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //t24: gtxtDiffuseRamp
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[1].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; //t24: gtxtNormal;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister =0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&m_pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
}

void CCartoonShader::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, CPlayer *pPlayer)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	CShader::Render(pd3dCommandList, pCamera);

	if (m_pTexture)
	{
		for (int i = 0; i < m_nTexture; i++) {
			m_pTexture[i]->UpdateShaderVariables(pd3dCommandList);
			pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pd3dCommandList->DrawInstanced(6, 1, 0, 0);
		}
	}
}

void CCartoonShader::ReleaseObjects()
{
	if (m_pTexture)
	{
		for (int i = 0; i < m_nTexture; i++) {
			m_pTexture[i]->Release();
		}

	}
}

void CCartoonShader::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext)
{
	m_nTexture = 1;
	m_pTexture = new CTexture*[m_nTexture];
	m_pTexture[0] = (CTexture *)pContext;
	

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, m_pTexture[0]->GetTextures());
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture[0], 0, true);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
CCartoonShaderByLaplacianShader::CCartoonShaderByLaplacianShader()
{

}

CCartoonShaderByLaplacianShader::~CCartoonShaderByLaplacianShader()
{

}

D3D12_SHADER_BYTECODE CCartoonShaderByLaplacianShader::CreatePixelShader(ID3DBlob **ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shaders.hlsl", "PSCartoonByLaplacianEdge", "ps_5_1", ppd3dShaderBlob));
}


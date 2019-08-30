// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// 3DGP.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

// TODO: �ʿ��� �߰� �����
// �� ������ �ƴ� STDAFX.H���� �����մϴ�.



/*���� ���ҽ��� �����ϴ� �Լ��̴�. ������ �� ������ ���� ���� ���ҽ��� �����ϰ� �ʱ�ȭ �����Ͱ� ������ �ʱ�ȭ
�Ѵ�.*/
ID3D12Resource *CreateBufferResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType,
	D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource **ppd3dUploadBuffer)
{
	//���̶� �������� �Ҵ� ������ �ִ� �޸� ����
	ID3D12Resource *pd3dBuffer = NULL;
	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type = d3dHeapType;		//���� ����
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	//���� ���� CPU-������ �Ӽ�
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;		//���� ���� �޸� Ǯ
	d3dHeapPropertiesDesc.CreationNodeMask = 1;		//0,1: ���� GPU �����
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;		//���� ����Ϳ��� ���ҽ��� �� �� �ִ� ����� ������ ��Ÿ���� ��Ʈ

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;	//���� => ���۴� ���� ���ø��� �ǹ̾���
	d3dResourceDesc.Alignment = 0;			//0 �̸� 64KB
	d3dResourceDesc.Width = nBytes;			//���� ũ��
	d3dResourceDesc.Height = 1;				//����ũ��
	d3dResourceDesc.DepthOrArraySize = 1;	//���� �Ǵ� �迭�� ũ��
	d3dResourceDesc.MipLevels = 1;			//�Ҹ� ����,0(�ڵ� ���)
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;	//���ҽ� ���� DXGI_FORMAT_UNKNOWN�� ������ �����Ǿ����� ������
	d3dResourceDesc.SampleDesc.Count = 1;			
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;	//������ ���ҽ��� 1���� ���ҽ��� �����ϱ� ���� ���
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;	//���� ������ ��ǥ�� ���
	//�Ķ���ͷ� �Ѿ�� �� Ÿ���� ���ε� ���̸� �ʱ� ���¸� ���ε����� �ʱ� ������ D3D12_RESOURCE_GENERIC_READ�� �������ش�.
	if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates =			
		D3D12_RESOURCE_STATE_GENERIC_READ;
	//�������� READBACK�̸� 
	else if (d3dHeapType == D3D12_HEAP_TYPE_READBACK) d3dResourceInitialStates =
		D3D12_RESOURCE_STATE_COPY_DEST;
	HRESULT hResult = pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc,
		D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, d3dResourceInitialStates, NULL,
		__uuidof(ID3D12Resource), (void **)&pd3dBuffer);
	if (pData)
	{
		switch (d3dHeapType)
		{
		case D3D12_HEAP_TYPE_DEFAULT:
		{
			if (ppd3dUploadBuffer)
			{
				//���ε� ���۸� �����Ѵ�.
				d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
				//���ҽ��� �������� ���� �����Ѵ�.
				pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc,
					D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
					__uuidof(ID3D12Resource), (void **)ppd3dUploadBuffer);
				//���ε� ���۸� �����Ͽ� �ʱ�ȭ �����͸� ���ε� ���ۿ� �����Ѵ�.

				D3D12_RANGE d3dReadRange = { 0, 0 };
				UINT8 *pBufferDataBegin = NULL;
				//map�Լ��� ȣ���ϸ� ���ҽ��� ���� �����͸� ��ȯ�Ѵ�.
				(*ppd3dUploadBuffer)->Map(0, &d3dReadRange, (void **)&pBufferDataBegin);	
				//ù��° ����: ���긮�ҽ� �ε���, 0�ϰ�� ���� , �� ��° ����: cpu�� ������ �޸� ����,NULL(��ü ���긮�ҽ�), �� ��° ����: ���� ���ҽ������� ������(���긮�ҽ��� �����ּ�),NULL(CPU�����ּҸ� ĳ��)
				memcpy(pBufferDataBegin, pData, nBytes);

				(*ppd3dUploadBuffer)->Unmap(0, NULL);
				//���ε� ������ ������ ����Ʈ ���ۿ� �����Ѵ�.

				//���ε� ���� �ִ� ������ �����Ѵ�.
				pd3dCommandList->CopyResource(pd3dBuffer, *ppd3dUploadBuffer);
				D3D12_RESOURCE_BARRIER d3dResourceBarrier;
				::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
				d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;	//���ҽ� ���̸� ��Ÿ���� ���� �庮
				d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				d3dResourceBarrier.Transition.pResource = pd3dBuffer;
				d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
				d3dResourceBarrier.Transition.Subresource =
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);
			}
			break;
		}
		case D3D12_HEAP_TYPE_UPLOAD:
		{
			D3D12_RANGE d3dReadRange = { 0, 0 };
			UINT8 *pBufferDataBegin = NULL;
			pd3dBuffer->Map(0, &d3dReadRange, (void **)&pBufferDataBegin);
			memcpy(pBufferDataBegin, pData, nBytes);
			pd3dBuffer->Unmap(0, NULL);
			break;
		}
		case D3D12_HEAP_TYPE_READBACK:
			break;
		}
	}
	return(pd3dBuffer);
}


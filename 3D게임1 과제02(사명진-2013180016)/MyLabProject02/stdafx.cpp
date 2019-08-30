// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// 3DGP.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.



/*버퍼 리소스를 생성하는 함수이다. 버퍼의 힙 유형에 따라 버퍼 리소스를 생성하고 초기화 데이터가 있으면 초기화
한다.*/
ID3D12Resource *CreateBufferResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType,
	D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource **ppd3dUploadBuffer)
{
	//힙이란 동적으로 할당 받을수 있는 메모리 영역
	ID3D12Resource *pd3dBuffer = NULL;
	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type = d3dHeapType;		//힙의 유형
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	//힙에 대한 CPU-페이지 속성
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;		//힙에 대한 메모리 풀
	d3dHeapPropertiesDesc.CreationNodeMask = 1;		//0,1: 단일 GPU 어댑터
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;		//다중 어댑터에서 리소스를 볼 수 있는 노드의 집합을 나타내는 비트

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;	//버퍼 => 버퍼는 다중 샘플링이 의미없음
	d3dResourceDesc.Alignment = 0;			//0 이면 64KB
	d3dResourceDesc.Width = nBytes;			//가로 크기
	d3dResourceDesc.Height = 1;				//세로크기
	d3dResourceDesc.DepthOrArraySize = 1;	//깊이 또는 배열의 크기
	d3dResourceDesc.MipLevels = 1;			//밈맵 레벨,0(자동 계산)
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;	//리소스 형식 DXGI_FORMAT_UNKNOWN은 형식이 지정되어있지 않은거
	d3dResourceDesc.SampleDesc.Count = 1;			
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;	//다차원 리소스를 1차원 리소스로 매핑하기 위한 방법
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;	//복사 연산의 목표로 사용
	//파라미터로 넘어온 힙 타입이 업로드 힙이면 초기 상태를 업로드힙의 초기 상태인 D3D12_RESOURCE_GENERIC_READ로 지정해준다.
	if (d3dHeapType == D3D12_HEAP_TYPE_UPLOAD) d3dResourceInitialStates =			
		D3D12_RESOURCE_STATE_GENERIC_READ;
	//힙유형이 READBACK이면 
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
				//업로드 버퍼를 생성한다.
				d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
				//리소스와 내부적인 힙을 생성한다.
				pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc,
					D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
					__uuidof(ID3D12Resource), (void **)ppd3dUploadBuffer);
				//업로드 버퍼를 매핑하여 초기화 데이터를 업로드 버퍼에 복사한다.

				D3D12_RANGE d3dReadRange = { 0, 0 };
				UINT8 *pBufferDataBegin = NULL;
				//map함수를 호출하면 리소스에 대한 포인터를 반환한다.
				(*ppd3dUploadBuffer)->Map(0, &d3dReadRange, (void **)&pBufferDataBegin);	
				//첫번째 인자: 서브리소스 인덱스, 0일경우 버퍼 , 두 번째 인자: cpu가 접근할 메모리 영역,NULL(전체 서브리소스), 세 번째 인자: 서브 리소스에대한 포인터(서브리소스의 시작주소),NULL(CPU가상주소를 캐싱)
				memcpy(pBufferDataBegin, pData, nBytes);

				(*ppd3dUploadBuffer)->Unmap(0, NULL);
				//업로드 버퍼의 내용을 디폴트 버퍼에 복사한다.

				//업로드 힙의 있는 내용을 복사한다.
				pd3dCommandList->CopyResource(pd3dBuffer, *ppd3dUploadBuffer);
				D3D12_RESOURCE_BARRIER d3dResourceBarrier;
				::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
				d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;	//리소스 전이를 나타내는 전이 장벽
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


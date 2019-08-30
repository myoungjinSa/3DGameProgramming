#include "stdafx.h"
#include "Mesh.h"

CMesh::CMesh()
{
}

CMesh::CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CMesh::ReleaseUploadBuffers()
{
	//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = NULL;
	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
	m_pd3dIndexUploadBuffer = NULL;
};

void CMesh::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);	//ù���� ����=��ƮƮ ���� ,�ι�° = ������ ���� ���� ���� ����, 
	// ���� ��ŸƮ ���� 0�̰� �ι�°������ 5���� �����ϰ� �迭�� 5�� �Ѱ��ָ� �ڵ����� 0���� 4������ ������ ����ȴ�. -> �� �ѹ��� ��ġ ó���ϰ� �ȴ�.

	if (m_pd3dIndexBuffer)
	{
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		// ���������ο� �ϳ��� �ε��� ���۸� ������ ���ִ�.
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
		//�ε��� ���۰� ������ �ε��� ���۸� ����������(IA: �Է� ������)�� �����ϰ� �ε����� ����Ͽ� �������Ѵ�.
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}
//
//int CMesh::CheckRayIntersection(XMVECTOR& xmPickRayOrigin, XMVECTOR& xmvPickRayDirection, float *pfNearHitDistance)
//{
//	int nIntersections = 0;
//	bool bIntersected = 
//	//bool bIntersected = 
//}



CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
}

CCubeMeshDiffused::CCubeMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, float fWidth, float fHeight, float fDepth) : CMesh(pd3dDevice,
		pd3dCommandList)
{
	//������ü�� ������(����)�� 8���̴�.
	m_nVertices = 36;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	//���� ���۴� ������ü�� ������ 8���� ���� ���� �����͸� ������.
	CDiffusedVertex pVertices[36];
	//�ո� �簢���� ���� �ﰢ��
	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT4(Colors::White));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(Colors::White));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT4(Colors::White));
	//�ո� �簢���� �Ʒ��� �ﰢ��
	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT4(Colors::White));
	pVertices[4] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT4(Colors::White));
	pVertices[5] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(Colors::White));
	//���� �簢���� ���� �ﰢ��
	pVertices[6] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(Colors::Green));
	pVertices[7] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT4(Colors::Green));
	pVertices[8] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(Colors::Green));
	//���� �簢���� �Ʒ��� �ﰢ��
	pVertices[9] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(Colors::Green));
	pVertices[10] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(Colors::Green));
	pVertices[11] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT4(Colors::Green));
	//�޸� �簢���� ���� �ﰢ��
	pVertices[12] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT4(Colors::Red));
	pVertices[13] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(Colors::Red));
	pVertices[14] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT4(Colors::Red));
	//�޸� �簢���� �Ʒ��� �ﰢ��
	pVertices[15] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT4(Colors::Red));
	pVertices[16] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT4(Colors::Red));
	pVertices[17] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(Colors::Red));
	//�Ʒ��� �簢���� ���� �ﰢ��
	pVertices[18] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(Colors::Yellow));
	pVertices[19] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT4(Colors::Yellow));
	pVertices[20] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(Colors::Yellow));
	//�Ʒ��� �簢���� �Ʒ��� �ﰢ��
	pVertices[21] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(Colors::Yellow));
	pVertices[22] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(Colors::Yellow));
	pVertices[23] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT4(Colors::Yellow));
	//���� �簢���� ���� �ﰢ��
	pVertices[24] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(Colors::Blue));
	pVertices[25] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT4(Colors::Blue));
	pVertices[26] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(Colors::Blue));
	//���� �簢���� �Ʒ��� �ﰢ��
	pVertices[27] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(Colors::Blue));
	pVertices[28] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(Colors::Blue));
	pVertices[29] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT4(Colors::Blue));
	//������ �簢���� ���� �ﰢ��
	pVertices[30] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(Colors::Black));
	pVertices[31] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT4(Colors::Black));
	pVertices[32] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(Colors::Black));
	//������ �簢���� �Ʒ��� �ﰢ��
	pVertices[33] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(Colors::Black));
	pVertices[34] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(Colors::Black));
	pVertices[35] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT4(Colors::Black));
	
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	this->m_DiffuseVertex = new CDiffusedVertex[36];
	for(unsigned int i=0;i<m_nVertices;i++){
		//���ؽ� ������ ��� �迭
		this->m_DiffuseVertex[i] = pVertices[i];

	}
	 
	/*�ε��� ���۴� ������ü�� 6���� ��(�簢��)�� ���� ���� ������ ���´�. �ﰢ�� ����Ʈ�� ������ü�� ǥ���� ����
	�Ƿ� �� ���� 2���� �ﰢ���� ������ �� �ﰢ���� 3���� ������ �ʿ��ϴ�. ��, �ε��� ���۴� ��ü 36(=6*2*3)���� ��
	������ ������ �Ѵ�.*/
	if (m_pd3dIndexBuffer) {
		m_nIndices = 36;
		UINT pnIndices[36];
		//�� �ո�(Front) �簢���� ���� �ﰢ��
		pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
		//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
		pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
		//�� ����(Top) �簢���� ���� �ﰢ��
		pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
		//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
		pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
		//�� �޸�(Back) �簢���� ���� �ﰢ��
		pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
		//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
		pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
		//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
		pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
		//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
		pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
		//�� ����(Left) �簢���� ���� �ﰢ��
		pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
		//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
		pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
		//�� ����(Right) �簢���� ���� �ﰢ��
		pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
		//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
		pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;
		//�ε��� ���۸� �����Ѵ�.
		m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices,
			sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER,
			&m_pd3dIndexUploadBuffer);
		//�ε��� ���� �並 �����Ѵ�.
		m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
		m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
	}
}


//

CCubeMeshDiffused::~CCubeMeshDiffused(){

	
}



CAirplaneMeshDiffused::CAirplaneMeshDiffused(ID3D12Device *pd3dDevice,
	ID3D12GraphicsCommandList *pd3dCommandList, float fWidth, float fHeight, float fDepth,
	XMFLOAT4 xmf4Color) : CMesh(pd3dDevice, pd3dCommandList)
{
	m_nVertices = 24 * 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	//���� �׸��� ���� ����� �޽��� ǥ���ϱ� ���� ���� �������̴�. 
	CDiffusedVertex pVertices[24 * 3];
	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;
	//����� �޽��� ���� ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), -fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), -fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	//����� �޽��� �Ʒ��� ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), +fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), +fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	//����� �޽��� ������ ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), -fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), +fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), +fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	//����� �޽��� ����/������ ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(+x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	//����� �޽��� ���� ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), +fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), -fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, +(fy + y3), +fz),
		Vector4::Add(xmf4Color, RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x2, +y2, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	//����� �޽��� ����/���� ��
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(0.0f, 0.0f, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-x1, -y1, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, +fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	pVertices[i++] = CDiffusedVertex(XMFLOAT3(-fx, -y3, -fz), Vector4::Add(xmf4Color,
		RANDOM_COLOR));
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
}
CAirplaneMeshDiffused::~CAirplaneMeshDiffused()
{
}


CWallMeshDiffused::CWallMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, float fWidth, float fHeight, float fDepth) : CMesh(pd3dDevice,
		pd3dCommandList)
{
	//������ü�� ������(����)�� 8���̴�.
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	//���� ���۴� ������ü�� ������ 8���� ���� ���� �����͸� ������.
	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, 0.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, 0.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz*2.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz*2.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[4] = CDiffusedVertex(XMFLOAT3(-fx, -fy, 0.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[5] = CDiffusedVertex(XMFLOAT3(+fx, -fy, 0.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[6] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz*2.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[7] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz*2.0f), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
	/*�ε��� ���۴� ������ü�� 6���� ��(�簢��)�� ���� ���� ������ ���´�. �ﰢ�� ����Ʈ�� ������ü�� ǥ���� ����
	�Ƿ� �� ���� 2���� �ﰢ���� ������ �� �ﰢ���� 3���� ������ �ʿ��ϴ�. ��, �ε��� ���۴� ��ü 36(=6*2*3)���� ��
	������ ������ �Ѵ�.*/
	m_nIndices = 36;
	UINT pnIndices[36];


	//�� �ո�(Front) �簢���� ���� �ﰢ��
	pnIndices[0] = 3; pnIndices[2] = 1; pnIndices[1] = 0;
	//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
	pnIndices[3] = 2; pnIndices[5] = 1; pnIndices[4] = 3;


	// �յ� ���� �ձ�
	//�� ����(Top) �簢���� ���� �ﰢ��
	//pnIndices[6] = 0; pnIndices[8] = 5; pnIndices[7] = 4;
	//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
	//pnIndices[9] = 1; pnIndices[11] = 5; pnIndices[10] = 0;



	//�� �޸�(Back) �簢���� ���� �ﰢ��
	pnIndices[12] = 3; pnIndices[14] = 4; pnIndices[13] = 7;
	//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
	pnIndices[15] = 0; pnIndices[17] = 4; pnIndices[16] = 3;
	//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
	pnIndices[18] = 1; pnIndices[20] = 6; pnIndices[19] = 5;
	//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
	pnIndices[21] = 2; pnIndices[23] = 6; pnIndices[22] = 1;


	// �յ� ���� ����.
	//�� ����(Left) �簢���� ���� �ﰢ��
	//pnIndices[24] = 2; pnIndices[26] = 7; pnIndices[25] = 6;
	//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
	//pnIndices[27] = 3; pnIndices[29] = 7; pnIndices[28] = 2;


	//�� ����(Right) �簢���� ���� �ﰢ��
	pnIndices[30] = 6; pnIndices[32] = 4; pnIndices[31] = 5;
	//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
	pnIndices[33] = 7; pnIndices[35] = 4; pnIndices[34] = 6;



	//�ε��� ���۸� �����Ѵ�.
	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices,
		sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_pd3dIndexUploadBuffer);
	//�ε��� ���� �並 �����Ѵ�.
	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
}


CWallMeshDiffused::~CWallMeshDiffused() {
}


// �Ѿ� ���� �޽� ����� 
CBulletMeshDiffused::CBulletMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, float fWidth, float fHeight, float fDepth) : CMesh(pd3dDevice,
		pd3dCommandList)
{
	//������ü�� ������(����)�� 8���̴�.
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	//���� ���۴� ������ü�� ������ 8���� ���� ���� �����͸� ������.
	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[4] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[5] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[6] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	pVertices[7] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices,
		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
	/*�ε��� ���۴� ������ü�� 6���� ��(�簢��)�� ���� ���� ������ ���´�. �ﰢ�� ����Ʈ�� ������ü�� ǥ���� ����
	�Ƿ� �� ���� 2���� �ﰢ���� ������ �� �ﰢ���� 3���� ������ �ʿ��ϴ�. ��, �ε��� ���۴� ��ü 36(=6*2*3)���� ��
	������ ������ �Ѵ�.*/
	m_nIndices = 36;
	UINT pnIndices[36];
	//�� �ո�(Front) �簢���� ���� �ﰢ��
	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
	//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
	//�� ����(Top) �簢���� ���� �ﰢ��
	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
	//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
	//�� �޸�(Back) �簢���� ���� �ﰢ��
	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
	//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
	//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
	//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
	//�� ����(Left) �簢���� ���� �ﰢ��
	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
	//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
	//�� ����(Right) �簢���� ���� �ﰢ��
	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
	//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;
	//�ε��� ���۸� �����Ѵ�.
	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices,
		sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&m_pd3dIndexUploadBuffer);
	//�ε��� ���� �並 �����Ѵ�.
	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
}


CBulletMeshDiffused::~CBulletMeshDiffused() {
}


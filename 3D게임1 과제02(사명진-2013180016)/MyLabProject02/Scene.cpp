#include "stdafx.h"
#include "Scene.h"
#include "Player.h"

CScene::CScene(ID3D12Device *pd3dDevice)
{
	//m_pd3dPipelineState = NULL;
	m_pd3dGraphicsRootSignature = NULL;
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList,CPlayer* Player)
{
	// ��Ʈ �ñ״��� : � ���ҽ����� �׷��Ƚ� ���������ο� ����Ǵ°��� ����
	//m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	m_nShaders = 1;							//���̴��� ����
	m_pShaders = new CObjectsShader;		//���̴� ��ü 1���� �������� �Ҵ��Ѵ�.
	
	
	m_pShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pShaders[0].BuildObjects(pd3dDevice, pd3dCommandList,Player);
	//m_pShaders[1].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//dynamic_cast<CWallShader&>(m_pShaders[1]).BuildObjects(pd3dDevice, pd3dCommandList, Player);*/
	
	CExplosiveObject::PrepareExplosion(pd3dDevice,pd3dCommandList);


}


void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseUploadBuffers();
}
void CScene::AnimateObjects(float fTimeElapsed, CPlayer* player)
{

	//cout << "SCene : " << player << endl;
	//cout << player->GetPosition().z << endl;


	for (int i = 0; i < m_nShaders; i++) {
		m_pShaders[i].AnimateObjects(fTimeElapsed, player);
	}
}


ID3D12RootSignature *CScene::GetGraphicsRootSignature()
{
	return(m_pd3dGraphicsRootSignature);
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	//��Ʈ �ñ״��Ķ� 
	//1 � ���ҽ����� �׷��Ƚ� ���������ο� ����Ǵ°��� �����ϴ°�
	//2 ��� ����Ʈ���� ���̴� ���ҽ��� ���� - ���̴��� �䱸�ϴ� ������ ���� ����
	//									ex>-�׷��� ��Ʈ �ñ״���
	//									   -��� ��Ʈ �ñ״���
	//��Ʈ �Ű�������?
	//	-�ϳ��� ��Ʈ �ñ״����� ũ��� 64���� 32-��Ʈ(DWORD)���� ��� 
	//  - ���� ����ϴ� �Ű������� ���ʿ�(�����޸�)��ġ�ϴ°��� ����
	//		EX> ��Ʈ ���(Root Constant) : ���̴��� ��� ���۷� ���� , 32-��Ʈ(DWORD)
	//			��Ʈ ������(Root Descriptor) : ��� ����(CBV),����(SRV,UAV),64��Ʈ
	//			������ ���̺�(Descriptor Table) : ���ҽ� ������ ���� ������ ǥ�� ,32-��Ʈ


	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;
	//��Ʈ ���
	D3D12_ROOT_PARAMETER pd3dRootParameters[2];
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;	//��Ʈ ���
	pd3dRootParameters[0].Constants.Num32BitValues = 20;		//�������� ��ȣ
	pd3dRootParameters[0].Constants.ShaderRegister = 0;			//�������� ����
	pd3dRootParameters[0].Constants.RegisterSpace = 0;			//32-��Ʈ ����� ����
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 1;
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =		//��Ʈ �ñ״��� ���̾ƿ��� ���� ���û���
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |	//IA�ܰ踦 ���
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |		//�潦�̴��� �����Ҽ� ����
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |		//������ ���̴��� ������ �� ����
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |	//���� ���̴��� ������ �� ����
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;		//�ȼ����̴��� ������ ������
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);	//��Ʈ �ñ״����� ����(�Ķ����) ����
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;				//
	d3dRootSignatureDesc.NumStaticSamplers = 0;							//���� ���÷��� ����(2032��)
	d3dRootSignatureDesc.pStaticSamplers = NULL;						
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;					//��Ʈ �ñ״��� ���̾ƿ��� ���� ���û���
	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pd3dSignatureBlob, &pd3dErrorBlob);

	//ù��° ����: ��Ʈ �ñ״��İ� ����� ��带 ��Ÿ���� ��Ʈ ,0�ϰ�� ���� GPU
	//�ι�° ����: ����ȭ�� ��Ʈ �ñ״��Ŀ� ���� ������ ������
	//����° ����: ��Ʈ �ñ״����� ũ��(����Ʈ ��)
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void
			**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
	return(pd3dGraphicsRootSignature);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].ReleaseShaderVariables();
		m_pShaders[i].ReleaseObjects();
	}
	if (m_pShaders) delete[] m_pShaders;
}


bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
	lParam)
{
	return(false);
}
void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam,
	LPARAM lParam,CPlayer *player)
{	
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam) 
		{
			case '1':
			{
				for (int i = 0; i < this->m_pShaders[0].m_nObjects; i++) {

					CExplosiveObject * pExplosiveObject = dynamic_cast<CExplosiveObject*>(m_pShaders[0].m_ppObjects[i]);
					pExplosiveObject->m_bBlowingUp = true;

				}

				break;
			}
			case VK_CONTROL:
				if (CPlayer::m_currentBullet < CPlayer::m_countBullet - 1) {
					player->m_pBulletObjects[CPlayer::m_currentBullet]->SetFire();
					player->m_pBulletObjects[CPlayer::m_currentBullet]->SetPosition(player->GetPosition());
					player->m_pBulletObjects[CPlayer::m_currentBullet]->SetMovingDirection(player->GetLookVector());					//CPlayer�� ��ġ���� ���� �����ؼ� ����
					player->m_pBulletObjects[CPlayer::m_currentBullet]->SetRotationAxis(player->m_pBulletObjects[CPlayer::m_currentBullet]->GetLook());
					player->m_pBulletObjects[CPlayer::m_currentBullet]->SetUpVector(player->GetUp());
					player->m_pBulletObjects[CPlayer::m_currentBullet]->SetRightVector(player->GetRight());
					if (CPlayer::m_currentBullet < CPlayer::m_countBullet - 1) {
						CPlayer::m_currentBullet++;
					}
				}
				break;
			case VK_SHIFT:
				CPlayer::LoadBullet(20);
				break;
			case 'Z':
				for (int i = 0; i < m_pShaders->m_nObjects; i++) {
					CExplosiveObject *pExplosiveObject = dynamic_cast<CExplosiveObject*>(m_pShaders->m_ppObjects[i]);
					ContainmentType containType = player->m_sphereBox->Contains(pExplosiveObject->m_xmOOBB);
					if (containType == CONTAINS) {
						pExplosiveObject->m_bBlowingUp = true;

					}
				}
				break;
		}
		default:
			break;
	
	break;

	
	
	}

}

//bool CScene::ProcessInput()
//{
//	return(false);
//}




void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].Render(pd3dCommandList, pCamera);
	}
}

CGameObject* CScene::PickObjectByCursor(int xClient, int yClient, CCamera* pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / pCamera->GetViewport().Width) - 1) / pCamera->GetProjectionMatrix()._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / pCamera->GetViewport().Height) - 1) / pCamera->GetProjectionMatrix()._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->GetViewMatrix());

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject *pNearestObject = NULL;

	for (int j = 0; j < m_nShaders; j++) {
		for (int i = 0; i < m_pShaders[j].m_nObjects; i++) {
			float fHitDistance = FLT_MAX;
			nIntersected = m_pShaders[j].m_ppObjects[i]->Pick(xmvPickPosition, xmmtxView, &fHitDistance);
			if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
			{
				fNearestHitDistance = fHitDistance;
				pNearestObject = m_pShaders[j].m_ppObjects[i];
			}

		}
	}
	if (pNearestObject) {
		float tmin = FLT_MAX;
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		pNearestObject->GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		for (int i = 0; i < 36; i += 6) {
			XMVECTOR v0 = XMLoadFloat3(&pNearestObject->GetMesh()->m_DiffuseVertex[i].GetPosition());
			XMVECTOR v1 = XMLoadFloat3(&pNearestObject->GetMesh()->m_DiffuseVertex[i + 1].GetPosition());
			XMVECTOR v2 = XMLoadFloat3(&pNearestObject->GetMesh()->m_DiffuseVertex[i + 2].GetPosition());

			float t = 0.0f;
			if (TriangleTests::Intersects(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, t)) {
				if (t < tmin) {
					tmin = t;

					pNearestObject->mPickedTriangle = i;
					
				}
			}
		}
		if (pNearestObject->mPickedTriangle >= 0 && pNearestObject->mPickedTriangle < 6) {
			
			pNearestObject->m_xmColor = 0;
		}
		else if (pNearestObject->mPickedTriangle >= 6 && pNearestObject->mPickedTriangle < 12) {
			
			pNearestObject->m_xmColor = 1;
		}
		else if (pNearestObject->mPickedTriangle >= 12 && pNearestObject->mPickedTriangle < 18) {
			
			pNearestObject->m_xmColor = 2;
		}
		else if (pNearestObject->mPickedTriangle >= 18 && pNearestObject->mPickedTriangle < 24) {
			
			pNearestObject->m_xmColor = 3;
		}
		else if (pNearestObject->mPickedTriangle >= 24 && pNearestObject->mPickedTriangle < 30) {
			
			pNearestObject->m_xmColor = 4;
		}
		else if (pNearestObject->mPickedTriangle >= 30 && pNearestObject->mPickedTriangle < 36) {
			
			pNearestObject->m_xmColor = 5;
		}
		else {
			pNearestObject->m_xmColor =20; 
		}
	}
	return(pNearestObject);
}
CScene::~CScene()
{
}

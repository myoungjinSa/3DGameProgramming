#include "stdafx.h"
#include "Scene.h"
inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::ComparePosition(int num) {
	
	bool ret = false;
		if ((m_ppObjects[num]->GetPosition().z) >= 0 && (m_ppObjects[num]->GetPosition().z < m_pPlayer->GetPosition().z)
			&&m_ppObjects[num]->GetPosition().x <= HALF_WIDTH && m_ppObjects[num]->GetPosition().x >=-HALF_WIDTH
			&&m_ppObjects[num]->GetPosition().y <= HALF_DEPTH && m_ppObjects[num]->GetPosition().y >= -HALF_DEPTH)
		{
			ret = true;
		}
		
		return ret;
}


void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					CExplosiveObject *pExplosiveObject = m_ppObjects[int(wParam - '1')];				//GameObject�� �ڽ�Ŭ������ ����ȯ�Ͽ� ����
					pExplosiveObject->m_bBlowingUp = true;
					break;
				}
				case 'A':
					for (int i = 0; i < m_nObjects; i++)
					{
						CExplosiveObject *pExplosiveObject = m_ppObjects[i];
						pExplosiveObject->m_bBlowingUp = true;
						

					}
					break;
				case 'Z':
					//�ݰ� 50m�̳��� ������ ��� ���� 
			
					if (CExplosiveObject::RedboxBoom > 0) {
						for (int i = 0; i < m_nObjects; i++) {
							CExplosiveObject *pExplosiveObject = m_ppObjects[i];
							ContainmentType containType = m_pPlayer->m_sphereBox->Contains(pExplosiveObject->m_xmOOBB);
							if (containType == CONTAINS) {


								pExplosiveObject->m_bBlowingUp = true;
								


							}
						}
						CExplosiveObject::RedboxBoom -= 1;
					}
				

					
					break;
				case VK_CONTROL:
					if (CPlayer::m_currentBullet < CPlayer::m_countBullet - 1) {
						m_pPlayer->m_pBulletObjects[CPlayer::m_currentBullet]->SetFire();
						m_pPlayer->m_pBulletObjects[CPlayer::m_currentBullet]->SetPosition(m_pPlayer->GetPosition());
						m_pPlayer->m_pBulletObjects[CPlayer::m_currentBullet]->SetMovingDirection(m_pPlayer->m_xmf3Look);					//CPlayer�� ��ġ���� ���� �����ؼ� ����
						m_pPlayer->m_pBulletObjects[CPlayer::m_currentBullet]->SetRotationAxis(m_pPlayer->m_pBulletObjects[CPlayer::m_currentBullet]->GetLook());


						if (CPlayer::m_currentBullet < CPlayer::m_countBullet-1) {
							CPlayer::m_currentBullet++;
						}
					}


					break;
				case VK_SHIFT:
					CPlayer::LoadBullet(20);												//����

					break;
			}
				default:
					break;
		break;
	}
}

void CScene::BuildObjects(HINSTANCE hInstance,CPlayer *pPlayer)
{
	
	SetImage(hInstance);
	m_pPlayer = pPlayer;
	CExplosiveObject::PrepareExplosion();									//������ �غ�

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 500.0f;
	CWallMesh *pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);
	pWallCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 500.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(0, 0, 0)); 
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);

	CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_nObjects = 1;
	m_ppObjects = new CExplosiveObject*[MAX_BOX_COUNT];
	for (int i = 0; i < MAX_BOX_COUNT; i++) {
		m_ppObjects[i] = new CExplosiveObject(m_pPlayer);
		m_ppObjects[i]->SetMesh(pObjectCubeMesh);
	}
	for (int i = 0; i < m_nObjects; i++) {
		if (i % 4 == 0) {
			m_ppObjects[i]->SetColor(RGB(255,0,255));
			m_ppObjects[i]->SetMovingSpeed(10.5f);
		}
		else if (i % 4 == 1) {
			m_ppObjects[i]->SetColor(RGB(0,255,0));
			m_ppObjects[i]->SetMovingSpeed(10.5f);
		}
		else if (i % 4 == 2) {
			m_ppObjects[i]->SetColor(RGB(0, 0, 255));
			m_ppObjects[i]->SetMovingSpeed(10.5f);
		}
		else if (i % 4 == 3) {
			m_ppObjects[i]->SetColor(RGB(255, 0, 0));
			m_ppObjects[i]->SetMovingSpeed(40.5f);
		}
		//InitBoxObject(i);
		m_ppObjects[i]->SetPosition(RandF(-fHalfWidth, fHalfWidth), RandF(-fHalfHeight, fHalfHeight), RandF(0, fHalfDepth*2.0f));
		m_ppObjects[i]->SetRotationAxis(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));
		m_ppObjects[i]->SetRotationSpeed(90.0f);
		m_ppObjects[i]->SetMovingDirection(XMFLOAT3(RandF(-1.0f,1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));
		
	}
	CCubeMesh *pBossMesh = new CCubeMesh(16.0f, 16.0f, 16.0f);
	pBossMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(8.0f, 8.0f, 8.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//------���� ���� ���� --------------------------------------------------------------------------------------
	m_pBossObject = new CBoss(pPlayer);
	m_pBossObject->SetMesh(pBossMesh);
	m_pBossObject->SetColor(RGB(95, 150, 225));

	m_pBossObject->SetPosition(0.0f, 0.0f, 800.0f);
	m_pBossObject->SetRotationAxis(XMFLOAT3(0.0f,1.0f,0.0f));
	m_pBossObject->SetRotationSpeed(120.0f);
	m_pBossObject->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 0.0f));
		
}

void CScene::GenerateBoxObject(int num,CPlayer *pPlayer) {
	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 500.0f;
	if (m_nObjects < MAX_BOX_COUNT-1) {
		m_nObjects += 1;
		if (num % 3 == 0) {
			m_ppObjects[num]->SetColor(RGB(255, 0, 255));
			m_ppObjects[num]->SetMovingSpeed(10.5f);
		}
		else if (num % 3 == 1) {
			m_ppObjects[num]->SetColor(RGB(0, 255, 0));
			m_ppObjects[num]->SetMovingSpeed(10.5f);
		}
		else if (num % 3 == 2) {
			m_ppObjects[num]->SetColor(RGB(0, 0, 255));
			m_ppObjects[num]->SetMovingSpeed(10.5f);
		}
		
		m_ppObjects[num]->SetPosition(RandF(-fHalfWidth, fHalfWidth), RandF(-fHalfHeight, fHalfHeight), RandF(pPlayer->GetPosition().z+100.0f, fHalfDepth*2.0f));
		m_ppObjects[num]->SetRotationAxis(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));
		m_ppObjects[num]->SetRotationSpeed(90.0f);
		m_ppObjects[num]->SetMovingDirection(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));
	}
	
}

void CScene::GenerateRedBoxObject(int num,CPlayer *pPlayer) {
	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 500.0f;

	if (m_nObjects < MAX_BOX_COUNT - 1) {
		m_nObjects += 1;
		m_ppObjects[num]->SetColor(RGB(255, 0, 0));
		m_ppObjects[num]->SetMovingSpeed(20.5f);
		m_ppObjects[num]->SetPosition(RandF(-fHalfWidth, fHalfWidth), RandF(-fHalfHeight, fHalfHeight), RandF(pPlayer->GetPosition().z + 100.0f, fHalfDepth*2.0f));
		m_ppObjects[num]->SetRotationAxis(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));
		m_ppObjects[num]->SetRotationSpeed(90.0f);
		m_ppObjects[num]->SetMovingDirection(XMFLOAT3(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f)));

	}
}

void CScene::GeneratePickingRay(int sx, int sy) {
	XMFLOAT4X4 P = this->m_pPlayer->m_pCamera->GetProj4x4f();				//������� �ʿ�	

	//�þ� ������ ���� �������� ����Ѵ�.
	float vx = (2.0f * sx / m_pPlayer->m_pCamera->m_Viewport.m_nWidth - 1.0f) / P(0, 0);
	float vy = (-2.0f * sy / m_pPlayer->m_pCamera->m_Viewport.m_nHeight + 1.0f) / P(1, 1);

	//�þ� ���� ������ ����
	// �� �������� �þ� ������ �������� ������. �̴� ������ �ٷ� �þ� ������ ������ �����ֱ� �����̴�.
	//�̰����� �þ� ������ ���� �������� ���Ҽ� �ְ� ��. 
	//�׷��� �� �������� ��ü ���� �þ� ������ �־�� ������ �� �ִ�.
	rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);
	
	
	//�þ� ����� ���ϱ����� ���� �������� �þ� �������� ��ȯ�ϹǷ� ,�þ� ����� ������� ���� ������ �þ� �������� ����������� ��ȯ�Ѵ�.
	//����, �þ� ���� ���� �������� rv(t) = q+tu�̰� �þ� ����� V��� �ϸ� ������� ���� �������� ������ ���� �־�����. 
	// rw(t) = q�����(V) + tu�����(V) = qw + tuw
	//�̶� ������ ���� q�� �����ν� (��,qw = 1) ��ȯ�ǰ� ������ ���� u�� ���ͷμ�(��, uw=0)��ȯ���� �����ϱ� �ٶ���.

	//���� ��ȯ
	XMMATRIX V = this->m_pPlayer->m_pCamera->GetView();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);


	
	for (int i = 0; i < m_nObjects; i++) {
		if (m_ppObjects[i]->m_bActive) {
			

			XMMATRIX W = XMLoadFloat4x4(&m_ppObjects[i]->m_xmf4x4World);				//�� �ڽ� ��ü�� ������� ��ǥ�� ��ķ� �����´�.
			XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);			//

																						//w=1�� ������ǥ ���͸� �����.

			//�迭�� �޽��� vi �������� ��ȯ.
			XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

			////w=1�� ������ǥ ���͸� �����
			rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		
			rayDir = XMVector3TransformNormal(rayDir, toLocal);

		
			//���� �׽�Ʈ�� ���� ���� ���� ���̸� �����.
			rayDir = XMVector3Normalize(rayDir);

			XMFLOAT4 tempOrigin(0.0f, 0.0f, 0.0f, 0.0f);
			XMFLOAT4 tempDir(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4(&tempOrigin, rayOrigin);
			XMStoreFloat4(&tempDir, rayDir);
			cout << "rayOrigin= " << "X: " << tempOrigin.x << ",Y: " << tempOrigin.y << ",Z: " << tempOrigin.z << ",W: " << tempOrigin.w << endl;
			cout << "rayDir= " << "X: " << tempDir.x << ",Y: " << tempDir.y << ",Z: " << tempDir.z << ",W: " << tempDir.w << endl;

			//cout << m_ppObjects[i]->GetPosition().x << ",    " << m_ppObjects[i]->GetPosition().y << ",   " << m_ppObjects[i]->GetPosition().z << endl;


			//cout << tempBox.Center.x << "," << tempBox.Center.y << "," << tempBox.Center.z << endl;

			
			float tMin =0.0f;			//������ ���� ���� z��
			if (this->m_ppObjects[i]->m_xmOOBB.Intersects(rayOrigin,rayDir,tMin)) {
				m_ppObjects[i]->m_bBlowingUp = true;
			}
			//AnimatePickingRay(i);
		}
	}
}

void CScene::AnimatePickingRay(int num)
{
	//�޽��� �ٿ�� �ڽ��� ģ�ٸ� �츮�� �޽� �ﰢ���� ����� ���Դϴ�.
	//�׷��� ���� / �ﰢ�� �׽�Ʈ���Ͻʽÿ�.
	//��� ���ڸ� ġ�� ������ �޽��� ĥ �� �����Ƿ� ���� / �ﰢ�� �׽�Ʈ�� �����ϴ� �� �������� ���ʽÿ�.
	float tMin = 1000.f;			//������ ���� ���� z��
	if (m_ppObjects[num]->m_xmOOBB.Intersects(rayOrigin, rayDir, tMin)) {
		m_ppObjects[num]->m_bBlowingUp = true;
	}
}

void CScene::SetClientRect(int x, int y) {
	this->m_ClientWidth = x;
	this->m_ClientHeight = y;
}
void CScene::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		GeneratePickingRay(x, y);
	}
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]) {

			delete m_ppObjects[i];
			m_ppObjects[i] = NULL;
		}
	}
	if (m_ppObjects) {
		delete[] m_ppObjects;
		m_ppObjects = NULL;
	}

	if (m_pWallsObject) {
		delete m_pWallsObject;
		m_pWallsObject = NULL;
	}

	for (int i = 0; i < CPlayer::m_countBullet; i++) {
		delete m_pPlayer->m_pBulletObjects[i];									//�Ѿ� ����
		m_pPlayer->m_pBulletObjects[i] = NULL;
	}
	if (m_pPlayer->m_pBulletObjects)
	{
		delete[] m_pPlayer->m_pBulletObjects;
		m_pPlayer->m_pBulletObjects = NULL;
	}

	if (m_pBossObject) {
		delete m_pBossObject;
		m_pBossObject = NULL;
	}
	
}

void CScene::CheckObjectByObjectCollisions()
{
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
			if (m_ppObjects[i]->m_dwColor != RGB(255, 0, 0)) {
				XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
				float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
				m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
				m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
				m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
				m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
				m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
				m_ppObjects[i]->m_pObjectCollided = NULL;
			}
			else {
				XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
				m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
				m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
				m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
				m_ppObjects[i]->m_pObjectCollided = NULL;
			}
		}
	}

}
void CScene::CheckObjectByBulletCollisions()
{
	
	for (int i = 0; i < m_nObjects; i++) {
		for (int j = 0; j < CPlayer::m_currentBullet; j++) {
			ContainmentType containType = m_ppObjects[i]->m_xmOOBB.Contains(m_pPlayer->m_pBulletObjects[j]->m_xmOOBB);
			if (containType == INTERSECTS) {
				
				//CExplosiveObject *mObjects = m_ppObjects[i];
				//mObjects->m_bBlowingUp = true;

				//���� �ּ�ģ �ڵ�� �Ʒ� ��������� ���� ������ �ڵ��� ����� �� ���� �ڵ�?
				
				m_ppObjects[i]->m_bBlowingUp = true;


			}
		}
	}
	for (int j = 0; j < CPlayer::m_currentBullet; j++) {
		ContainmentType containType = m_pBossObject->m_xmOOBB.Contains(m_pPlayer->m_pBulletObjects[j]->m_xmOOBB);
		if (containType == INTERSECTS) {
			m_pBossObject->m_bBlowingUp = true;
		}
	}

	for (int i = 1; i <= CBoss::m_currentBullet; i++) {
		
		ContainmentType containType = m_pBossObject->m_pBulletObjects[i]->m_xmOOBB.Contains(m_pPlayer->m_xmOOBB);
		if (containType == INTERSECTS) {
			m_pPlayer->SetPosition(0, 0, 100);
		}
		
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
			case DISJOINT:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
					if (intersectType == BACK)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
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
					PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
					if (intersectType == INTERSECTING)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case CONTAINS:
				break;
		}
	}
}


void CScene::Animate(float fElapsedTime)
{
	//cout << this->m_specialKeyCount << endl;
	m_fElapsedTimes += fElapsedTime;
	m_fRedElapsedTimes += fElapsedTime;
	m_pWallsObject->Animate(fElapsedTime);
	if (m_pWallsObject->m_xmOOBB.Contains(XMLoadFloat3(&m_pPlayer->m_xmf3Position)) ==DISJOINT)
	{
			m_pPlayer->SetPosition(0.0f, 0.0f, 100.0f);
			
		
	}
	if (m_fElapsedTimes > 1.0) {
		
		GenerateBoxObject(m_nObjects, m_pPlayer);
		m_fElapsedTimes = 0.0f;
	}
	if (m_fRedElapsedTimes > 10.0f) {
		GenerateRedBoxObject(m_nObjects, m_pPlayer);
		m_fRedElapsedTimes = 0.0f;
	}

	for (int i = 0; i < m_nObjects; i++) {

		m_ppObjects[i]->Animate(fElapsedTime);

		if (m_ppObjects[i]->m_bActive) {
			if (m_ppObjects[i]->m_xmOOBB.Contains(XMLoadFloat3(&m_pPlayer->m_xmf3Position)) == CONTAINS) {
				//m_pPlayer->SetPosition(0.0f, 0.0f, 100.0f);
				//m_nObjects = 1;
				//m_fElapsedTimes = 0.0f;
				//m_fRedElapsedTimes = 0.0f;
				//if (m_pBossObject->m_bActive == FALSE) {
				//	m_pBossObject->m_bActive = TRUE;
			//	}

			}
		}
	}

	for(int i =0;i<m_nObjects;i++){
		if (m_ppObjects[i]->m_bActive) {

			if (ComparePosition(i)) {
				m_pPlayer->ChangeColor();
				break;
			}
			else {
				m_pPlayer->ChangeColor(RGB(0, 0, 255));
			}

		}
		//cout << m_fElapsedTimes << endl;
	}
	
	for (int i = 0; i < CPlayer::m_currentBullet; i++) {
		m_pPlayer->m_pBulletObjects[i]->Animate(fElapsedTime, m_pPlayer->GetPosition());

	}

	
	m_pBossObject->Animate(fElapsedTime,m_pPlayer);
	for (int i = 0 ;i < CBoss::m_currentBullet; i++) {
		m_pBossObject->m_pBulletObjects[i]->Animate(fElapsedTime,m_pBossObject->GetPosition());
	}

	CheckObjectByWallCollisions();

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);

	m_pBossObject->Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i <= CPlayer::m_currentBullet; i++) {
		m_pPlayer->m_pBulletObjects[i]->Render(hDCFrameBuffer, pCamera);
	}
	
	for (int i = 0; i <= CBoss::m_currentBullet; i++) {
		m_pBossObject->m_pBulletObjects[i]->Render(hDCFrameBuffer, pCamera);
	}

	HDC subDC = CreateCompatibleDC(hDCFrameBuffer);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(subDC, this->hGameOverBmp);
	BitBlt(hDCFrameBuffer, 0, 0, this->GameOverBmp.bmWidth ,this->GameOverBmp.bmHeight, subDC, 0, 0, SRCCOPY);

	SelectObject(subDC, oldBitmap);
	//DeleteObject();


}

////-----------------CGameOverScene-------------------
void CScene::SetImage(HINSTANCE g_hInst) {

	this->hGameOverBmp = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(L"GameOver"));

	GetObject(hGameOverBmp, sizeof(BITMAP), &this->GameOverBmp);

}
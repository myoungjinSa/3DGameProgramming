#include "stdafx.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayer::CPlayer():CGameObject()
{
	m_pCamera = new CCamera();
	m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, 60.0f);
	m_pCamera->SetViewport(0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);

	this->m_sphereBox = new BoundingSphere(XMFLOAT3(0.0f, 0.0f, 0.0f), 50.0f);		//반경 50m의 구체 생성

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	CCubeMesh* pBulletMesh = new CCubeMesh(1.0f, 1.0f, 3.0f);			//x=1.0,y=1.0,z=5.0
	pBulletMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 1.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pBulletObjects = new CBulletObject*[CPlayer::m_countBullet];

	for (int i = 0; i < CPlayer::m_countBullet; i++) {
		m_pBulletObjects[i] = new CBulletObject();
		m_pBulletObjects[i]->SetMesh(pBulletMesh);
		m_pBulletObjects[i]->SetColor(RGB(255, 0, 0));
		m_pBulletObjects[i]->SetPosition(this->GetPosition());
		m_pBulletObjects[i]->SetRotationSpeed(900.0f);
		m_pBulletObjects[i]->SetMovingDirection(this->GetPosition());
		m_pBulletObjects[i]->SetMovingSpeed(120.0f);
	}
}

CPlayer::~CPlayer()
{
}

//static==================================
int CPlayer::m_countBullet = 20;
int CPlayer::m_currentBullet = 0;


void CPlayer::LoadBullet(int number) {

	m_currentBullet = 0;

}
//=========================================
void CPlayer::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	this->m_sphereBox->Center.x = x;
	this->m_sphereBox->Center.y = y;
	this->m_sphereBox->Center.z = z;

	CGameObject::SetPosition(x, y, z);
}

void CPlayer::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3CameraOffset = xmf3CameraOffset;
	m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}

void CPlayer::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, true);
	}
}

void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)											//가속도의 변화가 있으면 
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);	//이동에 속도를 더해서 m_Xf3Velocity에 넣어라
	}
	else
	{
		m_xmf3Position = Vector3::Add(xmf3Shift, m_xmf3Position);	//위치에 이동을 더해서 위치를 갱신하라
		m_sphereBox->Center = Vector3::Add(xmf3Shift, m_sphereBox->Center);	//바운딩 구 위치 갱신
		m_xmOOBB.Center = Vector3::Add(xmf3Shift, m_xmOOBB.Center);
		m_pCamera->Move(xmf3Shift);									//카메라에 이동
	}
}

void CPlayer::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z), false);
}

void CPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_pCamera->Rotate(fPitch, fYaw, fRoll);
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));

	for (int i = CPlayer::m_currentBullet; i < CPlayer::m_countBullet; i++) {
		this->m_pBulletObjects[i]->RotateBullet(fPitch, fYaw, fRoll);
	}
	
}

void CPlayer::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, false);

	m_pCamera->Update(this, m_xmf3Position, fTimeElapsed);								//카메라 업데이트
	m_pCamera->GenerateViewMatrix();

	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(Vector3::ScalarProduct(m_xmf3Velocity, -1.0f));
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fTimeElapsed;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);
}

void CPlayer::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;

	m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), m_xmf4x4World);

	CGameObject::Render(hDCFrameBuffer, pCamera);
}

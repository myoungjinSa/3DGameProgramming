#include "StdAfx.h"
#include "GameObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

XMVECTOR RandomUnitVectorOnSphere()
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();

	while (true)
	{
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);				//������ ��ġ���� �����ϰ� ����, �����̱� ������ ������ 0
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)/*v�� ũ�Ⱑ 1���� ������ �˻�*/) return(XMVector3Normalize(v));			//XMVector3Greater(XMVector3Greater(v1,v3)) -> v1��(x,y,z)����
																									//									->v2�� (x,y,z)������ ũ�� True��ȯ
																									//									->������ false��ȯ
	}																								//XMVector3LengthSq(v) -> ||v||^2 ��ȯ
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject() 
{ 
	m_pMesh = NULL; 
	m_xmf4x4World = Matrix4x4::Identity();

	m_dwColor = RGB(0, 0, 0);

	m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;

	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.05f;
}

CGameObject::CGameObject(CMesh *pMesh) : CGameObject()
{
	m_pMesh = pMesh; 
}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();				//�ڱ� �޽��� ������ ������Ŵ
}


void CGameObject::SetPosition(float x, float y, float z) 
{
	m_xmf4x4World._41 = x; 
	m_xmf4x4World._42 = y; 
	m_xmf4x4World._43 = z; 
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position) 
{ 
	m_xmf4x4World._41 = xmf3Position.x; 
	m_xmf4x4World._42 = xmf3Position.y; 
	m_xmf4x4World._43 = xmf3Position.z; 
}

XMFLOAT3 CGameObject::GetPosition() 
{ 
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43)); 
}

XMMATRIX CGameObject::GetWorld()
{
	return (XMLoadFloat4x4(&this->m_xmf4x4World));
}
XMFLOAT3 CGameObject::GetLook() 
{ 	
	XMFLOAT3 xmf3LookAt(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3LookAt = Vector3::Normalize(xmf3LookAt);
	return(xmf3LookAt);
}

XMFLOAT3 CGameObject::GetUp() 
{ 	
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{ 	
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3LookAt = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3LookAt, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fYaw, fPitch, fRoll);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CGameObject::Animate(float fElapsedTime)
{
	if (this->m_bActive) {
		if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
		if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

		if (m_pMesh)
		{
			m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
			XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
		}
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (m_pMesh)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		m_pMesh->Render(hDCFrameBuffer, m_xmf4x4World, pCamera);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallsObject::CWallsObject()
{
}

CWallsObject::~CWallsObject()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh *CExplosiveObject::m_pExplosionMesh = NULL;

unsigned int CExplosiveObject::RedboxBoom = 0;
CExplosiveObject::CExplosiveObject(CGameObject *pTarget)
{
	m_pTarget = pTarget;
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::PrepareExplosion()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());	//�����ϰ� ������ ���⺤�͸� ����

	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);										//��ȯ���� CMesh ������ �ڽ� Ŭ������ CCubeMesh�� ����
																							//����� �θ� Ŭ������ CMesh�� 
}
void CExplosiveObject::SetBoxParalyzed()
{
	if (this->m_dwColor == RGB(255, 0, 0)) {
		CExplosiveObject::RedboxBoom += 1;
	}
	this->m_bActive = false;
}

void CExplosiveObject::Animate(float fElapsedTime)
{
	if (this->m_bActive) {
		if (m_bBlowingUp)									//���� ����
		{
			m_fElapsedTimes += fElapsedTime;				//���� Ÿ��		
			if (m_fElapsedTimes <= m_fDuration)				//����ð��� 2.5���� ���������� 
			{
				XMFLOAT3 xmf3Position = GetPosition();		//�� ���� ������Ʈ�� ���� ��ǥ ����� 41,42,43���� �����´�
				for (int i = 0; i < EXPLOSION_DEBRISES; i++)
				{
					m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
					m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;		//���������κ��� ����m_pxmf3SphereVectors�� �ӵ� speed*����ð��� ���Ѱ��� ���Ѵ�.
					m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);//���⺤�͸� ������ �� ȸ����İ� �̵������ ���Ѵ�. R*T

				}	//�������� ��ŭ�� ������� �ϳ��ϳ� ��ġ���� �������ش�.
			}
			else											//m_fDuration�� m_fElapsedTime���� Ŀ����
			{
				static unsigned int hitCheckCount = 0;
				m_bBlowingUp = false;						//bool���� false
				
				m_fElapsedTimes = 0.0f;						//�ð��� �ʱ�ȭ
				// �� ���� �ڽ����� �ٽ� ������ ��ġ�� �����ǰ� �ϸ� ��
				SetBoxParalyzed();

				if (this->m_dwColor == RGB(255, 0, 0)) {
					
					
				}

			}
		}
		else
		{
			XMFLOAT3 xmf3TargetPosition = m_pTarget->GetPosition();			//Ÿ������ �ϴ� ������Ʈ�� ��ġ�� xmf3Position�� �޾ƿ´�.
			XMFLOAT3 xmf3Position = GetPosition();							//������Ʈ�� ��ġ�� xmf3Position�� �޾ƿ´�.	

			XMFLOAT3 xmf3Look = GetLook();
			XMFLOAT3 xmf3ToTarget = Vector3::Subtract(xmf3TargetPosition, xmf3Position, true);		//true�̱⶧���� �����͸� ����ȭ �� ���Ͱ� ���´�
			float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget);						//a��b �������� => |a|*|b|*cos�� �� ���Ҽ��ִ�. =>
																									//a,b�Ѵ� ����ȭ�� �߱� ������ cos�谪�� fDotProduct�� ��
			float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);
			XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);							//�������� ȸ������ ���Ѵ�.																
																												//	if (fAngle != 0.0f) Rotate(0.0f, fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);

			if (Vector3::Distance(xmf3TargetPosition, xmf3Position) > 0.0f)
			{
				if (m_fRotationSpeed != 0.0f) {
					Rotate(fAngle*fElapsedTime*((xmf3CrossProduct.x>0.0f) ? 1.0f : -1.0f), fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), fAngle*fElapsedTime*xmf3ToTarget.z*((xmf3CrossProduct.z > 0.0f) ? 1.0f : -1.0f));
				}
				//SetMovingDirection(Vector3::ScalarProduct(xmf3Look,fElapsedTime,true));
				this->SetMovingDirection(XMFLOAT3(Vector3::Subtract(xmf3TargetPosition, this->GetPosition())));
				if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
			}

			if (this->m_pMesh)				//OBB�ڽ� ������Ʈ
			{
				this->m_pMesh->m_xmOOBB.Transform(this->m_xmOOBB, XMLoadFloat4x4(&this->m_xmf4x4World));
				XMStoreFloat4(&this->m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&this->m_xmOOBB.Orientation)));
			}
			//CGameObject::Animate(fElapsedTime);				//�׳� �������� �ʴ� ��� ������
		}
	}
	
}


void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (this->m_bActive) {
		if (m_bBlowingUp)					//������ �Ͼ��
		{
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				if (m_pExplosionMesh)		//CMesh�� �����ϴ� ����
				{
					HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
					HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
					m_pExplosionMesh->Render(hDCFrameBuffer, m_pxmf4x4Transforms[i], pCamera);
					::SelectObject(hDCFrameBuffer, hOldPen);
					::DeleteObject(hPen);
				}
			}
		}
		else
		{
			CGameObject::Render(hDCFrameBuffer, pCamera);
		}
	}
}

/////////////////////////////Bullet//////////////////////////////

CBulletObject::CBulletObject() {


	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
}
CBulletObject::~CBulletObject() {

}
void CBulletObject::Animate(float fElapsedTime,XMFLOAT3 &xmfPosition)
{
	if (m_fired ) {
		//CBulletObject::m_currentBullet++;
		m_fElapsedTimes += fElapsedTime;

		m_fMovingRange += m_fMovingSpeed*fElapsedTime;					//�߻��ؼ� �Ÿ�
		
		if (GetTargetDistance(xmfPosition)>220.0f) {
			m_fired = false;
			return;
		}

		
		if (m_fRotationSpeed != 0.0f) 
			Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
		//cout << "Bullet: " << "(" << m_xmf3RotationAxis.x << "," << m_xmf3RotationAxis.y << "," << m_xmf3RotationAxis.z << ")" << endl;
		if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

		if (m_pMesh)
		{
			m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
			XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
		}

	}
	else {
		m_fired = false;
		m_fMovingRange = 0.0f;

	}
}

void CBulletObject::Render(HDC hDCFrameBuffer, CCamera *pCamera) 
{
	if (m_fired) {
		
		this->m_xmf4x4World._31 = this->m_xmf3MovingDirection.x;
		this->m_xmf4x4World._32 = this->m_xmf3MovingDirection.y;
		this->m_xmf4x4World._33 = this->m_xmf3MovingDirection.z;

		m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.0f), 0.0f, 90.0f), m_xmf4x4World);

		CGameObject::Render(hDCFrameBuffer, pCamera);
	
		//CGameObject::Render(hDCFrameBuffer,pCamera);
	}
}

float CBulletObject::GetTargetDistance(XMFLOAT3 &target) 
{
	return Vector3::Length(Vector3::Subtract(target, this->GetPosition()));
}

void CBulletObject::SetFire() {
	this->m_fired = true;
}

void CBulletObject::RotateBullet(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fYaw, fPitch, fRoll);

	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
	
}


/////////////////////////////////////////////////////
//static



///////////////////////////////////////////////////////////////////
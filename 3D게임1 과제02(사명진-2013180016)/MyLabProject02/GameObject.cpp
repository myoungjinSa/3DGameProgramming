#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"

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


//=================================================================
CGameObject::CGameObject()
{
	m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;
	m_xmColor = 20;

	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}

void CGameObject::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CGameObject::ReleaseUploadBuffers()
{
	//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
}

void CGameObject::Animate(float fTimeElapsed)
{

	if (this->m_bActive) {
		//if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed*fTimeElapsed);
		//if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fTimeElapsed);

		if (m_pMesh) {
			
			m_xmOOBBTransformed.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
			XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
		}
	}


}

void CGameObject::OnPrepareRender()
{
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender();

	//��ü�� ������ ���̴� ����(��� ����)�� �����Ѵ�.
	UpdateShaderVariables(pd3dCommandList);
	if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}


void CGameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis),
		XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}



void CGameObject::CreateShaderVariables(ID3D12Device *pd3dDevice,
	ID3D12GraphicsCommandList *pd3dCommandList)
{
}
void CGameObject::ReleaseShaderVariables()
{
}

void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));


	//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);

	pd3dCommandList->SetGraphicsRoot32BitConstant(0, this->m_xmColor, 16);
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}
void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}
//���� ��ü�� ���� z-�� ���͸� ��ȯ�Ѵ�.
XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32,
		m_xmf4x4World._33)));
}
//���� ��ü�� ���� y-�� ���͸� ��ȯ�Ѵ�.
XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22,
		m_xmf4x4World._23)));
}
//���� ��ü�� ���� x-�� ���͸� ��ȯ�Ѵ�.
XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12,
		m_xmf4x4World._13)));
}
//���� ��ü�� ���� x-�� �������� �̵��Ѵ�. 
void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//���� ��ü�� ���� y-�� �������� �̵��Ѵ�. 
void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//���� ��ü�� ���� z-�� �������� �̵��Ѵ�. 
void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y*fSpeed, m_xmf4x4World._43 + vDirection.z*fSpeed);
}


//���� ��ü�� �־��� ������ ȸ���Ѵ�.
void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch),
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDir)
{
	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4World) * xmmtxView);

	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDir = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDir = XMVector3Normalize(xmvPickRayDir - xmvPickRayOrigin);
}
int CGameObject::Pick(XMVECTOR& xmvPickPosition, XMMATRIX &xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;
	bool bIntersected = false;

	if (m_pMesh) {
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		
		bIntersected = m_xmOOBBTransformed.Intersects(xmvPickRayOrigin, xmvPickRayDirection, *pfHitDistance);
		if (bIntersected) {

			//for (int i = 0; i < 36; i += 6) {
			//	XMVECTOR v0 = XMLoadFloat3(&GetMesh()->m_DiffuseVertex[i].GetPosition());
			//	XMVECTOR v1 = XMLoadFloat3(&GetMesh()->m_DiffuseVertex[i + 1].GetPosition());
			//	XMVECTOR v2 = XMLoadFloat3(&GetMesh()->m_DiffuseVertex[i + 2].GetPosition());

			//	float t = 0.0f;
			//	if (TriangleTests::Intersects(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, t)) {
			//		if (t < tmin) {
			//			tmin = t;
			//			
			//			if (mPickedTriangle==40) {
			//				mPickedTriangle = i;
			//			}
			//		}
			//	}
			//}
			nIntersected++;
		}
		//nIntersected = m_pMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);
	}
	return (nIntersected);
}

//
CWallsObject::CWallsObject()
{

}
CWallsObject::~CWallsObject()
{

}




//=================================================================
//CRotatingObject


CRotatingObject::CRotatingObject()
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 90.0f;
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}





CRotatingObject::~CRotatingObject()
{
}

//================================================================
//CExplosiveObject



//static

XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh *CExplosiveObject::m_pExplosionMesh = NULL;



void CExplosiveObject::PrepareExplosion(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) {
	for (int i = 0; i < EXPLOSION_DEBRISES; i++)
	{
		XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());	//�����ϰ� ������ ���⺤�͸� ����
	}

	m_pExplosionMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 0.5f, 0.5f, 0.5f);
}

unsigned int CExplosiveObject::RedboxBoom = 0;

CExplosiveObject::CExplosiveObject(CGameObject* pTarget):CRotatingObject()
{
	m_pTarget = pTarget;
	this->SetMovingSpeed(10.0f);
	
	
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::SetBoxParalyzed() 
{
	this->m_bActive = false;

}

void CExplosiveObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));


	//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);

	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 1, &this->m_xmColor, 16);
}


void CExplosiveObject::Animate(float fElapsedTime,CPlayer *player)
{
	if (this->m_bActive) {
		if (m_bBlowingUp)
		{
			m_fElapsedTimes += fElapsedTime;

			if (m_fElapsedTimes <= m_fDuration)
			{
				XMFLOAT3 xmf3Position = GetPosition();
				for (int i = 0; i < EXPLOSION_DEBRISES; i++) {
					m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
					m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);	

					

				}//�������� ��ŭ�� ������� �ϳ��ϳ� ��ġ���� �������ش�.

			}
			else {
				m_bBlowingUp = false;

				m_fElapsedTimes = 0.0f;

				SetBoxParalyzed();
			}
		}
		else {
			XMFLOAT3 xmf3TargetPosition = m_pTarget->GetPosition();
			XMFLOAT3 xmf3Position = GetPosition();

			XMFLOAT3 xmf3Look = GetLook();
			XMFLOAT3 xmf3ToTarget = Vector3::Subtract(xmf3TargetPosition, xmf3Position, true);			// true�̱⶧���� �����͸� ����ȭ �� ���Ͱ� ���´�
			float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget);							//a��b �������� => |a|*|b|*cos�� �� ���Ҽ��ִ�. =>
																										//a,b�Ѵ� ����ȭ�� �߱� ������ cos�谪�� fDotProduct�� ��
			float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);
			XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);

			if (Vector3::Distance(xmf3TargetPosition, xmf3Position) > 0.0f) {
				if (GetRotationSpeed() != 0.0f) {
					//Rotate(fAngle*fElapsedTime*((xmf3CrossProduct.x>0.0f) ? 1.0f : -1.0f), fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), fAngle*fElapsedTime*xmf3ToTarget.z*((xmf3CrossProduct.z > 0.0f) ? 1.0f : -1.0f));
					Rotate(fAngle*fElapsedTime*((xmf3CrossProduct.x>0.0f) ? 0.0f : 0.0f), fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 0.0f : 0.0f), fAngle*fElapsedTime*xmf3ToTarget.z*((xmf3CrossProduct.z > 0.0f) ? 0.0f : 0.0f));
				}

				this->SetMovingDirection(XMFLOAT3(Vector3::Subtract(xmf3TargetPosition, this->GetPosition())));
				this->SetMovingDirection(XMFLOAT3(this->m_xmf3MovingDirection));

				if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

			}
			CGameObject::Animate(fElapsedTime);
			//CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
		}
	}

}


void CExplosiveObject::Render(ID3D12GraphicsCommandList* pd3dCommandList,CCamera *pCamera)
{
	if (this->m_bActive) {
		if (m_bBlowingUp)					//������ �Ͼ��
		{
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				if (m_pExplosionMesh)		//CMesh�� �����ϴ� ����
				{
					//���⿡ �޽� �׸� �ʿ�
					//��ü�� ������ ���̴� ����(��� ����)�� �����Ѵ�.
					XMFLOAT4X4 xmf4x4World;
					XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_pxmf4x4Transforms[i])));


					//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
					pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
					
					pd3dCommandList->SetGraphicsRoot32BitConstant(0, m_xmColor, 16);
					//pd3dCommandList->SetGraphicsRoot32BitConstant(0,m_xmColor,16);
					if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
					m_pExplosionMesh->Render(pd3dCommandList);
					
				}
			}
		}
		else
		{
			CGameObject::Render(pd3dCommandList,pCamera);
		}
	}
}

//=============================================================
//bullet

CBulletObject::CBulletObject() {


	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	this->m_xmf3RotationAxis = XMFLOAT3(0.0f, 0.0f, 1.0f);
}
CBulletObject::~CBulletObject() {

}

void CBulletObject::Animate(float fElapsedTime, XMFLOAT3 &xmfPosition)
{
	if (m_fired) {
		//CBulletObject::m_currentBullet++;
		m_fElapsedTimes += fElapsedTime;

		m_fMovingRange += m_fMovingSpeed * fElapsedTime;					//�߻��ؼ� �Ÿ�

		if (GetTargetDistance(xmfPosition)>220.0f) {
			m_fired = false;
			return;
		}


		if (m_fRotationSpeed != 0.0f)
			Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
		//cout << "Bullet: " << "(" << m_xmf3RotationAxis.x << "," << m_xmf3RotationAxis.y << "," << m_xmf3RotationAxis.z << ")" << endl;
		if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);


		CGameObject::Animate(fElapsedTime);

	}
	else {
		m_fired = false;
		m_fMovingRange = 0.0f;

	}
}

void CBulletObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{

	this->m_xmf4x4World._11 = this->m_xmf3Right.x;
	this->m_xmf4x4World._12 = this->m_xmf3Right.y;
	this->m_xmf4x4World._13 = this->m_xmf3Right.z;

	this->m_xmf4x4World._21 = this->m_xmf3Up.x;
	this->m_xmf4x4World._22 = this->m_xmf3Up.y;
	this->m_xmf4x4World._23 = this->m_xmf3Up.z;

	this->m_xmf4x4World._31 = this->m_xmf3MovingDirection.x;
	this->m_xmf4x4World._32 = this->m_xmf3MovingDirection.y;
	this->m_xmf4x4World._33 = this->m_xmf3MovingDirection.z;




	m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.0f), 0.0f, 90.0f), m_xmf4x4World);

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));


	//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}
void CBulletObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera *pCamera)
{
	if (m_fired) {
		CGameObject::Render(pd3dCommandList, pCamera);
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


//CBoss 
//****************************************************************
CBoss::CBoss(CGameObject *pTarget):CExplosiveObject(pTarget)
{

}

CBoss::~CBoss()
{

}

void CBoss::TakeShot(float fElapsedTime) 
{
	m_fElapsedTimes += fElapsedTime;
	CBulletObject *pBulletObject = new CBulletObject();
	if (m_fElapsedTimes > 0.2f) {
		if (CBoss::m_currentBullet < CBoss::m_countBullet - 1) {
			//CBulletObject **pBulletObject = new CBulletObject*();
			pBulletObject = this->m_pBulletObjects[CBoss::m_currentBullet];
			pBulletObject->SetFire();
			pBulletObject->SetPosition(this->GetPosition());
			pBulletObject->SetMovingDirection(Vector3::Subtract(m_pTarget->GetPosition(), this->GetPosition(), true));					//�ٶ󺸰��ִ� �������� ���� ���
			if (CBoss::m_currentBullet < CBoss::m_countBullet - 1) {
				CBoss::m_currentBullet++;
			}
		}
		else {
			CBoss::LoadBullet();
		}
		m_fElapsedTimes = 0.0f;
	}

}

void CBoss::Animate(float fElapsedTime, CPlayer *pPlayer)
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
					m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
				}	//�������� ��ŭ�� ������� �ϳ��ϳ� ��ġ���� �������ش�.
			}
			else											//m_fDuration�� m_fElapsedTime���� Ŀ����
			{
				m_bBlowingUp = false;						//bool���� false
				m_fElapsedTimes = 0.0f;						//�ð��� �ʱ�ȭ
															// �� ���� �ڽ����� �ٽ� ������ ��ġ�� �����ǰ� �ϸ� ��
				SetBoxParalyzed();
			}
		}
		else {

			XMFLOAT3 xmf3TargetPosition = m_pTarget->GetPosition();			//Ÿ������ �ϴ� ������Ʈ�� ��ġ�� xmf3Position�� �޾ƿ´�.
			XMFLOAT3 xmf3Position = GetPosition();							//������Ʈ�� ��ġ�� xmf3Position�� �޾ƿ´�.	

			XMFLOAT3 xmf3Look = GetLook();
			XMFLOAT3 xmf3ToTarget = Vector3::Subtract(xmf3TargetPosition, xmf3Position, true);		//true�̱⶧���� �����͸� ����ȭ �� ���Ͱ� ���´�
			float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget);						//a��b �������� => |a|*|b|*cos�� �� ���Ҽ��ִ�. =>
																									//a,b�Ѵ� ����ȭ�� �߱� ������ cos�谪�� fDotProduct�� ��

			float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);
			XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);							//�������� ȸ������ ���Ѵ�.																
																												//	if (fAngle != 0.0f) Rotate(0.0f, fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);
			if (m_fRotationSpeed != 0.0f) {
				Rotate(fAngle*fElapsedTime*((xmf3CrossProduct.x>0.0f) ? 1.0f : -1.0f), fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), fAngle*fElapsedTime*xmf3ToTarget.z*((xmf3CrossProduct.z > 0.0f) ? 1.0f : 0.0f));
			}
			this->SetMovingDirection(XMFLOAT3(Vector3::Subtract(xmf3TargetPosition, this->GetPosition())));
		//	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);


			if (Vector3::Distance(xmf3TargetPosition, xmf3Position) > DISTANCE_TO_TARGET_OBJECT &&
				Vector3::Distance(xmf3TargetPosition, xmf3Position)<DISTANCE_TO_TARGET_OBJECT + 350)
			{
				//XMFLOAT3 xmf3Look = GetLook();	
				//0.2�ʸ��� �Ѿ� �߻�
				TakeShot(fElapsedTime);

			}

			
			CGameObject::Animate(fElapsedTime);

		}
	}
}

void CBoss::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera *pCamera)
{
	if (this->m_bActive) {
		if (m_bBlowingUp)					//������ �Ͼ��
		{
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				if (m_pExplosionMesh)		//CMesh�� �����ϴ� ����
				{
					//���⿡ �޽� �׸� �ʿ�
					//��ü�� ������ ���̴� ����(��� ����)�� �����Ѵ�.


					XMFLOAT4X4 xmf4x4World;
					XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_pxmf4x4Transforms[i])));

					//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
					pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);

					if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);
					m_pExplosionMesh->Render(pd3dCommandList);

				}
			}
		}
		else
		{
			CGameObject::Render(pd3dCommandList, pCamera);
		}
	}
}
//static==============================================
int CBoss::m_countBullet = 50;
int CBoss::m_currentBullet = 0;

void CBoss::LoadBullet() {
	m_currentBullet = 0;
}

//====================================================
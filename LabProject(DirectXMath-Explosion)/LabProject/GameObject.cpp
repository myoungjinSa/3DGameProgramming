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
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);				//벡터의 위치값을 랜덤하게 설정, 벡터이기 때문에 마지막 0
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)/*v의 크기가 1보다 작은지 검사*/) return(XMVector3Normalize(v));			//XMVector3Greater(XMVector3Greater(v1,v3)) -> v1의(x,y,z)값이
																									//									->v2의 (x,y,z)값보다 크면 True반환
																									//									->작으면 false반환
	}																								//XMVector3LengthSq(v) -> ||v||^2 반환
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
	if (m_pMesh) m_pMesh->Release();				//자기 메쉬를 스스로 해제시킴
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
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());	//랜덤하게 생성된 방향벡터를 저장

	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);										//반환형은 CMesh 생성은 자식 클래스인 CCubeMesh로 생성
																							//제어는 부모 클래스인 CMesh로 
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
		if (m_bBlowingUp)									//폭발 여부
		{
			m_fElapsedTimes += fElapsedTime;				//폭발 타임		
			if (m_fElapsedTimes <= m_fDuration)				//경과시간이 2.5보다 작을때까지 
			{
				XMFLOAT3 xmf3Position = GetPosition();		//그 게임 오브젝트의 월드 좌표 행렬의 41,42,43값을 가져온다
				for (int i = 0; i < EXPLOSION_DEBRISES; i++)
				{
					m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
					m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;		//시작점으로부터 방향m_pxmf3SphereVectors와 속도 speed*경과시간을 곱한값을 더한다.
					m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
					m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);//방향벡터를 축으로 한 회전행렬과 이동행렬을 곱한다. R*T

				}	//조각개수 만큼의 파편들을 하나하나 위치값을 조정해준다.
			}
			else											//m_fDuration이 m_fElapsedTime보다 커지면
			{
				static unsigned int hitCheckCount = 0;
				m_bBlowingUp = false;						//bool변수 false
				
				m_fElapsedTimes = 0.0f;						//시간값 초기화
				// 이 곳에 박스들이 다시 랜덤한 위치에 생성되게 하면 됨
				SetBoxParalyzed();

				if (this->m_dwColor == RGB(255, 0, 0)) {
					
					
				}

			}
		}
		else
		{
			XMFLOAT3 xmf3TargetPosition = m_pTarget->GetPosition();			//타겟으로 하는 오브젝트의 위치를 xmf3Position에 받아온다.
			XMFLOAT3 xmf3Position = GetPosition();							//오브젝트의 위치를 xmf3Position에 받아온다.	

			XMFLOAT3 xmf3Look = GetLook();
			XMFLOAT3 xmf3ToTarget = Vector3::Subtract(xmf3TargetPosition, xmf3Position, true);		//true이기때문에 뺀벡터를 정규화 한 벡터가 나온다
			float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget);						//aοb 내적연산 => |a|*|b|*cosθ 를 구할수있다. =>
																									//a,b둘다 정규화를 했기 때문에 cosθ값이 fDotProduct로 들어감
			float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);
			XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);							//외적으로 회전축을 구한다.																
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

			if (this->m_pMesh)				//OBB박스 업데이트
			{
				this->m_pMesh->m_xmOOBB.Transform(this->m_xmOOBB, XMLoadFloat4x4(&this->m_xmf4x4World));
				XMStoreFloat4(&this->m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&this->m_xmOOBB.Orientation)));
			}
			//CGameObject::Animate(fElapsedTime);				//그냥 폭발하지 않는 모습 렌더링
		}
	}
	
}


void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (this->m_bActive) {
		if (m_bBlowingUp)					//폭발이 일어나면
		{
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				if (m_pExplosionMesh)		//CMesh로 제어하는 변수
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

		m_fMovingRange += m_fMovingSpeed*fElapsedTime;					//발사해서 거리
		
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
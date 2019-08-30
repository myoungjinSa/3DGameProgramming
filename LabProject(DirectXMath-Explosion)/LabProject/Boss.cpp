#include "stdafx.h"
#include "Boss.h"


CBoss::CBoss(CGameObject *pTarget):CExplosiveObject(pTarget)
{
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	CCubeMesh* pBulletMesh = new CCubeMesh(1.0f, 1.0f, 1.0f);			//x=1.0,y=1.0,z=5.0
	pBulletMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pBulletObjects = new CBulletObject*[CBoss::m_countBullet];

	for (int i = 0; i < CBoss::m_countBullet; i++) {
		m_pBulletObjects[i] = new CBulletObject();
		m_pBulletObjects[i]->SetMesh(pBulletMesh);
		m_pBulletObjects[i]->SetColor(RGB(255, 0, 0));
		//m_pBulletObjects[i]->SetPosition(this->GetPosition());
		m_pBulletObjects[i]->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
		m_pBulletObjects[i]->SetRotationSpeed(5000.0f);
		//m_pBulletObjects[i]->SetMovingDirection(this->GetPosition());
		m_pBulletObjects[i]->SetMovingSpeed(120.0f);

	}
}

CBoss::~CBoss()
{
}

void CBoss::Animate(float fElapsedTime,CPlayer *pPlayer) 
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
			SetMovingDirection(Vector3::Subtract(xmf3TargetPosition, this->GetPosition(), true));
			if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
			if (Vector3::Distance(xmf3TargetPosition, xmf3Position) > DISTANCE_TO_TARGET_OBJECT &&
				Vector3::Distance(xmf3TargetPosition, xmf3Position)<DISTANCE_TO_TARGET_OBJECT + 350)
			{
				//XMFLOAT3 xmf3Look = GetLook();	
				//1�ʸ��� �Ѿ� �߻�
				TakeShot(fElapsedTime);
			}
			
			//cout <<"("<< this->GetPosition().x << "," << this->GetPosition().y << "," << this->GetPosition().z<<")";
			if (this->m_pMesh)				//OBB�ڽ� ������Ʈ
			{
				this->m_pMesh->m_xmOOBB.Transform(this->m_xmOOBB, XMLoadFloat4x4(&this->m_xmf4x4World));
				XMStoreFloat4(&this->m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&this->m_xmOOBB.Orientation)));
			}

		}
	}
}

void CBoss::Render(HDC hDCFrameBuffer,CCamera *pCamera)
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

void CBoss::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	
	CGameObject::SetPosition(x, y, z);
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
			pBulletObject->SetMovingDirection(this->GetLook());					//�ٶ󺸰��ִ� �������� ���� ���
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

//static==============================================
int CBoss::m_countBullet = 50;
int CBoss::m_currentBullet = 0;

void CBoss::LoadBullet() {
	m_currentBullet = 0;
}

//====================================================



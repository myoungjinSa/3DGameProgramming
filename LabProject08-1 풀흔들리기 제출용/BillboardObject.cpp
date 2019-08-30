#include "stdafx.h"
#include "BillboardObject.h"


CBillboardObject::CBillboardObject()
{
}


CBillboardObject::~CBillboardObject()
{
}

void CBillboardObject::Animate(float fTimeElapsed, CCamera* pCamera)
{
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	SetLookAt(xmf3CameraPosition);
}

void CBillboardObject::SetLookAt(XMFLOAT3& xmfTarget)
{
	XMFLOAT3 xmf3Up(0.0f, 1.0f, 0.0f);
	XMFLOAT3 xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMFLOAT3 xmf3Look = Vector3::SubtractNormalize(xmfTarget, xmf3Position, true);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3Look, true);

	m_xmf4x4World._11 = xmf3Right.x;
	m_xmf4x4World._12 = xmf3Right.y;
	m_xmf4x4World._13 = xmf3Right.z;
	m_xmf4x4World._21 = xmf3Up.x;
	m_xmf4x4World._22 = xmf3Up.y;
	m_xmf4x4World._23 = xmf3Up.z;
	m_xmf4x4World._31 = xmf3Look.x;
	m_xmf4x4World._32 = xmf3Look.y;
	m_xmf4x4World._33 = xmf3Look.z;

}

void CBillboardObject::Render(ID3D12GraphicsCommandList *pd3dCommandList,int objNum ,CCamera *pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera,objNum);
}


//////////////////////////////////////////////////////
//CGrassBillboardObject

CGrassBillboardObject::CGrassBillboardObject()
{
	totalTime = 0.0f;
}

CGrassBillboardObject::~CGrassBillboardObject()
{

}

void CGrassBillboardObject::Animate(float fTimeElapsed, CCamera* pCamera,bool isIntersect)
{
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	SetLookAt(xmf3CameraPosition);
	
	totalTime += sinf(fTimeElapsed);
	float fAngle = sinf(0.5f*totalTime);
	//XMFLOAT3 xmf3RotationAxis = XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);

	//XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, rotationSpeed* fTimeElapsed);
	
	XMFLOAT4X4 mtxRotate = Matrix4x4::Identity();
	if (isIntersect) {
		fAngle = 1.5f*cosf(3.0f*totalTime);
		mtxRotate = Matrix4x4::RotationYawPitchRoll(0.0f, 0.0f, fAngle);
	}
	else {
		mtxRotate = Matrix4x4::RotationYawPitchRoll(0.0f, 0.0f, fAngle*60.0f);
	}
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);


}

void CGrassBillboardObject::SetLookAt(XMFLOAT3& xmfTarget)
{
	XMFLOAT3 xmf3Up(0.0f, 1.0f, 0.0f);
	XMFLOAT3 xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMFLOAT3 xmf3Look = Vector3::SubtractNormalize(xmfTarget, xmf3Position, true);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3Look, true);



	m_xmf4x4World._11 = xmf3Right.x;
	m_xmf4x4World._12 = xmf3Right.y;
	m_xmf4x4World._13 = xmf3Right.z;
	m_xmf4x4World._21 = xmf3Up.x;
	m_xmf4x4World._22 = xmf3Up.y;
	m_xmf4x4World._23 = xmf3Up.z;
	m_xmf4x4World._31 = xmf3Look.x;
	m_xmf4x4World._32 = xmf3Look.y;
	m_xmf4x4World._33 = xmf3Look.z;


}

void CGrassBillboardObject::Render(ID3D12GraphicsCommandList *pd3dCommandList,int objNum, CCamera *pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera,objNum);
}


//////////////////////////////////////////////////////////////////////////////////////
//CFlowerBillboardObject
CFlowerBillboardObject::CFlowerBillboardObject()
{

}

CFlowerBillboardObject::~CFlowerBillboardObject()
{

}

void CFlowerBillboardObject::Animate(float fTimeElapsed, CCamera* pCamera)
{
	XMFLOAT3 xmf3CameraPosition = pCamera->GetPosition();
	SetLookAt(xmf3CameraPosition);



}

void CFlowerBillboardObject::SetLookAt(XMFLOAT3& xmfTarget)
{
	XMFLOAT3 xmf3Up(0.0f, 1.0f, 0.0f);
	XMFLOAT3 xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMFLOAT3 xmf3Look = Vector3::SubtractNormalize(xmfTarget, xmf3Position, true);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3Up, xmf3Look, true);
	


	m_xmf4x4World._11 = xmf3Right.x;
	m_xmf4x4World._12 = xmf3Right.y;
	m_xmf4x4World._13 = xmf3Right.z;
	m_xmf4x4World._21 = xmf3Up.x;
	m_xmf4x4World._22 = xmf3Up.y;
	m_xmf4x4World._23 = xmf3Up.z;
	m_xmf4x4World._31 = xmf3Look.x;
	m_xmf4x4World._32 = xmf3Look.y;
	m_xmf4x4World._33 = xmf3Look.z;

}



/////////////////////////////////////////////////////////////////////////////////////////

CTexturedTransformObject::CTexturedTransformObject()
{

}

CTexturedTransformObject::~CTexturedTransformObject()
{

}

void CTexturedTransformObject::Animate(float fTimeElapsed, CCamera *pCamera,bool IsIntersect)
{

	CGrassBillboardObject::Animate(fTimeElapsed,pCamera,IsIntersect);





}
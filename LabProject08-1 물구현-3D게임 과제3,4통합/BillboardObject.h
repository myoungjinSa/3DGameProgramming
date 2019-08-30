#pragma once
#include "Object.h"


class CMaterial;
class CBillboardObject :public CGameObject
{
public:
	CBillboardObject();
	virtual ~CBillboardObject();

public:
	virtual void Animate(float fTimeElapsed, CCamera *pCamera);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,int objNum ,CCamera *pCamera = NULL);

	void SetLookAt(XMFLOAT3& xmfTarget);

};


class CGrassBillboardObject : public CGameObject
{
public:
	CGrassBillboardObject();
	virtual ~CGrassBillboardObject();



public:


	virtual void Animate(float fTimeElapsed, CCamera *pCamera,bool isIntersect);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int objNum,CCamera *pCamera = NULL);

	void SetLookAt(XMFLOAT3& xmfTarget);
private:
	double totalTime;

};

class CFlowerBillboardObject : public CGameObject
{
public:
	CFlowerBillboardObject();
	virtual ~CFlowerBillboardObject();



public:


	virtual void Animate(float fTimeElapsed, CCamera *pCamera);
	//virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int objNum, CCamera *pCamera = NULL);

	void SetLookAt(XMFLOAT3& xmfTarget);


};

////////////////////////////////////////////////////////////////////////////////
class CExplosionBillboardObject : public CBillboardObject
{
public:
	CExplosionBillboardObject();
	virtual ~CExplosionBillboardObject();

	/*virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();*/
	void SetLookAt(XMFLOAT3& xmfTarget);


	virtual void Animate(float fElapsedTime, CCamera* pCamera);


	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int objNum, CCamera *pCamera = NULL);


	UINT GetExplosionTime() { return m_fExplosionDeltaTime; }
	void SetExplode(bool explosion);

private:
	bool	m_bExplode;
	UINT  m_fExplosionDeltaTime;
};



class UIBillboardObject : public CBillboardObject
{
public:
	UIBillboardObject();
	virtual ~UIBillboardObject();

	//void SetLookAt(XMFLOAT3& xmfTarget);

	virtual void Animate(float fElapsedTime, CCamera* pCamera);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,  CCamera *pCamera = NULL);

};


#pragma once
#include "Object.h"


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



class CTexturedTransformObject : public CGrassBillboardObject
{
public:
	CTexturedTransformObject();
	virtual ~CTexturedTransformObject();
public:

	virtual void Animate(float fTimeElapsed, CCamera *pCamera,bool IsIntersect);



};
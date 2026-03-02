#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CImGui : public CBase
{
	DECLARE_SINGLETON(CImGui)
private:
	explicit CImGui();
	virtual ~CImGui();
public:
	void Init(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice);
	_int Update();
	void LateUpdate();
	void Render();
private:
	void Render_MainMenuBar();
	void Render_AssetBrowser();
	void Render_Viewport();
	void Render_Inspector();
	void Render_Hierachy();
private:
	void Save_Level(const char* szPath);
	void Load_Level(const char* szPath);
private:
	int m_iSelectedAsset = -1; //선택된 에셋
	int m_iSelectedObject = -1;
	bool m_bPlacementMode = false;
	struct ObjectData
	{
		string strName;
		int iType;
		float fX, fY, fZ;
		float fRotY;
		float fScaleX, fScaleY, fScaleZ;
	};
	//배치된 오브젝트 데이터
	vector<ObjectData> m_vecObjects;
	bool m_bEditorMode = false;
private:
	virtual void Free();
};
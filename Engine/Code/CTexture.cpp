#include "CTexture.h"

CTexture::CTexture()
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{
	m_vecTexture.reserve(rhs.m_vecTexture.size());

	m_vecTexture = rhs.m_vecTexture;

	for (size_t i = 0; i < rhs.m_vecTexture.size(); ++i)
	{
		m_vecTexture[i]->AddRef();
	}
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9* pTexture = nullptr;

	for (int i = 0; i < iCnt; ++i)
	{
		_tchar szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eID)
		{
		case Engine::TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, 
				(LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case Engine::TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName,
				(LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		default:
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

//매개 변수로 texture의 주소를 받아서 텍스쳐를 로드한다
HRESULT CTexture::Ready_Texture(const _tchar* pPath)
{
	//여기서 이게 진짜 pPath인지 검증 필요 -> wstring으로 교체? 
	//주소를 그냥 pPath로 넣어도 되는 거야?
	//이거 때문에 m_pTexture가 터지는 거 아니야? 그치?

	//텍스쳐 로딩
	if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, pPath, &m_pTexture)))
		return E_FAIL;

	return S_OK;
}

void CTexture::Set_Texture(_uint iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eID, const _tchar* pPath, const _uint& iCnt)
{
	CTexture* pTexture = new CTexture(pGraphicDev);

	if (FAILED(pTexture->Ready_Texture(eID, pPath, iCnt)))
	{
		Safe_Release(pTexture);
		return nullptr;
	}

	return pTexture;
}

//CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
//{
//	CTexture* pTexture = new CTexture(pGraphicDev);
//
//	if (FAILED(pTexture->Ready_Texture(pPath)))
//	{
//		Safe_Release(pTexture);
//		return nullptr;
//	}
//
//	return pTexture;
//}

CComponent* CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), Safe_Release<IDirect3DBaseTexture9*>);
	CComponent::Free();
}

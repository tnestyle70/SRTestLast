#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh : public CVIBuffer
{
private:
	explicit CMesh();
	virtual ~CMesh();
private:
	virtual void Free();
};

END

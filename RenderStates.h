﻿//***************************************************************************************
// RenderStates.h Frank Luna (C) 2011 Wszelkie prawa zastrzeżone.
//   
// Definicja obiektów stanów renderowania.  
//***************************************************************************************

#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include "d3dUtil.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;
	 
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
};

#endif // RENDERSTATES_H
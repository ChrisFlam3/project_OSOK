//***************************************************************************************
// Vertex.h Frank Luna (C) 2011 Wszelkie prawa zastrzeżone.
//
// Definicja struktur wierzchołka i formatów wejścia.
//***************************************************************************************

#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 TangentU;

};

#endif // VERTEX_H

#ifndef __DX_ENGINE_H__
#define __DX_ENGINE_H__

#include "common.h"

#ifdef DXENGINE_EXPORTS
#define DXENGINE_API __declspec(dllexport)
#else
#define DXENGINE_API __declspec(dllimport)
#endif // DXENGINE_EXPORTS

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "GlobalManager.h"
#include "WndBase.h"
#include "Scene.h"

#include "Camera.h"

#include "D3DGui.h"
#include "CallBoard.h"
#include "MeshModel.h"
#include "BoneMesh.h"
#include "SkyBox.h"
#include "SnowParticle.h"

#endif // __DX_ENGINE_H__
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Camera.h"
#include "windows.h"
#include "xnacollision.h"
#include "Vertex.h"
#include "BasicModel.h"




class project:public D3DApp
{
public:

	//Konstruktor i destruktor
	project(HINSTANCE hInstance);
	~project();

	//Funkcje dziedziczne
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	//Funkcje obsługi myszy
	void OnMouseDown(WPARAM btnState,int x, int y);
	void OnMouseUp(WPARAM btnState,int x, int y);
	void OnMouseMove(WPARAM btnState,int x, int y);

	//Funkcje inicjalizacyjne budowy
private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();
	void BuildRenderStates();
	void Pick(int sx, int sy);
	void sky(ID3D11Device* device, const std::wstring& cubemapFilename, float skySphereRadius);
	void skydraw(ID3D11DeviceContext* dc, const Camera& camera);

	//Stałe i zmienne
	private:
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		ID3D11Buffer* mVB;
		ID3D11Buffer* mIB;
		ID3D11Buffer* mVBSky;
		UINT mIndexCountSky;
		ID3D11Buffer* mIBSky;


		ID3DX11Effect* mFX;
		ID3DX11Effect* mFXSky;
		ID3DX11Effect* mFXN;
		ID3DX11EffectTechnique* mTechNormal;
		ID3DX11EffectTechnique* mTechSky;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectTechnique* mTech2;
		ID3DX11EffectTechnique* mTech3;
		ID3DX11EffectTechnique* mTech4;
		ID3DX11EffectTechnique* mTechActive;
		ID3DX11EffectMatrixVariable* mfxWorldViewProj;


	ID3D11ShaderResourceView* mCubeMapSRV;
	ID3D11ShaderResourceView* mDiffuseMapSRV1;
	ID3D11ShaderResourceView* mDiffuseMapSRV2;
	ID3D11ShaderResourceView* mDiffuseMapSRV3;
	ID3D11ShaderResourceView* mDiffuseMapSRV4;
	ID3D11ShaderResourceView* mCrateNormalMap;
	ID3D11ShaderResourceView* mFloorNormalMap;
	XMFLOAT4X4 mTexTransform1;
	XMFLOAT4X4 mTexTransform2;
	XMFLOAT4X4 mTexTransform3;


	PointLight mPointLight;
	PointLight mPointLight1;

	Material mFloorMat;
	Material mBoxMat;
	Material mSphereMat;
	Material mShadowMat;

	ID3DX11EffectMatrixVariable* mfxWorld;
	ID3DX11EffectMatrixVariable* mfxWorldNormal;
	ID3DX11EffectMatrixVariable* mfxWorldViewProjSky;
	ID3DX11EffectMatrixVariable* mfxWorldViewProjNormal;
	ID3DX11EffectMatrixVariable* mfxViewProjNormal;
	ID3DX11EffectMatrixVariable* mfxWorldInvTranspose;
	ID3DX11EffectMatrixVariable* mfxWorldInvTransposeNormal;
	ID3DX11EffectVectorVariable* mfxEyePosW;
	ID3DX11EffectVectorVariable* mfxEyePosWNormal;
	ID3DX11EffectVariable* mfxPointLight;
	ID3DX11EffectVariable* mfxPointLightNormal;
	ID3DX11EffectVariable* mfxMaterial;
	ID3DX11EffectVariable* mfxMaterialNormal;
	ID3DX11EffectVectorVariable* mfxFogColor;
	ID3DX11EffectScalarVariable* mfxFogStart;
	ID3DX11EffectScalarVariable* mfxFogRange;
	ID3DX11EffectScalarVariable* mfxHightScaleNormal;
	ID3DX11EffectScalarVariable* mfxMaxTessDistanceNormal;
	ID3DX11EffectScalarVariable* mfxMinTessDistanceNormal;
	ID3DX11EffectScalarVariable* mfxMinTessFactorNormal;
	ID3DX11EffectScalarVariable* mfxMaxTessFactorNormal;


	ID3DX11EffectShaderResourceVariable* mfxDiffuseMap;
	ID3DX11EffectShaderResourceVariable* mfxDiffuseMapSky;
	ID3DX11EffectShaderResourceVariable* mfxDiffuseMapSkyNormal;
	ID3DX11EffectShaderResourceVariable* mfxDiffuseMapNormal;
	ID3DX11EffectShaderResourceVariable* mfxNormalMap;

	ID3DX11EffectMatrixVariable* mfxTexTransform;
	ID3DX11EffectMatrixVariable* mfxTexTransformNormal;



	ID3D11InputLayout* mInputLayout;
	ID3D11InputLayout* mInputLayoutSky;
	ID3D11InputLayout* mInputLayoutNormal;

	ID3D11BlendState*  TransparentBS ;
	ID3D11DepthStencilState* NoDoubleBlendDSS;
	ID3D11DepthStencilState* LessEqualDSS;
	ID3D11DepthStencilState* EqualsDSS;
	ID3D11RasterizerState* NoCullRS;

	ID3D11RasterizerState* WireframeRS;

	XMFLOAT4X4 mBoxWorld[11];
	XMFLOAT4X4 mFloorWorld;
	XMFLOAT4X4 mSphereWorld;

	int mBoxVertexOffset;
	int mFloorVertexOffset;
	int mSphereVertexOffset;

	UINT mBoxIndexOffset;
	UINT mFloorIndexOffset;
	UINT mSphereIndexOffset;

	UINT mBoxIndexCount;
	UINT mFloorIndexCount;
	UINT mSphereIndexCount;

	UINT mBoxVertexCount;

	Camera mCam;

	XMFLOAT2 mForceTexOffset;

	POINT mLastMousePos;

	XNA::Sphere CameraBound;
	XNA::AxisAlignedBox BoxBound;
	XNA::Sphere ArenaBound;
	XNA::OrientedBox BoxBoundO;

	UINT mPickedTriangle;










	TextureMgr mTexMgr;
	BasicModel* mModel;
	std::vector<BasicModelInstance> mModelInstances;

	
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Sprawdzenie pamięci w trybie debugowania.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	project theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}

project::project(HINSTANCE hInstance):D3DApp(hInstance),mVB(0),mIB(0),mFX(0),mTech(0),mTechSky(0),mfxWorld(0),mfxWorldInvTranspose(0),mfxEyePosW(0),mfxPointLight(0),mfxMaterial(0),mfxWorldViewProj(0),mInputLayout(0),mDiffuseMapSRV1(0),mDiffuseMapSRV2(0),mDiffuseMapSRV3(0),mForceTexOffset(0.0f,0.0f),mPickedTriangle(-1),mModel(0)
{
	mMainWndCaption = L"project OSOK alpha";
	mEnable4xMsaa = true;
	mLastMousePos.x = 960;
	mLastMousePos.y = 540;

	mCam.SetPosition(1.0f, 1.2f, 1.0f);

	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mFloorWorld, I);
	XMStoreFloat4x4(&mTexTransform1,I);
	XMStoreFloat4x4(&mTexTransform2,I);
	XMStoreFloat4x4(&mTexTransform3,I);

	XMMATRIX tex1Scale=XMMatrixScaling(0.05f,0.05f,0.05f);
	XMStoreFloat4x4(&mTexTransform1,tex1Scale);
	
	XMMATRIX tex3Scale=XMMatrixScaling(1.0f,1.0f,1.0f);
	XMStoreFloat4x4(&mTexTransform3,tex3Scale);
	
	
	
	
	
	XMMATRIX boxScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset = XMMatrixTranslation(2.0f, 1.0f, 5.0f);
				
	XMStoreFloat4x4(&mBoxWorld[0], boxScale*boxOffset);

	XMMATRIX boxScale2 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset2 = XMMatrixTranslation(10.0f, 1.0f, 7.0f);
	XMStoreFloat4x4(&mBoxWorld[1], XMMatrixMultiply(boxScale2, boxOffset2));

	XMMATRIX boxScale3 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset3 = XMMatrixTranslation(15.0f, 1.0f, -9.0f);
	XMStoreFloat4x4(&mBoxWorld[2], XMMatrixMultiply(boxScale3, boxOffset3));

	XMMATRIX boxScale4 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset4 = XMMatrixTranslation(-10.0f, 1.0f, 18.0f);
	XMStoreFloat4x4(&mBoxWorld[3], XMMatrixMultiply(boxScale4, boxOffset4));

	XMMATRIX boxScale5 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset5 = XMMatrixTranslation(-5.0f, 1.0f, -19.0f);
	XMStoreFloat4x4(&mBoxWorld[4], XMMatrixMultiply(boxScale5, boxOffset5));

	XMMATRIX boxScale6 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset6 = XMMatrixTranslation(18.0f, 1.0f, 18.0f);
	XMStoreFloat4x4(&mBoxWorld[5], XMMatrixMultiply(boxScale6, boxOffset6));

	XMMATRIX boxScale7 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset7 = XMMatrixTranslation(0.0f, 1.0, -19.0f);
	XMStoreFloat4x4(&mBoxWorld[6], XMMatrixMultiply(boxScale7, boxOffset7));

	XMMATRIX boxScale8 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset8 = XMMatrixTranslation(-19.0f, 1.0f, 6.0f);
	XMStoreFloat4x4(&mBoxWorld[7], XMMatrixMultiply(boxScale8, boxOffset8));

	XMMATRIX boxScale9 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset9 = XMMatrixTranslation(17.0f, 1.0f, 15.0f);
	XMStoreFloat4x4(&mBoxWorld[8], XMMatrixMultiply(boxScale9, boxOffset9));

	XMMATRIX boxScale10 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX boxOffset10 = XMMatrixTranslation(9.0f, 1.0f, 17.0f);
	XMStoreFloat4x4(&mBoxWorld[9], XMMatrixMultiply(boxScale10, boxOffset10));


	XMMATRIX boxScale11 = XMMatrixScaling(0.0f, 0.0f, 0.0f);
	XMMATRIX boxOffset11 = XMMatrixTranslation(9.0f, 1.0f, 17.0f);
	XMStoreFloat4x4(&mBoxWorld[10], XMMatrixMultiply(boxScale11, boxOffset11));

	



	XMMATRIX sphereScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX sphereOffset = XMMatrixTranslation(0.0f, -10.0f, 0.0f);
	XMStoreFloat4x4(&mSphereWorld, XMMatrixMultiply(sphereScale, sphereOffset));

	XMMATRIX floorScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX floorOffset = XMMatrixTranslation(0.0f, -0.1f, 0.0f);
	XMStoreFloat4x4(&mFloorWorld, XMMatrixMultiply(floorScale, floorOffset));

	mPointLight.Ambient  = XMFLOAT4(0.6f, 0.6f, 1.0f, 0.6f);
	mPointLight.Diffuse  = XMFLOAT4(2.0f, 2.0f, 1.2f, 2.0f);
	mPointLight.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mPointLight.Att      = XMFLOAT3(0.0f, 0.07f, 0.0f);
	mPointLight.Range    = 30.0f;

	mPointLight1.Ambient  = XMFLOAT4(0.6f, 0.6f, 1.0f, 0.6f);
	mPointLight1.Diffuse  = XMFLOAT4(2.0f, 2.0f, 1.2f, 2.0f);
	mPointLight1.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mPointLight1.Att      = XMFLOAT3(0.0f, 0.07f, 0.0f);
	mPointLight1.Range    = 150.0f;

	mBoxMat.Ambient  = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mBoxMat.Diffuse  = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 8.0f);
	mBoxMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	mFloorMat.Ambient  =  XMFLOAT4(0.3f, 0.3f, 0.2f, 0.3f);
	mFloorMat.Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.3f, 0.2f);
	mFloorMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 8.0f);
	mFloorMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	mSphereMat.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSphereMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	mSphereMat.Specular = XMFLOAT4(10.0f, 10.0f, 10.0f, 10.0f);

	mShadowMat.Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mShadowMat.Diffuse  = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	mShadowMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);


	mPointLight.Position.x=0.0f;
	mPointLight.Position.z=0.0f;
	mPointLight.Position.y=1.0f;

	mPointLight1.Position.x=60.0f;
	mPointLight1.Position.z=60.0f;
	mPointLight1.Position.y=20.0f;


	

}



project::~project()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(mDiffuseMapSRV1);
	ReleaseCOM(mDiffuseMapSRV2);
	ReleaseCOM(mDiffuseMapSRV3);

}

bool project::Init()
{
	if(!D3DApp::Init())
		return false;

	// Należy najpierw zainicjalizować Effects, ponieważ InputLayouts zależy od sygnatur shadera.
	BuildFX();
	BuildVertexLayout();
	BuildRenderStates();
	mTexMgr.Init(md3dDevice);

	sky(md3dDevice, L"Textures/sky.dds", 5000.0f);

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/floor.jpg", 0, 0, &mDiffuseMapSRV1, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/block.jpg", 0, 0, &mDiffuseMapSRV2, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/bricks.jpg", 0, 0, &mDiffuseMapSRV3, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/bullet.dds", 0, 0, &mDiffuseMapSRV4, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/cratenormal.dds", 0, 0, &mCrateNormalMap, 0 ));

	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/floornormal.dds", 0, 0, &mFloorNormalMap, 0 ));

	BuildGeometryBuffers();

	mModel = new BasicModel(md3dDevice, mTexMgr, "Models\\model.m3d", L"Textures\\");
	BasicModelInstance modelInstance;
	modelInstance.Model = mModel;
	XMMATRIX modelScale = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	XMMATRIX modelRot   = XMMatrixRotationY(0.0f);
	XMMATRIX modelOffset = XMMatrixTranslation(90.0f, 1.0f, 90.0f);
	XMStoreFloat4x4(&modelInstance.World, modelScale*modelRot*modelOffset);
	mModelInstances.push_back(modelInstance);


	ShowCursor(0);
	return true;
}

void project::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void project::UpdateScene(float dt)
{

		XMMATRIX tex2Scale=XMMatrixScaling(20.0f,20.0f,0.0f);
	mForceTexOffset.y-=0.05f*dt;
	mForceTexOffset.x-=0.03f*dt;
	XMMATRIX ForceOffset = XMMatrixTranslation(mForceTexOffset.x, mForceTexOffset.y, 1.0f);
	XMStoreFloat4x4(&mTexTransform2, tex2Scale*ForceOffset);


	
			
			XMVECTOR scale;
			XMVECTOR rotQuat;
			XMVECTOR translation;
			XNA::AxisAlignedBox WBoxBound;
			CameraBound.Radius=1.0f;
			ArenaBound.Center=XMFLOAT3(0.0f,0.0f,0.0f);
			ArenaBound.Radius=26.5f;

	//
	// Kontroluj kamerę.
	//
			
			

			XMFLOAT3 older=mCam.GetPosition();
	if( GetAsyncKeyState('W') & 0x8000 ){
	
		
		int intersects=0;
		XMFLOAT3 older=mCam.GetPosition();
		mCam.Walk(1.0f);
		CameraBound.Center=mCam.GetPosition();
		for(int i=0;i<10;i++){
			XMMATRIX W = XMLoadFloat4x4(&mBoxWorld[i]);
			XMMatrixDecompose(&scale, &rotQuat, &translation, W);
			XNA::TransformAxisAlignedBox(&WBoxBound, &BoxBound, XMVectorGetX(scale), rotQuat, translation);

		if(XNA::IntersectSphereAxisAlignedBox(&CameraBound, &WBoxBound) == 0)
		;
		
		else
			intersects++;

	}
		if(intersects==0){
			mCam.Walk(-1.0f);
			mCam.Walk(10.0f*dt);
		}else{
			int intersects2=0;
			
	

		mCam.Strafe(1.0f);
		CameraBound.Center=mCam.GetPosition();
		for(int i=0;i<10;i++){
			XMMATRIX W = XMLoadFloat4x4(&mBoxWorld[i]);
			XMMatrixDecompose(&scale, &rotQuat, &translation, W);
			XNA::TransformAxisAlignedBox(&WBoxBound, &BoxBound, XMVectorGetX(scale), rotQuat, translation);

		if(XNA::IntersectSphereAxisAlignedBox(&CameraBound, &WBoxBound) == 0)
		;
		
		else
			intersects2++;

	}
		if(intersects2==0){
			mCam.SetPosition(older);
			
			mCam.Strafe(0.1f);
			mCam.Walk(2.0f*dt);
		}else{
			mCam.SetPosition(older);
		
			mCam.Strafe(-0.1f);
			mCam.Walk(2.0f*dt);
		}


		
		
		
		}





		}
	
	
		

		
	if( GetAsyncKeyState('S') & 0x8000 ){
		int intersects=0;
		XMFLOAT3 older=mCam.GetPosition();
		mCam.Walk(-1.0f);
		CameraBound.Center=mCam.GetPosition();
		for(int i=0;i<10;i++){
			XMMATRIX W = XMLoadFloat4x4(&mBoxWorld[i]);
			XMMatrixDecompose(&scale, &rotQuat, &translation, W);
			XNA::TransformAxisAlignedBox(&WBoxBound, &BoxBound, XMVectorGetX(scale), rotQuat, translation);

		if(XNA::IntersectSphereAxisAlignedBox(&CameraBound, &WBoxBound) == 0)
		;
		
		else
			intersects++;

	}
		if(intersects==0){
			mCam.Walk(1.0f);
			mCam.Walk(-10.0f*dt);
		}else
			mCam.SetPosition(older);}


	if( GetAsyncKeyState('A') & 0x8000 ){
				int intersects=0;
		XMFLOAT3 older=mCam.GetPosition();
		mCam.Strafe(-1.0f);
		CameraBound.Center=mCam.GetPosition();
		for(int i=0;i<10;i++){
			XMMATRIX W = XMLoadFloat4x4(&mBoxWorld[i]);
			XMMatrixDecompose(&scale, &rotQuat, &translation, W);
			XNA::TransformAxisAlignedBox(&WBoxBound, &BoxBound, XMVectorGetX(scale), rotQuat, translation);

		if(XNA::IntersectSphereAxisAlignedBox(&CameraBound, &WBoxBound) == 0)
		;
		
		else
			intersects++;

	}
		if(intersects==0){
			mCam.Strafe(1.0f);
			mCam.Strafe(-10.0f*dt);
		}else
			mCam.SetPosition(older);}


	if( GetAsyncKeyState('D') & 0x8000 ){
				int intersects=0;
		XMFLOAT3 older=mCam.GetPosition();
		mCam.Strafe(1.0f);
		CameraBound.Center=mCam.GetPosition();
		for(int i=0;i<10;i++){
			XMMATRIX W = XMLoadFloat4x4(&mBoxWorld[i]);
			XMMatrixDecompose(&scale, &rotQuat, &translation, W);
			XNA::TransformAxisAlignedBox(&WBoxBound, &BoxBound, XMVectorGetX(scale), rotQuat, translation);

		if(XNA::IntersectSphereAxisAlignedBox(&CameraBound, &WBoxBound) == 0)
		;
		
		else
			intersects++;

	}
		if(intersects==0){
			mCam.Strafe(-1.0f);
			mCam.Strafe(10.0f*dt);
		}else
			mCam.SetPosition(older);
	
	}

	

	if(XNA::IntersectSphereSphere(&CameraBound,&ArenaBound)==0)
		mCam.SetPosition(older);
		



	}

void project::BuildRenderStates(){

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &WireframeRS));

D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&noCullDesc, &NoCullRS));

	
	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(md3dDevice->CreateBlendState(&transparentDesc, &TransparentBS));


	
	D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
	noDoubleBlendDesc.DepthEnable      = true;
	noDoubleBlendDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
	noDoubleBlendDesc.DepthFunc        = D3D11_COMPARISON_LESS; 
	noDoubleBlendDesc.StencilEnable    = true;
    noDoubleBlendDesc.StencilReadMask  = 0xff;
    noDoubleBlendDesc.StencilWriteMask = 0xff;

	noDoubleBlendDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.FrontFace.StencilFunc   = D3D11_COMPARISON_EQUAL;


	noDoubleBlendDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.BackFace.StencilFunc   = D3D11_COMPARISON_EQUAL;

	HR(md3dDevice->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDSS));





	D3D11_DEPTH_STENCIL_DESC lessEqualDesc;
	lessEqualDesc.DepthEnable      = true;
	lessEqualDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    lessEqualDesc.DepthFunc        = D3D11_COMPARISON_LESS_EQUAL; 
    lessEqualDesc.StencilEnable    = false;

	HR(md3dDevice->CreateDepthStencilState(&lessEqualDesc, &LessEqualDSS));

		D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

	md3dDevice->CreateDepthStencilState(&equalsDesc, &EqualsDSS);

	
}

void project::BuildFX(){

	std::ifstream fin("fx/Basic.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, md3dDevice, &mFX));




	mTech=mFX->GetTechniqueByName("Light1Tex");
	mTech2=mFX->GetTechniqueByName("Light0Tex");
	mTech3=mFX->GetTechniqueByName("Light1");
	mTech4=mFX->GetTechniqueByName("Light1TexAlphaClipFog");

	mfxWorldViewProj=mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxWorld= mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxWorldInvTranspose= mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mfxTexTransform= mFX->GetVariableByName("gTexTransform")->AsMatrix();
	mfxEyePosW= mFX->GetVariableByName("gEyePosW")->AsVector();
	mfxFogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	mfxFogStart= mFX->GetVariableByName("gFogStart")->AsScalar();
	mfxFogRange= mFX->GetVariableByName("gFogRange")->AsScalar();
	mfxPointLight= mFX->GetVariableByName("gPointLight");
	mfxMaterial= mFX->GetVariableByName("gMaterial");
	mfxDiffuseMap= mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();


		std::ifstream fin2("fx/Sky.fxo", std::ios::binary);

	fin2.seekg(0, std::ios_base::end);
	int size2 = (int)fin2.tellg();
	fin2.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader2(size2);

	fin2.read(&compiledShader2[0], size2);
	fin2.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader2[0], size2, 
		0, md3dDevice, &mFXSky));




	mTechSky=mFXSky->GetTechniqueByName("SkyTech");
	mfxWorldViewProjSky= mFXSky->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxDiffuseMapSky=mFXSky->GetVariableByName("gCubeMap")->AsShaderResource();


	std::ifstream fin3("fx/DisplacementMap.fxo", std::ios::binary);

	fin3.seekg(0, std::ios_base::end);
	int size3 = (int)fin3.tellg();
	fin3.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader3(size3);

	fin3.read(&compiledShader3[0], size3);
	fin3.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader3[0], size3, 
		0, md3dDevice, &mFXN));




	mTechNormal=mFXN->GetTechniqueByName("Light1Tex");
	mfxHightScaleNormal= mFXN->GetVariableByName("gHeightScale")->AsScalar();
	mfxMaxTessDistanceNormal= mFXN->GetVariableByName("gMaxTessDistance")->AsScalar();
	mfxMinTessDistanceNormal= mFXN->GetVariableByName("gMinTessDistance")->AsScalar();
	mfxMinTessFactorNormal= mFXN->GetVariableByName("gMinTessFactor")->AsScalar();
	mfxMaxTessFactorNormal= mFXN->GetVariableByName("gMaxTessFactor")->AsScalar();
	mfxWorldNormal=mFXN->GetVariableByName("gWorld")->AsMatrix();
	mfxWorldInvTransposeNormal= mFXN->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mfxViewProjNormal= mFXN->GetVariableByName("gViewProj")->AsMatrix();
	mfxWorldViewProjNormal= mFXN->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxTexTransformNormal= mFXN->GetVariableByName("gTexTransform")->AsMatrix();
	mfxMaterialNormal= mFXN->GetVariableByName("gMaterial");
	mfxDiffuseMapNormal= mFXN->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxNormalMap= mFXN->GetVariableByName("gNormalMap")->AsShaderResource();
	mfxEyePosWNormal= mFXN->GetVariableByName("gEyePosW")->AsVector();
	mfxPointLightNormal= mFXN->GetVariableByName("gPointLight");
	mfxDiffuseMapSkyNormal=mFXN->GetVariableByName("gCubeMap")->AsShaderResource();


}

void project::DrawScene(){

	

	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	skydraw(md3dImmediateContext, mCam);


	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);	

 
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};

	UINT stride = sizeof(Vertex);
    UINT offset = 0;

	mCam.UpdateViewMatrix();



	XMMATRIX view     = mCam.View();
	XMMATRIX proj     = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	mfxPointLight->SetRawValue(&mPointLight1,0,sizeof(mPointLight1));
	mfxEyePosW->SetRawValue(&mCam.GetPosition(),0,sizeof(XMFLOAT3));
	mfxFogColor->SetFloatVector(reinterpret_cast<const float*>(&Colors::Black));
	mfxFogStart->SetFloat(15000.0f);
	mfxFogRange->SetFloat(1750000.0f);


	mfxPointLightNormal->SetRawValue(&mPointLight,0,sizeof(mPointLight));
	mfxEyePosWNormal->SetRawValue(&mCam.GetPosition(),0,sizeof(XMFLOAT3));
	mfxHightScaleNormal->SetFloat(0.04f);
	mfxMaxTessDistanceNormal->SetFloat(1.0f);
	mfxMinTessDistanceNormal->SetFloat(15.0f);
	mfxMinTessFactorNormal->SetFloat(1.0f);
	mfxMaxTessFactorNormal->SetFloat(1.0f);
	mfxDiffuseMapSkyNormal->SetResource(mCubeMapSRV);

	
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->IASetInputLayout(mInputLayoutNormal);

		if( GetAsyncKeyState('1') & 0x8000 )
md3dImmediateContext->RSSetState(WireframeRS);
	
mTechActive=mTech;
D3DX11_TECHNIQUE_DESC techDesc1;
	mTechActive->GetDesc(&techDesc1);

	for(UINT p = 0; p < techDesc1.Passes; ++p)
    {

			XMMATRIX world = XMLoadFloat4x4(&mModelInstances[0].World);
			XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
			XMMATRIX worldViewProj = world*view*proj;

			mfxWorld->SetMatrix(reinterpret_cast<float*>(&world));
			mfxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
			XMMATRIX transform=XMLoadFloat4x4(&mTexTransform3);
		mfxTexTransform->SetMatrix(reinterpret_cast<float*>(&transform));
		
			
			for(UINT subset = 0; subset < mModelInstances[0].Model->SubsetCount; ++subset)
			{
				mfxMaterial->SetRawValue(&mModelInstances[0].Model->Mat[mModelInstances[0].Model->Subsets[subset].MaterialNum],0,sizeof(Material));
				mfxDiffuseMap->SetResource(mModelInstances[0].Model->DiffuseMapSRV[mModelInstances[0].Model->Subsets[subset].MaterialNum]);
				//Effects::NormalMapFX->SetNormalMap(mModelInstances[modelIndex].Model->NormalMapSRV[mModelInstances[modelIndex].Model->Subsets[subset].MaterialNum]);
				
				mTechActive->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
				mModelInstances[0].Model->ModelMesh.Draw(md3dImmediateContext, subset);

			
		}
    }
	mfxPointLight->SetRawValue(&mPointLight,0,sizeof(mPointLight));
	md3dImmediateContext->IASetVertexBuffers(0,1,&mVB,&stride,&offset);
	md3dImmediateContext->IASetIndexBuffer(mIB,DXGI_FORMAT_R32_UINT,0);
	
	md3dImmediateContext->IASetInputLayout(mInputLayoutNormal);
md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);	

	mTechActive=mTechNormal;
	D3DX11_TECHNIQUE_DESC techDesc;
	mTechActive->GetDesc(&techDesc);



	for(UINT p = 0; p < techDesc.Passes; ++p)
    {

		

		XMMATRIX world=XMLoadFloat4x4(&mFloorWorld);
		XMMATRIX worldInvTranspose =MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj=world*view*proj;

		mfxWorldNormal->SetMatrix(reinterpret_cast<float*>(&world));
		mfxWorldInvTransposeNormal->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		mfxWorldViewProjNormal->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		mfxViewProjNormal->SetMatrix(reinterpret_cast<float*>(&viewProj));
		mfxMaterialNormal->SetRawValue(&mFloorMat,0,sizeof(mFloorMat));
		XMMATRIX transform=XMLoadFloat4x4(&mTexTransform1);
		mfxTexTransformNormal->SetMatrix(reinterpret_cast<float*>(&transform));
		mfxDiffuseMapNormal->SetResource(mDiffuseMapSRV1);
		mfxNormalMap->SetResource(mFloorNormalMap);

		

		mTechActive->GetPassByIndex(p)->Apply(0,md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mFloorIndexCount,mFloorIndexOffset,mFloorVertexOffset);
	}

	md3dImmediateContext->HSSetShader(0, 0, 0);
	md3dImmediateContext->DSSetShader(0, 0, 0);
	
	mfxMinTessFactorNormal->SetFloat(3.0f);
	mfxMaxTessFactorNormal->SetFloat(64.0f);



	
		mTechActive=mTechNormal;

		mTechActive->GetDesc(&techDesc);
		for(UINT p = 0; p < techDesc.Passes; ++p)
    {
	for(int i=0;i<10;i++){
		XMMATRIX world=XMLoadFloat4x4(&mBoxWorld[i]);
		XMMATRIX worldInvTranspose =MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj=world*view*proj;

		mfxWorldNormal->SetMatrix(reinterpret_cast<float*>(&world));
		mfxWorldInvTransposeNormal->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		mfxWorldViewProjNormal->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		mfxViewProjNormal->SetMatrix(reinterpret_cast<float*>(&viewProj));
		mfxMaterialNormal->SetRawValue(&mBoxMat,0,sizeof(mBoxMat));
		XMMATRIX transform=XMLoadFloat4x4(&mTexTransform3);
		mfxTexTransformNormal->SetMatrix(reinterpret_cast<float*>(&transform));
		mfxDiffuseMapNormal->SetResource(mDiffuseMapSRV3);
		mfxNormalMap->SetResource(mCrateNormalMap);
		
	


		mTechActive->GetPassByIndex(p)->Apply(0,md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mBoxIndexCount,mBoxIndexOffset,mBoxVertexOffset);
	
	
	




	}
	
		}
		
		md3dImmediateContext->HSSetShader(0, 0, 0);
	md3dImmediateContext->DSSetShader(0, 0, 0);

			md3dImmediateContext->IASetInputLayout(mInputLayout);
md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	

		mTechActive=mTech4;
		mTechActive->GetDesc(&techDesc);
		for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		XMMATRIX world=XMLoadFloat4x4(&mBoxWorld[10]);
		XMMATRIX worldInvTranspose =MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj=world*view*proj;

		mfxWorld->SetMatrix(reinterpret_cast<float*>(&world));
		mfxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		mfxMaterial->SetRawValue(&mBoxMat,0,sizeof(mBoxMat));
		XMMATRIX transform=XMLoadFloat4x4(&mTexTransform3);
		mfxTexTransform->SetMatrix(reinterpret_cast<float*>(&transform));
		mfxDiffuseMap->SetResource(mDiffuseMapSRV4);
	
	
	
		mTechActive->GetPassByIndex(p)->Apply(0,md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mBoxIndexCount,mBoxIndexOffset,mBoxVertexOffset);

		
	}
				
		
	
	mTechActive=mTech2;
		mTechActive->GetDesc(&techDesc);
			for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		XMMATRIX world=XMLoadFloat4x4(&mSphereWorld);
		XMMATRIX worldInvTranspose =MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj=world*view*proj;

		mfxWorld->SetMatrix(reinterpret_cast<float*>(&world));
		mfxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		mfxMaterial->SetRawValue(&mSphereMat,0,sizeof(mSphereMat));
		XMMATRIX transform=XMLoadFloat4x4(&mTexTransform2);
		mfxTexTransform->SetMatrix(reinterpret_cast<float*>(&transform));
		mfxDiffuseMap->SetResource(mDiffuseMapSRV2);

		md3dImmediateContext->RSSetState(NoCullRS);
	md3dImmediateContext->OMSetBlendState(TransparentBS, blendFactor, 0xffffffff);
	

		mTechActive->GetPassByIndex(p)->Apply(0,md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSphereIndexCount,mSphereIndexOffset,mSphereVertexOffset);
			
		md3dImmediateContext->RSSetState(0);	
		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	
			}


				

	mTechActive=mTech3;
		mTechActive->GetDesc(&techDesc);
		for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		for(int i=0;i<10;i++){
			
		XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
		XMVECTOR toMainLight = XMLoadFloat4(&XMFLOAT4(mPointLight.Position.x,mPointLight.Position.y,mPointLight.Position.z,1.0f));
		XMMATRIX S =  XMMatrixShadow(shadowPlane, toMainLight);
		XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.001f, 0.0f);

		// Ustaw stałe dla obiektu.
		XMMATRIX world = XMLoadFloat4x4(&mBoxWorld[i])*S;
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;
		
		mfxWorld->SetMatrix(reinterpret_cast<const float*>(&world));
		mfxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&worldInvTranspose));
		mfxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&worldViewProj));
		mfxMaterial->SetRawValue(&mShadowMat,0,sizeof(mShadowMat));
	
		


		md3dImmediateContext->OMSetDepthStencilState(NoDoubleBlendDSS, 0);
		md3dImmediateContext->OMSetBlendState(TransparentBS, blendFactor, 0xffffffff);

		mTechActive->GetPassByIndex(p)->Apply(0,md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mBoxIndexCount,mBoxIndexOffset,mBoxVertexOffset);
		
		
		md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
		md3dImmediateContext->OMSetDepthStencilState(0, 0);
		}

			
	}
	
	
		
	
	
	HR(mSwapChain->Present(0, 0));


}

void project::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);

	if( (btnState & MK_RBUTTON) != 0 )
	{
		Pick(960, 540);
	}
}

void project::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void project::OnMouseMove(WPARAM btnState, int x, int y)
{
	
	
		// Każdy piksel powinien odpowiadać jednej czwartej stopnia.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	POINT H;
	GetCursorPos(&H);
	if(H.x<240)
	SetCursorPos(1680,H.y);
	if(H.x>1680)
	SetCursorPos(240,H.y);
	

	mLastMousePos.x = x;
	mLastMousePos.y = y;
	
	
}

void project::BuildGeometryBuffers(){
	GeometryGenerator::MeshData box;
	GeometryGenerator::MeshData cylinder;
	GeometryGenerator::MeshData sphere;


	GeometryGenerator geoGen;
	geoGen.CreateCylinder(30.0f,30.0f,0.1f,20,20,cylinder);
	geoGen.CreateBox(2.0f,2.0f,2.0f,box);
	geoGen.Subdivide(box);
	geoGen.CreateSphere(31.0f,20,20,sphere);

	mFloorVertexOffset=0;
	mBoxVertexOffset=cylinder.Vertices.size();
	mSphereVertexOffset=mBoxVertexOffset+box.Vertices.size();

	mFloorIndexCount=cylinder.Indices.size();
	mBoxIndexCount=box.Indices.size();
	mSphereIndexCount=sphere.Indices.size();

	mFloorIndexOffset=0;
	mBoxIndexOffset=mFloorIndexCount;
	mSphereIndexOffset=mBoxIndexOffset+mBoxIndexCount;

	mBoxVertexCount=box.Vertices.size();

	UINT totalVertexCount=
		cylinder.Vertices.size()+box.Vertices.size()+sphere.Vertices.size();

	UINT totalIndexCount=
		mFloorIndexCount+mBoxIndexCount+mSphereIndexCount;

	
	vertices.resize(totalVertexCount);
	
	UINT k = 0;

	for(size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].Tex    = cylinder.Vertices[i].TexC;
		vertices[k].TangentU    = cylinder.Vertices[i].TangentU;
	}

	for(size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Tex    = box.Vertices[i].TexC;
		vertices[k].TangentU    = box.Vertices[i].TangentU;
	}

	for(size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos    = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].Tex    = sphere.Vertices[i].TexC;
		vertices[k].TangentU   = sphere.Vertices[i].TangentU;
	}

	XNA::ComputeBoundingAxisAlignedBoxFromPoints(&BoxBound,box.Vertices.size(),&box.Vertices[0].Position,sizeof(Vertex));
	XNA::ComputeBoundingOrientedBoxFromPoints(&BoxBoundO,box.Vertices.size(),&box.Vertices[0].Position,sizeof(Vertex));

	 D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * totalVertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	
	
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));

}

void project::BuildVertexLayout(){
		D3D11_INPUT_ELEMENT_DESC Pos[] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}

};

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
	D3D11_INPUT_ELEMENT_DESC PosNormalTexTan[4] = 
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
	D3DX11_PASS_DESC passDesc;

	mTechSky->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(Pos, 1, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayoutSky));

	mTechNormal->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(PosNormalTexTan, 4, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayoutNormal));


	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayout));


}

void project::Pick(int sx, int sy)
{
	int inter=0;
	XMMATRIX P = mCam.Proj();

	// Oblicz promień wskazujący w przestrzeni widoku.
	float vx = (+2.0f*sx/mClientWidth  - 1.0f)/P(0,0);
	float vy = (-2.0f*sy/mClientHeight + 1.0f)/P(1,1);

	// Definicja promienia w przestrzeni widoku.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir    = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Przekształć promień do lokalnej przestrzeni siatki.
	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = XMLoadFloat4x4(&mBoxWorld[0]);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	XMVECTOR rayOrigin2 = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir2    = XMVectorSet(vx, vy, 1.0f, 0.0f);

	rayOrigin2 = XMVector3TransformCoord(rayOrigin2, invView);
	rayDir2 = XMVector3TransformNormal(rayDir2, invView);

	rayDir = XMVector3Normalize(rayDir);

	rayDir2 = XMVector3Normalize(rayDir2);




			XMVECTOR scale;
			XMVECTOR rotQuat;
			XMVECTOR translation;
			XNA::AxisAlignedBox WBoxBound;
			XNA::OrientedBox W10BoxBound;

			XMMATRIX Wi = XMLoadFloat4x4(&mBoxWorld[0]);
			XMMatrixDecompose(&scale, &rotQuat, &translation, Wi);
			XNA::TransformAxisAlignedBox(&WBoxBound, &BoxBound, XMVectorGetX(scale), rotQuat, translation);


	mPickedTriangle = -1;
	float tmin = 0.0f;
	if(XNA::IntersectRayAxisAlignedBox(rayOrigin2, rayDir2, &WBoxBound, &tmin))
	{
		// Znajdź najbliższy trójkąt przecinany przez promień.
			XMFLOAT3 point;
			XMStoreFloat3(&point,(rayOrigin2+rayDir2*tmin));
				XMMATRIX boxScale11 = XMMatrixScaling(0.2f, 0.2f, 0.001f);
					XMMATRIX boxOffset11 = XMMatrixTranslation(point.x,point.y,point.z);
					

	
	XMVECTOR control1;
	XMVECTOR control2;
	XMVECTOR control3;
	XMVECTOR control4;

	control1=XMVectorSet(point.x+0.15f,point.y,point.z,1);
	control2=XMVectorSet(point.x-0.15f,point.y,point.z,1);
	control3=XMVectorSet(point.x,point.y+0.15f,point.z,1);
	control4=XMVectorSet(point.x,point.y-0.15f,point.z,1);


	XMMATRIX trans1=XMMatrixTranslation(-point.x,-point.y,-point.z);
	XMMATRIX trans2=XMMatrixTranslation(point.x,point.y,point.z);
	
	int r=0;
	WBoxBound.Extents=XMFLOAT3(WBoxBound.Extents.x+0.001f,WBoxBound.Extents.y+0.001f,WBoxBound.Extents.z+0.001f);
	for(int i=0;i<100000;i++){
	XMMATRIX rot=XMMatrixRotationY(0.001f);
	r++;
	XMMATRIX all=trans1*rot*trans2;

	control1=XMVector3Transform(control1,all);
	control2=XMVector3Transform(control2,all);

	if(XNA::IntersectPointAxisAlignedBox(control1,&WBoxBound)&&XNA::IntersectPointAxisAlignedBox(control2,&WBoxBound)==1){
		break;
	
		
	}
	}



	XMMATRIX rotacja=XMMatrixRotationY(r*0.001f);

	if(r!=100000)
	XMStoreFloat4x4(&mBoxWorld[10],boxScale11*rotacja*boxOffset11);




	}}


	void project::sky(ID3D11Device* device, const std::wstring& cubemapFilename, float skySphereRadius)
{

	
	HR(D3DX11CreateShaderResourceViewFromFile(device, cubemapFilename.c_str(), 0, 0, &mCubeMapSRV, 0));

	GeometryGenerator::MeshData sphere;
	GeometryGenerator geoGen;
	geoGen.CreateSphere(skySphereRadius, 30, 30, sphere);

	std::vector<XMFLOAT3> vertices(sphere.Vertices.size());

	for(size_t i = 0; i < sphere.Vertices.size(); ++i)
	{
		vertices[i] = sphere.Vertices[i].Position;
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];

	
    HR(device->CreateBuffer(&vbd, &vinitData, &mVBSky));
	
	
	mIndexCountSky = sphere.Indices.size();

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(USHORT) * mIndexCountSky;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
    ibd.MiscFlags = 0;

	std::vector<USHORT> indices16;
	indices16.assign(sphere.Indices.begin(), sphere.Indices.end());

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices16[0];

	
    HR(device->CreateBuffer(&ibd, &iinitData, &mIBSky));
}


	void project::skydraw(ID3D11DeviceContext* dc, const Camera& camera)
{

	// umieść niebo w przestrzeni świata tak, aby w jego centralnym punkcie znajdowało się oko
	XMFLOAT3 eyePos = camera.GetPosition();
	XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);


	XMMATRIX WVP = XMMatrixMultiply(T, camera.ViewProj());

	mfxWorldViewProjSky->SetMatrix(reinterpret_cast<float*>(&WVP));
	mfxDiffuseMapSky->SetResource(mCubeMapSRV);


	UINT stride = sizeof(XMFLOAT3);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, &mVBSky, &stride, &offset);
	dc->IASetIndexBuffer(mIBSky, DXGI_FORMAT_R16_UINT, 0);
	dc->IASetInputLayout(mInputLayoutSky);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	D3DX11_TECHNIQUE_DESC techDesc;
    mTechSky->GetDesc( &techDesc );

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        ID3DX11EffectPass* pass = mTechSky->GetPassByIndex(p);

		pass->Apply(0, dc);
		
		dc->DrawIndexed(mIndexCountSky, 0, 0);
	}
}








































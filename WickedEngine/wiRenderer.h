#pragma once
#include "CommonInclude.h"
#include "wiEnums.h"
#include "wiGraphicsAPI.h"
#include "SamplerMapping.h"
#include "ConstantBufferMapping.h"
#include "ResourceMapping.h"
#include "wiSPTree.h"
#include "wiWindowRegistration.h"

#include <unordered_set>

struct Transform;
struct Vertex;
struct SkinnedVertex;
struct Material;
struct Object;
struct Mesh;
struct Armature;
struct Bone;
struct KeyFrame;
struct SHCAM;
struct Light;
struct Decal;
struct WorldInfo;
struct Wind;
struct Camera;
struct HitSphere;
struct RAY;
struct Camera;
struct Model;
struct Scene;

class  Lines;
class  Cube;
class  wiTranslator;
class  wiParticle;
class  wiEmittedParticle;
class  wiHairParticle;
class  wiSprite;
class  wiSPTree;
class  TaskThread;
struct Cullable;
class  PHYSICS;
class  wiRenderTarget;
class  wiWaterPlane;

typedef std::map<std::string, Mesh*> MeshCollection;
typedef std::map<std::string, Material*> MaterialCollection;


class wiRenderer
{
public:
	static wiGraphicsTypes::GraphicsDevice* graphicsDevice;
	static wiGraphicsTypes::GraphicsDevice* GetDevice() { return graphicsDevice; }


	static void InitDevice(wiWindowRegistration::window_type window, bool fullscreen = false);
	static void Present(std::function<void()> drawToScreen1=nullptr, std::function<void()> drawToScreen2=nullptr, std::function<void()> drawToScreen3=nullptr);

	
	static wiGraphicsTypes::Sampler				*samplers[SSLOT_COUNT];
	static wiGraphicsTypes::VertexShader		*vertexShaders[VSTYPE_LAST];
	static wiGraphicsTypes::PixelShader			*pixelShaders[PSTYPE_LAST];
	static wiGraphicsTypes::GeometryShader		*geometryShaders[GSTYPE_LAST];
	static wiGraphicsTypes::HullShader			*hullShaders[HSTYPE_LAST];
	static wiGraphicsTypes::DomainShader		*domainShaders[DSTYPE_LAST];
	static wiGraphicsTypes::ComputeShader		*computeShaders[CSTYPE_LAST];
	static wiGraphicsTypes::VertexLayout		*vertexLayouts[VLTYPE_LAST];
	static wiGraphicsTypes::RasterizerState		*rasterizers[RSTYPE_LAST];
	static wiGraphicsTypes::DepthStencilState	*depthStencils[DSSTYPE_LAST];
	static wiGraphicsTypes::BlendState			*blendStates[BSTYPE_LAST];
	static wiGraphicsTypes::GPUBuffer			*constantBuffers[CBTYPE_LAST];
	static wiGraphicsTypes::GPUBuffer			*resourceBuffers[RBTYPE_LAST];
	static wiGraphicsTypes::Texture				*textures[TEXTYPE_LAST];
	static wiGraphicsTypes::Sampler				*customsamplers[SSTYPE_LAST];

	
	static float GAMMA;
	static int SHADOWRES_2D, SHADOWRES_CUBE, SHADOWCOUNT_2D, SHADOWCOUNT_CUBE, SOFTSHADOWQUALITY_2D;
	static bool HAIRPARTICLEENABLED, EMITTERSENABLED;
	static float SPECULARAA;
	static float renderTime, renderTime_Prev, deltaTime;
	static XMFLOAT2 temporalAAJitter, temporalAAJitterPrev;
	static float RESOLUTIONSCALE;

	static void SetShadowProps2D(int resolution, int count, int softShadowQuality);
	static void SetShadowPropsCube(int resolution, int count);

	// Constant Buffers:
	// Persistent buffers:
	GFX_STRUCT WorldCB
	{
		XMFLOAT2 mScreenWidthHeight;
		XMFLOAT2 mInternalResolution;
		float	 mGamma;
		XMFLOAT3 mHorizon;
		XMFLOAT3 mZenith;				float pad0;
		XMFLOAT3 mAmbient;				float pad1;
		XMFLOAT3 mFog;
		float mSpecularAA;
		float mVoxelRadianceDataSize;
		UINT mVoxelRadianceDataRes;
		UINT mVoxelRadianceDataConeTracingQuality;
		float mVoxelRadianceDataFalloff;
		XMFLOAT3 mVoxelRadianceDataCenter;
		BOOL mAdvancedRefractions;

		CB_SETBINDSLOT(CBSLOT_RENDERER_WORLD)

		ALIGN_16
	};
	GFX_STRUCT FrameCB
	{
		float mTime;
		float mTimePrev;
		float mDeltaTime;				float pad0;
		XMFLOAT3 mWindDirection;		float pad1;
		float mWindWaveSize;
		float mWindRandomness;
		UINT mFrameCount;
		int mSunLightArrayIndex;
		XMFLOAT2 mTemporalAAJitter;
		XMFLOAT2 mTemporalAAJitterPrev;
		XMMATRIX mVP;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMFLOAT3 mCamPos;
		float mCamDistanceFromOrigin;
		XMMATRIX mPrevV;
		XMMATRIX mPrevP;
		XMMATRIX mPrevVP;
		XMMATRIX mPrevInvVP;
		XMMATRIX mReflVP;
		XMMATRIX mInvV;
		XMMATRIX mInvP;
		XMMATRIX mInvVP;
		XMFLOAT3 mAt;
		float    mZNearP;
		XMFLOAT3 mUp;
		float    mZFarP;
		XMFLOAT4 mFrustumPlanesWS[6];
		
		CB_SETBINDSLOT(CBSLOT_RENDERER_FRAME)

		ALIGN_16
	};
	GFX_STRUCT CameraCB
	{
		XMMATRIX mVP;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMFLOAT3 mCamPos;				float pad0;

		CB_SETBINDSLOT(CBSLOT_RENDERER_CAMERA)

		ALIGN_16
	};
	GFX_STRUCT MiscCB
	{
		XMMATRIX mTransform;
		XMFLOAT4 mColor;

		CB_SETBINDSLOT(CBSLOT_RENDERER_MISC)

		ALIGN_16
	};

	// On demand buffers:
	GFX_STRUCT VolumeLightCB
	{
		XMMATRIX world;
		XMFLOAT4 col;
		XMFLOAT4 enerdis;

		CB_SETBINDSLOT(CBSLOT_RENDERER_VOLUMELIGHT)

		ALIGN_16
	};
	GFX_STRUCT DecalCB
	{
		XMMATRIX mDecalVP;
		int hasTexNor;
		XMFLOAT3 eye;
		float opacity;
		XMFLOAT3 front;

		CB_SETBINDSLOT(CBSLOT_RENDERER_DECAL)

		ALIGN_16
	};
	GFX_STRUCT CubeMapRenderCB
	{
		XMMATRIX mViewProjection[6];

		CB_SETBINDSLOT(CBSLOT_RENDERER_CUBEMAPRENDER)

		ALIGN_16
	};
	GFX_STRUCT APICB
	{
		XMFLOAT4 clipPlane;
		float	alphaRef;
		float	pad[3];

		APICB(const XMFLOAT4& clipPlane = XMFLOAT4(0, 0, 0, 0), float alphaRef = 0.75f) :clipPlane(clipPlane), alphaRef(alphaRef) {}

		CB_SETBINDSLOT(CBSLOT_API)

		ALIGN_16
	};
	GFX_STRUCT TessellationCB
	{
		XMFLOAT4 tessellationFactors;
		
		CB_SETBINDSLOT(CBSLOT_RENDERER_TESSELLATION)

		ALIGN_16
	};
	GFX_STRUCT DispatchParamsCB
	{
		UINT numThreadGroups[3];	
		UINT value0;
		UINT numThreads[3];
		UINT value1;

		CB_SETBINDSLOT(CBSLOT_RENDERER_DISPATCHPARAMS)

		ALIGN_16
	};

	// Resource Buffers:

	GFX_STRUCT LightArrayType
	{
		XMFLOAT3 posVS;
		float distance;
		XMFLOAT4 col;
		XMFLOAT3 posWS;
		float energy;
		XMFLOAT3 directionVS;
		float shadowKernel;
		XMFLOAT3 directionWS;
		UINT type;
		float shadowBias;
		int shadowMap_index;
		float coneAngle;
		float coneAngleCos;
		XMFLOAT4 texMulAdd;
		XMMATRIX shadowMatrix[3];

		STRUCTUREDBUFFER_SETBINDSLOT(SBSLOT_LIGHTARRAY)

		ALIGN_16
	};

protected:

	void UpdateSpheres();
	static void SetUpBoneLines();
	static void UpdateBoneLines();
	static void SetUpCubes();
	static void UpdateCubes();
	


	static bool	wireRender, debugSpheres, debugBoneLines, debugPartitionTree, debugEnvProbes, debugEmitters, gridHelper, voxelHelper, advancedLightCulling, advancedRefractions;
	static bool requestReflectionRendering;


	static wiGraphicsTypes::Texture2D* enviroMap,*colorGrading;
	static void LoadBuffers();
	static void LoadShaders();
	static void SetUpStates();
	static int vertexCount;
	static int visibleCount;


	static void addVertexCount(const int& toadd){vertexCount+=toadd;}


	static float GameSpeed, overrideGameSpeed;

	static Scene* scene;

	static wiWaterPlane waterPlane;

	static bool debugLightCulling;
	static bool occlusionCulling;
	static bool temporalAA, temporalAADEBUG;

	struct VoxelizedSceneData
	{
		bool enabled;
		int res;
		float voxelsize;
		XMFLOAT3 center;
		XMFLOAT3 extents;
		int coneTracingQuality;
		float falloff;
		bool secondaryBounceEnabled;
		bool centerChangedThisFrame;

		VoxelizedSceneData() :enabled(false), res(256), voxelsize(1.0f), center(XMFLOAT3(0, 0, 0)), extents(XMFLOAT3(0, 0, 0)), coneTracingQuality(8),
			falloff(1), secondaryBounceEnabled(true), centerChangedThisFrame(true)
		{}
	} static voxelSceneData;

	static wiGraphicsTypes::GPUQuery occlusionQueries[256];

public:
	static std::string SHADERPATH;

	wiRenderer();
	void CleanUp();
	static void SetUpStaticComponents();
	static void CleanUpStatic();
	
	static void FixedUpdate();
	// Render data that needs to be updated on the main thread!
	static void UpdatePerFrameData(float dt);
	static void UpdateRenderData(GRAPHICSTHREAD threadID);
	static void OcclusionCulling_Render(GRAPHICSTHREAD threadID);
	static void OcclusionCulling_Read();
	static void UpdateImages();
	static void ManageImages();
	static void PutDecal(Decal* decal);
	static void PutWaterRipple(const std::string& image, const XMFLOAT3& pos);
	static void ManageWaterRipples();
	static void DrawWaterRipples(GRAPHICSTHREAD threadID);
	static void SetGameSpeed(float value){GameSpeed=value; if(GameSpeed<0) GameSpeed=0;};
	static float GetGameSpeed();
	static void SetResolutionScale(float value) { RESOLUTIONSCALE = value; }
	static float GetResolutionScale() { return RESOLUTIONSCALE; }
	static XMUINT2 GetInternalResolution() { return XMUINT2((UINT)ceilf(GetDevice()->GetScreenWidth()*GetResolutionScale()), (UINT)ceilf(GetDevice()->GetScreenHeight()*GetResolutionScale())); }
	static bool ResolutionChanged();
	static void SetGamma(float value) { GAMMA = value; }
	static float GetGamma() { return GAMMA; }
	static void SetWireRender(bool value) { wireRender = value; }
	static bool IsWireRender() { return wireRender; }
	static void ToggleDebugSpheres(){debugSpheres=!debugSpheres;}
	static void SetToDrawDebugSpheres(bool param){debugSpheres=param;}
	static bool GetToDrawDebugSpheres() { return debugSpheres; }
	static void SetToDrawDebugBoneLines(bool param) { debugBoneLines = param; }
	static bool GetToDrawDebugBoneLines() { return debugBoneLines; }
	static void SetToDrawDebugPartitionTree(bool param){debugPartitionTree=param;}
	static bool GetToDrawDebugPartitionTree(){return debugPartitionTree;}
	static bool GetToDrawDebugEnvProbes() { return debugEnvProbes; }
	static void SetToDrawDebugEnvProbes(bool value) { debugEnvProbes = value; }
	static void SetToDrawDebugEmitters(bool param) { debugEmitters = param; }
	static bool GetToDrawDebugEmitters() { return debugEmitters; }
	static bool GetToDrawGridHelper() { return gridHelper; }
	static void SetToDrawGridHelper(bool value) { gridHelper = value; }
	static bool GetToDrawVoxelHelper() { return voxelHelper; }
	static void SetToDrawVoxelHelper(bool value) { voxelHelper = value; }
	static void SetDebugLightCulling(bool enabled) { debugLightCulling = enabled; }
	static bool GetDebugLightCulling() { return debugLightCulling; }
	static void SetAdvancedLightCulling(bool enabled) { advancedLightCulling = enabled; }
	static bool GetAdvancedLightCulling() { return advancedLightCulling; }
	static void SetOcclusionCullingEnabled(bool enabled); // also inits query pool!
	static bool GetOcclusionCullingEnabled() { return occlusionCulling; }
	static void SetTemporalAAEnabled(bool enabled) { temporalAA = enabled; }
	static bool GetTemporalAAEnabled() { return temporalAA; }
	static void SetTemporalAADebugEnabled(bool enabled) { temporalAADEBUG = enabled; }
	static bool GetTemporalAADebugEnabled() { return temporalAADEBUG; }
	static void SetVoxelRadianceEnabled(bool enabled) { voxelSceneData.enabled = enabled; }
	static bool GetVoxelRadianceEnabled() { return voxelSceneData.enabled; }
	static void SetVoxelRadianceSecondaryBounceEnabled(bool enabled) { voxelSceneData.secondaryBounceEnabled = enabled; }
	static bool GetVoxelRadianceSecondaryBounceEnabled() { return voxelSceneData.secondaryBounceEnabled; }
	static void SetVoxelRadianceVoxelSize(float value) { voxelSceneData.voxelsize = value; }
	static float GetVoxelRadianceVoxelSize() { return voxelSceneData.voxelsize; }
	//static void SetVoxelRadianceResolution(int value) { voxelSceneData.res = value; }
	static int GetVoxelRadianceResolution() { return voxelSceneData.res; }
	static void SetVoxelRadianceConeTracingQuality(int value) { voxelSceneData.coneTracingQuality = value; }
	static int GetVoxelRadianceConeTracingQuality() { return voxelSceneData.coneTracingQuality; }
	static float GetVoxelRadianceFalloff() { return voxelSceneData.falloff; }
	static void SetVoxelRadianceFalloff(float value) { voxelSceneData.falloff = value; }
	static void SetSpecularAAParam(float value) { SPECULARAA = value; }
	static float GetSpecularAAParam() { return SPECULARAA; }
	static void SetAdvancedRefractionsEnabled(bool value) { advancedRefractions = value; }
	static bool GetAdvancedRefractionsEnabled(); // also needs additional driver support for now...
	static bool IsRequestedReflectionRendering() { return requestReflectionRendering; }
	static wiGraphicsTypes::Texture2D* GetColorGrading(){return colorGrading;};
	static void SetColorGrading(wiGraphicsTypes::Texture2D* tex){colorGrading=tex;};
	static void SetEnviromentMap(wiGraphicsTypes::Texture2D* tex){ enviroMap = tex; }
	static wiGraphicsTypes::Texture2D* GetEnviromentMap(){ return enviroMap; }
	static wiGraphicsTypes::Texture2D* GetLuminance(wiGraphicsTypes::Texture2D* sourceImage, GRAPHICSTHREAD threadID);
	static wiWaterPlane GetWaterPlane();

	static Transform* getTransformByName(const std::string& name);
	static Transform* getTransformByID(unsigned long long id);
	static Armature* getArmatureByName(const std::string& get);
	static int getActionByName(Armature* armature, const std::string& get);
	static int getBoneByName(Armature* armature, const std::string& get);
	static Material* getMaterialByName(const std::string& get);
	HitSphere* getSphereByName(const std::string& get);
	static Object* getObjectByName(const std::string& name);
	static Light* getLightByName(const std::string& name);

	static void ReloadShaders(const std::string& path = "");
	static void BindPersistentState(GRAPHICSTHREAD threadID);
	static void RebindPersistentState(GRAPHICSTHREAD threadID);

	static Vertex TransformVertex(const Mesh* mesh, int vertexI, const XMMATRIX& mat = XMMatrixIdentity());

	struct FrameCulling
	{
		CulledCollection culledRenderer;
		CulledCollection culledRenderer_opaque;
		CulledCollection culledRenderer_transparent;
		std::vector<wiHairParticle*> culledHairParticleSystems;
		CulledList culledLights;
		UINT culledLight_count; // because forward_list doesn't have size()
		std::vector<wiEmittedParticle*> culledEmittedParticleSystems;
		std::list<Decal*> culledDecals;

		void Clear()
		{
			culledRenderer.clear();
			culledRenderer_opaque.clear();
			culledRenderer_transparent.clear();
			culledHairParticleSystems.clear();
			culledLights.clear();
			culledLight_count = 0;
			culledEmittedParticleSystems.clear();
			culledDecals.clear();
		}
	};
	static std::unordered_map<Camera*, FrameCulling> frameCullings;
	
public:
	

	static void UpdateWorldCB(GRAPHICSTHREAD threadID);
	static void UpdateFrameCB(GRAPHICSTHREAD threadID);
	static void UpdateCameraCB(Camera* camera, GRAPHICSTHREAD threadID);
	static void SetClipPlane(const XMFLOAT4& clipPlane, GRAPHICSTHREAD threadID);
	static void SetAlphaRef(float alphaRef, GRAPHICSTHREAD threadID);
	static void ResetAlphaRef(GRAPHICSTHREAD threadID) { SetAlphaRef(0.75f, threadID); }
	static void UpdateGBuffer(wiGraphicsTypes::Texture2D* slot0, wiGraphicsTypes::Texture2D* slot1, wiGraphicsTypes::Texture2D* slot2, wiGraphicsTypes::Texture2D* slot3, wiGraphicsTypes::Texture2D* slot4, GRAPHICSTHREAD threadID);
	static void UpdateDepthBuffer(wiGraphicsTypes::Texture2D* depth, wiGraphicsTypes::Texture2D* linearDepth, GRAPHICSTHREAD threadID);
	
	static void RenderMeshes(const XMFLOAT3& eye, const CulledCollection& culledRenderer, SHADERTYPE shaderType, UINT renderTypeFlags, GRAPHICSTHREAD threadID, bool tessellation = false, bool occlusionCulling = false);
	static void DrawSky(GRAPHICSTHREAD threadID);
	static void DrawSun(GRAPHICSTHREAD threadID);
	static void DrawWorld(Camera* camera, bool tessellation, GRAPHICSTHREAD threadID, SHADERTYPE shaderType, wiGraphicsTypes::Texture2D* refRes, bool grass, bool occlusionCulling);
	static void DrawForShadowMap(GRAPHICSTHREAD threadID);
	static void DrawWorldTransparent(Camera* camera, SHADERTYPE shaderType, wiGraphicsTypes::Texture2D* refracRes, wiGraphicsTypes::Texture2D* refRes
		, wiGraphicsTypes::Texture2D* waterRippleNormals, GRAPHICSTHREAD threadID, bool grass, bool occlusionCulling);
	void DrawDebugSpheres(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugBoneLines(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugLines(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugBoxes(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawTranslators(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugEnvProbes(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugGridHelper(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugVoxels(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawDebugEmitters(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawSoftParticles(Camera* camera, GRAPHICSTHREAD threadID, bool dark = false);
	static void DrawSoftPremulParticles(Camera* camera, GRAPHICSTHREAD threadID, bool dark = false);
	static void DrawTrails(GRAPHICSTHREAD threadID, wiGraphicsTypes::Texture2D* refracRes);
	static void DrawImagesAdd(GRAPHICSTHREAD threadID, wiGraphicsTypes::Texture2D* refracRes);
	//alpha-opaque
	static void DrawImages(GRAPHICSTHREAD threadID, wiGraphicsTypes::Texture2D* refracRes);
	static void DrawImagesNormals(GRAPHICSTHREAD threadID, wiGraphicsTypes::Texture2D* refracRes);
	static void DrawLights(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawVolumeLights(Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawLensFlares(GRAPHICSTHREAD threadID);
	static void DrawDecals(Camera* camera, GRAPHICSTHREAD threadID);
	static void RefreshEnvProbes(GRAPHICSTHREAD threadID);
	static void VoxelRadiance(GRAPHICSTHREAD threadID);

	static void ComputeTiledLightCulling(bool deferred, GRAPHICSTHREAD threadID);
	static void ResolveMSAADepthBuffer(wiGraphicsTypes::Texture2D* dst, wiGraphicsTypes::Texture2D* src, GRAPHICSTHREAD threadID);

	enum MIPGENFILTER
	{
		MIPGENFILTER_POINT,
		MIPGENFILTER_LINEAR,
		MIPGENFILTER_LINEAR_MAXIMUM,
		MIPGENFILTER_GAUSSIAN,
	};
	static void GenerateMipChain(wiGraphicsTypes::Texture1D* texture, MIPGENFILTER filter, GRAPHICSTHREAD threadID);
	static void GenerateMipChain(wiGraphicsTypes::Texture2D* texture, MIPGENFILTER filter, GRAPHICSTHREAD threadID);
	static void GenerateMipChain(wiGraphicsTypes::Texture3D* texture, MIPGENFILTER filter, GRAPHICSTHREAD threadID);

	static void ManageDecalAtlas(GRAPHICSTHREAD threadID);
	
	static XMVECTOR GetSunPosition();
	static XMFLOAT4 GetSunColor();
	static int GetSunArrayIndex();
	static int getVertexCount(){return vertexCount;}
	static void resetVertexCount(){vertexCount=0;}
	static int getVisibleObjectCount(){return visibleCount;}
	static void resetVisibleObjectCount(){visibleCount=0;}

	static void FinishLoading();
	static wiSPTree* spTree;
	static wiSPTree* spTree_lights;

	// The scene holds all models, world information and wind information
	static Scene& GetScene();

	std::vector<Camera> cameras;
	std::vector<HitSphere*> spheres;
	static std::vector<Lines*> boneLines;
	static std::vector<Lines*> linesTemp;
	static std::vector<Cube> cubes;

	static std::unordered_set<Object*> objectsWithTrails;
	static std::unordered_set<wiEmittedParticle*> emitterSystems;
	
	static std::deque<wiSprite*> images;
	static std::deque<wiSprite*> waterRipples;
	static void CleanUpStaticTemp();


	static wiRenderTarget normalMapRT, imagesRT, imagesRTAdd;
	
	static Camera* cam, *refCam, *prevFrameCam;
	static Camera* getCamera(){ return cam; }
	static Camera* getRefCamera(){ return refCam; }

	float getSphereRadius(const int& index);

	std::string DIRECTORY;

	struct Picked
	{
		Transform* transform;
		Object* object;
		Light* light;
		Decal* decal;
		EnvironmentProbe* envProbe;
		XMFLOAT3 position,normal;
		float distance;
		int subsetIndex;

		Picked()
		{
			Clear();
		}

		// Subset index, position, normal, distance don't distinguish between pickeds! 
		bool operator==(const Picked& other)
		{
			return
				transform == other.transform &&
				object == other.object &&
				light == other.light &&
				decal == other.decal &&
				envProbe == other.envProbe
				;
		}
		void Clear()
		{
			distance = 0;
			subsetIndex = -1;
			SAFE_INIT(transform);
			SAFE_INIT(object);
			SAFE_INIT(light);
			SAFE_INIT(decal);
			SAFE_INIT(envProbe);
		}
	};

	// Pick closest object in the world
	// pickType: PICKTYPE enum values ocncatenated with | operator
	// layer : concatenated string of layers to check against, empty string : all layers will be checked
	// layerDisable : concatenated string of layers to NOT check against
	static Picked Pick(long cursorX, long cursorY, int pickType = PICK_OPAQUE, const std::string& layer = "", const std::string& layerDisable = "");
	static Picked Pick(RAY& ray, int pickType = PICK_OPAQUE, const std::string& layer = "", const std::string& layerDisable = "");
	static RAY getPickRay(long cursorX, long cursorY);
	static void RayIntersectMeshes(const RAY& ray, const CulledList& culledObjects, std::vector<Picked>& points,
		int pickType = PICK_OPAQUE, bool dynamicObjects = true, const std::string& layer = "", const std::string& layerDisable = "", bool onlyVisible = false);
	static void CalculateVertexAO(Object* object);

	static PHYSICS* physicsEngine;
	static void SynchronizeWithPhysicsEngine(float dt = 1.0f / 60.0f);

	static Model* LoadModel(const std::string& dir, const std::string& name, const XMMATRIX& transform = XMMatrixIdentity(), const std::string& ident = "common");
	static void LoadWorldInfo(const std::string& dir, const std::string& name);
	static void LoadDefaultLighting();

	static void PutEnvProbe(const XMFLOAT3& position, int resolution = 256);

	static void CreateImpostor(Mesh* mesh);

	static std::vector<wiTranslator*> renderableTranslators;
	// Add translator to render in next frame
	static void AddRenderableTranslator(wiTranslator* translator);

	static std::vector<std::pair<XMFLOAT4X4,XMFLOAT4>> renderableBoxes;
	// Add box to render in next frame
	static void AddRenderableBox(const XMFLOAT4X4& boxMatrix, const XMFLOAT4& color = XMFLOAT4(1,1,1,1));

	// Add model to the scene
	static void AddModel(Model* value);

	// Add Object Instance
	static void Add(Object* value);
	static void Add(const std::list<Object*>& objects);
	// Add Light Instance
	static void Add(Light* value);
	static void Add(const std::list<Light*>& lights);

	// Remove from the scene
	static void Remove(Object* value);
	static void Remove(Light* value);
	static void Remove(Decal* value);
	static void Remove(EnvironmentProbe* value);
};


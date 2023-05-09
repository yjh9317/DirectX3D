#pragma once

#include <Physx/include/PxPhysics.h>
#include <Physx/include/PxPhysicsAPI.h>


using namespace physx;

class CPhysx
	: public CSingleton<CPhysx>
{
	SINGLE(CPhysx);
private:
	physx::PxDefaultErrorCallback	m_DefaultErrorCallback;
	physx::PxDefaultAllocator		m_DefaultAllocatorCallback;

	// CPU 리소스를 효율적으로 공유할 수 있도록 하기 위해 구현하는 것을 추천
	physx::PxDefaultCpuDispatcher*	m_pDispatcher = NULL;
	
	physx::PxTolerancesScale		m_ToleranceScale;
	physx::PxFoundation*			m_pFoundation = NULL;
	// PxFoundation이 있어야 gPhysics를 생성할 수 있다.

	physx::PxPhysics*				m_pPhysics = NULL;
	// Scene
	physx::PxScene*					m_pScene = NULL;
	physx::PxCooking*				m_pCooking =NULL;
	// m_pPhysics를 사용해 createMaterial해서 사용
	// 충돌체 마찰력, Dynamic 마찰력, 탄성력을 지정하여 사용
	physx::PxMaterial*				m_pMaterial = NULL;
	
	physx::PxPvd*					m_Pvd = NULL;

public:
	void Init();
	void CreateSimulation();
	void RunSimulation();
	void Release();

	PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);
	void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);

	// Returns a SphereGeometry object
	physx::PxSphereGeometry createSphereGeometry(physx::PxReal radius);

	// Returns a CapsuleGeometry object
	physx::PxCapsuleGeometry createCapsuleGeometry(physx::PxReal radius, physx::PxReal halfHeight);

	// Returns a Cooked ConvexMesh Object
	physx::PxConvexMesh* createConvexMesh(physx::PxVec3* pointCloud, physx::PxU32 numVertices);

	// Returns a ConvexMeshGeometry object (calls createConvexMesh)
	physx::PxConvexMeshGeometry createConvexMeshGeometry(physx::PxVec3* pointCloud, physx::PxU32 numVertices);

	// Returns a ConvexMeshGeometry object (uses the ConvexMesh passed)
	physx::PxConvexMeshGeometry createConvexMeshGeometry(physx::PxConvexMesh& mesh);

	// Returns a height field
	physx::PxHeightField* createHeightField(physx::PxHeightFieldSample* field, uint32_t nbRows, uint32_t nbCols, physx::PxReal convexEdgeThreshold = 0.0f, physx::PxReal thickness = -1.0f);

	// Returns a heightfield geometry
	physx::PxHeightFieldGeometry createHeightFieldGeometry(physx::PxHeightField* heightField);

	// Returns a triangle mesh
	physx::PxTriangleMesh* createTriangleMesh(physx::PxVec3* vertices, physx::PxU32 numVertices, physx::PxU32* indices, physx::PxU32 numIndices);

	// Returns a triangle mesh geometry (good for use as level geometry)
	physx::PxTriangleMeshGeometry createTriangleMeshGeometry(physx::PxTriangleMesh* mesh);

	// Adds a rigid dynamic actor to the scene, and returns a pointer reference to it
	//physx::PxRigidDynamic* addRigidDynamic(physx::PxVec3 position, physx::PxQuat orientation, physx::PxGeometry** components, physx::PxVec3* componentLinearOffsets, physx::PxQuat* componentAngularOffsets, physx::PxU32 numComponents, physx::PxReal Mass, physx::PxVec3 MomentOfInertia, physx::PxVec3 initialLinearVelocity, physx::PxVec3 initialAngularVelocity, Material mat, physx::PxReal linearDamping = 0.0f, physx::PxReal angularDamping = 0.0f);

	// Adds a rigid dynamic actor to the scene and applies aerodynamics to it at each update
	//physx::PxRigidDynamic* addRigidAerodynamic(physx::PxVec3 position, physx::PxQuat orientation, physx::PxGeometry** components, physx::PxVec3* componentLinearOffsets, physx::PxQuat* componentAngularOffsets, physx::PxU32 numComponents, physx::PxReal Mass, physx::PxVec3 MomentOfInertia, physx::PxVec3 initialLinearVelocity, physx::PxVec3 initialAngularVelocity, Material mat, physx::PxReal linearDamping = 0.0f, physx::PxReal angularDamping = 0.0f, physx::PxReal lift = 0.0f, physx::PxReal drag = 0.0f, physx::PxReal planformArea = PI);

	// Adds a rigid static actor to the scene, and returns a pointer reference to it
	//physx::PxRigidStatic* addRigidStatic(physx::PxVec3 position, physx::PxQuat orientation, physx::PxGeometry** components, physx::PxVec3* componentLinearOffsets, physx::PxQuat* componentAngularOffsets, physx::PxU32 numComponents, Material mat);

	// Sets the array of rigid actors to contain all of the actors in the scene
	void getActors(std::vector<physx::PxRigidActor*>& actors);

	// Sets the gravitational force in the scene
	void setGravity(Vec3 gravity);

	// Sets the frequency of the engine (in Hz)
	void setFrequency(uint32_t frequency);

	// Returns the inertia tensor of an axis-aligned cube centered on the origin
	static Vec3 InertiaTensorSolidCube(physx::PxReal width, physx::PxReal mass);

	// Returns the inertia tensor of a solid sphere centered about the origin
	static Vec3 InertiaTensorSolidSphere(physx::PxReal radius, physx::PxReal mass);

	// Returns the inertia tensor of a hollow sphere centered about the origin
	static Vec3 InertiaTensorHollowSphere(physx::PxReal radius, physx::PxReal mass);

	// Reurns the inertia tensor of a capsule centered about the origin and aligned with the x axis
	static Vec3 InertiaTensorSolidCapsule(physx::PxReal radius, physx::PxReal halfHeight, physx::PxReal mass);

	// returns the value of physics (do not use unless absolutely necessary)
	physx::PxPhysics* getPhysics();

	// returns the values of cooking (do not use unless absolutely necessary)
	physx::PxCooking* getCooking();

	friend class CSceneMgr;
};


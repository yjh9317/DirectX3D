#include "pch.h"
#include "CPhysx.h"

PxRigidDynamic* pDynamic;

CPhysx::CPhysx()
{
};

CPhysx::~CPhysx()
{
	Release();
}

void CPhysx::Init()
{
	// Foundation을 만들어 줍니다.
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);

	m_Pvd = physx::PxCreatePvd(*m_pFoundation);
	PxPvdTransport*  transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// PhysX를 만들어 줍니다.
	m_ToleranceScale.length = 100;
	m_ToleranceScale.speed = 981;

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_Pvd);

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(m_ToleranceScale));

	// Scene을 Set 해줍니다.
	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	// Dispatcher를 Set 만들어 줍니다.
	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	// 위에서 설정한대로 Scene을 만들어 줍니다.
	m_pScene = m_pPhysics->createScene(sceneDesc);


	physx::PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *m_pMaterial);
	m_pScene->addActor(*groundPlane);

	Vec4 WorldPos = { groundPlane->getGlobalPose().p.x
									  , groundPlane->getGlobalPose().p.y
									  , groundPlane->getGlobalPose().p.z , 0};

	PxReal stackZ = 10.0f;

	for (PxU32 i = 0; i < 5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
	
	pDynamic = createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, 0, 0));	

	// PxActor 요소
	PxMaterial;
	PxShape;
	PxRigidStatic;
	PxRigidDynamic;
}

PxRigidDynamic* CPhysx::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0,0,0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_pPhysics, t, geometry, *m_pMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_pScene->addActor(*dynamic);
	return dynamic;
}

void CPhysx::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = m_pPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_pMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = m_pPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			m_pScene->addActor(*body);
		}
	}
	shape->release();
}

void CPhysx::CreateSimulation()
{

}

void CPhysx::RunSimulation()
{
	m_pScene->simulate(1.0f / 60.0f);
	m_pScene->fetchResults(true);
}

void CPhysx::Release()
{
	if (m_pCooking)
	{
		m_pCooking->release();
		m_pCooking = nullptr;
	}

	if (m_pScene)
	{
		m_pScene->release();
		m_pScene = nullptr;
	}

	if (m_pDispatcher)
	{
		m_pDispatcher->release();
		m_pDispatcher = nullptr;
	}

	if (m_pPhysics)
	{
		m_pPhysics->release();
		m_pPhysics = nullptr;
	}

	if (m_Pvd)
	{
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = nullptr;

		if (transport)
		{
			transport->release();
			transport = nullptr;
		}
	}

	if (m_pFoundation)
	{
		m_pFoundation->release();
		m_pFoundation = nullptr;
	}

}

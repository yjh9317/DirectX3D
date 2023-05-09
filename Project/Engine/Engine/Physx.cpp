#include "pch.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

using namespace physx;

physx::PxDefaultAllocator       mDefaultAllocatorCallback;
physx::PxDefaultErrorCallback   mDefaultErrorCallback;
physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
physx::PxTolerancesScale        mToleranceScale;

physx::PxFoundation* mFoundation = NULL;
physx::PxPhysics* mPhysics = NULL;

physx::PxScene* mScene = NULL;
physx::PxMaterial* mMaterial = NULL;

physx::PxPvd* mPvd = NULL;

//init physx


void init()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);

	if (!mFoundation)
		throw("PxCreateFoundtaion Failed");

	mPvd = PxCreatePvd(*mFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	mToleranceScale.length = 100;	// typical length of an object
	mToleranceScale.speed = 981;	// typcial speed of an object, gravity*1s is a reasonable choice
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);

	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	mScene = mPhysics->createScene(sceneDesc);

	return;
}
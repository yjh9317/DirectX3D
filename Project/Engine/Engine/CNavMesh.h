#pragma once

#include <Recast/recast/Recast/Recast.h>
#include <Recast/recast/Recast/RecastAlloc.h>
#include <Recast/recast/Recast/RecastAssert.h>

#include <Recast/recast/Detour/DetourNavMesh.h>
#include <Recast/recast/Detour/DetourNavMeshBuilder.h>
#include <Recast/recast/Detour/DetourCommon.h>


class CNavMesh
	: public CSingleton<CNavMesh>
{
	SINGLE(CNavMesh)

private:

	bool m_keepInterResults;
	float m_totalBuildTimeMs;

	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcConfig m_cfg;
	rcPolyMeshDetail* m_dmesh;
	dtNavMesh* m_navMesh;
	

private:
	// Explicitly disabled copy constructor and copy assignment operator.
	CNavMesh(const CNavMesh&);
	CNavMesh& operator=(const CNavMesh&);

	void CleanUp();
public:


	

};


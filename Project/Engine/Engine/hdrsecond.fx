#ifndef _HDRSECOND
#define _HDRSECOND

#include "value.fx"

StructuredBuffer<float> AverageValues1D : register(t0);
RWStructuredBuffer<float> AverageLum : register(u0);

#define Domain      g_int_0
#define GroupSize   g_int_1
#define Res         g_vec2_0

#define MAX_GROUPS 64

// Group shared memory to store the intermidiate results
groupshared float SharedAvgFinal[MAX_GROUPS];

[numthreads(64, 1, 1)]
void DownScaleSecondPass(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID,
    uint3 dispatchThreadId : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex)
{
	// Fill the shared memory with the 1D values
    float avgLum = 0.0;
    if (dispatchThreadId.x < GroupSize)
    {
        avgLum = AverageValues1D[dispatchThreadId.x];
    }
    SharedAvgFinal[dispatchThreadId.x] = avgLum;

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 64 to 16
    if (dispatchThreadId.x % 4 == 0)
    {
		// Calculate the luminance sum for this step
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadId.x + 1 < GroupSize ? (int) SharedAvgFinal[dispatchThreadId.x + 1] : (int) avgLum;
        stepAvgLum += dispatchThreadId.x + 2 < GroupSize ? (int)SharedAvgFinal[dispatchThreadId.x + 2] : (int)avgLum;
        stepAvgLum += dispatchThreadId.x + 3 < GroupSize ? (int)SharedAvgFinal[dispatchThreadId.x + 3] : (int) avgLum;
		
		// Store the results
        avgLum = stepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 16 to 4
    if (dispatchThreadId.x % 16 == 0)
    {
		// Calculate the luminance sum for this step
        float stepAvgLum = avgLum;
        stepAvgLum += dispatchThreadId.x + 4 < GroupSize ? (int) SharedAvgFinal[dispatchThreadId.x + 4] : (int) avgLum;
        stepAvgLum += dispatchThreadId.x + 8 < GroupSize ? (int) SharedAvgFinal[dispatchThreadId.x + 8] : (int)avgLum;
        stepAvgLum += dispatchThreadId.x + 12 < GroupSize? (int) SharedAvgFinal[dispatchThreadId.x + 12] : (int) avgLum;

		// Store the results
        avgLum = stepAvgLum;
        SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
    }

    GroupMemoryBarrierWithGroupSync(); // Sync before next step

	// Downscale from 4 to 1
    if (dispatchThreadId.x == 0)
    {
		// Calculate the average luminace
        float fFinalLumValue = avgLum;
        fFinalLumValue += dispatchThreadId.x + 16 < GroupSize ? SharedAvgFinal[dispatchThreadId.x + 16] : (int)avgLum;
        fFinalLumValue += dispatchThreadId.x + 32 < GroupSize ? SharedAvgFinal[dispatchThreadId.x + 32] : (int)avgLum;
        fFinalLumValue += dispatchThreadId.x + 48 < GroupSize ? SharedAvgFinal[dispatchThreadId.x + 48] : (int)avgLum;
        fFinalLumValue /= 64.0;

		// Store the final value
        AverageLum[0] = max(fFinalLumValue, 0.0001);

    }
}
#endif
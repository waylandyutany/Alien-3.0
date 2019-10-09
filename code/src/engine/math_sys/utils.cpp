#include "StdAfx.h"

#include "utils.h"

namespace ae { namespace math {

	// *****************************************************************************************************************
	// *****************************************************************************************************************

	float CalcBoundingSphere(const ae::math::float3* pts, ae::u32 numPts, ae::math::float3& center)
	{
		center = ae::math::float3(0,0,0);
		float	maxDist(0);
		ae::u32		i;

		for (i = 0; i < numPts; i++)
			center += pts[i];
		center *= 1.0f / numPts;

		for (i = 0; i < numPts; i++)
		{
			ae::math::float3 diff = pts[i] - center;
			float currDist = diff.sizeSquared();
			if (currDist > maxDist)
				maxDist = currDist;
		}
		return (float)sqrtf(maxDist);
	}

	// *****************************************************************************************************************
	// *****************************************************************************************************************

	float CalcMinBoundingSphere(const ae::math::float3* pts, ae::u32 numPts, ae::math::float3& center)
	{
		double	dx, dy, dz, cen[3], rad;
		double	rad_sq, xspan, yspan, zspan, maxspan;
		double	oldToP, oldToPSq, oldToNew;
		double	xmin[3] = {0,0,0}, xmax[3] = {0,0,0}, ymin[3] = {0,0,0}, ymax[3] = {0,0,0}, zmin[3] = {0,0,0}, zmax[3] = {0,0,0}, dia1[3], dia2[3];
		ae::u32	i;

		xmin[0] = ymin[1] = zmin[2] = FLT_MAX;
		xmax[0] = ymax[1] = zmax[2] = -FLT_MAX;

		for (i = 0; i < numPts; i++)
		{
			if (pts[i][0] < xmin[0]) 
			{
				xmin[0] = pts[i][0];
				xmin[1] = pts[i][1];
				xmin[2] = pts[i][2];
			}

			if (pts[i][0] > xmax[0]) 
			{
				xmax[0] = pts[i][0];
				xmax[1] = pts[i][1];
				xmax[2] = pts[i][2];
			}

			if (pts[i][1] < ymin[1]) 
			{
				ymin[0] = pts[i][0];
				ymin[1] = pts[i][1];
				ymin[2] = pts[i][2];
			}

			if (pts[i][1] > ymax[1]) 
			{
				ymax[0] = pts[i][0];
				ymax[1] = pts[i][1];
				ymax[2] = pts[i][2];
			}

			if (pts[i][2] < zmin[2]) 
			{
				zmin[0] = pts[i][0];
				zmin[1] = pts[i][1];
				zmin[2] = pts[i][2];
			}

			if (pts[i][2] > zmax[2]) 
			{
				zmax[0] = pts[i][0];
				zmax[1] = pts[i][1];
				zmax[2] = pts[i][2];
			}
		}

		dx = xmax[0] - xmin[0];
		dy = xmax[1] - xmin[1];
		dz = xmax[2] - xmin[2];
		xspan = dx * dx + dy * dy + dz * dz;

		dx = ymax[0] - ymin[0];
		dy = ymax[1] - ymin[1];
		dz = ymax[2] - ymin[2];
		yspan = dx * dx + dy * dy + dz * dz;

		dx = zmax[0] - zmin[0];
		dy = zmax[1] - zmin[1];
		dz = zmax[2] - zmin[2];
		zspan = dx * dx + dy * dy + dz * dz;

		dia1[0] = xmin[0];
		dia1[1] = xmin[1];
		dia1[2] = xmin[2];

		dia2[0] = xmax[0];
		dia2[1] = xmax[1];
		dia2[2] = xmax[2];

		maxspan = xspan;

		if (yspan > maxspan)
		{
			maxspan = yspan;

			dia1[0] = ymin[0];
			dia1[1] = ymin[1];
			dia1[2] = ymin[2];

			dia2[0] = ymax[0];
			dia2[1] = ymax[1];
			dia2[2] = ymax[2];
		}

		if (zspan > maxspan)
		{
			dia1[0] = zmin[0];
			dia1[1] = zmin[1];
			dia1[2] = zmin[2];

			dia2[0] = zmax[0];
			dia2[1] = zmax[1];
			dia2[2] = zmax[2];
		}

		cen[0] = (dia1[0] + dia2[0]) * 0.5;
		cen[1] = (dia1[1] + dia2[1]) * 0.5;
		cen[2] = (dia1[2] + dia2[2]) * 0.5;

		dx = dia2[0] - cen[0];
		dy = dia2[1] - cen[1];
		dz = dia2[2] - cen[2];

		rad_sq	= dx * dx + dy * dy + dz * dz;
		rad		= sqrt(rad_sq);

		for (i = 0; i < numPts; i++)
		{
			dx = pts[i][0] - cen[0];
			dy = pts[i][1] - cen[1];
			dz = pts[i][2] - cen[2];

			oldToPSq = dx * dx + dy * dy + dz * dz;

			if (oldToPSq > rad_sq)
			{
				oldToP		= sqrt(oldToPSq);
				rad			= (rad + oldToP) * 0.5;
				rad_sq		= rad * rad;
				oldToNew		= oldToP - rad;

				cen[0] = (rad * cen[0] + oldToNew * pts[i][0]) / oldToP;
				cen[1] = (rad * cen[1] + oldToNew * pts[i][1]) / oldToP;
				cen[2] = (rad * cen[2] + oldToNew * pts[i][2]) / oldToP;
			}	
		}

		center.x = (float)cen[0];
		center.y = (float)cen[1];
		center.z = (float)cen[2];
		return (float)rad;
	}

	// *****************************************************************************************************************
	// *****************************************************************************************************************
	void CalcOrthonormalBasis(const ae::math::float3 &dir, ae::math::float3 &right, ae::math::float3 &up)
	{
		if (fabs(dir.x) > fabs(dir.y))
		{
			// |d.e1| > |d.e2|   

			// R = e2 cross d   
			const float	s(1.0f / sqrtf(dir.z * dir.z + dir.x * dir.x));

			right.x = dir.z * s;
			right.y = 0;
			right.z = -dir.x * s;

			// U = d cross R   
			up.x =  dir.y * right.z;
			up.y =  dir.z * right.x - dir.x * right.z;
			up.z = -dir.y * right.x;
		}
		else
		{
			// |d.e1| <= |d.e2| R = e1 cross d   
			const float	s(1.0f / sqrtf(dir.z * dir.z + dir.y * dir.y));

			right.x =  0;
			right.y = -dir.z * s;
			right.z =  dir.y * s;

			// U = d cross R   
			up.x =  dir.y * right.z - dir.z * right.y;
			up.y = -dir.x * right.z;
			up.z =  dir.x * right.y;
		}
	}

	// *****************************************************************************************************************
	// *****************************************************************************************************************

	void CalcOrthoProjection(ae::math::float4x4& m, float l,float r,float t,float b,float zn,float zf)
	{
		m.m[0][0] = 2.0f / (r - l);		m.m[1][0] = 0;					m.m[2][0] = 0;					m.m[3][0] = (l + r) / (l - r);
		m.m[0][1] = 0;					m.m[1][1] = 2.0f / (t - b);		m.m[2][1] = 0;					m.m[3][1] = (t + b) / (b - t);
		m.m[0][2] = 0;					m.m[1][2] = 0;					m.m[2][2] = 1.0f / (zf - zn);	m.m[3][2] = zn / (zn - zf);
		m.m[0][3] = 0;					m.m[1][3] = 0;					m.m[2][3] = 0;					m.m[3][3] = 1;
	}

	// *****************************************************************************************************************
	// *****************************************************************************************************************

} } // namespace ae { namespace math {

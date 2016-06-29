#pragma once

#include "../preHeader.h"

class Geo2D
{
public:
	/* general */
	static void			InterpLinear(glm::vec2 & v, double t, const glm::vec2 & v0, const glm::vec2 & v1);

	/* reflection over line */
	static void			PointReflectionOverLine(glm::vec2 & v, const glm::vec2 & p, const glm::vec2 & v1, const glm::vec2 & v2);
	//static SVGPattern * PatternReflectionOverLine(SVGPattern * pattern, const glm::vec2 & v1, const glm::vec2 & v2);

	/* line */
	static void			LineParams(double & A, double & B, double & C, const glm::vec2 & v1, const glm::vec2 & v2);
	static void			PointOnLine(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1);

	/* point and line		*/
	static bool			IsPointOnLineseg(const glm::vec2 & v, const glm::vec2 & v1, const glm::vec2 & v2, double precision = 0.0000001);
	static double		DistanceBetweenPointLineseg(glm::vec2 & v, const glm::vec2 & p, const glm::vec2 & v1, const glm::vec2 & v2);
	static double		tOfPointOnLine(const glm::vec2 & v, const glm::vec2 & v0, const glm::vec2 & v1);

	/* bezier */
	static void			BezierParams(glm::vec4 & coex, glm::vec4 & coey, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static double		DistanceBetweenPointBezier(glm::vec2 & v, const glm::vec2 & p, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			SplitBezier(std::vector<glm::vec2> & b1, std::vector<glm::vec2> & b2, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			SplitBezier(std::vector<std::vector<glm::vec2>> & beziers, const std::vector<glm::vec2> & pts, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static double		tOfPointOnBezier(const glm::vec2 & v, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			CubicRoots(std::vector<double> & roots, const glm::vec4 & bezier_coeff);
	static void			PointOnBezier(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			PointOnBezier(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2);
	static void			PointOnCubicBezier_deCasteljau(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			PointOnQuadratic_deCasteljau(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2);
	static void			GetPointProjection(double & t, const glm::vec2 & p, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static double		RefineProjection_(double t, double dist, double precision, const glm::vec2 & p, const  glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);

	/* tangents and normals of Bezier */
	static void			DerivativeOnCubicBezier(glm::vec2 & dt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			SecondDerivativeOnCubicBezier(glm::vec2 & dt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			TangentOnCubicBeizer(glm::vec2 & ta, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static void			NormalOnCubicBezier(glm::vec2 & no, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);

	/* the art length of bezier curve */
	/* refer to http://pomax.github.io/bezierinfo/#arclength */
	static double		GetCubicBeizerArcLength(double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
	static double		CubicBeizerArcLengthTo_t(double l, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3);
};
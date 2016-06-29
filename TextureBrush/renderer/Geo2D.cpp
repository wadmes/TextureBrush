#include "Geo2D.h"

void Geo2D::BezierParams(glm::vec4 & coex, glm::vec4 & coey, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	double x0,y0,x1,y1,x2,y2,x3,y3;
	
	x0 = p0[0];
	y0 = p0[1];

	x1 = p1[0];
	y1 = p1[1];

	x2 = p2[0];
	y2 = p2[1];

	x3 = p3[0];
	y3 = p3[1];

	double ax,bx,cx;
	double ay,by,cy;

	cx = 3 * (x1 - x0);
	bx = 3 * (x2 - x1) - cx;
	ax = x3 - x0 - cx - bx;

	cy = 3 * (y1 - y0);
	by = 3 * (y2 - y1) - cy;
	ay = y3 - y0 - cy - by;

	coex = glm::vec4(ax, bx, cx, x0);
	coey = glm::vec4(ay, by, cy, y0);
}

void Geo2D::InterpLinear(glm::vec2 & v, double t, const glm::vec2 & v0, const glm::vec2 & v1)
{
	v[0] = v0[0] + t * (v1[0] - v0[0]);
	v[1] = v0[1] + t * (v1[1] - v0[1]);
}

void Geo2D::PointOnLine(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1)
{
	Geo2D::InterpLinear(pt,t,p0,p1);
}

void Geo2D::PointOnBezier(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2)
{
	Geo2D::PointOnQuadratic_deCasteljau(pt,t,p0,p1,p2);
}

void Geo2D::PointOnBezier(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	/*v4d coex, coey;
	Geo2D::BezierParams(coex,coey,p0,p1,p2,p3);

	pt[0] = coex[0] * t * t * t + coex[1] * t * t + coex[2] * t + coex[3];
	pt[1] = coey[0] * t * t * t + coey[1] * t * t + coey[2] * t + coey[3];*/
	Geo2D::PointOnCubicBezier_deCasteljau(pt,t,p0,p1,p2,p3);
}

void Geo2D::PointOnQuadratic_deCasteljau(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2)
{
	glm::vec2 p10, p11;
	Geo2D::InterpLinear(p10,t,p0,p1);
	Geo2D::InterpLinear(p11,t,p1,p2);
	Geo2D::InterpLinear(pt,t,p10,p11);
}

void Geo2D::PointOnCubicBezier_deCasteljau(glm::vec2 & pt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	glm::vec2 p10, p11, p12, p20, p21;
	Geo2D::InterpLinear(p10,t,p0,p1);
	Geo2D::InterpLinear(p11,t,p1,p2);
	Geo2D::InterpLinear(p12,t,p2,p3);
	Geo2D::InterpLinear(p20,t,p10,p11);
	Geo2D::InterpLinear(p21,t,p11,p12);
	Geo2D::InterpLinear(pt,t,p20,p21);	
}

void Geo2D::DerivativeOnCubicBezier(glm::vec2 & dt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	glm::vec2 A, B, C;
	A = (p1 - p0) * 3.0f;
	B = (p2 - p1) * 3.0f;
	C = (p3 - p2) * 3.0f;
	
	Geo2D::PointOnBezier(dt,t,A,B,C);
}

void Geo2D::SecondDerivativeOnCubicBezier(glm::vec2 & dt, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	glm::vec2 A, B, C;

	A = (p1 - p0) * 3.0f;
	B = (p2 - p1) * 3.0f;
	C = (p3 - p2) * 3.0f;

	glm::vec2 AA, BB;

	AA = (B - A) * 2.0f;
	BB = (C - B) * 2.0f;

	Geo2D::InterpLinear(dt,t,AA,BB);
}

void Geo2D::TangentOnCubicBeizer(glm::vec2 & ta, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	glm::vec2 dt;
	Geo2D::DerivativeOnCubicBezier(dt,t,p0,p1,p2,p3);
	double l = sqrt(dt[0] * dt[0] + dt[1] * dt[1]);
	ta = glm::vec2(dt[0] / l, dt[1] / l);
}

void Geo2D::NormalOnCubicBezier(glm::vec2 & no, double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	glm::vec2 dt;
	Geo2D::TangentOnCubicBeizer(dt,t,p0,p1,p2,p3);
	no = glm::vec2(-dt[1], dt[0]);
}

double Geo2D::GetCubicBeizerArcLength(double t, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	// Legendre-Gauss abscissae (xi values, defined at i=n as the roots of the nth order Legendre polynomial Pn(x))
	double Tvalues[] = {-0.0765265211334973337546404093988382110047,0.0765265211334973337546404093988382110047,-0.2277858511416450780804961953685746247430,0.2277858511416450780804961953685746247430,-0.3737060887154195606725481770249272373957,0.3737060887154195606725481770249272373957,-0.5108670019508270980043640509552509984254,0.5108670019508270980043640509552509984254,-0.6360536807265150254528366962262859367433,0.6360536807265150254528366962262859367433,-0.7463319064601507926143050703556415903107,0.7463319064601507926143050703556415903107,-0.8391169718222188233945290617015206853296,0.8391169718222188233945290617015206853296,-0.9122344282513259058677524412032981130491,0.9122344282513259058677524412032981130491,-0.9639719272779137912676661311972772219120,0.9639719272779137912676661311972772219120,-0.9931285991850949247861223884713202782226,0.9931285991850949247861223884713202782226};
	// Legendre-Gauss weights (wi values, defined by a function linked to in the Bezier primer article)
	double Cvalues[] = {0.1527533871307258506980843319550975934919,0.1527533871307258506980843319550975934919,0.1491729864726037467878287370019694366926,0.1491729864726037467878287370019694366926,0.1420961093183820513292983250671649330345,0.1420961093183820513292983250671649330345,0.1316886384491766268984944997481631349161,0.1316886384491766268984944997481631349161,0.1181945319615184173123773777113822870050,0.1181945319615184173123773777113822870050,0.1019301198172404350367501354803498761666,0.1019301198172404350367501354803498761666,0.0832767415767047487247581432220462061001,0.0832767415767047487247581432220462061001,0.0626720483341090635695065351870416063516,0.0626720483341090635695065351870416063516,0.0406014298003869413310399522749321098790,0.0406014298003869413310399522749321098790,0.0176140071391521183118619623518528163621,0.0176140071391521183118619623518528163621};
	
	double z   = t / 2;
	double sum = 0;

	for(int i = 0;i<20;i++)
	{
		double corrected_z  = z * Tvalues[i] + z;

		glm::vec2 dt;
		Geo2D::DerivativeOnCubicBezier(dt,corrected_z,p0,p1,p2,p3);

		double B			= sqrt(dt[0] * dt[0] + dt[1] * dt[1]);
		sum				   += Cvalues[i] * B; 
	}
	return z * sum;
}

double Geo2D::CubicBeizerArcLengthTo_t(double l, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	std::vector<double> ll; 
	for(int i = 0;i<1000;i++)
	{
		double t = i / 1000.0;
		ll.push_back(Geo2D::GetCubicBeizerArcLength(t,p0,p1,p2,p3));
	}
	
	double ans = 0.5;

	for(int i = 0;i<1000;i++)
	{
		double a = ll[i];
		double b = ll[i+1];
		if(l >= a && l < b)
		{
			ans = i / 1000.0;
			break;
		}
	}	

	return ans;	
}

void Geo2D::GetPointProjection(double & t, const glm::vec2 & p, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	double mindist = 1e20;

	for(int i = 0;i<200;i++)
	{
		double ii = (double)i / 199.0;
		glm::vec2 tp;
		Geo2D::PointOnCubicBezier_deCasteljau(tp,ii,p0,p1,p2,p3);
		
		double xx = p[0] - tp[0];
		double yy = p[1] - tp[1];
		
		double dist = (xx * xx + yy * yy);
		if(dist < mindist)
		{
			mindist = dist;
			t = ii;
		}
	}

	t = Geo2D::RefineProjection_(t,mindist,1.0/(1.01*200),p,p0,p1,p2,p3);
}

double Geo2D::RefineProjection_(double t, double dist, double precision, const glm::vec2 & p, const  glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	if(precision < 0.00001) return t;

	double prev_t = t - precision,
		   next_t = t + precision;
	
	glm::vec2 prev_pt, next_pt;
	
	Geo2D::PointOnCubicBezier_deCasteljau(prev_pt,prev_t,p0,p1,p2,p3);
	Geo2D::PointOnCubicBezier_deCasteljau(next_pt,next_t,p0,p1,p2,p3);

	double prev_x = prev_pt[0] - p[0],
		   prev_y = prev_pt[1] - p[1],
		   next_x = next_pt[0] - p[0],
		   next_y = next_pt[1] - p[1];
	
	double prev_dist = (prev_x * prev_x + prev_y * prev_y),
		   next_dist = (next_x * next_x + next_y * next_y);
	
	
	if(prev_t >=0 && prev_dist < dist) { return Geo2D::RefineProjection_(prev_t,prev_dist,precision,p,p0,p1,p2,p3); }
	if(next_t <=1 && next_dist < dist) { return Geo2D::RefineProjection_(next_t,next_dist,precision,p,p0,p1,p2,p3); }

	return Geo2D::RefineProjection_(t,dist,precision/2,p,p0,p1,p2,p3);
}
#pragma once

namespace TextureSynthesis
{

const float EPSILON = 0.000001f;

const float MY_PI = 3.14159265359f;
const float MY_2_PI = 2.0f * MY_PI;
const float MY_HALF_PI = 0.5f * MY_PI;


#define SAFE_DELETE(p)  { if(p) delete (p); (p) = NULL; }

#define SAFE_DELETE_ARRAY(p)  { if(p) delete [] (p); (p) = NULL; }

} // end namespace
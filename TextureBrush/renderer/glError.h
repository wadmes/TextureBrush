#pragma once

void _check_gl_error(const char *file, int line);

#define checkGLError() _check_gl_error(__FILE__,__LINE__)
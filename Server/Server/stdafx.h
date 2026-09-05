#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <tchar.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#if __has_include(<gl/glew.h>)
#include <gl/glew.h>
#elif __has_include(<GL/glew.h>)
#include <GL/glew.h>
#elif __has_include(<gl/GL.h>)
#include <gl/GL.h>
#elif __has_include(<GL/gl.h>)
#include <GL/gl.h>
#endif

#if __has_include(<glm/glm.hpp>)
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#elif __has_include(<glm.hpp>)
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#endif

#pragma comment(lib, "ws2_32.lib")

#pragma once
// 미리 컴파일된 헤더 -> 모든 cpp파일에 있어야 함

#include <Windows.h>
#include <stdio.h>

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
using std::wstring;
#include <map>
using std::map;
using std::make_pair;
#include <set>
using std::set;
#include <list>
using std::list;
#include <random>
#include <functional>
using std::function;

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#include <math.h>
#include <assert.h> // 오류 검출
#pragma comment(lib, "Msimg32.lib") // TransparentBlt 라이브러리 참조

#include "define.h"
#include "struct.h"
#include "func.h"
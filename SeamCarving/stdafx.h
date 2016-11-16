// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "iostream"
#include "limits"

#include <stdio.h>
#include <tchar.h>

#include "opencv2/core/core.hpp"

#include "ImageReader.h"
#include "ImageDisplay.h"
#include "SeamDetector.h"

const int MAX_INT = std::numeric_limits<int>::max();
const int MIN_INT = std::numeric_limits<int>::min();
const char MAX_CHAR = std::numeric_limits<char>::max();
const char MIN_CHAR = std::numeric_limits<char>::min();


// TODO: reference additional headers your program requires here

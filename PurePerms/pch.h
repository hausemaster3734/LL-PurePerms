#ifndef PCH_H
#define PCH_H

#include <LLAPI.h> // Не добавляйте #include <Windows.h>, будет конфликт с импортом из LLAPI.h (#define NOMINMAX затрётся — https://github.com/LiteLDev/LiteLoaderBDS/issues/587).

#endif //PCH_H
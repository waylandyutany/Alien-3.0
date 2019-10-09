#pragma once

#ifndef AE_EDITOR_PCH_H
#define AE_EDITOR_PCH_H

#include "lib_editor.h"

#include "../engine/pch.h"

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "windows_sys/base/win32/pch_win32.h"
#endif // #if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#endif // AE_EDITOR_PCH_H
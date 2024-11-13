// Copyright (c) 2024 Clarisma / GeoDesk contributors
// SPDX-License-Identifier: LGPL-3.0-only


#if defined(_WIN32) 
#include "MappedFile_windows.cxx"
#elif defined(__linux__) || defined(__APPLE__) 
#include "MappedFile_linux.cxx"
#else
#error "Platform not supported"
#endif

namespace clarisma {

// Place platform-independent functions here

} // namespace clarisma

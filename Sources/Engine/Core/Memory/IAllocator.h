//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

struct IAllocator
{
	virtual void* Malloc(unsigned long Count, const char* Tag) = 0;
	virtual void* Realloc(void* Original, unsigned long Count, const char* Tag) = 0;
	virtual void Free(void* Original) = 0;
	virtual void DumpAllocs() = 0;
	virtual void HeapCheck() = 0;
};

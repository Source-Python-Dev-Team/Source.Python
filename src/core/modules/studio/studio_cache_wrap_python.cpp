/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "studio.h"
#include "datacache/imdlcache.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IMDLCache *modelcache;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_model_cache_data_type(scope);
void export_model_cache_interface(scope);


//-----------------------------------------------------------------------------
// Declare the _studio._cache module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_studio, _cache)
{
	export_model_cache_interface(_cache);
	export_model_cache_data_type(_cache);
}


//-----------------------------------------------------------------------------
// Exports IMDLCache.
//-----------------------------------------------------------------------------
void export_model_cache_interface(scope _cache)
{
	class_<IMDLCache, IMDLCache *, boost::noncopyable> ModelCache("ModelCache", no_init);

	// Methods...
	ModelCache.def("find_model", &IMDLCache::FindMDL);
	ModelCache.def("add_reference", &IMDLCache::AddRef);
	ModelCache.def("release", &IMDLCache::Release);
	ModelCache.def("get_reference_count", &IMDLCache::GetRef);
	ModelCache.def("get_model_name", &IMDLCache::GetModelName);
	ModelCache.def("get_model_header", &IMDLCache::GetStudioHdr, reference_existing_object_policy());

	// Add memory tools...
	ModelCache ADD_MEM_TOOLS(IMDLCache);

	// Singleton...
	_cache.attr("model_cache") = object(ptr(modelcache));
}


//-----------------------------------------------------------------------------
// Exports MDLCacheDataType_t.
//-----------------------------------------------------------------------------
void export_model_cache_data_type(scope _cache)
{
	enum_<MDLCacheDataType_t> ModelCacheDataType("ModelCacheDataType");

	// Values...
	ModelCacheDataType.value("STUDIOHDR", MDLCACHE_STUDIOHDR);
	ModelCacheDataType.value("STUDIOHWDATA", MDLCACHE_STUDIOHWDATA);
	ModelCacheDataType.value("VCOLLIDE", MDLCACHE_VCOLLIDE);
	ModelCacheDataType.value("ANIMBLOCK", MDLCACHE_ANIMBLOCK);
	ModelCacheDataType.value("VIRTUALMODEL", MDLCACHE_VIRTUALMODEL);
	ModelCacheDataType.value("VERTEXES", MDLCACHE_VERTEXES);
	ModelCacheDataType.value("DECODEDANIMBLOCK",MDLCACHE_DECODEDANIMBLOCK);
}

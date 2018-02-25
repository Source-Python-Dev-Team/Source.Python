/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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
#include "model_types.h"
#include "studio.h"

//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_model_type(scope);
void export_studio_flags(scope);


//-----------------------------------------------------------------------------
// Declare the _studio._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_studio, _constants)
{
	export_model_type(_constants);
	export_studio_flags(_constants);

	_constants.attr("INVALID_ATTACHMENT_INDEX") = INVALID_ATTACHMENT_INDEX;
	_constants.attr("INVALID_BONE_INDEX") = INVALID_BONE_INDEX;
}


//-----------------------------------------------------------------------------
// Exports modtype_t.
//-----------------------------------------------------------------------------
void export_model_type(scope _constants)
{
	enum_<modtype_t> ModelType("ModelType");

	// Values...
	ModelType.value("BAD", mod_bad);
	ModelType.value("BRUSH", mod_brush);
	ModelType.value("SPRITE", mod_sprite);
	ModelType.value("STUDIO", mod_studio);
}


//-----------------------------------------------------------------------------
// Exports StudioFlags.
//-----------------------------------------------------------------------------
void export_studio_flags(scope _constants)
{
	_constants.attr("STUDIO_NONE") = STUDIO_NONE;
	_constants.attr("STUDIO_RENDER") = STUDIO_RENDER;
	_constants.attr("STUDIO_VIEWXFORMATTACHMENTS") = STUDIO_VIEWXFORMATTACHMENTS;
	_constants.attr("STUDIO_DRAWTRANSLUCENTSUBMODELS") = STUDIO_DRAWTRANSLUCENTSUBMODELS;
	_constants.attr("STUDIO_TWOPASS") = STUDIO_TWOPASS;
	_constants.attr("STUDIO_STATIC_LIGHTING") = STUDIO_STATIC_LIGHTING;
	_constants.attr("STUDIO_WIREFRAME") = STUDIO_WIREFRAME;
	_constants.attr("STUDIO_ITEM_BLINK") = STUDIO_ITEM_BLINK;
	_constants.attr("STUDIO_NOSHADOWS") = STUDIO_NOSHADOWS;
	_constants.attr("STUDIO_WIREFRAME_VCOLLIDE") = STUDIO_WIREFRAME_VCOLLIDE;
	_constants.attr("STUDIO_TRANSPARENCY") = STUDIO_TRANSPARENCY;
	_constants.attr("STUDIO_SHADOWDEPTHTEXTURE") = STUDIO_SHADOWDEPTHTEXTURE;
}

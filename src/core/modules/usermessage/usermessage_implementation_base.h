/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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

#ifndef _USERMESSAGE_IMPLEMENTATION_BASE_H_
#define _USERMESSAGE_IMPLEMENTATION_BASE_H_

#include "irecipientfilter.h"
#include "eiface.h"
#include "modules/recipientfilter/mrecipientfilter.h"

extern IVEngineServer* engine;

//---------------------------------------------------------------------------------
// Purpose: Abstract interface for all usermessage implementations
//---------------------------------------------------------------------------------

abstract_class IUsermessageImplementationBase
{
public:
	IUsermessageImplementationBase(const MRecipientFilter &recipient_filter, const char *message_name);
	// Inline virtual destructor -- ensures all usermessages calls the right destructor
	virtual ~IUsermessageImplementationBase() {}

	// Pure-virtual methods which must be inherited and overwritten in the inherited
	// classes
	virtual void set_char(const char *field_name, char field_value, int index=-1) = 0;
	virtual void set_byte(const char *field_name, unsigned char field_value, int index=-1) = 0;
	virtual void set_short(const char *field_name, signed short field_value, int index=-1) = 0;
	virtual void set_long(const char *field_name, signed long field_value, int index=-1) = 0;
	virtual void set_float(const char *field_name, float field_value, int index=-1) = 0;
	virtual void set_bool(const char *field_name, bool field_value, int index=-1) = 0;

	// Unknown sized buffers
	virtual void set_buffer(const char *field_name, void *buffer, unsigned int num_bytes, int index=-1) = 0;
	virtual void set_string(const char *field_name, const char *field_value, int index=-1) = 0;

protected:
	virtual void set_message_index() = 0;
	virtual void send_message_internal() = 0;

protected:
	const char *m_message_name;
	const MRecipientFilter &m_recipient_filter;
	int m_message_index;
};

#endif // _USERMESSAGE_IMPLEMENTATION_BASE_H_
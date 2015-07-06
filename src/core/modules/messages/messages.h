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

#ifndef _USERMESSAGE_H_
#define _USERMESSAGE_H_

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "irecipientfilter.h"
#include "utilities/wrap_macros.h"
#include ENGINE_INCLUDE_PATH(messages_implementation.h)
#include "public/engine/iserverplugin.h"

//-----------------------------------------------------------------------------
// CUserMessage.
//-----------------------------------------------------------------------------
class CUserMessage : public CUserMessageImplementation
{
public:
	CUserMessage(const MRecipientFilter &recipient_filter, const char *message_name);
	~CUserMessage();

	virtual void send_message();

	const char *get_message_name() const;
	const int get_message_index() const;
	const MRecipientFilter &get_recipient_filter() const;
	bool has_been_sent() const;

	// Virtual function call override (calls to base class)
	// Pure-virtual methods which must be inherited and overwritten in the inherited
	// classes
	virtual void set_char(const char *field_name, char field_value, int index=-1);
	virtual void set_byte(const char *field_name, unsigned char field_value, int index=-1);
	virtual void set_short(const char *field_name, signed short field_value, int index=-1);
	virtual void set_long(const char *field_name, signed long field_value, int index=-1);
	virtual void set_float(const char *field_name, float field_value, int index=-1);
	virtual void set_bool(const char *field_name, bool field_value, int index=-1);

	// Unknown sized buffers
	virtual void set_buffer(const char *field_name, void *buffer, unsigned int num_bytes, int index=-1);
	virtual void set_string(const char *field_name, const char *field_value, int index=-1);

private:
	bool m_sent;
};

//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data );

#endif // _USERMESSAGE_H_

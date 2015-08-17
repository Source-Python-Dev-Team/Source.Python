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

#ifndef _USERMESSAGE_IMPLEMENTATION_H_
#define _USERMESSAGE_IMPLEMENTATION_H_

#include "bitbuf.h"

#include "../messages_implementation_base.h"

extern IServerGameDLL *servergamedll;

class CUserMessageImplementation : public IUsermessageImplementationBase
{
public:
	CUserMessageImplementation(const MRecipientFilter &recipient_filter, const char *message_name);

	// Pure-virtual methods which must be inherited and overwritten in the inherited
	// classes
	virtual void set_bool(const char *field_name, bool field_value, int index=-1);
	virtual void set_char(const char *field_name, char field_value, int index=-1);
	virtual void set_byte(const char *field_name, unsigned char field_value, int index=-1);
	virtual void set_short(const char *field_name, int field_value, int index=-1);
	virtual void set_long(const char *field_name, signed long field_value, int index=-1);
	virtual void set_float(const char *field_name, float field_value, int index=-1);
	virtual void set_color(const char *field_name, Color field_value, int index=-1);

	// Unknown sized buffers
	virtual void set_buffer(const char *field_name, void *buffer, unsigned int num_bytes, int index=-1);
	virtual void set_string(const char *field_name, const char *field_value, int index=-1);

protected:
	virtual void set_message_index();
	virtual void send_message_internal();

private:
	bf_write *m_buffer;
};

#endif // _USERMESSAGE_IMPLEMENTATION_H_

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
*
* Original source:
* https://developer.valvesoftware.com/wiki/Ingame_menu_for_server_plugins_(CS:S_only)
*/

#ifndef _FILTERS_RECIPIENTS_WRAP_H
#define _FILTERS_RECIPIENTS_WRAP_H

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
#include "irecipientfilter.h"
#include "bitvec.h"
#include "tier1/utlvector.h"


//---------------------------------------------------------------------------------
// Recipient filter class.
//---------------------------------------------------------------------------------
class MRecipientFilter : public IRecipientFilter
{
public:
	MRecipientFilter();
	virtual ~MRecipientFilter();

	virtual bool IsReliable( void ) const;
	virtual bool IsInitMessage( void ) const;

	virtual int GetRecipientCount( void ) const;
	virtual int GetRecipientIndex( int slot ) const;
	void AddAllPlayers();
	void AddRecipient(int iPlayer);
	void RemoveRecipient(int iPlayer);
	void RemoveAllPlayers();
	bool HasRecipient(int iPlayer);

private:
	bool				m_bReliable;
	bool				m_bInitMessage;
	CUtlVector< int >	m_Recipients;
	
	// If using prediction rules, the filter itself suppresses local player
	bool				m_bUsingPredictionRules;
	// If ignoring prediction cull, then external systems can determine
	//  whether this is a special case where culling should not occur
	bool				m_bIgnorePredictionCull;
};


#endif // _FILTERS_RECIPIENTS_WRAP_H

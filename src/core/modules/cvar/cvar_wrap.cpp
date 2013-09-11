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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "cvar_wrap.h"

//-----------------------------------------------------------------------------
// CConVar constructors.
//-----------------------------------------------------------------------------
CConVar::CConVar( ConVar* pConVar )
{
	m_Name = pConVar->GetName();
}

CConVar::CConVar( const char *pName )
{
	m_Name = strdup(pName);

	ConVar *convar = g_pCVar->FindVar(m_Name);
	if( !convar )
	{
		ConVar *convar = new ConVar(m_Name, "0", 0);
	}
}

CConVar::CConVar( const char *pName, const char *pDefaultValue, int flags )
{
	m_Name = strdup(pName);

	ConVar *convar = g_pCVar->FindVar(m_Name);
	if( !convar )
	{
		ConVar *convar = new ConVar(m_Name, pDefaultValue, flags);
	}
}

CConVar::CConVar( const char *pName, const char *pDefaultValue,
		int flags, const char *pHelpString )
{
	m_Name = strdup(pName);

	ConVar *convar = g_pCVar->FindVar(m_Name);
	if( !convar )
	{
		ConVar *convar = new ConVar(m_Name, pDefaultValue, flags, strdup(pHelpString));
	}
}

CConVar::CConVar( const char *pName, const char *pDefaultValue, int flags,
		const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax )
{
	m_Name = strdup(pName);

	ConVar *convar = g_pCVar->FindVar(m_Name);
	if( !convar )
	{
		ConVar *convar = new ConVar(m_Name, pDefaultValue, flags,
			strdup(pHelpString), bMin, fMin, bMax, fMax);
	}
}

//-----------------------------------------------------------------------------
// CConVar methods.
//-----------------------------------------------------------------------------
const char* CConVar::get_help_text()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetHelpText();
}

const char *CConVar::get_name()
{
	return m_Name;
}

bool CConVar::is_flag_set( int iFlag )
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->IsFlagSet(iFlag);
}

void CConVar::add_flags( int iFlags )
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	convar->AddFlags(iFlags);
}

void CConVar::remove_flags( int iFlags )
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	command->RemoveFlags(iFlags);
}

int CConVar::get_flags()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetFlags();
}

bool CConVar::is_command()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->IsCommand();
}

float CConVar::get_float()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetFloat();
}

int CConVar::get_int()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetInt();
}

bool CConVar::get_bool()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetBool();
}

char const *CConVar::get_string()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetString();
}

void CConVar::set_float( float flValue )
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	convar->SetValue(flValue);
}

void CConVar::set_int( int iValue )
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	convar->SetValue(iValue);
}

void CConVar::set_bool( bool bValue )
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	convar->SetValue(bValue);
}

void CConVar::set_string( const char *szValue )
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	convar->SetValue(szValue);
}

void CConVar::revert()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	convar->Revert();
}

const char *CConVar::get_default()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	return convar->GetDefault();
}

bool CConVar::has_min()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	float fMin;
	return convar->GetMin(fMin);
}

bool CConVar::has_max()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	float fMax;
	return convar->GetMax(fMax);
}

float CConVar::get_min_value()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	float fMin;
	convar->GetMin(fMin);
	return fMin;
}

float CConVar::get_max_value()
{
	ConVar *convar = g_pCVar->FindVar(m_Name);
	float fMax;
	convar->GetMax(fMax);
	return fMax;
}

//-----------------------------------------------------------------------------
// CConCommandBase constructors.
//-----------------------------------------------------------------------------
CConCommandBase::CConCommandBase( const char *pName,
	const char *pHelpString, int flags )
{
	ConCommandBase *command = g_pCVar->FindCommandBase(pName);
	if( !command )
	{
		ConCommandBase *command = new ConCommandBase(pName, strdup(pHelpString), flags);
	}

	m_Name = pName;
}

CConCommandBase::CConCommandBase( ConCommandBase* pConCommandBase )
{
	m_Name = pConCommandBase->GetName();
}

//-----------------------------------------------------------------------------
// CConCommandBase methods.
//-----------------------------------------------------------------------------
bool CConCommandBase::is_command()
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	return command->IsCommand();
}

bool CConCommandBase::is_registered()
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	return command->IsRegistered();
}

bool CConCommandBase::is_flag_set( int iFlag )
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	return command->IsFlagSet(iFlag);
}

void CConCommandBase::add_flags( int iFlags )
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	command->AddFlags(iFlags);
}

void CConCommandBase::remove_flags( int iFlags )
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	command->RemoveFlags(iFlags);
}

int CConCommandBase::get_flags()
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	return command->GetFlags();
}

const char *CConCommandBase::get_name()
{
	return m_Name;
}

const char *CConCommandBase::get_help_text()
{
	ConCommandBase* command = g_pCVar->FindCommandBase(m_Name);
	return command->GetHelpText();
}

//-----------------------------------------------------------------------------
// CCvar methods.
//-----------------------------------------------------------------------------
void CCvar::register_con_command( CConCommandBase *pCommandBase )
{
	ConCommandBase* command = g_pCVar->FindCommandBase(pCommandBase->get_name());
	g_pCVar->RegisterConCommand(command);
}

void CCvar::unregister_con_command( CConCommandBase *pCommandBase )
{
	ConCommandBase* command = g_pCVar->FindCommandBase(pCommandBase->get_name());
	g_pCVar->UnregisterConCommand(command);
}

CConCommandBase *CCvar::find_command_base( const char *szName )
{
	ConCommandBase *pCommandBase = g_pCVar->FindCommandBase(szName);
	if( !pCommandBase )
	{
		return NULL;
	}

	CConCommandBase *nCommandBase = new CConCommandBase(pCommandBase);
	return nCommandBase;
}

CConVar *CCvar::find_var( const char *szName )
{
	ConVar *pConVar = g_pCVar->FindVar(szName);
	if( !pConVar )
	{
		return NULL;
	}

	CConVar *nConVar = new CConVar(pConVar);
	return nConVar;
}

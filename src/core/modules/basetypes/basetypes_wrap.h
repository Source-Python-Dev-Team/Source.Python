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

#ifndef _BASETYPES_WRAP_H
#define _BASETYPES_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"
#include "modules/memory/memory_tools.h"
#include "utility/conversions.h"


//-----------------------------------------------------------------------------
// Color extension class.
//-----------------------------------------------------------------------------
class ColorExt
{
public:
	static Color WithAlpha(const Color& color, unsigned char a)
	{
		return Color(color.r(), color.g(), color.b(), a);
	}

	static str GetHexString(const Color& color)
	{
		char buffer[10];
		if (color.a() == 255)
			sprintf(buffer, "\x07%02X%02X%02X", color.r(), color.g(), color.b());
		else
			sprintf(buffer, "\x08%02X%02X%02X%02X", color.r(), color.g(), color.b(), color.a());

		return str(buffer);
	}
};


//-----------------------------------------------------------------------------
// SendTable extension class.
//-----------------------------------------------------------------------------
class SendTableSharedExt
{
public:
	static SendProp *__getitem__(SendTable *pSendTable, int iIndex)
	{
		if (iIndex < 0 || iIndex > (pSendTable->m_nProps - 1))
		{
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Invalid index.");
		}
		return pSendTable->GetProp(iIndex);
	}
};


//-----------------------------------------------------------------------------
// SendProp extension class.
//-----------------------------------------------------------------------------
class SendPropSharedExt
{
public:
	enum SendPropFlags
	{
		UNSIGNED = SPROP_UNSIGNED,
		COORD = SPROP_COORD,
		NO_SCALE = SPROP_NOSCALE,
		ROUND_DOWN = SPROP_ROUNDDOWN,
		ROUND_UP = SPROP_ROUNDUP,
		NORMAL = SPROP_NORMAL,
		EXCLUDE = SPROP_EXCLUDE,
		XYZE = SPROP_XYZE,
		INSIDE_ARRAY = SPROP_INSIDEARRAY,
		PROXY_ALWAYS_YES = SPROP_PROXY_ALWAYS_YES,
		IS_A_VECTOR_ELEM = SPROP_IS_A_VECTOR_ELEM,
		COLLAPSIBLE = SPROP_COLLAPSIBLE,
		COORD_MP = SPROP_COORD_MP,
		COORD_MP_LOW_PRECISION = SPROP_COORD_MP_LOWPRECISION,
		COORD_MP_INTEGRAL = SPROP_COORD_MP_INTEGRAL,
		CHANGE_OFTEN = SPROP_CHANGES_OFTEN
	};

	static DVariant *call_proxy(SendProp *pSendProp, unsigned int uiEntityIndex, unsigned int uiOffset, int iElement)
	{
		if (pSendProp->GetType() == DPT_DataTable)
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is a DataTable.", pSendProp->GetName());
		}
		if (pSendProp->IsExcludeProp())
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is exluded.", pSendProp->GetName());
		}
		CPointer *pEntity = PointerFromIndex(uiEntityIndex);
		if (!pEntity)
		{
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Unable to find an entity " \
				"matching the given index \"%u\".", uiEntityIndex);
		}
		DVariant *pVariant = new DVariant();
		pVariant->m_Type = pSendProp->m_Type;
		pSendProp->GetProxyFn()(pSendProp, (const void *)(pEntity->m_ulAddr + (uiOffset - pSendProp->GetOffset())),
			(const void *)(pEntity->m_ulAddr + uiOffset),
			pVariant, iElement, (int)uiEntityIndex);
		return pVariant;
	}
};


//-----------------------------------------------------------------------------
// SendProp destructor declaration.
//-----------------------------------------------------------------------------
SendProp::~SendProp()
{
	// Declared here to fix undefined symbol issues...
}


//-----------------------------------------------------------------------------
// DVariant extension class.
//-----------------------------------------------------------------------------
class SendPropVariantExt
{
public:
	template<SendPropType prop_type, typename return_type, return_type DVariant::*member>
	static return_type get_typed_value(DVariant *pVariant)
	{
		if (pVariant->m_Type != prop_type)
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
		}
		return pVariant->*member;
	}

	template<SendPropType prop_type, typename member_type, member_type DVariant::*member>
	static void set_typed_value(DVariant *pVariant, member_type value)
	{
		pVariant->m_Type = prop_type;
		pVariant->*member = value;
	}

	static const char *get_string(DVariant *pVariant)
	{
		return (const char *)get_typed_value<DPT_String, char *, &DVariant::m_pString>(pVariant);
	}

	static void set_string(DVariant *pVariant, const char *pString)
	{
		set_typed_value<DPT_String, char *, &DVariant::m_pString>(pVariant, (char *)pString);
	}

	static CPointer *get_data(DVariant *pVariant)
	{
		return new CPointer((unsigned long)get_typed_value<DPT_DataTable, void *, &DVariant::m_pData>(pVariant));
	}

	static void set_data(DVariant *pVariant, CPointer *pData)
	{
		set_typed_value<DPT_DataTable, void *, &DVariant::m_pData>(pVariant, (void *)pData->m_ulAddr);
	}

	static Vector *get_vector(DVariant *pVariant)
	{
		if (pVariant->m_Type != DPT_Vector)
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
		}
		return new Vector(pVariant->m_Vector[0], pVariant->m_Vector[1], pVariant->m_Vector[2]);
	}

	static void set_vector(DVariant *pVariant, Vector *pVector)
	{
		pVariant->m_Type = DPT_Vector;
		pVariant->m_Vector[0] = pVector->x;
		pVariant->m_Vector[1] = pVector->y;
		pVariant->m_Vector[2] = pVector->z;
	}
};


//-----------------------------------------------------------------------------
// CTakeDamageInfo wrapper class.
//-----------------------------------------------------------------------------
class TakeDamageInfoBaseWrapper: public CTakeDamageInfo
{
public:
	int get_inflictor()
	{
		if (m_hInflictor.IsValid())
		{
			return m_hInflictor.GetEntryIndex();
		}
		return -1;
	}
	
	void set_inflictor(unsigned int iInflictor)
	{
		m_hInflictor = BaseHandleFromIndex(iInflictor);
	}
	
	int get_attacker()
	{
		if (m_hAttacker.IsValid())
		{
			return m_hAttacker.GetEntryIndex();
		}
		return -1;
	}
	
	void set_attacker(unsigned int iAttacker)
	{
		m_hAttacker = BaseHandleFromIndex(iAttacker);
	}
	
	int get_weapon()
	{
		if (m_hWeapon.IsValid())
		{
			return m_hWeapon.GetEntryIndex();
		}
		return -1;
	}
	
	void set_weapon(unsigned int iWeapon)
	{
		m_hWeapon = BaseHandleFromIndex(iWeapon);
	}
	
	void set_base_damage(float flBaseDamage)
	{
		m_flBaseDamage = flBaseDamage;
	}
	
	int get_damaged_other_players()
	{
		return m_iDamagedOtherPlayers;
	}
	
	void set_damaged_other_players(int iDamagedOtherPlayers)
	{
		m_iDamagedOtherPlayers = iDamagedOtherPlayers;
	}
};


//-----------------------------------------------------------------------------
// CTakeDamageInfo extension class.
//-----------------------------------------------------------------------------
class TakeDamageInfoSharedExt
{
public:
	static CTakeDamageInfo *__init__()
	{
		CTakeDamageInfo *pTakeDamageInfo = new CTakeDamageInfo();
		set_inflictor(pTakeDamageInfo, 0);
		set_attacker(pTakeDamageInfo, 0);
		return pTakeDamageInfo;
	}
	
	static int get_inflictor(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_inflictor();
	}
	
	static void set_inflictor(CTakeDamageInfo *pTakeDamageInfo, unsigned int iInflictor)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_inflictor(iInflictor);
	}
	
	static int get_attacker(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_attacker();
	}
	
	static void set_attacker(CTakeDamageInfo *pTakeDamageInfo, unsigned int iAttacker)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_attacker(iAttacker);
	}
	
	static int get_weapon(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_weapon();
	}
	
	static void set_weapon(CTakeDamageInfo *pTakeDamageInfo, unsigned int iWeapon)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_weapon(iWeapon);
	}
	
	static void set_base_damage(CTakeDamageInfo *pTakeDamageInfo, float flBaseDamage)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_base_damage(flBaseDamage);
	}
	
	static int get_damaged_other_players(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_damaged_other_players();
	}
	
	static void set_damaged_other_players(CTakeDamageInfo *pTakeDamageInfo, int iDamagedOtherPlayers)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_damaged_other_players(iDamagedOtherPlayers);
	}
};


//-----------------------------------------------------------------------------
// bf_write extension class.
//-----------------------------------------------------------------------------
class bf_writeExt
{
public:
	static void __del__(bf_write* buffer)
	{
		delete buffer->GetData();
	}

	static boost::shared_ptr<bf_write> __init__(int buffer_size)
	{
		return boost::shared_ptr<bf_write>(new bf_write(new unsigned char[buffer_size], buffer_size), &__del__);
	}

	static list GetData(bf_write& buffer)
	{
		list result;

		unsigned char* data = buffer.GetData();
		for (int i=0; i < buffer.GetNumBytesWritten(); i++) {
			result.append(data[i]);
		}

		return result;
	}
};


//-----------------------------------------------------------------------------
// bf_read extension class.
//-----------------------------------------------------------------------------
class bf_readExt
{
public:
	static void __del__(bf_read* buffer)
	{
		delete buffer->GetBasePointer();
	}

	static boost::shared_ptr<bf_read> __init__(bf_write& buffer)
	{
		int size = buffer.GetNumBytesWritten();
		void* pData = new unsigned char[size];
		memcpy(pData, buffer.GetData(), size);
		return boost::shared_ptr<bf_read>(new bf_read(pData, size), &__del__);
	}

	static list GetData(bf_read& buffer)
	{
		list result;
		
		const unsigned char* data = buffer.GetBasePointer();
		for (int i=0; i < (int) buffer.m_nDataBytes; i++) {
			result.append(data[i]);
		}

		return result;
	}

	static int GetNumBytesRead(bf_read& buffer)
	{
		return BitByte(buffer.GetNumBitsRead());
	}

	static str ReadString(bf_read& buffer)
	{
		char* pStr = new char[buffer.m_nDataBytes];
		buffer.ReadString(pStr, buffer.m_nDataBytes);

		// Let Boost handle deallocating the string
		str result = str((const char *) pStr);
		delete pStr;
		return result;
	}
};


#endif // _BASETYPES_WRAP_H

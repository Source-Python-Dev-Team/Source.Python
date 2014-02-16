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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "entity_props.h"

//---------------------------------------------------------------------------------
// Utility function to find send table props.
//---------------------------------------------------------------------------------
SendProp* UTIL_FindSendProp( SendTable* send_table, const char* prop_name, int &iOffset )
{
	int			prop_count = send_table->GetNumProps();
	SendProp*	prop;

	// Loop through each property in the datatable
	for( int i = 0; i < prop_count; i++ )
	{
		// Get the current SendProp
		prop = send_table->GetProp(i);

		// Is this the SendProp that we are looking for?
		if( V_strcmp(prop->GetName(), prop_name) == 0 )
		{
			// Increment the offset count
			iOffset += prop->GetOffset();

			// Return the SendProp
			return prop;
		}

		// Is the current SendProp a datatable?
		if( prop->GetDataTable() )
		{
			// Does the datatable contain the SendProp?
			if((prop = UTIL_FindSendProp(prop->GetDataTable(), prop_name, iOffset)) != NULL)
			{
				// Return the SendProp
				return prop;
			}
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------------
// CSendPropHashtable code.
//----------------------------------------------------------------------------------
CSendPropHashTable::CSendPropHashTable() :
	m_prop_table(
		32768, 
		0, 
		0, 
		&CSendPropHashTable::hash_compare, 
		&CSendPropHashTable::hash_key
	)
{

}

CSendPropHashTable::~CSendPropHashTable()
{
	// Free the names of the props since we use strdup.
#if 0
	for( int i = 0; i < m_prop_table.Count(); i++ )
	{
		CPropOffset* prop_offset = &m_prop_table[i];
		free((void *)prop_offset->prop_name);
		prop_offset->prop_name = NULL;
	}
#endif
}

bool CSendPropHashTable::hash_compare( const CPropOffset& a, const CPropOffset& b )
{
	return (V_strcmp(a.prop_name, b.prop_name) == 0);
}

//----------------------------------------------------------------------------------
// This amazing hash function borrowed from:
// http://www.azillionmonkeys.com/qed/hash.html
//----------------------------------------------------------------------------------
#define get16bits(d) ((((unsigned int)(((const unsigned char *)(d))[1])) << 8)\
	+(unsigned int)(((const unsigned char *)(d))[0]) )

unsigned int SuperFastHash (const char * data, int len) {
	unsigned hash = len, tmp;
	int rem;

	if (len <= 0 || data == NULL) return 0;

	rem = len & 3;
	len >>= 2;

	/* Main loop */
	for (;len > 0; len--) {
		hash  += get16bits (data);
		tmp    = (get16bits (data+2) << 11) ^ hash;
		hash   = (hash << 16) ^ tmp;
		data  += 2*sizeof (unsigned short);
		hash  += hash >> 11;
	}

	/* Handle end cases */
	switch (rem) {
	case 3: hash += get16bits (data);
		hash ^= hash << 16;
		hash ^= ((signed char)data[sizeof (unsigned short)]) << 18;
		hash += hash >> 11;
		break;
	case 2: hash += get16bits (data);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;
	case 1: hash += (signed char)*data;
		hash ^= hash << 10;
		hash += hash >> 1;
	}

	/* Force "avalanching" of final 127 bits */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}

unsigned int CSendPropHashTable::hash_key( const CPropOffset& a )
{
	return SuperFastHash(a.prop_name, V_strlen(a.prop_name));
}

SendProp* CSendPropHashTable::get_prop( const char* prop_name, int &iOffset )
{
	// Construct a temporary hash table element
	// to search with.
	CPropOffset prop_offset_data;
	
	prop_offset_data.prop_name = prop_name;
	prop_offset_data.prop = NULL;

	// Search the table.
	UtlHashHandle_t prop_offset_handle = 
		m_prop_table.Find(prop_offset_data);

	// Return the value if we found one.
	if( prop_offset_handle != m_prop_table.InvalidHandle() )
	{
		// Set the offset value
		iOffset = m_prop_table.Element(prop_offset_handle).prop_offset;

		// Return the SendProp
		return m_prop_table.Element(prop_offset_handle).prop;
	}

	// Could not find the requested prop.
	return NULL;
}

void CSendPropHashTable::insert_offset( const char* name, SendProp* prop, int iOffset )
{
	// Construct the prop structure. Since this is basically a
	// struct, no need to deep copy.
	CPropOffset prop_data;

	// Copy the prop name since it won't hang around in memory after
	// return.
	prop_data.prop_name = strdup(name);

	// Store off the offset.
	prop_data.prop = prop;

	// Store off the offset value.
	prop_data.prop_offset = iOffset;

	// Put this data in the table.
	m_prop_table.Insert(prop_data);
}

void CSendPropHashTable::remove_offset( const char* prop_name )
{
	// Construct a temp structure to find the offset.
	CPropOffset prop_offset;

	prop_offset.prop_name = prop_name;
	prop_offset.prop = NULL;

	// Find the index of the element.
	UtlHashHandle_t prop_offset_handle = 
		m_prop_table.Find(prop_offset);

	// If we found the offset, get rid of the prop data.
	if( prop_offset_handle != m_prop_table.InvalidHandle() )
	{
		// Free the string memory first.
		CPropOffset* element = &m_prop_table[prop_offset_handle];
		delete element->prop_name;

		// Remove the data now.
		m_prop_table.Remove(prop_offset_handle);
	}
}

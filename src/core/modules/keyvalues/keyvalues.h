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

#ifndef _KEYVALUES_H
#define _KEYVALUES_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "tier1/KeyValues.h"
#include "filesystem.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;


//-----------------------------------------------------------------------------
// KeyValues iter class.
//-----------------------------------------------------------------------------
class KeyValuesIter
{
public:
	KeyValuesIter(KeyValues* pKeyValues) {
		m_pKeyValues = pKeyValues;
		m_pCurrentKey = pKeyValues;
	}

	const char* __next__() {
		// First iteration?
		if (m_pCurrentKey == m_pKeyValues) {
			m_pCurrentKey = m_pKeyValues->GetFirstSubKey();
		}
		else {
			m_pCurrentKey = m_pCurrentKey->GetNextKey();
		}

		if (!m_pCurrentKey) {
			BOOST_RAISE_EXCEPTION(PyExc_StopIteration, "No more keys.");
		}

		return m_pCurrentKey->GetName();
	}

private:
	KeyValues* m_pKeyValues;
	KeyValues* m_pCurrentKey;
};


//-----------------------------------------------------------------------------
// KeyValues extension class.
//-----------------------------------------------------------------------------
class KeyValuesExt
{
public:
	static boost::shared_ptr<KeyValues> __init__1(const char* setName)
	{ return boost::shared_ptr<KeyValues>(new KeyValues(setName), &__del__); }

	static boost::shared_ptr<KeyValues> __init__2(const char *setName, const char *firstKey, const char *firstValue)
	{ return boost::shared_ptr<KeyValues>(new KeyValues(setName, firstKey, firstValue), &__del__); }

	static boost::shared_ptr<KeyValues> __init__3(const char *setName, const char *firstKey, int firstValue)
	{ return boost::shared_ptr<KeyValues>(new KeyValues(setName, firstKey, firstValue), &__del__); }

	static boost::shared_ptr<KeyValues> __init__4(const char *setName, const char *firstKey, const char *firstValue, const char *secondKey, const char *secondValue)
	{ return boost::shared_ptr<KeyValues>(new KeyValues(setName, firstKey, firstValue, secondKey, secondValue), &__del__); }

	static boost::shared_ptr<KeyValues> __init__5(const char *setName, const char *firstKey, int firstValue, const char *secondKey, int secondValue)
	{ return boost::shared_ptr<KeyValues>(new KeyValues(setName, firstKey, firstValue, secondKey, secondValue), &__del__); }

	static boost::shared_ptr<KeyValues> make_copy(KeyValues* pKeys)
	{ return boost::shared_ptr<KeyValues>(pKeys->MakeCopy(), &__del__); }

	static void __del__(KeyValues* pKeyValues)
	{ pKeyValues->deleteThis(); }

	static bool GetBool(KeyValues* pKeyValues, const char * szName = NULL, bool bDefault = false)
	{ return pKeyValues->GetInt(szName, bDefault); }

	static void SetBool(KeyValues* pKeyValues, const char * szName, bool bValue)
	{ pKeyValues->SetInt(szName, bValue); }

	static bool LoadFromFile(KeyValues* pKeyValues, const char * szFile)
	{ return pKeyValues->LoadFromFile(filesystem, szFile); }

	static bool SaveToFile(KeyValues* pKeyValues, const char * szFile)
	{ return pKeyValues->SaveToFile(filesystem, szFile); }

	static Color GetColor(KeyValues* pKeyValues, const char* szKeyName, const Color &defaultColor = Color())
	{
		if (!pKeyValues->FindKey(szKeyName))
			return defaultColor;

		return pKeyValues->GetColor(szKeyName);
	}

	static object __getitem__(KeyValues* pKeyValues, const char* key)
	{
		KeyValues* subkey = pKeyValues->FindKey(key);
		if (!subkey) {
			BOOST_RAISE_EXCEPTION(PyExc_KeyError, "Key '%s' does not exist.", key);
		}

		switch (subkey->GetDataType())
		{
			case KeyValues::TYPE_NONE: return object(ptr(subkey)); break;
			case KeyValues::TYPE_WSTRING:
			case KeyValues::TYPE_STRING: return object(subkey->GetString()); break;
			case KeyValues::TYPE_INT: return object(subkey->GetInt()); break;
			case KeyValues::TYPE_FLOAT: return object(subkey->GetFloat()); break;
			case KeyValues::TYPE_PTR: return object(CPointer((unsigned long) subkey->GetPtr())); break;
			case KeyValues::TYPE_COLOR: return object(subkey->GetColor()); break;
			case KeyValues::TYPE_UINT64: return object(subkey->GetUint64()); break;
			default: BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Unsupported type '%i' for key '%s'.", subkey->GetDataType(), subkey->GetName());
		}
		return object();
	}

	static void __setitem__(KeyValues* pKeyValues, const char* key, object value)
	{
		KeyValues* subkey = pKeyValues->FindKey(key, true);
		if (!subkey) {
			BOOST_RAISE_EXCEPTION(PyExc_KeyError, "Key '%s' does not exist.", key);
		}

		switch (subkey->GetDataType())
		{
			case KeyValues::TYPE_NONE: {
				KeyValues* pValue = extract<KeyValues*>(value);
				subkey->SetPtr(NULL, (void*) pValue);
				break;
			}
			case KeyValues::TYPE_WSTRING:
			case KeyValues::TYPE_STRING: {
				const char* szValue = extract<const char*>(value);
				subkey->SetString(NULL, szValue);
				break;
			}
			case KeyValues::TYPE_INT: {
				int iValue = extract<int>(value);
				subkey->SetInt(NULL, iValue);
				break; 
			}
			case KeyValues::TYPE_FLOAT: {
				float fValue = extract<float>(value);
				subkey->SetFloat(NULL, fValue); 
				break;
			}
			case KeyValues::TYPE_PTR: {
				CPointer* pValue = extract<CPointer*>(value);
				subkey->SetPtr(NULL, (void *) pValue->m_ulAddr);
				break;
			}
			case KeyValues::TYPE_COLOR: {
				Color color = extract<Color>(value);
				subkey->SetColor(NULL, color);
				break;
			}
			case KeyValues::TYPE_UINT64: {
				uint64 iValue = extract<uint64>(value);
				subkey->SetUint64(NULL, iValue);
				break;
			}
			default: BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Unsupported type '%i' for key '%s'.", subkey->GetDataType(), subkey->GetName());
		}
	}

	static void __delitem__(KeyValues* pKeyValues, const char* key)
	{
		KeyValues* subkey = pKeyValues->FindKey(key);
		if (!subkey) {
			BOOST_RAISE_EXCEPTION(PyExc_KeyError, "Key '%s' does not exist.", key);
		}

		pKeyValues->RemoveSubKey(subkey);
	}

	static KeyValuesIter __iter__(KeyValues* pKeyValues)
	{ return KeyValuesIter(pKeyValues); }

	static dict as_dict(KeyValues* pKeyValues) {
		dict result;

		KeyValues* pCurrent = pKeyValues->GetFirstSubKey();
		while (pCurrent) {
			const char* name = pCurrent->GetName();

			if (pCurrent->GetDataType() == KeyValues::TYPE_NONE) {
				result[name] = as_dict(pCurrent);
			}
			else {
				result[name] = __getitem__(pKeyValues, name);
			}

			pCurrent = pCurrent->GetNextKey();
		}

		return result;
	}
};


#endif // _KEYVALUES_H

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

	static boost::shared_ptr<KeyValues> LoadFromFile(const char * szFile)
	{ 
		KeyValues* pKeyValues = new KeyValues("");
		if (!pKeyValues->LoadFromFile(filesystem, szFile)) {
			pKeyValues->deleteThis();
			return NULL;
		}
		return boost::shared_ptr<KeyValues>(pKeyValues, &__del__);
	}
	
	static bool LoadFromFile2(KeyValues* pKeyValues, const char * szFile)
	{ 
		return pKeyValues->LoadFromFile(filesystem, szFile);
	}

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

	static boost::shared_ptr<KeyValues> from_dict(const char* name, dict source)
	{
		KeyValues* root = new KeyValues(name);
		AddDict(root, source);
		return boost::shared_ptr<KeyValues>(root, &__del__);
	}

	static void AddDict(KeyValues* key, dict source)
	{
		list keys = source.keys();
		for (int i=0; i < len(keys); ++i) {
			const char* key_name = extract<const char*>(keys[i]);
			object value = source[key_name];

			if (PyLong_Check(value.ptr())) {
				int overflow;
				long long_result = PyLong_AsLongAndOverflow(value.ptr(), &overflow);
				if (overflow == 0) {
					key->SetInt(key_name, long_result);
					continue;
				}

				PY_LONG_LONG long_long_result = PyLong_AsLongLong(value.ptr());
				if (long_long_result < 0) {
					// KeyValues can't handle negative long long values.
					BOOST_RAISE_EXCEPTION(PyExc_OverflowError, "%lli is too big.", long_long_result)
				}

				key->SetUint64(key_name, long_long_result);
				continue;
			}

			extract<float> get_float(value);
			if (get_float.check()) {
				key->SetFloat(key_name, get_float());
				continue;
			}

			extract<const char*> get_string(value);
			if (get_string.check()) {
				key->SetString(key_name, get_string());
				continue;
			}

			extract<Color> get_color(value);
			if (get_color.check()) {
				key->SetColor(key_name, get_color());
				continue;
			}

			extract<CPointer*> get_ptr(value);
			if (get_ptr.check()) {
				key->SetPtr(key_name, (void*) get_ptr()->m_ulAddr);
				continue;
			}

			extract<dict> get_dict(value);
			if (get_dict.check()) {
				AddDict(key->FindKey(key_name, true), get_dict());
				continue;
			}

			object type_name = value.attr("__class__").attr("__name__");
			object py_value_str = str(value);
			const char* type_name_str = extract<const char*>(type_name);
			const char* value_str = extract<const char*>(py_value_str);
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Invalid type (%s) of value '%s'", type_name_str, value_str)
		}
	}
};


#endif // _KEYVALUES_H

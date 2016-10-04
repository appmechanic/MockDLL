#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>


#include <string>
#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>


#include "Gun_BLE.h"









// misc.cpp


#define btwn(lo,x,hi)	(lo <= x && x <= hi)
void GDH_Log(const char *format, ...);
void StrCatF(char *s, int size, const char *format, ...);
const void *memmem(const void *l, size_t l_len, const void *s, size_t s_len);

int a2hex(int c);
int hex2a(int h);
char *hex_string(unsigned char *data, int len, char *outputString);

float NormalizeAngle(float a);




// -----------------------------------------------------------------------------------------------





struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
#define HASH_NEW(name, keyType, valueType)		typedef std::map<keyType, valueType> name##_Type; \
												name##_Type *name##_Inst = new name##_Type();  

#define HASH_NEW_STR(name, keyType, valueType)	typedef std::map<keyType, valueType, cmp_str> name##_Type; \
												name##_Type *name##_Inst = new name##_Type();  
#define HASH_INSERT(name, key, val)			name##_Inst->insert( name##_Type::value_type( key, val ) )  
#define HASH_FIND_(name, key, itNbr)		name##_Type::const_iterator it##itNbr = name##_Inst->find(key);  
#define HASH_FIND(name, key)				HASH_FIND_(name, key, 1)  
#define HASH_FOUND_(name, itNbr)			it##itNbr != name##_Inst->end()  
#define HASH_FOUND(name)					HASH_FOUND_(name, 1)  
#define HASH_GET_(itNbr)					it##itNbr._Ptr->_Myval.second  
#define HASH_GET()							HASH_GET_(1)  
#define HASH_LEN(name)						((int)name##_Inst->size())  
#define HASH_REMOVE_(name, itNbr)				name##_Inst->erase( it##itNbr )  
#define HASH_REMOVE(name)							HASH_REMOVE_(name, 1)  

#define LIST_FOREACH_(name, itemType, itNbr)		for (std::list<itemType>::const_iterator it##itNbr=name.begin(),end##itNbr=name.end(); it##itNbr!=end##itNbr; ++it##itNbr)   
#define LIST_FOREACH(name, itemType)				LIST_FOREACH_(name, itemType, 1)  
#define LIST_FOREACH_PTR_(name, itemType, itNbr)	for (std::list<itemType>::const_iterator it##itNbr=name->begin(),end##itNbr=name->end(); it##itNbr!=end##itNbr; ++it##itNbr)   
#define LIST_FOREACH_PTR(name, itemType)			LIST_FOREACH_IT_PTR(name, itemType, 1)  
#define LIST_FOREACH_REVERSE_(name, itemType, itNbr)		for (std::list<itemType>::const_reverse_iterator it##itNbr=name.rbegin(),end##itNbr=name.rend(); it##itNbr!=end##itNbr; ++it##itNbr)   
#define LIST_FOREACH_REVERSE(name, itemType)				LIST_FOREACH_REVERSE_(name, itemType, 1)  
#define LIST_ITEM(name, inx)						std::next(name.begin(), inx)._Ptr->_Myval  
#define LIST_GET_(itNbr)							(*it##itNbr)
#define LIST_GET()									LIST_GET_(1)

// No incrementor at the end.  You must increment if you are not removing the item.  When you remove... it1 = list.erase(it1);
#define LIST_FOREACH_REMOVAL_(name, itemType, itNbr)		for (std::list<itemType>::const_iterator it##itNbr=name.begin(),end##itNbr=name.end(); it##itNbr!=end##itNbr;)   
#define LIST_FOREACH_REMOVAL(name, itemType)				LIST_FOREACH_REMOVAL_(name, itemType, 1)  
#define LIST_ERASE_ITEM_(name, itNbr)						(it##itNbr = name.erase(it##itNbr))
#define LIST_ERASE_ITEM(name)								LIST_ERASE_ITEM_(name, 1)
#define LIST_NEXT_(itNbr)									(it##itNbr)++
#define LIST_NEXT()											LIST_NEXT_(1)


#define VEC_FOREACH_(name, itemType, itNbr)		for (std::vector<itemType>::const_iterator it##itNbr=name.begin(),end##itNbr=name.end(); it##itNbr!=end##itNbr; ++it##itNbr)   
#define VEC_FOREACH(name, itemType)				VEC_FOREACH_(name, itemType, 1)  

#define HZ_DURATION(num)		(1000/num) // 2 Hz = every 500ms, or 4 Hz = every 250ms.

#define BOOL_TO_CHAR(b)		(b?'Y':'N')
#define btwn(lo,x,hi)	(lo <= x && x <= hi)





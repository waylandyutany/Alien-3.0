#include "StdAfx.h"

#include "strings.h"

namespace ae {

size_t SearchStringBuffer(const char* string, const size_t stringSize, const char* searchString)
{
	const size_t slen = strlen(searchString);
	const size_t send(stringSize - slen);
	for(size_t i = 0; i < send; i++)
		if(!memcmp(&string[i], searchString, slen))
			return i;
	return (size_t)-1;
}

} // namespace ae {


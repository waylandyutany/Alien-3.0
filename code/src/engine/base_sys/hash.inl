
namespace ae { namespace base {

static const T_Hash64 hash64_init =(T_Hash64)0xcbf29ce484222325ULL;
static const T_Hash32 hash32_init =(T_Hash32)0x811c9dc5;

inline T_Hash32 CalculateHash32(const char* str)
{
//	if(!str || !*str) return 0;
	T_Hash32 hash32(hash32_init);
	unsigned const char *sb = (unsigned const char *)str;

	while(*sb)
	{
		hash32 += (hash32 << 1) + (hash32 << 4) + (hash32 << 7) + (hash32 << 8) + (hash32 << 24);
		hash32 ^= (T_Hash32)*sb++;
	}

	return hash32;
}

inline T_Hash32 CalculateHashI32(const char* str)
{
//	if(!str || !*str) return 0;
	T_Hash32 hash32(hash32_init);
	unsigned const char *sb = (unsigned const char *)str;

	while(*sb)
	{
		hash32 += (hash32 << 1) + (hash32 << 4) + (hash32 << 7) + (hash32 << 8) + (hash32 << 24);
		hash32 ^= (T_Hash32)tolower(*sb++);
	}

	return hash32;
}

inline T_Hash32 CalculateHash32(const void* buff, size_t size)
{
//	if(!buff || !size) return 0;
	T_Hash32 hash32(hash32_init);
	unsigned const char *bpos = (unsigned const char *)buff;
	unsigned const char *bend = bpos + size;

	while (bpos < bend)
	{
		hash32 += (hash32 << 1) + (hash32 << 4) + (hash32 << 7) + (hash32 << 8) + (hash32 << 24);
		hash32 ^= (T_Hash32)*bpos++;
	}

	return hash32;
}

inline T_Hash64 CalculateHash64(const char* str)
{
//	if(!str || !*str) return 0;
	T_Hash64 hash64(hash64_init);
	unsigned const char *sb = (unsigned const char *)str;

	while(*sb)
	{
		hash64 += (hash64 << 1) + (hash64 << 4) + (hash64 << 5) + (hash64 << 7) + (hash64 << 8) + (hash64 << 40);
		hash64 ^= (T_Hash64)*sb++;
	}

	return hash64;
}

inline T_Hash64 CalculateHashI64(const char* str)
{
//	if(!str || !*str) return 0;
	T_Hash64 hash64(hash64_init);
	unsigned const char *sb = (unsigned const char *)str;

	while(*sb)
	{
		hash64 += (hash64 << 1) + (hash64 << 4) + (hash64 << 5) + (hash64 << 7) + (hash64 << 8) + (hash64 << 40);
		hash64 ^= (T_Hash64)tolower(*sb++);
	}

	return hash64;
}

inline T_Hash64 CalculateHash64(const void* buff, size_t size)
{
//	if(!buff || !size) return 0;
	T_Hash64 hash64(hash64_init);
	unsigned const char *bpos = (unsigned const char *)buff;
	unsigned const char *bend = bpos + size;

	while (bpos < bend)
	{
		hash64 += (hash64 << 1) + (hash64 << 4) + (hash64 << 5) + (hash64 << 7) + (hash64 << 8) + (hash64 << 40);
		hash64 ^= (T_Hash64)*bpos++;
	}

	return hash64;
}

} } // namespace ae { namespace base {

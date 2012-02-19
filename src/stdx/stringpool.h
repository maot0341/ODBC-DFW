#pragma warning(disable : 4786)

#ifndef __STDX_STRINGPOOL_H__
#define __STDX_STRINGPOOL_H__

#include <string>
#include <set>

namespace stdx
{
using namespace std;
//---------------------------------------------------------------------------
// Stringpool
// no duplicatet strings - less memory usage
//---------------------------------------------------------------------------
class CStringpool
{
public:
	CStringpool() : m_count(0), m_found(0) {}
	void clear() { m_data.clear(); reset(); }
	void reset() { m_count = m_found = 0; }

	const char * operator[] (const char* s) { return operator[](string(s)); }
	const char * operator[] (const string &);

protected:
	size_t m_count;
	size_t m_found;
	set<string> m_data;
};
//---------------------------------------------------------------------------
inline
const char *
CStringpool::operator[] (const string & key)
{
	pair<set<string>::iterator, bool> aResult = m_data.insert(key);
	const set<string>::iterator iKey = aResult.first;
	const bool bInsert = aResult.second;
	if (!bInsert)
		m_found++;
	m_count++;
	const string & s = *iKey;
	const char * szKey = s.c_str();
	return szKey;
}
//---------------------------------------------------------------------------
} // namespace stdx
#endif
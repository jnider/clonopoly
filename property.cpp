#include "property.h"

CProperty::CProperty()
{
}

CProperty::CProperty(const char* name, int value, PropertySet set, int rent[], int mortgage, int houseCost, int x, int y, int w, int h)
 : m_name(name), m_value(value), m_set(set), m_mortgage(mortgage), m_houseCost(houseCost), m_owner(-1),
   m_mortgaged(false)
{
}

CProperty::~CProperty()
{
}

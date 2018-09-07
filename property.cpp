#include "property.h"

CProperty::CProperty() : m_owner(-1)
{
}

/*
CProperty::CProperty(const char* name, int val, PropertySet, int* rent, int mortgage, int cost, SDL_Rect& loc)
{
}
*/

CProperty::CProperty(const char* name) : m_name(name)
{
}

CProperty::~CProperty()
{
}

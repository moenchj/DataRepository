#include "BaseDO.h"

BaseDO::BaseDO(long long id)
: _id(id)
{}

void BaseDO::id(long long id)
{
    _id = id;
}

long long BaseDO::id() const
{
    return _id;
}

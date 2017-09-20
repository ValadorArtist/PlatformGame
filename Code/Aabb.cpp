#include "Aabb.h"

// sprawdzanie czy prostok¹ty przecinaj¹ sie??
bool Aabb::Collides(const Aabb & box) const
{
    if (m_min_x > box.m_max_x || m_max_x < box.m_min_x || m_min_y > box.m_max_y || m_max_y < box.m_min_y) 
    {
        return false;
    }
    return true;
}

// sprawdzanie czy this jest powy¿ej box'a
bool Aabb::IsOver(const Aabb & box) const 
{
    return Collides(box) && m_min_y < box.m_max_y && box.m_max_y < m_max_y;
}

// sprawdzanie czy this jest poni¿ej box'a
bool Aabb::IsUnder(const Aabb & box) const
{
    return box.IsOver(*this);
}
// sprawdzanie czy this jest po lewej stronie box'a
bool Aabb::IsOnLeftOf(const Aabb & box) const 
{
    return Collides(box) && m_min_x < box.m_min_x && box.m_min_x < m_max_x;
}

// sprawdzanie czy this jest po prawej stronie box'a
bool Aabb::IsOnRightOf(const Aabb & box) const 
{
    return box.IsOnLeftOf(*this);
}

std::ostream& operator<<(std::ostream& os, const Aabb& box) 
{
    os << "[ " << box.GetMinX() << ", " << box.GetMinY() << "; " << box.GetMaxX() << ", " << box.GetMaxY() << " ] ";
    return os;
}

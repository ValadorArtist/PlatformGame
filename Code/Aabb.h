#ifndef __AABB_H__
#define __AABB_H__

#include <cassert>
#include <ostream>
#include <boost/shared_ptr.hpp> 

// Axis-Aligned Bounding Box
class Aabb
{
public:
       
// metody klasy
    Aabb(double min_x, double min_y, double max_x, double max_y)
    : m_min_x(min_x), m_min_y(min_y), m_max_x(max_x), m_max_y(max_y)
    {
    /* sprawdzanie asercji, aby unikn¹c b³ednego przekazania argumentów
    i b³edów przy tworzeniu obiektu */
    
      assert(m_min_x < m_max_x);
      assert(m_min_y < m_max_y);
      
    }
    
    bool IsOnRightOf(const Aabb& box) const;
    bool IsOnLeftOf(const Aabb& box) const;
    bool Collides(const Aabb& box) const;
    bool IsOver(const Aabb& box) const;
    bool IsUnder(const Aabb& box) const;

    double GetMaxX() const 
    { return m_max_x; }
    
    double GetMaxY() const 
    { return m_max_y; }
    
    double GetMinX() const 
    { return m_min_x; }
    
    double GetMinY() const 
    { return m_min_y; }
    
    Aabb Move(double min_x_offset, double min_y_offset, double max_x_offset, double max_y_offset) 
    {
        return Aabb(m_min_x + min_x_offset, m_min_y + min_y_offset, m_max_x + max_x_offset, m_max_y + max_y_offset);
    }

    friend std::ostream& operator<< (std::ostream& os, const Aabb& box);
    
private:
        
// pola klasy

// pozycja lewego dolnego naro¿nika obiektu
    double m_min_x;
    double m_min_y;

// pozycja prawego górnego naro¿nika obiektu
    double m_max_x;
    double m_max_y;
};

typedef boost::shared_ptr<Aabb> AabbPtr;

#endif

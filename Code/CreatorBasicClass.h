#ifndef __CREATOR_BASIC_CLASS_H__
#define __CREATOR_BASIC_CLASS_H__

#include <list>

#include "Creator.h"

// Klasa bazowa dla klas, które bed¹ dodawac kreatory
class CreatorBasicClass 
{
public:
    void AddCreator(CreatorPtr creator) 
    {
        m_creators.push_back(creator);
    }

    void DropAllCreators() 
    {
        m_creators.clear();
    }

    std::list<CreatorPtr> GetCreators() const 
    {
        return m_creators;
    }

private:
    std::list<CreatorPtr> m_creators;
};

#endif

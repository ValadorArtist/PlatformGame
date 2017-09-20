#ifndef __CREATURE_FACTORY_H__
#define __CREATURE_FACTORY_H__

#include <boost/shared_ptr.hpp>

class Creature;
typedef boost::shared_ptr<Creature> CreaturePtr;

class CreatureFactory 
{
public:
    explicit CreatureFactory();
    CreaturePtr CreateCreature(CR::CreatureType type, double x, double y);
    CreaturePtr CreateCreature(const std::string& name, double x, double y);
};

typedef boost::shared_ptr<CreatureFactory> CreatureFactoryPtr;

#endif

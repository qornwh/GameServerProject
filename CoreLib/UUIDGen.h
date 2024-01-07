#pragma once
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators

class UUIDGen
{
public:
    static boost::uuids::uuid Generator();

private:
    static boost::uuids::random_generator generator;
};

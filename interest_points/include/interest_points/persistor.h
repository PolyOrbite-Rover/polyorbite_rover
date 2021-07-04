#pragma once

#include <string>
#include "interest_points/InterestPoint.h"

class Persistor
{
public:
    Persistor();
    ~Persistor();

    void addInterestPoint(const interest_points::InterestPoint& interest_point);
    void updateInterestPoint(const interest_points::InterestPoint& interest_point);

private:
    std::string file_name;
};
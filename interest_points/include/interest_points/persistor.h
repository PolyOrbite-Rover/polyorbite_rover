#pragma once

#include <mysql.h>
#include <string>

#include <iostream>
#include <fstream>

#include "ros/ros.h"
#include "interest_points/InterestPoint.h"

class Persistor
{
public:
    Persistor();
    ~Persistor();

    void addInterestPoint(const interest_points::InterestPoint& interest_point);
    void updateInterestPoint(const interest_points::InterestPoint& interest_point);

    void getInterestPointsPaged(const int& page, const int& pageSize);
    void getInterestPoints();

private:
    MYSQL* database;
    std::string file_name;

    void query(const std::string& queryTemplate);
    void query(const std::string& queryTemplate, const std::vector<MYSQL_BIND>& parameters);
    void fetchQueryResult();
    void writeFile(const std::string& name, const std::string& content);

    MYSQL_BIND makeMySqlStringParameter(const std::string& value, long unsigned int& count);
};
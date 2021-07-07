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

private:
    MYSQL* database;
    std::string file_name;

    void query(const std::string& query, const std::vector<MYSQL_BIND>& parameters);
    void writeFile(const std::string& name, const std::string& content);

    MYSQL_BIND makeMySqlStringParameter(const std::string& value, long unsigned int& count);
};
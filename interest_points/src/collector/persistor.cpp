#include "interest_points/persistor.h"

Persistor::Persistor()
{
    database = mysql_init(NULL);

    if(database == NULL)
    {
        ROS_FATAL("Could not initialize database connector");
        exit(1);
    }

    bool errorWhileConnecting = mysql_real_connect(database, "localhost", "root", "", NULL, 0, NULL, 0) == NULL;
    if(errorWhileConnecting)
    {
        ROS_FATAL("Could not connect to database");
        mysql_close(database);
        exit(1);
    }
}

Persistor::~Persistor()
{
    mysql_close(database);
}

void Persistor::addInterestPoint(const interest_points::InterestPoint& interest_point)
{
    std::string dataFileName = interest_point.uuid;
    writeFile(dataFileName, interest_point.data);

    std::string queryTemplate = "INSERT INTO polyorbite_rover.interest_points (uuid, name, data) VALUES (?, ?, ?)";
    std::vector<MYSQL_BIND> parameters;

    long unsigned int uuidLength = interest_point.uuid.length();
    long unsigned int nameLength = interest_point.name.length();
    long unsigned int dataLength = dataFileName.length();

    parameters.push_back(makeMySqlStringParameter(interest_point.uuid, uuidLength));
    parameters.push_back(makeMySqlStringParameter(interest_point.name, nameLength));
    parameters.push_back(makeMySqlStringParameter(dataFileName, dataLength));

    query(queryTemplate, parameters);
}

void Persistor::updateInterestPoint(const interest_points::InterestPoint& interest_point)
{
    std::string queryTemplate = "UPDATE polyorbite_rover.interest_points SET name = ? WHERE uuid = ?";
    std::vector<MYSQL_BIND> parameters;

    long unsigned int nameLength = interest_point.name.length();
    long unsigned int uuidLength = interest_point.uuid.length();

    parameters.push_back(makeMySqlStringParameter(interest_point.name, nameLength));
    parameters.push_back(makeMySqlStringParameter(interest_point.uuid, uuidLength));

    query(queryTemplate, parameters);
}

std::vector<interest_points::InterestPoint>
Persistor::getInterestPointsPaged(const int& page, const int& pageSize)
{
    
}

std::vector<interest_points::InterestPoint>
Persistor::getInterestPoints()
{
    query(std::string("SELECT uuid, creationTimestamp, name, data FROM polyorbite_rover.interest_points"));
    fetchQueryResult();
}

void Persistor::query(const std::string& queryTemplate)
{
    query(queryTemplate, {});
}

void Persistor::query(const std::string& queryTemplate, const std::vector<MYSQL_BIND>& parameters)
{
    MYSQL_STMT *stmt = mysql_stmt_init(database);

    bool isOutOfMemory = stmt == NULL;
    if(isOutOfMemory)
    {
        ROS_FATAL("Could not execute query because program is out of memory");
        exit(1);
    }

    bool preparationFailed = mysql_stmt_prepare(stmt, queryTemplate.c_str(), queryTemplate.length());
    if(preparationFailed)
    {
        ROS_FATAL("Query preparation failed");
        exit(1);
    }

    bool parameterBindingFailed = mysql_stmt_bind_param(stmt, (MYSQL_BIND*)(&parameters[0]));
    if(parameterBindingFailed)
    {
        ROS_FATAL("Failed to bind query parameter");
        exit(1);
    }

    bool queryExecutionFailed = mysql_stmt_execute(stmt);
    if(queryExecutionFailed)
    {
        ROS_FATAL("Query execution failed");
        exit(1);
    }

    bool closureFailed = mysql_stmt_close(stmt);
    if(closureFailed)
    {
        ROS_FATAL("Could not close the stms after query");
        exit(1);
    }
}

void Persistor::fetchQueryResult()
{
    MYSQL_RES* resultHandle = mysql_store_result(database);
    int nbRows = mysql_num_rows(resultHandle);
    int nbFields = mysql_num_fields(resultHandle);

    std::vector<std::map<std::string, std::string>> result;

    std::vector<std::string> fieldNames;

    MYSQL_FIELD* fields = mysql_fetch_fields(resultHandle);

    for(int i = 0; i < nbFields; ++i)
    {
        char* fieldName = fields[i].name;
        ROS_INFO("Field: %s", fieldName);
    }

    MYSQL_ROW row;
    while(row = mysql_fetch_row(resultHandle))
    {

    }

    mysql_free_result(resultHandle);
}

void Persistor::writeFile(const std::string& name, const std::string& content)
{
    std::ofstream file(name);
    file << content;
    file.close();
}

MYSQL_BIND Persistor::makeMySqlStringParameter(const std::string& value, long unsigned int& count)
{
    MYSQL_BIND parameter;
    
    parameter.buffer_type = MYSQL_TYPE_STRING;
    parameter.buffer = (char*) value.c_str();
    parameter.length = &count;
    parameter.is_null = 0;

    return parameter;
}
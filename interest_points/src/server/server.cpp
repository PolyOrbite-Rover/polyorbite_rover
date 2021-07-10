#include "interest_points/server.h"

Server::Server(const ros::NodeHandle& node)
{

}

void Server::initialize()
{
    service = node.advertiseService("interest_points/query/all", &Server::handleQuery, this);
}

bool Server::handleQuery(
    interest_points::InterestPointsQueryRequest& request,
    interest_points::InterestPointsQueryResponse& response
) {
    bool isPagedQuery = request.pageSize >= 0;
    
    return true;
}
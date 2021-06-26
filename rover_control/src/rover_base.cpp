#include "rover_base/rover_hardware.h"
#include "controller_manager/controller_manager.h"
#include "ros/callback_queue.h"

#include <boost/chrono.hpp>

typedef boost::chrono::steady_clock time_source;

void executeControl(
    polyorbite_rover::RoverHardware& rover,
    controller_manager::ControllerManager &controllerManager,
    time_source::time_point &previousTime
) {
    time_source::time_point currentTime = time_source::now();
    boost::chrono::duration<double> elapsedDuration = currentTime - previousTime;
    ros::Duration elapsed(elapsedDuration.count());
    previousTime = currentTime;

    rover.updateJointsFromHardware();
    controllerManager.update(ros::Time::now(), elapsed);
    rover.writeCommandsToHardware();
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "rover_base");

    ros::NodeHandle nodeHandle;
    ros::NodeHandle privateNodeHandle("~");

    double controlFrequency;
    privateNodeHandle.param<double>("control_frequency", controlFrequency, 10.0);

    polyorbite_rover::RoverHardware rover(nodeHandle, privateNodeHandle, controlFrequency);
    controller_manager::ControllerManager controllerManager(&rover, nodeHandle);

    ros::CallbackQueue executionQueue;
    ros::AsyncSpinner executionSpinner(1, &executionQueue);

    time_source::time_point previousTime = time_source::now();
    ros::TimerOptions controlTimer(
        ros::Duration(1 / controlFrequency),
        boost::bind(
            executeControl,
            boost::ref(rover),
            boost::ref(controllerManager),
            boost::ref(previousTime)
        ),
        &executionQueue
    );

    ros::Timer controlLoop = nodeHandle.createTimer(controlTimer);

    executionSpinner.start();

    ros::spin();

    return 0;
}
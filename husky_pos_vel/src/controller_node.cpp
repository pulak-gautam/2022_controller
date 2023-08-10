#include "controller.hpp"

int main(int argc, char **argv){
    ros::init(argc, argv, "controller_node");
    ros::NodeHandle nh;
    ros::Rate r(10);
    
    ROS_INFO("controller_node started");
    controller::Controller controller(nh, r, "/husky/reference_pos", "/husky_velocity_controller/odom", "/husky_velocity_controller/cmd_vel");

    controller.run();

    return 0;
    
}
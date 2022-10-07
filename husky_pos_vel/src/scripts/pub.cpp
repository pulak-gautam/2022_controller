#include <ros/ros.h>
#include <husky_pos_vel/Position.h>
#include <string>

int main(int argc, char **argv){
    ros::init(argc, argv, "pub"); 
    int x = 10, y = 12;

    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<husky_pos_vel::Position>("/husky/reference_pos", 100);

    ros::Rate r(10);

    while(ros::ok()){
        husky_pos_vel::Position msg;

        nh.getParam("/pub/x", x);
        nh.getParam("/pub/y", y);

        msg.x = x;
        msg.y = y;

        pub.publish(msg);
        r.sleep();
    }
    
    return 0;
}
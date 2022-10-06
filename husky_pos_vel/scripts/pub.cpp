#include <ros/ros.h>
#include <husky_pos_vel/Position.h>

int main(int argc, char **argv){
    ros::init(argc, argv, "pub"); 
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<husky_pos_vel::Position>("/husky/reference_pos", 100);

    ros::Rate r(10);

    while(ros::ok()){
        husky_pos_vel::Position msg;
        msg.x = 4;
        msg.y = 5;
        
        pub.publish(msg);

        r.sleep();
    }
    
    return 0;
}
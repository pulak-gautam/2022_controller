#include "publisher.hpp"

namespace publisher {
    Publisher::Publisher(ros::NodeHandle nh, ros::Rate r) : nh(nh), r(r) {
        pub = nh.advertise<husky_pos_vel::Position>("/husky/reference_pos", 100);
    }

    void Publisher::run() {
        int x, y;
        nh.getParam("/publisher_node/x", x);
        nh.getParam("/publisher_node/y", y);
        // ROS_INFO("x: %d, y: %d", x, y); // debug line
        msg.x = x;
        msg.y = y;

        pub.publish(msg);
        r.sleep();
    }
}
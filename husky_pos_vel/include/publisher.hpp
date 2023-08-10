#pragma once

#include <ros/ros.h>
#include <husky_pos_vel/Position.h>
#include <string>

namespace publisher {
    class Publisher {
        public:
            Publisher(ros::NodeHandle nh, ros::Rate r); // constructor
            ~Publisher(){}; // destructor
            void run();
        private:
            ros::NodeHandle nh;
            ros::Publisher pub;
            ros::Rate r;
            husky_pos_vel::Position msg;
            int x, y;
    };
}

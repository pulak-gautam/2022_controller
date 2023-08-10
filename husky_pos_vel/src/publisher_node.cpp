#include "publisher.hpp"

int main(int argc, char **argv){
    ros::init(argc, argv, "publisher_node"); 
    ros::NodeHandle nh;
    ros:: Rate r(10);

    publisher::Publisher pub(nh, r);
    while(ros::ok()){
        pub.run();
    }
    
    return 0;
}
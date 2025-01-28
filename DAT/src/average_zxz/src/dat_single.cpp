/**
 * @file offb_node.cpp
 * @brief Offboard control example node, written with MAVROS version 0.19.x, PX4 Pro Flight
 * Stack and tested in Gazebo SITL
 */

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <fstream>
#include <string>
#include <vector>

// osqp-eigen
#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Dense>

#include "DATnode.h"




int main(int argc, char **argv)
{
    ros::init(argc, argv, "dat_main");
    ros::NodeHandle nh;
    int UAVID;


    nh.param<int>("/dat_main/ID", UAVID,0);
    ROS_ERROR("I am %d",UAVID);

    DATnode node(0);

    //new std::thread(&UdpDATServer,"192.168.1.120",12001,UAVID);
    //new std::thread(&UdpDATListen,12001);


    //new std::thread(&dynamics_local);
    //new std::thread(&printer);


    ros::Rate rate(20.0);

    while(ros::ok()&&!node.endflag){

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}



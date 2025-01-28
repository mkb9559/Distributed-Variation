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
    ros::init(argc, argv, "dat_sim");
    ros::NodeHandle nh;

    DATnode node0(0);
    DATnode node1(1);
    DATnode node2(2);
    DATnode node3(3);


    ros::Rate rate(20.0);

    while(ros::ok()&&!node0.endflag){

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}



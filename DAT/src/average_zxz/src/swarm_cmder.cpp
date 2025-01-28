/**
 * @file offb_node.cpp
 * @brief Offboard control example node, written with MAVROS version 0.19.x, PX4 Pro Flight
 * Stack and tested in Gazebo SITL
 */

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
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
#include <mutex>


#include "UDPdata.h"
#include "Point3D.h"

#define simmode

#define num 4

static geometry_msgs::Point p_aim;
static geometry_msgs::Point p_ref;
static geometry_msgs::Point dp_ref;
static geometry_msgs::Point u_ref;

static geometry_msgs::Pose pos[num];
static geometry_msgs::Point opt[num];
static geometry_msgs::Point filter_p[num];
static geometry_msgs::Point filter_dp[num];
static geometry_msgs::Point filter_ddp[num];

static geometry_msgs::Point obs[2];

ros::Time time_ms;

std::mutex data_lock;

static int cmdd=0;

void Add_three(char* seq,double tpa,double tpb,double tpc)
{
    int sdlen=int(strlen(seq));
    sprintf(seq+sdlen,",%.6lf,%.6lf,%.6lf",tpa,tpb,tpc);
    sdlen=int(strlen(seq));
    seq[sdlen+1] = '\0';
}
void printerdata()
{
    time_t tt = time(NULL);
    tm* t=localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    std::string fileName = "/home/zxz/DAT/data/pose"+std::to_string(t->tm_year + 1900)
        +std::to_string(t->tm_mon + 1)
        +std::to_string(t->tm_mday)
        +std::to_string(t->tm_hour)
        +std::to_string(t->tm_min)
        +std::to_string(t->tm_sec)+".csv";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ofstream::out | std::ofstream::app);
    char logStr[256] = {0};
    int len;
    sprintf(logStr, "time, p0_x, p0_y,p0_z, p1_x,p1_y,p1_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,",p2_x, p2_y,p2_z, p3_x,p3_y,p3_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,",p4_x,p4_y,p4_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,"\n");
    //logStr[len+2] = '\0';
    myfile.write(logStr, len+1);

    while(cmdd!=9)
    {
        usleep(10000);
        data_lock.lock();
        sprintf(logStr, "%lf",ros::Time().now().toSec());

        Add_three(logStr,pos[0].position.x,pos[0].position.y,pos[0].position.z);
        Add_three(logStr,pos[1].position.x,pos[1].position.y,pos[1].position.z);
        Add_three(logStr,pos[2].position.x,pos[2].position.y,pos[2].position.z);
        Add_three(logStr,pos[3].position.x,pos[3].position.y,pos[3].position.z);
        data_lock.unlock();


        len=int(strlen(logStr));
        sprintf(logStr+len,"\n");
        myfile.write(logStr, len+1);
    }

}

void UdpCmdListen(const uint16_t cport)
{
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        ROS_ERROR("Network Error");
        return;
    }

    /* 将套接字和IP、端口绑定 */
    struct sockaddr_in addr_lis;
    int len;
    memset(&addr_lis, 0, sizeof(struct sockaddr_in));
    addr_lis.sin_family = AF_INET;
    addr_lis.sin_port = htons(cport);
    /* INADDR_ANY表示不管是哪个网卡接收到数据，只要目的端口是SERV_PORT，就会被该应用程序接收到 */
    addr_lis.sin_addr.s_addr = htonl(INADDR_ANY);  //自动获取IP地址
    len = sizeof(addr_lis);

    /* 绑定socket */
    if(bind(sock_fd, (struct sockaddr *)&addr_lis, sizeof(addr_lis)) < 0)
    {
      perror("bind error:");
      exit(1);
    }

    int recv_num;
    struct sockaddr_in addr_client;

    int needRecv = sizeof (Cmddata);
    char *recv_buf=(char*)malloc(needRecv);

    Cmddata* data_tmp=(Cmddata*)malloc(needRecv);

    while(ros::ok()){
        recv_num = recvfrom(sock_fd, recv_buf, needRecv, 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

        memcpy(data_tmp,recv_buf,needRecv);
        data_lock.lock();
        filter_p[data_tmp->mid].x   =  data_tmp->filter_p_x;
        filter_p[data_tmp->mid].y   =  data_tmp->filter_p_y;
        filter_p[data_tmp->mid].z   =  data_tmp->filter_p_z;
        filter_dp[data_tmp->mid].x  =  data_tmp->filter_dp_x;
        filter_dp[data_tmp->mid].y  =  data_tmp->filter_dp_y;
        filter_dp[data_tmp->mid].z  =  data_tmp->filter_dp_z;
        filter_ddp[data_tmp->mid].x =  data_tmp->filter_ddp_x;
        filter_ddp[data_tmp->mid].y =  data_tmp->filter_ddp_y;
        filter_ddp[data_tmp->mid].z =  data_tmp->filter_ddp_z;
        data_lock.unlock();

        //ROS_ERROR("Rec: %s, len=%d",recv_buf,recv_num);

    }
}
Point3D ExtSignal(int id)
{
    Point3D ans = Point3D(0,0,0);
    Point3D tmp_ans;
    double tmp_pt;
    Point3D pi  = Point3D(pos[id].position.x,pos[id].position.y,pos[id].position.z);
    double f=0;
    for (int i=0;i<2;i++)
    {
        Point3D vec = Point3D(pi.x - obs->x, pi.y - obs->y,0);
        if(vec.norm1()<obs[i].z)
        {
            tmp_pt  = 1/vec.norm1()-1/obs[i].z;
            tmp_ans = (vec/vec.norm1())*(tmp_pt>10?10:tmp_pt);
            ans = ans + tmp_ans;
        }
    }
    return ans;
}

void UdpRefServer(const char* ip,const uint16_t cport,const int UAVID)
{
    ROS_ERROR("UDP %d sender on.",UAVID);
    std::string ss;
    geometry_msgs::PoseStamped offset;
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        ROS_ERROR("Network Error");
        return;
    }
    struct sockaddr_in addr_client;
    int len;
    memset(&addr_client, 0, sizeof(struct sockaddr_in));
    addr_client.sin_family = AF_INET;
    addr_client.sin_addr.s_addr = inet_addr(ip);
    addr_client.sin_port = htons(cport);
    len = sizeof(addr_client);


    int send_num;
    ros::Rate rate(100.0);

    char* send_buf;

    Refdata send_tmp;
    Point3D tmp_opt;

    while(ros::ok())
    {
        data_lock.lock();
        send_tmp.cmdd = cmdd;
        send_tmp.rostime = (ros::Time().now()-time_ms).toSec();
        send_tmp.p_ref_x   = p_ref.x;  send_tmp.p_ref_y   = p_ref.y;  send_tmp.p_ref_z   = p_ref.z;
        send_tmp.dp_ref_x  = dp_ref.x; send_tmp.dp_ref_y  = dp_ref.y; send_tmp.dp_ref_z  = dp_ref.z;
        send_tmp.ddp_ref_x = u_ref.x;  send_tmp.ddp_ref_y = u_ref.y;  send_tmp.ddp_ref_z = u_ref.z;
        tmp_opt = ExtSignal(UAVID);
        /*
        send_tmp.opt_x = tmp_opt.x;
        send_tmp.opt_y = tmp_opt.y;
        send_tmp.opt_z = tmp_opt.z;
        */
        send_tmp.opt_x = -5+rand()%10;
        send_tmp.opt_y = -5+rand()%10;
        send_tmp.opt_z = 0;
        data_lock.unlock();

        send_buf = (char *) &send_tmp;
        send_num = sendto(sock_fd,&send_tmp,sizeof (send_tmp),0,(struct sockaddr *)&addr_client,len);

        if(send_num < 0)
        {
            ROS_ERROR("Send Fail!, UAV = %d",UAVID);
            //perror("sendto error:");
            //exit(1);
        }
        //ROS_INFO("Current Pub: %s",send_buf);
        //std::cout<<ssize_t(strlen(send_buf))<<std::endl;
        rate.sleep();
    }

}


void PrintInfo(){
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>--------<<<<<<<<<<<<<<<<<<<<<<<<<<< "<< std::endl;
    std::cout << " 1 2 for Mission start"<< std::endl;
    std::cout << " 9 for end"<< std::endl;
    std::cin>> cmdd;
    switch(cmdd){
        case  1:ROS_ERROR("Mission start!");break;
        case  2:ROS_ERROR("Mission0 start!");break;
        case  9:ROS_ERROR("Mission Kill!");break;
    }
}

inline double satfuc(double v,double bd)
{
    if(v>bd)return bd;
    else if(v<-bd) return -bd;
    else return v;
}
void tracking_dynamics()
{
    static double v_d = 0.3;
    static double a_d = 0.1;
    static double k1  = 1;
    static double k2  = 10;
    geometry_msgs::Point v_tmp;
    ros::Time time_start,time_end;
    while(ros::ok()){
        if(cmdd==0||cmdd==2)continue;
        time_start = ros::Time().now();
        usleep(10000);

        data_lock.lock();

        v_tmp.x = satfuc(k1*(p_aim.x-p_ref.x),v_d);
        v_tmp.y = satfuc(k1*(p_aim.y-p_ref.y),v_d);
        v_tmp.z = satfuc(k1*(p_aim.z-p_ref.z),v_d);

        u_ref.x = satfuc(k2*(v_tmp.x-dp_ref.x),a_d);
        u_ref.y = satfuc(k2*(v_tmp.y-dp_ref.y),a_d);
        u_ref.z = satfuc(k2*(v_tmp.z-dp_ref.z),a_d);
        time_end = ros::Time().now();

        dp_ref.x = dp_ref.x+u_ref.x*(time_end-time_start).toSec();
        dp_ref.y = dp_ref.y+u_ref.y*(time_end-time_start).toSec();
        dp_ref.z = dp_ref.z+u_ref.z*(time_end-time_start).toSec();

        p_ref.x = p_ref.x+dp_ref.x*(time_end-time_start).toSec();
        p_ref.y = p_ref.y+dp_ref.y*(time_end-time_start).toSec();
        p_ref.z = p_ref.z+dp_ref.z*(time_end-time_start).toSec();
        //std::cout<<"pos:"<<p_ref.x<<","<<p_ref.y<<","<<p_ref.z<<std::endl;

        data_lock.unlock();
    }

}
void inits()
{
    // aim position
    p_aim.x = 3;
    p_aim.y = 1;
    p_aim.z = 1.2;
    // initial position
    p_ref.x = -2.2;
    p_ref.y = 1;
    p_ref.z = p_aim.z;

    dp_ref.x = 0;dp_ref.y = 0;dp_ref.z = 0;
    u_ref.x  = 0;u_ref.y  = 0;u_ref.z  = 0;

    obs[0].x = 0; obs[0].y = 0; obs[0].z = 0;
    obs[1].x = 0; obs[1].y = 0; obs[1].z = 0;



}

int main(int argc, char **argv){
    ros::init(argc, argv, "swarm_cmd");
    ros::NodeHandle nh;
    ros::Rate rate(20.0);

    inits();
    time_ms = ros::Time().now();
    new std::thread(&tracking_dynamics);
    //new std::thread(&printer);

#ifdef simmode
/*
    new std::thread(&UdpRefServer,"127.0.0.1",17801,0);
    new std::thread(&UdpRefServer,"127.0.0.1",17802,1);
    new std::thread(&UdpRefServer,"127.0.0.1",17803,2);
    new std::thread(&UdpRefServer,"127.0.0.1",17804,3);
    new std::thread(&UdpRefServer,"127.0.0.1",17805,4);
*/

    new std::thread(&UdpRefServer,"192.168.1.41",17801,0);
    new std::thread(&UdpRefServer,"192.168.1.41",17802,1);
    new std::thread(&UdpRefServer,"192.168.1.41",17803,2);
    new std::thread(&UdpRefServer,"192.168.1.41",17804,3);

    new std::thread(&UdpCmdListen,16800);

    new std::thread(&printerdata);
    new std::thread(&PrintInfo);

#else
    new std::thread(&UdpRefServer,"192.168.1.30",17800,0);
    new std::thread(&UdpRefServer,"192.168.1.31",17800,1);
    new std::thread(&UdpRefServer,"192.168.1.32",17800,2);
    new std::thread(&UdpRefServer,"192.168.1.33",17800,3);
    new std::thread(&UdpRefServer,"192.168.1.34",17800,4);

    new std::thread(&UdpCmdListen,16800);
#endif

    while(ros::ok()){
        if(cmdd==9)
        {
            sleep(3);
            break;
        }
        ros::spinOnce();
        rate.sleep();

    }

    return 0;
}



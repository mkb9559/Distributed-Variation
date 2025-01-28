#include "DATnode.h"


void DATnode::UdpDATServer()
{
    std::string ss;
    geometry_msgs::PoseStamped offset;
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0)
    {
        ROS_ERROR("Network Error");
        return;
    }

    struct sockaddr_in addr_client[num];
    int len[num];
    for(int i=0;i<num;i++)
    {
        memset(&addr_client[i], 0, sizeof(struct sockaddr_in));
        addr_client[i].sin_family = AF_INET;
        addr_client[i].sin_addr.s_addr = inet_addr(networks[i].uip);
        addr_client[i].sin_port = htons(networks[i].uport);
        len[i] = sizeof(addr_client[i]);
    }

    int send_num;
    char* send_buf;

    DATdata send_tmp;
    send_tmp.mid = mid;
    while(!endflag)
    {
        //usleep(10000);
        usleep(1000);
        data_lock.lock();
        send_tmp.kappa   = local_kappa[mid];
        send_tmp.d_kappa = d_local_kappa[mid];
        send_tmp.q_x     = local_q[mid].x;
        send_tmp.q_y     = local_q[mid].y;
        send_tmp.d_q_x   = d_local_q[mid].x;
        send_tmp.d_q_y   = d_local_q[mid].y;
        data_lock.unlock();

        send_buf = (char *) &send_tmp;

        for(int i=0;i<num;i++)
        {
            if(i==mid||!adj[mid][i])continue;
            //send_num = sendto(sock_fd, send_buf, ssize_t(strlen(send_buf)), 0, (struct sockaddr *)&addr_client[i], len[i]);
            send_num = sendto(sock_fd,&send_tmp,sizeof (send_tmp),0,(struct sockaddr *)&addr_client[i],len[i]);
            if(send_num < 0)
            {
                ROS_ERROR("Send Fail!, IP: %s",networks[i].uip);
                //perror("sendto error:");
                //exit(1);
            }
            //ROS_ERROR("Sent to: %s",send_buf);
        }

    }
}

void DATnode::UdpCmdServer()
{
    ROS_ERROR("UDP %d",mid);
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
    addr_client.sin_addr.s_addr = inet_addr(netcen.uip);
    addr_client.sin_port = htons(netcen.uport);
    len = sizeof(addr_client);

    int send_num;
    char* send_buf;

    Cmddata send_tmp;
    send_tmp.mid = mid;

    while(!endflag)
    {
        usleep(100000);

        data_lock.lock();
        send_tmp.filter_p_x   = filter_desired_p.x;
        send_tmp.filter_p_y   = filter_desired_p.y;
        send_tmp.filter_p_z   = filter_desired_p.z;
        send_tmp.filter_dp_x  = filter_desired_d_p.x;
        send_tmp.filter_dp_y  = filter_desired_d_p.y;
        send_tmp.filter_dp_z  = filter_desired_d_p.z;
        send_tmp.filter_ddp_x = filter_desired_dd_p.x;
        send_tmp.filter_ddp_y = filter_desired_dd_p.y;
        send_tmp.filter_ddp_z = filter_desired_dd_p.z;
        data_lock.unlock();


        send_buf = (char *) &send_tmp;
        send_num = sendto(sock_fd,&send_tmp,sizeof (send_tmp),0,(struct sockaddr *)&addr_client,len);

    }

}
void DATnode::UdpDATListen()
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
    addr_lis.sin_port = htons(networks[mid].uport);
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

    int needRecv = sizeof (DATdata);
    char *recv_buf=(char*)malloc(needRecv);

    DATdata* data_tmp=(DATdata*)malloc(needRecv);

    while(!endflag)
    {

        recv_num = recvfrom(sock_fd, recv_buf, needRecv, 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

        memcpy(data_tmp,recv_buf,needRecv);
        data_lock.lock();
        local_kappa[data_tmp->mid]   = data_tmp->kappa;
        d_local_kappa[data_tmp->mid] = data_tmp->d_kappa;
        local_q[data_tmp->mid].x     = data_tmp->q_x;
        d_local_q[data_tmp->mid].x   = data_tmp->d_q_x;
        local_q[data_tmp->mid].y     = data_tmp->q_y;
        d_local_q[data_tmp->mid].y   = data_tmp->d_q_y;
        data_lock.unlock();
    }
}

void DATnode::UdpRefListen()
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
    addr_lis.sin_port = htons(networks[mid].refport);
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

    int needRecv = sizeof (Refdata);
    char *recv_buf=(char*)malloc(needRecv);

    Refdata* data_tmp=(Refdata*)malloc(needRecv);

    while(!endflag)
    {
        recv_num = recvfrom(sock_fd, recv_buf, needRecv, 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

        memcpy(data_tmp,recv_buf,needRecv);

        if(data_tmp->cmdd==9)endflag = true;
        data_lock.lock();
        mission_time = data_tmp->rostime;
        p_ref    = Point3D(data_tmp->p_ref_x,   data_tmp->p_ref_y,   data_tmp->p_ref_z);
        d_p_ref  = Point3D(data_tmp->dp_ref_x,  data_tmp->dp_ref_y,  data_tmp->dp_ref_z);
        dd_p_ref = Point3D(data_tmp->ddp_ref_x, data_tmp->ddp_ref_y, data_tmp->ddp_ref_z);
        optf     = Point3D(data_tmp->opt_x,     data_tmp->opt_y,     data_tmp->opt_z);
        data_lock.unlock();

    }

}





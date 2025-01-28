#include "DATnode.h"


void DATnode::dynamics_local()
{
    ros::Time time_start,time_end;
    double Ts;
    //sleep(2);
    //for(int i=1;i<10;i++)
    while(!endflag)
    {
        time_start = ros::Time().now();
        usleep(1000);

        data_lock.lock();
        //optf=Point3D(pow(-1,mid)*2*mid,0,0);

        //std::cout<<"time:"<<(time_end-time_start).toSec()<<"s"<<std::endl;

        //hat_kappa_u = NorminalControl_kappa(dd_hat_kappa,d_hat_kappa,hat_kappa,1,0);
        //hat_kappa_u = CBFmain_kappa(dd_hat_kappa,d_hat_kappa,hat_kappa,0);
        ROS_ERROR("optf:%lf,%lf,%lf",optf.x,optf.y,optf.z);
        hat_kappa_u = CBFmain_kappa(dd_hat_kappa,d_hat_kappa,hat_kappa,optf2optkappa(optf));
        hat_q_u     = CBFmain_q_2D(dd_hat_q,d_hat_q,hat_q,dd_p_ref,d_p_ref,p_ref,optf);

        //hat_q_u     = CBFmain_q_2D(dd_hat_q,d_hat_q,hat_q,dd_p_ref,d_p_ref,p_ref,Point3D(10,0,0));

        //mission_time = ros::Time().now().toSec();
        time_end   = ros::Time().now();
        Ts         = (time_end-time_start).toSec();


        local_kappa_u = DATmain_kappa(dd_hat_kappa,d_hat_kappa,hat_kappa);
        local_q_u     = DATmain_q(dd_hat_q,d_hat_q,hat_q);

        desired_p    = local_q[mid] + norminal_formation[mid]*local_kappa[mid];
        desired_d_p  = d_local_q[mid] + norminal_formation[mid]*d_local_kappa[mid];
        //desired_dd_p = dd_p_ref + local_q_u + norminal_formation[mid]*local_kappa_u;
        desired_dd_p = dd_p_ref;

        filter_desired_dd_p = desired_dd_p+(desired_p-filter_desired_p)*filter_kp-filter_desired_d_p*filter_kd;

        //Euler method for integral
        dd_hat_kappa = dd_hat_kappa + hat_kappa_u*Ts;
        d_hat_kappa  = d_hat_kappa  + dd_hat_kappa*Ts;
        hat_kappa    = hat_kappa    + d_hat_kappa*Ts;

        hat_q     = hat_q    + d_hat_q*Ts;
        d_hat_q   = d_hat_q  + dd_hat_q*Ts;
        dd_hat_q  = dd_hat_q + hat_q_u*Ts;

        local_kappa[mid]   = local_kappa[mid]   + d_local_kappa[mid]*Ts;
        d_local_kappa[mid] = d_local_kappa[mid] + local_kappa_u*Ts;

        local_q[mid]   = local_q[mid]    + d_local_q[mid]*Ts;
        d_local_q[mid] = d_local_q[mid]  + local_q_u*Ts;

        filter_desired_p   = filter_desired_p + filter_desired_d_p*Ts;
        filter_desired_d_p = filter_desired_d_p + filter_desired_dd_p*Ts;

        data_lock.unlock();

        //std::cout<<"kappa:"<<dd_hat_kappa<<","<<d_hat_kappa<<","<<hat_kappa<<std::endl;

    }
}

double DATnode::optf2optkappa(Point3D f)
{
    double ans,tmp;
    int cnt=0;
    ans = 0;
    if(f.norm2()<0.1)return 0;
    else
    {
        Point3D rjid;
        for (int j=0;j<num;j++)
        {
            if(mid!=j && adj[mid][j]==true)
            {
                rjid = norminal_formation[j]-norminal_formation[mid];
                tmp  = ((f*rjid)/rjid.norm1());
                if(tmp>0)
                {
                    ans = ans + tmp/rjid.norm1();
                    cnt = cnt +1;
                }
            }
        }
        if(cnt>0) return -ans/((double)cnt);
        else return 0;
    }

}
void DATnode::printer()
{
    time_t tt = time(NULL);
    tm* t=localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    std::string fileName = "/home/rock/DAT/data/"+std::to_string(mid)+"-robot"+std::to_string(t->tm_year + 1900)
        +std::to_string(t->tm_mon + 1)
        +std::to_string(t->tm_mday)
        +std::to_string(t->tm_hour)
        +std::to_string(t->tm_min)
        +std::to_string(t->tm_sec)+".csv";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ofstream::out | std::ofstream::app);
    char logStr[512] = {0};
    int len;
    sprintf(logStr, "time, hat_kappa, d_hat_kappa, dd_hat_kappa");
    len=int(strlen(logStr));
    sprintf(logStr+len,",hat_q_x,         d_hat_q_x,     dd_hat_q_x");
    len=int(strlen(logStr));
    sprintf(logStr+len,",hat_q_y,         d_hat_q_y,     dd_hat_q_y");
    len=int(strlen(logStr));
    sprintf(logStr+len,",hat_kappa_u,     hat_q_u_x,     hat_q_u_y");
    len=int(strlen(logStr));
    sprintf(logStr+len,",hat_kappa_u_ref, hat_q_u_ref_x, hat_q_u_ref_y");
    len=int(strlen(logStr));
    sprintf(logStr+len,",h_1_kappa,       h_2_kappa,     h_3_q");
    len=int(strlen(logStr));
    sprintf(logStr+len,",local_kappa,     d_local_kappa, local_kappa_u");
    len=int(strlen(logStr));
    sprintf(logStr+len,",local_q_x,       d_local_q_x,   local_q_u_x");
    len=int(strlen(logStr));
    sprintf(logStr+len,",local_q_y,       d_local_q_y,   local_q_u_y");
    len=int(strlen(logStr));
    sprintf(logStr+len,",desired_p_x,     desired_p_y,   desired_p_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,",filter_p_x,      filter_p_y,    filter_p_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,",filter_dp_x,     filter_dp_y,   filter_dp_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,",pref_x,          pref_y,        pref_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,",opt_x,           opt_y,         opt_z");
    len=int(strlen(logStr));
    sprintf(logStr+len,"\n");
    //logStr[len+2] = '\0';
    myfile.write(logStr, len+1);

    while(!endflag)
    {
        usleep(10000);
        data_lock.lock();
        sprintf(logStr, "%lf",mission_time);
        Add_three(logStr,hat_kappa,          d_hat_kappa,         dd_hat_kappa);
        Add_three(logStr,hat_q.x,            d_hat_q.x,            dd_hat_q.x);
        Add_three(logStr,hat_q.y,            d_hat_q.y,            dd_hat_q.y);
        Add_three(logStr,hat_kappa_u,        hat_q_u.x,            hat_q_u.y);
        Add_three(logStr,hat_kappa_u_ref,    hat_q_u_ref.x,        hat_q_u_ref.y);
        Add_three(logStr,h_1_kappa,          h_2_kappa,            h_3_q);
        Add_three(logStr,local_kappa[mid],   d_local_kappa[mid],   local_kappa_u);
        Add_three(logStr,local_q[mid].x,     d_local_q[mid].x,     local_q_u.x);
        Add_three(logStr,local_q[mid].y,     d_local_q[mid].y,     local_q_u.y);
        Add_three(logStr,desired_p.x,        desired_p.y,          desired_p.z);
        Add_three(logStr,filter_desired_p.x, filter_desired_p.y,   filter_desired_p.z);
        Add_three(logStr,filter_desired_d_p.x,filter_desired_d_p.y,filter_desired_d_p.z);
        Add_three(logStr,p_ref.x,            p_ref.y,              p_ref.z);
        Add_three(logStr,optf.x,             optf.y,               optf.z);
        data_lock.unlock();

        len=int(strlen(logStr));
        sprintf(logStr+len,"\n");
        myfile.write(logStr, len+1);
    }

}
void DATnode::printerDAT()
{
    time_t tt = time(NULL);
    tm* t=localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    std::string fileName = "/home/rock/DAT/data/"+std::to_string(mid)+"-DAT"+std::to_string(t->tm_year + 1900)
        +std::to_string(t->tm_mon + 1)
        +std::to_string(t->tm_mday)
        +std::to_string(t->tm_hour)
        +std::to_string(t->tm_min)
        +std::to_string(t->tm_sec)+".csv";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ofstream::out | std::ofstream::app);
    char logStr[256] = {0};
    int len;
    sprintf(logStr, "time, local_kappa0, local_kappa1,local_kappa2, local_kappa3");
    len=int(strlen(logStr));
    sprintf(logStr+len,",d_local_kappa0, d_local_kappa1,d_local_kappa2,d_local_kappa3");
    len=int(strlen(logStr));
    sprintf(logStr+len,",null1");
    len=int(strlen(logStr));
    sprintf(logStr+len,"\n");
    //logStr[len+2] = '\0';
    myfile.write(logStr, len+1);

    while(!endflag)
    {
        usleep(10000);
        data_lock.lock();
        sprintf(logStr, "%lf",mission_time);
        /*
        Add_three(logStr,local_kappa[0], local_kappa[1], local_kappa[2]);
        Add_three(logStr,local_kappa[3], local_kappa[4], d_local_kappa[0]);
        Add_three(logStr,d_local_kappa[1], d_local_kappa[2], d_local_kappa[3]);
        Add_three(logStr,d_local_kappa[4], 0.0, 0.0);
*/
        Add_three(logStr,local_q[0].x, local_q[1].x, local_q[2].x);
        Add_three(logStr,local_q[3].x, d_local_q[0].x,d_local_q[1].x);
        Add_three(logStr,d_local_q[2].x, d_local_q[3].x, 0.0);
        data_lock.unlock();


        len=int(strlen(logStr));
        sprintf(logStr+len,"\n");
        myfile.write(logStr, len+1);
    }

}
void DATnode::Add_three(char* seq,double tpa,double tpb,double tpc)
{
    int sdlen=int(strlen(seq));
    sprintf(seq+sdlen,",%.6lf,%.6lf,%.6lf",tpa,tpb,tpc);
    sdlen=int(strlen(seq));
    seq[sdlen+1] = '\0';
}
void DATnode::Add_three(char* seq,Point3D p)
{
    int sdlen=int(strlen(seq));
    sprintf(seq+sdlen,",%.6lf,%.6lf,%.6lf",p.x,p.y,p.z);
    sdlen=int(strlen(seq));
    seq[sdlen+1] = '\0';
}

double DATnode::filter_pos(double a,double b,double f)
{
    return fabs(b-a)>f?a+f*signed(b-a):b;
}
Point3D DATnode::filter_pos(Point3D a,Point3D b,double f)
{
    return Point3D(filter_pos(a.x,b.x,f),
                   filter_pos(a.y,b.y,f),
                   filter_pos(a.z,b.z,f));
}




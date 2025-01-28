#include "DATnode.h"


const bool DATnode::adj[num][num]={{ 0, 1, 0, 1},
                                   { 1, 0, 1, 0},
                                   { 0, 1, 0, 1},
                                   { 1, 0, 1, 0}};
//for sim

const Point3D DATnode::ref0 = Point3D(-2.7,1,1.2);
/*
const Netzxz DATnode::networks[num]={Netzxz("127.0.1.1",16801,17801),
                                     Netzxz("127.0.1.1",16802,17802),
                                     Netzxz("127.0.1.1",16803,17803),
                                     Netzxz("127.0.1.1",16804,17804),
                                     Netzxz("127.0.1.1",16805,17805)};

const Netzxz DATnode::networks[num]={Netzxz("192.168.1.41",16801,17801),
                                     Netzxz("192.168.1.41",16802,17802),
                                     Netzxz("192.168.1.41",16803,17803),
                                     Netzxz("192.168.1.41",16804,17804)};
                                     */
const Netzxz DATnode::networks[num]={Netzxz("192.168.1.30",16801,17801),
                                     Netzxz("192.168.1.31",16802,17802),
                                     Netzxz("192.168.1.32",16803,17803),
                                     Netzxz("192.168.1.33",16804,17804)};
//const Netzxz DATnode::netcen = Netzxz("192.168.1.41",16800,17800);
const Netzxz DATnode::netcen = Netzxz("192.168.1.28",16800,17800);




const double DATnode::ak      = 2.8363;
const double DATnode::bk      = 2.0111;
const double DATnode::delta1  = 1.5;
const double DATnode::delta2  = 1.4087;
const double DATnode::delta3  = 1.4276;

const double DATnode::k1_kappa = 10;
const double DATnode::kk_kappa = 100;

const double DATnode::k1_q = 10;
const double DATnode::kk_q = 5;

const double DATnode::gamma_q = 1.0;
const double DATnode::miu_q   = 3.5;

const double DATnode::gamma_kappa = 1.0;
const double DATnode::kappa_ref   = 1.0;
const double DATnode::kappa_min   = 0.3;
const double DATnode::miu_kappa   = 1.5;

const double DATnode::fai_kappa   = 10;
const double DATnode::pi_kappa    = 5;
const double DATnode::fai_q       = 20;
const double DATnode::pi_q        = 10;

const double DATnode::filter_x    = 0.5;
const double DATnode::filter_y    = 0.5;
const double DATnode::filter_z    = 0.5;

const double DATnode::filter_kp    = 15;
const double DATnode::filter_kd    = 30;

DATnode::DATnode(int id)
{
    this->mid = id;
    this->endflag = false;
    this->mission_time = 0;
/*
    this->norminal_formation[0] = Point3D( 0.3,  0.5, 0);
    this->norminal_formation[1] = Point3D( 0.3, -0.5, 0);
    this->norminal_formation[2] = Point3D(-0.7, -0.5, 0);
    this->norminal_formation[3] = Point3D(-0.7,  0.5, 0);
    */
  /*
    this->norminal_formation[0] = Point3D( 0.6,  0, 0);
    this->norminal_formation[1] = Point3D( 0, -0.6, 0);
    this->norminal_formation[2] = Point3D(-0.6, 0, 0);
    this->norminal_formation[3] = Point3D(0,  0.6, 0);
    */
    this->norminal_formation[0] = Point3D( 0.6,0.6, 0);
    this->norminal_formation[1] = Point3D( 0.6,-0.6, 0);
    this->norminal_formation[2] = Point3D(-0.6,-0.6, 0);
    this->norminal_formation[3] = Point3D(-0.6, 0.6, 0);


    this->optf        = Point3D(0,0,0);

    /************************CBF_kappa**********************/
    this->hat_kappa_u     = 0;
    this->hat_kappa_u_ref = 0;
    this->dd_hat_kappa    = 0;
    this->d_hat_kappa     = 0;
    this->hat_kappa       = 1;


    this->h_1_kappa       = 0;
    this->h_2_kappa       = 0;

    /************************CBF_q**********************/
    this->p_ref    = ref0;
    this->d_p_ref  = Point3D(0,0,0);
    this->dd_p_ref = Point3D(0,0,0);

    this->hat_q_u     = Point3D(0,0,0);
    this->hat_q_u_ref = Point3D(0,0,0);
    this->hat_q       = this->norminal_formation[this->mid] + this->ref0;
    this->d_hat_q     = Point3D(0,0,0);
    this->dd_hat_q    = Point3D(0,0,0);


    this->desired_p    = this->norminal_formation[this->mid] + this->ref0;
    this->desired_d_p  = Point3D(0,0,0);
    this->desired_dd_p = Point3D(0,0,0);

    this->filter_desired_p    = this->norminal_formation[this->mid] + this->ref0;
    this->filter_desired_d_p  = Point3D(0,0,0);
    this->filter_desired_dd_p = Point3D(0,0,0);

    this->h_3_q       = 0;

    for (int i=0;i<num;i++)
    {
        this->local_q[i]       = this->norminal_formation[i] + this->ref0;
        this->d_local_q[i]     = Point3D(0,0,0);
        this->local_kappa[i]   = (double(rand()%100))/100.0;
        //this->local_kappa[i]   = 1;
        this->d_local_kappa[i] = 0;
    }


    this->local_q_u     = Point3D(0,0,0);
    this->local_kappa_u = 0;



    this->run();
}
void DATnode::run()
{

    new std::thread(&DATnode::dynamics_local,this);
    new std::thread(&DATnode::printer,this);
    new std::thread(&DATnode::printerDAT,this);

    new std::thread(&DATnode::UdpDATServer,this);
    new std::thread(&DATnode::UdpDATListen,this);
    new std::thread(&DATnode::UdpCmdServer,this);
    new std::thread(&DATnode::UdpRefListen,this);


}

DATnode::~DATnode()
{

}

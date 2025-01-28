#ifndef DATNODE_H_
#define DATNODE_H_

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
#include <mutex>

// osqp-eigen
#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Dense>

#include "Point3D.h"
#include "Netzxz.h"
#include "UDPdata.h"


#define num    4




class DATnode
{
public:
  DATnode(int id);
  ~DATnode();


  //for mission
  int mid;
  Point3D norminal_formation[num];
  static const bool adj[num][num];
  static const Netzxz networks[num];
  static const Netzxz netcen;
  static const Point3D ref0;

  bool endflag;
  double mission_time;


  //For obstacle information
  Point3D optf;



  //For DAT, from CBF

  std::mutex data_lock;

  double hat_kappa,    d_hat_kappa,     dd_hat_kappa;
  double hat_kappa_u,  hat_kappa_u_ref;
  double h_1_kappa,    h_2_kappa;

  Point3D p_ref, d_p_ref, dd_p_ref;

  Point3D hat_q,    d_hat_q,     dd_hat_q;
  Point3D hat_q_u, hat_q_u_ref;
  double h_3_q;

  static const double ak, bk;
  static const double delta1,   delta2,   delta3;
  static const double k1_kappa, kk_kappa, k1_q,   kk_q;
  static const double gamma_q,     miu_q;
  static const double gamma_kappa, miu_kappa;
  static const double kappa_ref,   kappa_min;

  static const double fai_kappa, pi_kappa;
  static const double fai_q,     pi_q;




  // For control, from DAT
  double local_kappa[num];
  double d_local_kappa[num];
  double local_kappa_u;



  Point3D local_q[num];
  Point3D d_local_q[num];
  Point3D local_q_u;



  // For formation control
  Point3D ref_p;
  Point3D ref_d_p;
  Point3D ref_dd_p;

  Point3D desired_p;
  Point3D desired_d_p;
  Point3D desired_dd_p;

  Point3D filter_desired_p;
  Point3D filter_desired_d_p;
  Point3D filter_desired_dd_p;

  static const double filter_x, filter_y ,filter_z;
  static const double filter_kp,     filter_kd;


  void run();

  void printer();
  void printerDAT();
  void dynamics_local();
  void UdpDATServer();
  void UdpCmdServer();
  void UdpDATListen();
  void UdpRefListen();



private:

  double optf2optkappa(Point3D op);

  double NorminalControl_kappa(double dd_est,double d_est,double est,double ref,double opt);
  double CBFmain_kappa(double dd_est,double d_est,double est, double opt);

  Point3D NorminalControl_q_2D(Point3D dd_est,Point3D d_est,Point3D est,
                           Point3D dd_ref,Point3D d_ref,Point3D ref,Point3D opt);
  Point3D CBFmain_q_2D(Point3D dd_est,Point3D d_est,Point3D est,
                   Point3D dd_ref,Point3D d_ref,Point3D ref,Point3D opt);

  double DATmain_kappa(double dd_est,double d_est,double est);
  Point3D DATmain_q(Point3D dd_est,Point3D d_est,Point3D est);

  void Add_three(char* seq,double tpa,double tpb,double tpc);
  void Add_three(char* seq,Point3D p);

  double msign(double a);
  Point3D msign(Point3D p);

  double filter_pos(double a,double b,double f);
  Point3D filter_pos(Point3D a,Point3D b,double f);




};


#endif

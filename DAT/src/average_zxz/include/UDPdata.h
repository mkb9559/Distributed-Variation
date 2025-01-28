#ifndef UDPdata_H_
#define UDPdata_H_


struct DATdata
{
    int mid;
    double kappa,d_kappa;
    double q_x,q_y,d_q_x,d_q_y;
};

struct Cmddata
{
    int mid;
    double filter_p_x,   filter_p_y,   filter_p_z;
    double filter_dp_x,  filter_dp_y,  filter_dp_z;
    double filter_ddp_x, filter_ddp_y, filter_ddp_z;
};

struct Refdata
{
    int cmdd;
    double rostime;
    double p_ref_x,   p_ref_y,   p_ref_z;
    double dp_ref_x,  dp_ref_y,  dp_ref_z;
    double ddp_ref_x, ddp_ref_y, ddp_ref_z;
    double opt_x,     opt_y,     opt_z;
};

#endif


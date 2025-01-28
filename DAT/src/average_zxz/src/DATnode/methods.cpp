#include "DATnode.h"




double DATnode::NorminalControl_kappa(double dd_est,double d_est,double est,double ref,double opt)
{
    double s=dd_est + ak*d_est + bk*(est-ref);
    return -k1_kappa*s - ak*dd_est - bk*d_est + kk_kappa*opt;
}

Point3D DATnode::NorminalControl_q_2D(Point3D dd_est,Point3D d_est,Point3D est,
                         Point3D dd_ref,Point3D d_ref,Point3D ref,Point3D opt)
{
    Point3D s,u_ref_tmp;
    s = dd_est+((d_est-d_ref)*ak)+((est-ref)*bk);
    //s.x = dd_est.x + ak*(d_est.x-d_ref.x) + bk*(est.x-ref.x);
    //s.y = dd_est.y + ak*(d_est.y-d_ref.y) + bk*(est.y-ref.y);
    //s.z = 0;
    u_ref_tmp = (s*(-k1_q)) - ((dd_est-dd_ref)*ak) - ((d_est-d_ref)*bk)+ (opt*kk_q);
    //u_ref.x = -k1*s.x - ak*(dd_est.x-dd_ref.x) - bk*(d_est.x-d_ref.x) + kk*opt.x;
    //u_ref.y = -k1*s.y - ak*(dd_est.y-dd_ref.y) - bk*(d_est.y-d_ref.y) + kk*opt.y;
    return u_ref_tmp;
}

Point3D DATnode::CBFmain_q_2D(Point3D dd_est,Point3D d_est,Point3D est,
                 Point3D dd_ref,Point3D d_ref,Point3D ref,Point3D opt)
{
    OsqpEigen::Solver           solver;
    Eigen::VectorXd             QPsolution;
    Eigen::SparseMatrix<double> Hessian;
    Eigen::VectorXd             gradient;
    Eigen::VectorXd             lowerbound;
    Eigen::VectorXd             upperbound;
    Eigen::SparseMatrix<double> linearmatrix;
    int numOfVar  = 4;
    int numofCons = 3; //1 CBF + 2 u_ref
    Point3D s_tmp,tp,u_ref,ans;
    u_ref  = NorminalControl_q_2D(dd_est,d_est,est,dd_ref,d_ref,ref,opt);

    s_tmp  = dd_est+((d_est-d_ref)*ak)+((est-ref)*bk);
    tp     = ((dd_est-dd_ref)*ak) + ((d_est-d_ref)*bk);

    h_3_q  = miu_q*miu_q-s_tmp.norm2();
    //ROS_ERROR("miu_q:%lf",miu_q);
    //ROS_ERROR("s_tmp:%lf",s_tmp.norm2());

    hat_q_u_ref = u_ref;

    /****************Constraints*****************************/
    Hessian.resize(numOfVar,numOfVar);
    gradient.resize(numOfVar);
    lowerbound.resize(numofCons);
    upperbound.resize(numofCons);
    linearmatrix.resize(numofCons,numOfVar);

    Hessian.insert(0,0)=  1;Hessian.insert(0,1)=  0;Hessian.insert(0,2)= -1;Hessian.insert(0,3)=  0;
    Hessian.insert(1,0)=  0;Hessian.insert(1,1)=  1;Hessian.insert(1,2)=  0;Hessian.insert(1,3)= -1;
    Hessian.insert(2,0)= -1;Hessian.insert(2,1)=  0;Hessian.insert(2,2)=  1;Hessian.insert(2,3)=  0;
    Hessian.insert(3,0)=  0;Hessian.insert(3,1)= -1;Hessian.insert(3,2)=  0;Hessian.insert(3,3)=  1;

    //std::cout<<"Hessian:"<<Hessian<<std::endl;
    gradient<<0,0,0,0;
    //std::cout<<"gradient:"<<gradient<<std::endl;

    lowerbound<<-OsqpEigen::INFTY, u_ref.x, u_ref.y;
    upperbound<<-2*(s_tmp*tp) + gamma_q*h_3_q, u_ref.x,u_ref.y;
    //std::cout<<"lowerbound:"<<lowerbound<<std::endl;
    //std::cout<<"upperbound:"<<upperbound<<std::endl;

    linearmatrix.insert(0,0) = 2*s_tmp.x;
    linearmatrix.insert(0,1) = 2*s_tmp.y;
    linearmatrix.insert(0,2) = 0;
    linearmatrix.insert(0,3) = 0;
    linearmatrix.insert(1,0) = 0;
    linearmatrix.insert(1,1) = 0;
    linearmatrix.insert(1,2) = 1;
    linearmatrix.insert(1,3) = 0;
    linearmatrix.insert(2,0) = 0;
    linearmatrix.insert(2,1) = 0;
    linearmatrix.insert(2,2) = 0;
    linearmatrix.insert(2,3) = 1;
    //std::cout<<"linearmatrix:"<<linearmatrix<<std::endl;

    /****************Solver*****************************/
    //solver.settings()->setWarmStart(true);
    solver.data()->setNumberOfVariables(numOfVar);
    solver.data()->setNumberOfConstraints(numofCons);
    solver.data()->setHessianMatrix(Hessian);
    solver.data()->setGradient(gradient);
    solver.data()->setLinearConstraintsMatrix(linearmatrix);
    solver.data()->setLowerBound(lowerbound);
    solver.data()->setUpperBound(upperbound);

    if(!solver.initSolver())
    {
        ans.x=0;ans.y=0;ans.z=0;
        return ans;
    }
    solver.solveProblem();

    QPsolution = solver.getSolution();

    //std::cout<<"The solution:"<<QPsolution<<std::endl;
    ans.x = QPsolution[0];
    ans.y = QPsolution[1];
    ans.z = 0;
    return ans;
}

double DATnode::CBFmain_kappa(double dd_est,double d_est,double est, double opt)
{
    OsqpEigen::Solver           solver;
    Eigen::VectorXd             QPsolution;
    Eigen::SparseMatrix<double> Hessian;
    Eigen::VectorXd             gradient;
    Eigen::VectorXd             lowerbound;
    Eigen::VectorXd             upperbound;
    Eigen::SparseMatrix<double> linearmatrix;
    int numOfVar  = 2;
    int numofCons = 3;
    double u_ref  = NorminalControl_kappa(dd_est,d_est,est,kappa_ref,opt);
    hat_kappa_u_ref = u_ref;

    h_1_kappa = miu_kappa*miu_kappa-pow(dd_est+ak*d_est+bk*(est-kappa_min),2);
    h_2_kappa = est-kappa_min;

    //ROS_ERROR("%lf,%lf",h_1_kappa,h_2_kappa);

    /****************Constraints*****************************/
    Hessian.resize(numOfVar,numOfVar);
    gradient.resize(numOfVar);
    lowerbound.resize(numofCons);
    upperbound.resize(numofCons);
    linearmatrix.resize(numofCons,numOfVar);

    Hessian.insert(0,0) =  1;
    Hessian.insert(0,1) = -1;
    Hessian.insert(1,0) = -1;
    Hessian.insert(1,1) =  1;
    //std::cout<<"Hessian:"<<Hessian<<std::endl;
    gradient<<0,0;
    //std::cout<<"gradient:"<<gradient<<std::endl;

    lowerbound<<-OsqpEigen::INFTY, -OsqpEigen::INFTY, u_ref;
    upperbound<<-2*(dd_est+ak*d_est+bk*(est-kappa_min))*(ak*dd_est+bk*d_est)+gamma_kappa*(miu_kappa*miu_kappa-pow(dd_est+ak*d_est+bk*(est-kappa_min),2)),
              (delta1+delta2+delta3)*dd_est+(delta2*delta1+delta3*delta1+delta3*delta2)*d_est+delta1*delta2*delta3*(est-kappa_min),
              u_ref;
    //std::cout<<"lowerbound:"<<lowerbound<<std::endl;
    //std::cout<<"upperbound:"<<upperbound<<std::endl;

    linearmatrix.insert(0,0) = 2*(dd_est+ak*d_est+bk*(est-kappa_min));
    linearmatrix.insert(0,1) = 0;
    linearmatrix.insert(1,0) = -1;
    linearmatrix.insert(1,1) = 0;
    linearmatrix.insert(2,0) = 0;
    linearmatrix.insert(2,1) = 1;
    //std::cout<<"linearmatrix:"<<linearmatrix<<std::endl;

    /****************Solver*****************************/
    //solver.settings()->setWarmStart(true);
    solver.data()->setNumberOfVariables(numOfVar);
    solver.data()->setNumberOfConstraints(numofCons);
    solver.data()->setHessianMatrix(Hessian);
    solver.data()->setGradient(gradient);
    solver.data()->setLinearConstraintsMatrix(linearmatrix);
    solver.data()->setLowerBound(lowerbound);
    solver.data()->setUpperBound(upperbound);

    if(!solver.initSolver()) return 1;
    solver.solveProblem();

    QPsolution = solver.getSolution();

    //std::cout<<"The solution:"<<QPsolution<<std::endl;

    return QPsolution[0];
}

double DATnode::DATmain_kappa(double dd_est, double d_est, double est)
{
    double ans = dd_est - ak*(d_local_kappa[mid]-d_est) - bk*(local_kappa[mid]-est);
    for(int j=0;j<num;j++)
    {
        if(j==mid || !adj[mid][j])continue;
        ans = ans - fai_kappa * msign(local_kappa[mid]   - local_kappa[j])
                  - pi_kappa  * msign(d_local_kappa[mid] - d_local_kappa[j]);
    }
    return ans;
}

Point3D DATnode::DATmain_q(Point3D dd_est, Point3D d_est, Point3D est)
{
    Point3D ans = dd_est - ((d_local_q[mid]-d_est)*ak) - ((local_q[mid]-est)*bk);
    for(int j=0;j<num;j++)
    {
        if(j==mid || !adj[mid][j])continue;
        ans = ans - (msign(local_q[mid]   - local_q[j]) * fai_q)
                  - (msign(d_local_q[mid] - d_local_q[j]) * pi_q);
    }
    return ans;
}

double DATnode::msign(double a)
{
    return a/(fabs(a)+0.02);

    //if(fabs(a)<0.001)return 0;
    //return a>0?1:-1;


}
Point3D DATnode::msign(Point3D p)
{
    return Point3D(msign(p.x),msign(p.y),msign(p.z));
}






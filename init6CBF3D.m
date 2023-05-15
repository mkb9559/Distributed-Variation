clc;
clear all;
n=7;

% K_est0  = 0.4*ones(n,1)+rand(n,1);
K_est0  = ones(n,1);
dK_est0 = zeros(n,1);
ddK_est0= zeros(n,1);
K0      = ones(n,1);
% K0  = 0.4*ones(n,1)+rand(n,1);
dK0     = zeros(n,1);
K_ref   = 1;
K_min   = 0.3;


ddpref_est0 = zeros(3*n,1);
dpref_est0  = zeros(3*n,1);
pref_est0   = zeros(3*n,1);


dpref0       = zeros(3*n,1);
pref0        = zeros(3*n,1);
p0           = rand(3*n,1);
dp0          = zeros(3*n,1);

obss=[ 15  16 -5  19   0  8  4;
       50 -18 -5  55  22 -5  5;
        8  17 -5   8  17  6 5;
       24  -9 -5  24  -9  6 8;
        8 -15 -5   8 -15  6 8;
       50   6 -5  50   6  6 6];
% obss=[15  5 -5 15  5 5 3];


LL =[3    -1    -1     0     0    -1    0,
    -1     3    -1     0    -1    -1    0,
    -1    -1     3    -1     0    -1    0,
     0     0    -1     4    -1     0   -1,
     0    -1     0    -1     4     0   -1,
    -1    -1    -1     0     0     4   -1,
     0     0     0    -1    -1    -1    3];
         
topology = sign(abs(LL));
   
pd=4*[ 1.5  0  0.5;
       0.5  1 -0.5;
       0.5 -1 -0.5;
      -1.5 -1 -0.5;
      -1.5  1 -0.5;
       0.5  0  1;
      -0.5  0  1];
   

pd=reshape(pd',3*n,1);

vp = [-pi/6 -pi/6 pi/9];
figure(1);
hold on;
for i=1:n
    plot3(pd(3*(i-1)+1),pd(3*(i-1)+2),pd(3*(i-1)+3),'b*','LineWidth',2);
end
for i=1:n
    for j=i+1:n
        if(topology(i,j)<0.5)
            continue;
        end
        line([pd(3*(i-1)+1) pd(3*(j-1)+1)],[pd(3*(i-1)+2) pd(3*(j-1)+2)],[pd(3*(i-1)+3) pd(3*(j-1)+3)],'Color','k','LineWidth',2);
    end
end
for i=1:length(obss(:,1))
    cylinder3alpha([obss(i,1) obss(i,2) obss(i,3)],[obss(i,4) obss(i,5) obss(i,6)],obss(i,7),10,[186 205 174]./255,1,0,1);
end
% view(vp);
axis equal;
xlabel('${x}(m)$','fontsize',18,'interpreter','latex');
ylabel('${y}(m)$','fontsize',18,'interpreter','latex');
zlabel('${z}(m)$','fontsize',18,'interpreter','latex');







 
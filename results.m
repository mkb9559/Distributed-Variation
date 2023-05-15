skp2 = 200;
endt=length(out.tt);

figure(10);hold on; box on;
for i=1:n
    plot(out.tt(1:skp2:endt),out.K_est(1:skp2:endt,i),'LineWidth',2);
end
plot([0 80],[K_min K_min],'--k','LineWidth',2);
plot(out.tt,out.K(:,1),'r','LineWidth',3);
plot([0 80],[K_ref K_ref],'--g','LineWidth',2);

axis([ 0 80 0 1.1]);
xlabel('${t}(s)$','fontsize',32,'interpreter','latex');
ylabel('$\rho_i$','fontsize',32,'interpreter','latex');
set(gca,'FontSize',32);
h=legend('$\rho_1$','$\rho_2$','$\rho_3$','$\rho_4$',...
    '$\rho_5$','$\rho_6$','$\rho_7$','$\kappa_{min}$','$\kappa_i$','$\kappa^{*}$','location','southeast');
set(h,'FontSize',32,'interpreter','latex');

figure(11);hold on; box on;
for i=1:n
    plot3(out.pref_est(1:skp2:endt,3*(i-1)+1),out.pref_est(1:skp2:endt,3*(i-1)+2),out.pref_est(1:skp2:endt,3*(i-1)+3),'LineWidth',2);
end
plot3(out.prefd(1:skp2:endt,1),out.prefd(1:skp2:endt,2),out.prefd(1:skp2:endt,3),'m','LineWidth',4);
% plot3(out.pref(endt,1),out.pref(endt,2),out.pref(endt,3),'m*','LineWidth',2);
plot3(out.pref(1:skp2:endt,1),out.pref(1:skp2:endt,2),out.pref(1:skp2:endt,3),'r','LineWidth',4);
for i=1:length(obss(:,1))
     h4=cylinder3alpha([obss(i,1) obss(i,2) obss(i,3)],[obss(i,4) obss(i,5) obss(i,6)],obss(i,7),30,[186 186 186]./255,1,0,1);
end
axis equal;
xlabel('${x}(m)$','fontsize',18,'interpreter','latex');
ylabel('${y}(m)$','fontsize',18,'interpreter','latex');
zlabel('${z}(m)$','fontsize',18,'interpreter','latex');
view([-70 -20 40]);
axis([ -5 75 -15 15 -10 10]);
set(gca,'FontSize',18);
h=legend('$\textbf{\emph{w}}_1$','$\textbf{\emph{w}}_2$','$\textbf{\emph{w}}_3$','$\textbf{\emph{w}}_4$',...
    '$\textbf{\emph{w}}_5$','$\textbf{\emph{w}}_6$','$\textbf{\emph{w}}_7$','$\textbf{\emph{p}}_{*}$',...
    '$\textbf{\emph{q}}_i$','Obstacles','location','southeast');
set(h,'FontSize',18,'interpreter','latex');


figure(12);hold on; box on;
for i=1:n
    out.h1_kappa(1,i)=out.h1_kappa(100,i);
    plot(out.tt(1:skp2:endt),out.h1_kappa(1:skp2:endt,i),'','LineWidth',2);
end
xlabel('${t}(s)$','fontsize',32,'interpreter','latex');
ylabel('$h_1(\rho_i,\dot{\rho}_i,\ddot{\rho}_i)$','fontsize',32,'interpreter','latex');
set(gca,'FontSize',32);
h=legend('$h_1(\rho_1,\dot{\rho}_1,\ddot{\rho}_1)$','$h_1(\rho_2,\dot{\rho}_2,\ddot{\rho}_2)$',...
    '$h_1(\rho_3,\dot{\rho}_3,\ddot{\rho}_3)$','$h_1(\rho_4,\dot{\rho}_4,\ddot{\rho}_4)$',...
    '$h_1(\rho_5,\dot{\rho}_5,\ddot{\rho}_5)$','$h_1(\rho_6,\dot{\rho}_6,\ddot{\rho}_6)$',...
    '$h_1(\rho_7,\dot{\rho}_7,\ddot{\rho}_7)$','location','northeast');
set(h,'FontSize',32,'interpreter','latex');

figure(13);hold on; box on;
for i=1:n
    out.h1_q(1,i)=out.h1_q(100,i);
    plot(out.tt(1:skp2:endt),out.h1_q(1:skp2:endt,i),'','LineWidth',2);
end
xlabel('${t}(s)$','fontsize',32,'interpreter','latex');
ylabel('$h_3(\textbf{\emph{w}}_i,\dot{\textbf{\emph{w}}}_i,\ddot{\textbf{\emph{w}}}_i)$','fontsize',32,'interpreter','latex');
set(gca,'FontSize',32);
h=legend('$h_3(\textbf{\emph{w}}_1,\dot{\textbf{\emph{w}}}_1,\ddot{\textbf{\emph{w}}}_1)$',...
    '$h_3(\textbf{\emph{w}}_2,\dot{\textbf{\emph{w}}}_2,\ddot{\textbf{\emph{w}}}_2)$',...
    '$h_3(\textbf{\emph{w}}_3,\dot{\textbf{\emph{w}}}_3,\ddot{\textbf{\emph{w}}}_3)$',...
    '$h_3(\textbf{\emph{w}}_4,\dot{\textbf{\emph{w}}}_4,\ddot{\textbf{\emph{w}}}_4)$',...
    '$h_3(\textbf{\emph{w}}_5,\dot{\textbf{\emph{w}}}_5,\ddot{\textbf{\emph{w}}}_5)$',...
    '$h_3(\textbf{\emph{w}}_6,\dot{\textbf{\emph{w}}}_6,\ddot{\textbf{\emph{w}}}_6)$',...
    '$h_3(\textbf{\emph{w}}_7,\dot{\textbf{\emph{w}}}_7,\ddot{\textbf{\emph{w}}}_7)$',...
    'location','southeast');
set(h,'FontSize',32,'interpreter','latex');

figure(14);hold on; box on;
%for i=1:n
%    plot(out.tt(1:1:endt),out.p1(1:1:endt,3*(i-1)+2),'Color',[0.00,0.45,0.74],'LineWidth',1);
%    plot(out.tt(1:1:endt),out.p2(1:1:endt,3*(i-1)+2),'Color',[0.85,0.33,0.10],'LineWidth',2);
%end
plot(out.tt(1:1:endt),out.p1(1:1:endt,2),'LineWidth',1);
plot(out.tt(1:1:endt),out.p1(1:1:endt,5),'LineWidth',1);
plot(out.tt(1:1:endt),out.p1(1:1:endt,8),'LineWidth',1);
plot(out.tt(1:1:endt),out.p2(1:1:endt,2),'m','LineWidth',2);
plot(out.tt(1:1:endt),out.p2(1:1:endt,5),'g','LineWidth',2);
plot(out.tt(1:1:endt),out.p2(1:1:endt,8),'k','LineWidth',2);

xlabel('${t}(s)$','fontsize',32,'interpreter','latex');
ylabel('$y(m)$','fontsize',32,'interpreter','latex');
set(gca,'FontSize',32);
h=legend('$y_1~measured$',...
    '$y_2~measured$',...
    '$y_3~measured$',...
    '$y_1~filtered$',...
    '$y_2~filtered$',...
    '$y_3~filtered$',...
    'location','southeast');
set(h,'FontSize',32,'interpreter','latex');

sizz=24;
figure(15);
subplot(3,1,1);hold on; box on;
plot(out.tt(1:1:endt),4*out.ffr(1:1:endt,1),'b','LineWidth',1);
plot(out.tt(1:1:endt),4*out.ff(1:1:endt,1),'r','LineWidth',2);
xlabel('${t}(s)$','fontsize',sizz,'interpreter','latex');
ylabel('$f_{1x}$','fontsize',sizz,'interpreter','latex');
set(gca,'FontSize',sizz);

subplot(3,1,2);hold on; box on;
plot(out.tt(1:1:endt),4*out.ffr(1:1:endt,2),'b','LineWidth',1);
plot(out.tt(1:1:endt),4*out.ff(1:1:endt,2),'r','LineWidth',2);
xlabel('${t}(s)$','fontsize',sizz,'interpreter','latex');
ylabel('$f_{1y}$','fontsize',sizz,'interpreter','latex');
set(gca,'FontSize',sizz);

subplot(3,1,3);hold on; box on;
plot(out.tt(1:1:endt),4*out.ffr(1:1:endt,3),'b','LineWidth',1);
plot(out.tt(1:1:endt),4*out.ff(1:1:endt,3),'r','LineWidth',2);
xlabel('${t}(s)$','fontsize',sizz,'interpreter','latex');
ylabel('$f_{1z}$','fontsize',sizz,'interpreter','latex');
set(gca,'FontSize',sizz);

h=legend('From noisy position measurements',...
    'From filtered position measurements',...
    'location','southeast');
set(h,'FontSize',sizz);




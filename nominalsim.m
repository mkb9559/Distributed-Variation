% p=[ 6  4 6,
%     4 -5 6,
%    -5 -4 4,
%    -5  6 6,
%     6  4 8,
%     4 -5 8,
%    -5 -4 8,
%    -5  6 8,
%     0  0 0,
%     0  0 -2];




ff=figure(9);
set(ff,'position',[100,100,1300,500]);
subplot(1,2,1);
hold on; axis equal
% plot edges
killr=0.05;
for i=1:n
    for j=i:n
        if(topology(i,j)<0.5)
            continue;
        end
        killr=0.05;
        if(i==1&&j==2)
            killr=0.1;
        end
        if(i==1&&j==6)
            killr=0.08;
        end
        if(i==2&&j==6)
            killr=0.15;
        end
        if(i==6&&j==7)
            killr=0.1;
        end
        if(i==5&&j==7)
            killr=0.1;
        end
        vv=pd(3*(i-1)+1:3*(i-1)+3)-pd(3*(j-1)+1:3*(j-1)+3);
        if(i==1&&j==2||i==2&&j==5||i==2&&j==6||i==2&&j==3)
            line([pd(3*(i-1)+1)-vv(1)*killr pd(3*(j-1)+1)+vv(1)*killr],...
                [pd(3*(i-1)+2)-vv(2)*killr pd(3*(j-1)+2)+vv(2)*killr],...
                [pd(3*(i-1)+3)-vv(3)*killr pd(3*(j-1)+3)+vv(3)*killr],'Color','k','LineStyle','--','LineWidth',2);
        else
            line([pd(3*(i-1)+1)-vv(1)*killr pd(3*(j-1)+1)+vv(1)*killr],...
                [pd(3*(i-1)+2)-vv(2)*killr pd(3*(j-1)+2)+vv(2)*killr],...
                [pd(3*(i-1)+3)-vv(3)*killr pd(3*(j-1)+3)+vv(3)*killr],'Color','k','LineWidth',2);
        end
    end
end
% plot nodes
for i=1:n
    plot3(pd(3*(i-1)+1),pd(3*(i-1)+2),pd(3*(i-1)+3), 'o', ...
        'MarkerSize', 15,...
        'linewidth', 2,...
        'MarkerEdgeColor', 'b');
    text(pd(3*(i-1)+1),pd(3*(i-1)+2),pd(3*(i-1)+3), num2str(i),...
        'color', 'b', 'FontSize', 13, 'horizontalAlignment', 'center', 'FontName', 'times');
end
% plot3(0,0,0, 'o', ...
%         'MarkerSize', 15,...
%         'linewidth', 2,...
%         'MarkerEdgeColor', 'k');
% text(0,0,0, '$\textbf{\emph{p}}^{*}$',...
%         'color', 'k', 'FontSize', 13, 'horizontalAlignment', 'center', 'FontName', 'times','interpreter','latex');
box on;
% set(gca,'Visible','off');
view([20 -70 70]);
set(gca,'FontSize',18);
axis equal;
xlabel('${x}(m)$','fontsize',18,'interpreter','latex');
ylabel('${y}(m)$','fontsize',18,'interpreter','latex');
zlabel('${z}(m)$','fontsize',18,'interpreter','latex');

subplot(1,2,2);
sz=18;
ss=3;
add=1;
text(0,ss,'$\textbf{\emph{r}}_1^*=[~~6~~~~~0~~~~~2]^{T}$','FontSize',sz,'interpreter','latex');
text(0,ss-add,'$\textbf{\emph{r}}_2^*=[~~2~~~~~4~~-2]^{T}$','FontSize',sz,'interpreter','latex');
text(0,ss-2*add,'$\textbf{\emph{r}}_3^*=[~~2~-4~~-2]^{T}$','FontSize',sz,'interpreter','latex');
text(0,ss-3*add,'$\textbf{\emph{r}}_4^*=[-6~-4~~-2]^{T}$','FontSize',sz,'interpreter','latex');
text(0,ss-4*add,'$\textbf{\emph{r}}_5^*=[-6~~~~~4~~-2]^{T}$','FontSize',sz,'interpreter','latex');
text(0,ss-5*add,'$\textbf{\emph{r}}_6^*=[~~2~~~~~4~~~~~~4]^{T}$','FontSize',sz,'interpreter','latex');
text(0,ss-6*add,'$\textbf{\emph{r}}_7^*=[-2~~~~~0~~~~~~4]^{T}$','FontSize',sz,'interpreter','latex');
set(gca,'Visible','off');
axis equal;
axis([-2 2 -4.5 4.5]);


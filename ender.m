ss   = 1;
skp  = 1000;
skp2 = 200;
skp3 = 15000;

figure(1);hold on;box on;
view([-70 -15 80]);
endt=length(out.tt);

h1=plot3(out.prefd(1:skp2:length(out.tt),1),out.prefd(1:skp2:length(out.tt),2),out.prefd(1:skp2:length(out.tt),3),'m','LineWidth',2);
% plot3(out.pref(endt,1),out.pref(endt,2),out.pref(endt,3),'m*','LineWidth',2);
h2=plot3(out.pref(1:skp2:endt,1),out.pref(1:skp2:endt,2),out.pref(1:skp2:endt,3),'r','LineWidth',2);
for i=1:length(obss(:,1))
    h5=cylinder3alpha([obss(i,1) obss(i,2) obss(i,3)],[obss(i,4) obss(i,5) obss(i,6)],obss(i,7),30,[186 205 174]./255,1,0,1);
end
for i=1:n
    h4=plot3(out.p(1:skp2:endt,3*(i-1)+1),out.p(1:skp2:endt,3*(i-1)+2),out.p(1:skp2:endt,3*(i-1)+3),'Color',[30 144 255]./255,'LineWidth',1);
end
cnts=0;
for sndt=10000:skp3:length(out.tt)
    for i=1:n
        for j=i+1:n
            if(topology(i,j)<0.5)
            continue;
            end
            line([out.p(sndt,3*(i-1)+1) out.p(sndt,3*(j-1)+1)],[out.p(sndt,3*(i-1)+2) out.p(sndt,3*(j-1)+2)],[out.p(sndt,3*(i-1)+3) out.p(sndt,3*(j-1)+3)],'Color','k','LineWidth',2);
        end
    end
    for i=1:n
        h3=plot3(out.p(sndt,3*(i-1)+1),out.p(sndt,3*(i-1)+2),out.p(sndt,3*(i-1)+3),'b*','LineWidth',2);
    end
    if(cnts==0)
        text(out.pref(sndt,1)-10,out.pref(sndt,2)+12,out.pref(sndt,3)+5,'$t=10s$','FontSize',18,'interpreter','latex');
        cnts=cnts+1;continue;
    end
    if(cnts==1)
        text(out.pref(sndt,1)-8,out.pref(sndt,2)+12,out.pref(sndt,3)+6,'$t=25s$','FontSize',18,'interpreter','latex');
        cnts=cnts+1;continue;
    end
    if(cnts==2)
        text(out.pref(sndt,1)-8,out.pref(sndt,2)+12,out.pref(sndt,3)+6,'$t=40s$','FontSize',18,'interpreter','latex');
        cnts=cnts+1;continue;
    end
    if(cnts==3)
        text(out.pref(sndt,1)-3,out.pref(sndt,2)-6,out.pref(sndt,3)+3,'$t=55s$','FontSize',18,'interpreter','latex');
        cnts=cnts+1;continue;
    end
    if(cnts==4)
        text(out.pref(sndt,1),out.pref(sndt,2)-6,out.pref(sndt,3)+3,'$t=70s$','FontSize',18,'interpreter','latex');
        cnts=cnts+1;continue;
    end
end

    
xlabel('${x}(m)$','fontsize',18,'interpreter','latex');
ylabel('${y}(m)$','fontsize',18,'interpreter','latex');
zlabel('${z}(m)$','fontsize',18,'interpreter','latex');
set(gca,'FontSize',18);
h=legend([h1 h2 h3 h4 h5],'Reference trajectory','Translated reference trajectory','Position of robots','Trajectory of robots','Obstacles','location','southeast','NumColumns',2);
set(h,'FontSize',18);
axis equal;


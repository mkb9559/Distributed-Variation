ss   = 1;
skp  = 1000;
skp2 = 200;
viewK=2;

for endt=ss:skp:length(out.tt)
    endt
    figure(1);
    subplot(1,2,1);
    cla;
    hold on;box on;
    view([-70 -15 80]);
    
    for i=1:n
        for j=i+1:n
            if(topology(i,j)<0.5)
                continue;
            end
            line([out.p(endt,3*(i-1)+1) out.p(endt,3*(j-1)+1)],[out.p(endt,3*(i-1)+2) out.p(endt,3*(j-1)+2)],[out.p(endt,3*(i-1)+3) out.p(endt,3*(j-1)+3)],'Color','k','LineWidth',2);
        end
    end
    for i=1:n
%         dis=1000;
%         f=[0 0 1];
%         for j=1:length(obss(:,1))
%             a = obss(j,4:6)-obss(j,1:3);
%             b = out.p(endt,3*(i-1)+1:3*(i-1)+3)-obss(j,1:3);
%             
%             proj = a*b'./norm(a);
%             ftmp    = (b-proj*a./norm(a));
%             disf    = norm(ftmp);
%             if(disf<dis)
%                 dis=disf;
%                 f=ftmp;
%             end
%         end
%         f=2*f./norm(f);
        if(norm(out.ff(endt,3*(i-1)+1:3*(i-1)+3))>0.1)
            quiver3(out.p(endt,3*(i-1)+1),out.p(endt,3*(i-1)+2),out.p(endt,3*(i-1)+3),...
                out.ff(endt,3*(i-1)+1),out.ff(endt,3*(i-1)+2),out.ff(endt,3*(i-1)+3),'g','LineWidth',2);
        end
        plot3(out.p(endt,3*(i-1)+1),out.p(endt,3*(i-1)+2),out.p(endt,3*(i-1)+3),'b*','LineWidth',2);
        plot3(out.p(1:skp2:endt,3*(i-1)+1),out.p(1:skp2:endt,3*(i-1)+2),out.p(1:skp2:endt,3*(i-1)+3),'b');
    end
    plot3(out.prefd(endt,1),out.prefd(endt,2),out.prefd(endt,3),'r*','LineWidth',2);
    plot3(out.prefd(1:skp2:length(out.tt),1),out.prefd(1:skp2:length(out.tt),2),out.prefd(1:skp2:length(out.tt),3),'r','LineWidth',2);
    plot3(out.pref(endt,1),out.pref(endt,2),out.pref(endt,3),'m*','LineWidth',2);
    plot3(out.pref(1:skp2:endt,1),out.pref(1:skp2:endt,2),out.pref(1:skp2:endt,3),'m','LineWidth',2);
    for i=1:length(obss(:,1))
        cylinder3alpha([obss(i,1) obss(i,2) obss(i,3)],[obss(i,4) obss(i,5) obss(i,6)],obss(i,7),10,[186 205 174]./255,1,0,1);
    end
    
    xlabel('${x}(m)$','fontsize',18,'interpreter','latex');
    ylabel('${y}(m)$','fontsize',18,'interpreter','latex');
    zlabel('${z}(m)$','fontsize',18,'interpreter','latex');
    set(gca,'FontSize',18);
%     set(h,'FontSize',18);
    axis equal;
%     axis([ -5 75 -30 30 -15 15]);

    subplot(1,2,2);
    cla;
    axis([ out.prefd(endt,1)-10 out.prefd(endt,1)+10 out.prefd(endt,2)-10 out.prefd(endt,2)+10 out.prefd(endt,3)-10 out.prefd(endt,3)+10]);
    hold on;box on;
    view([-80 -10 90]);
    for i=1:n
        for j=i+1:n
            if(topology(i,j)<0.5)
                continue;
            end
            line([out.p(endt,3*(i-1)+1) out.p(endt,3*(j-1)+1)],[out.p(endt,3*(i-1)+2) out.p(endt,3*(j-1)+2)],[out.p(endt,3*(i-1)+3) out.p(endt,3*(j-1)+3)],'Color','k','LineWidth',2);
        end
    end
    for i=1:n
        if(norm(out.ff(endt,3*(i-1)+1:3*(i-1)+3))>0.1)
            quiver3(out.p(endt,3*(i-1)+1),out.p(endt,3*(i-1)+2),out.p(endt,3*(i-1)+3),...
                out.ff(endt,3*(i-1)+1),out.ff(endt,3*(i-1)+2),out.ff(endt,3*(i-1)+3),'g','LineWidth',2);
        end
        plot3(out.p(endt,3*(i-1)+1),out.p(endt,3*(i-1)+2),out.p(endt,3*(i-1)+3),'b*','LineWidth',2);
        plot3(out.p(1:skp2:endt,3*(i-1)+1),out.p(1:skp2:endt,3*(i-1)+2),out.p(1:skp2:endt,3*(i-1)+3),'b');
    end
    plot3(out.prefd(endt,1),out.prefd(endt,2),out.prefd(endt,3),'r*','LineWidth',2);
    plot3(out.prefd(1:skp2:length(out.tt),1),out.prefd(1:skp2:length(out.tt),2),out.prefd(1:skp2:length(out.tt),3),'r','LineWidth',2);
    plot3(out.pref(endt,1),out.pref(endt,2),out.pref(endt,3),'m*','LineWidth',2);
    plot3(out.pref(1:skp2:endt,1),out.pref(1:skp2:endt,2),out.pref(1:skp2:endt,3),'m','LineWidth',2);
    for i=1:length(obss(:,1))
        cylinder3alpha([obss(i,1) obss(i,2) obss(i,3)],[obss(i,4) obss(i,5) obss(i,6)],obss(i,7),10,[186 205 174]./255,1,0,1);
    end
    
    xlabel('${x}(m)$','fontsize',18,'interpreter','latex');
    ylabel('${y}(m)$','fontsize',18,'interpreter','latex');
    zlabel('${z}(m)$','fontsize',18,'interpreter','latex');
    set(gca,'FontSize',18);
%     set(h,'FontSize',18);
    
    
    
    
    pause(0.2);
end
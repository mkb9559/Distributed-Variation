ss   = 1;
skp  = 100;
skp2 = 10;

for endt=ss:skp:length(out.tt)
    endt
    figure(1);
    axis([x_min x_max y_min y_max 0 z_max]);
    axis equal;
    cla;
    hold on;
    mesh(Xsqe,Ysqe,map);
    for i=1:n
        for j=i+1:n
            if(topology(i,j)==0) continue;
            end
            line([out.p(endt,(i-1)*2+1) out.p(endt,(j-1)*2+1)],[out.p(endt,(i-1)*2+2) out.p(endt,(j-1)*2+2)],[z_max z_max],'Color','k','LineWidth',2);         
        end
    end
    for i=leader
        if(ismember(i,major))
            plot3(out.p(endt,(i-1)*2+1),out.p(endt,(i-1)*2+2),z_max,'r*','LineWidth',2);
            plot3(out.p(1:skp2:endt,(i-1)*2+1),out.p(1:skp2:endt,(i-1)*2+2),z_max*ones(length(out.p(1:skp2:endt,(i-1)*2+2))),'r','LineWidth',1);
        else
            plot3(out.p(endt,(i-1)*2+1),out.p(endt,(i-1)*2+2),z_max,'m*','LineWidth',2);
            plot3(out.p(1:skp2:endt,(i-1)*2+1),out.p(1:skp2:endt,(i-1)*2+2),z_max*ones(length(out.p(1:skp2:endt,(i-1)*2+2))),'m','LineWidth',1);
        end
    end
    for i=follower
        plot3(out.p(endt,(i-1)*2+1),out.p(endt,(i-1)*2+2),z_max,'b*','LineWidth',2);
        plot3(out.p(1:skp2:endt,(i-1)*2+1),out.p(1:skp2:endt,(i-1)*2+2),z_max*ones(length(out.p(1:skp2:endt,(i-1)*2+2))),'b','LineWidth',1);
    end
    axis equal;
    pause(0.1);
end
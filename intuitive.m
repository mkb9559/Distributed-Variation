figure(1);
plot3(0,0,0,'b*','LineWidth',2);
cylinder3alpha([1 -1 -3],[3 -3 3],1,30,[186 205 174]./255,1,0,0.5);
h=quiver3(1,-1,-3,2,-2,6,'g','LineWidth',2);
set(h,'maxheadsize',0.5); 
k1=1.1;
h=quiver3(1,-1,-3,-1*k1,1*k1,3*k1,'r','LineWidth',2);
set(h,'maxheadsize',0.5); 
axis equal;
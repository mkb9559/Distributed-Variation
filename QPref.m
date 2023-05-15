function [x,fval,exitflag]=QPref(A,f,b,H,Aeq,beq)
[x,fval,exitflag]=quadprog(H,f,A,b,Aeq,beq);
end

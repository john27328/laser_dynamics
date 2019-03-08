clear
stacksize('max')
beta0 = 3500;
W31 = 360;
W32 = 40;
W21 = 67;
h = 6.62e-34;
c=3e8;
sigma_a_pump = 1e-24;
N=4.1e26;
n=1.7;
lambda_p=793e-9;
lambda_g=1.98e-6;
sigma_e_gen=0.25e-24;
sigma_a_gen= 0.04e-24;
P=400;
d=3e-3;
alpha_i = 1;
beta_j=0.2;
L=0.15;
l=0.042;
Le=L+(n-1)*l;
Va=%pi*d^2/4*l;
V=Le/l*Va;
gamma_1=-log(1-0.005);
gamma_2=-log(1-0.2)
gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
B_e=alpha_i*c/V*sigma_e_gen;
B_a=beta_j*c/V*sigma_a_gen;
K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*%pi*d^2)
K=(lambda_p*P*4)/(N*l*h*c*%pi*d^2);

y=[1;0;0;0];
//t=0:0.000001:0.16;
t0=0;
t1=0.16;
dt=0.000001;
i0=1; i1=(t1-t0)/dt
t=0;
for i=i0:1:i1 
    dy(3,i)=(K*y(1,i)-beta0*y(1,i)*y(3,i)-(W31+W32)*y(3,i))*dt; //4-Й ЧЛЕН =0
    dy(2,i)=(2*beta0*y(1,i)*y(3,i)+W32*y(3,i)-W21*y(2,i)-y(4,i)*(B_e*y(2,i)-B_a*y(1,i)))*dt;
    dy(1,i)=(-beta0*y(1,i)*y(3,i)-K*y(1,i)+W31*y(3,i)+W21*y(2,i)+y(4,i)*(B_e*y(2,i)-B_a*y(1,i)))*dt;
    dy(4,i)=(N*Va*(y(4,i)+1)*(B_e*y(2,i)-B_a*y(1,i))-y(4,i)*rho)*dt;
    y(1,i+1)=y(1,i)+dy(1,i);
    y(2,i+1)=y(2,i)+dy(2,i);
    y(3,i+1)=y(3,i)+dy(3,i);
    y(4,i+1)=y(4,i)+dy(4,i);
    t(i+1)=t(i)+dt
end



y4=1-y(1,:)-y(2,:)-y(3,:);
//plot(t,y,leg="n1@n2@n3@q");
//clf;
//xtitle('населенности','t, с', 'n');
//plot(t,y,t,y4);xgrid;
//legend('n1', 'n2', 'n3', 'q', 'n4')

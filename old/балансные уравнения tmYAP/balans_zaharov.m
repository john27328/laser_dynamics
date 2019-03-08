function balans_zaharov
clear;
beta0 = 3500;
W31 = 360;
W32 = 40;
W21 = 67;
h = 6.62e-34;
c=3e8;
sigma_a_pump = 1e-24;
N=4e26;
n=1.7;
lambda_p=793e-9;
lambda_g=1.98e-6;
sigma_e_gen=0.25e-24;
sigma_a_gen= 0.04e-24;
P=70*0.9;
d=0.8e-3;
alpha_i = 1;
beta_j=0.2;
L=0.15;
l=0.015;
Le=L+(n-1)*l;
Va=pi*d^2/4*l;
V=Le/l*Va;
gamma_1=-log(1-0.005);
gamma_2=-log(1-0.2)
gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l)));
rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2);
B_e=alpha_i*c/V*sigma_e_gen;
B_a=beta_j*c/V*sigma_a_gen;
K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*pi*d^2)
K=(lambda_p*P*4)/(N*l*h*c*pi*d^2);


x0=[1;0;0;1];

% Определение параметров управления ходом решения уравнения.
% RelTol — относительная точность решения 1E-5,
% AbsTol — абсолютная точность решения 1E-5,
% InitialStep — начальное значение шага изменения переменной 0.025,
% MaxStep — максимальное значение шага изменения переменной 0.1.
%par=odeset( 'RelTol' , 1e-5 , 'AbsTol' , 1e-5 , 'InitialStep', 0.000001 , 'MaxStep' , 0.00001 ) ;
% Решение дифференциального уравнения методом Рунге-Кутта 2–3 порядка.

%[X23 , Y23]=ode23(@balans , [0 0.06] , x0 , par ) ;

[X23 , Y23]=ode23(@balans , [0 0.06] , x0) ;

function dy=balans(t,y)
    dy=zeros(4,1);
    dy(3)=K*y(1)-beta0*y(1)*y(3)-(W31+W32)*y(3); 
    dy(2)=2*beta0*y(1)*y(3)+W32*y(3)-W21*y(2)-y(4)*(B_e*y(2)-B_a*y(1));
    dy(1)=-beta0*y(1)*y(3)-K*y(1)+W31*y(3)+W21*y(2)+y(4)*(B_e*y(2)-B_a*y(1));
    dy(4)=N*Va*(y(4))*(B_e*y(2)-B_a*y(1))-y(4)*rho;
end




















clear
stacksize('max')
beta0 = 3500; //K*N0 // c //ок
//K - коэффициент кросс релаксации
//N0 - концентрация
W31 = 360; // A31 //см^-3 //ок
W32 = 40; // A32 //см^-3 //ок
W21 = 67; //см^-3 //ок
h = 6.62e-34; //постоянная планка//ок
c=3e8; //скорость света //ок
sigma_a_pump = 1e-24;//поглощение накачки //не используется 
N=4.1e26;  // было 4.1e20 см^-3// м^-3 ? //ок?
n=1.7; // коэффициент преломления //ок
lambda_p=793e-9; // м //ок
lambda_g=1.98e-6; // м //ок
sigma_e_gen=0.25e-24; // м //опечатка в дисертации//ок
sigma_a_gen= 0.04e-24; // м //опечатка в дисертации//ок
P=400; //поглощенная мощность //Вт //ок
d=3e-3; //диаметр накачки 800 мкм //м //ок
alpha_i = 1; // подгонка
beta_j=0.2; // подгонка
L=0.15; //длина резонатора 15 см // м //ок?
l=0.042; //длина кристалла 1,5 см // м /ок?
Le=L+(n-1)*l; //оптическая длина резонатора //ок
Va=%pi*d^2/4*l; // объем прокачиваемой области //ок
V=Le/l*Va; // оптический объем моды генерации //ок
gamma_1=-log(1-0.005); // глухое зеркало //ок
gamma_2=-log(1-0.2); // выходное зеркало //ок
gamma_i=-log(1-exp(-1/(sigma_a_gen*N*l))); //внутренние потери // ыыыыыыы!!!!!!
rho=c/Le*(gamma_i+(gamma_1+gamma_2)/2); // ок
B_e=alpha_i*c/V*sigma_e_gen;//подгонка //ок
B_a=beta_j*c/V*sigma_a_gen;//подгонка //ок
//K=1*(lambda_p*P*sigma_a_pump*4)/(h*c*%pi*d^2)
K=(lambda_p*P*4)/(N*l*h*c*%pi*d^2);// ok

function dy=balans(t,y)
    dy=zeros(4,1);
    dy(3)=K*y(1)-beta0*y(1)*y(3)-(W31+W32)*y(3); //4-Й ЧЛЕН =0
    //dy(3)=K-beta0*y(1)*y(3)-(W31+W32)*y(3);
    dy(2)=2*beta0*y(1)*y(3)+W32*y(3)-W21*y(2)-y(4)*(B_e*y(2)-B_a*y(1));
    dy(1)=-beta0*y(1)*y(3)-K*y(1)+W31*y(3)+W21*y(2)+y(4)*(B_e*y(2)-B_a*y(1));
    //dy(1)=-beta0*y(1)*y(3)-K+W31*y(3)+W21*y(2)+y(4)*(B_e*y(2)-B_a*y(1));
    dy(4)=N*Va*(y(4)+1)*(B_e*y(2)-B_a*y(1))-y(4)*rho;
endfunction

x0=[1;0;0;0];
t0=0;
t=0:0.000001:0.16;
y=ode("rkf",x0,t0,t,balans);
//y4=1-y(1,:)-y(2,:)-y(3,:);
//write_csv(y,'./data.csv')
//plot(t,y,leg="n1@n2@n3@q");
Pgen = gamma_2*c/2/Le*h*c/lambda_g*y(4,:);
clf;
subplot(2,2,1);
xtitle('населенности','t, с', 'n');
plot(t,y(1,:),t,y(2,:),t,y(3,:));xgrid;
legend('n1', 'n2', 'n3')
subplot(2,2,3);
xtitle('плотность фотонов','t, с', 'n');
plot(t,y(4,:));xgrid;
subplot(1,2,2);
xtitle('мощность генерации','t, с', 'P, W');
plot(t,Pgen);xgrid;
legend('q')

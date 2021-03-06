clear
stacksize('max')

//K= ? //коэффициент кросс релаксации // ?
N=7.8e+26;//концентрация активных ионов //?
beta0 = K*N0; //K*N0 // c //?
W31 = ?; // A31 //см^-3 //?
W32 = ?; // A32 //см^-3 //?
W21 = ?; //см^-3 //ок?
h = 6.62e-34; //постоянная планка//?
c=3e8; //скорость света //ок
//sigma_a_pump = ?;//поглощение накачки //не используется 
  // было 4.1e20 см^-3// м^-3 ? //ок?
n=1.93; //из диплома // коэффициент преломления //ок
lambda_p=793e-9; // м //ок
lambda_g=1.93e-6; // м //ок

E1k=[0 3 65 114 210 237 271 282 313 440 574 628];
E2k=[5624 5627 5716 5722 5819 5843 5935 5965 5983];
h=1.055e-34;
c=3e8;
k=1.38e-23;
T=300;

E1=E1k*h*c;
Z1=0;
for i=1:length(E1)
    Z1=Z1+exp(-(E1(i)-E1(1))/k/T);
end


E2=E2k*h*c;
Z2=0;
for i=1:length(E2)
    Z2=Z2+exp(-(E2(i)-E2(1))/k/T);
end

Z1_Z2=Z1/Z2;

Kabs=0.07//поглощение генерации из эксперимента
sigma_a_gen= Kabs/N; //искать через sigma_a_gen= // сечение генерации на 1,9 // м //опечатка в дисертации//ок
sigma_e_gen= sigma_a_gen*Z1_Z2*exp((E2(1)-h*c/lambda_g)/k/T); // K_погл/N // м //опечатка в дисертации//ок

//поиск sigma_a_gen
//a = csvRead('./спектр');

P=500; //поглощенная мощность //Вт //ок
d=3e-3; //диаметр накачки 3мм //м //ок
alpha_i = 1; // подгонка //?
beta_j=0.2; // подгонка //?
L=0.2; //длина резонатора 20 см // м //ок?
l=0.06; //длина кристалла 6 см // м /ок?
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
    dy(2)=2*beta0*y(1)*y(3)+W32*y(3)-W21*y(2)-y(4)*(B_e*y(2)-B_a*y(1));
    dy(1)=-beta0*y(1)*y(3)-K*y(1)+W31*y(3)+W21*y(2)+y(4)*(B_e*y(2)-B_a*y(1));
    dy(4)=N*Va*(y(4)+1)*(B_e*y(2)-B_a*y(1))-y(4)*rho;
endfunction

x0=[1;0;0;0];
t0=0;
t=0:0.00000003:0.02;
//y=ode("rkf",x0,t0,t,balans);
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

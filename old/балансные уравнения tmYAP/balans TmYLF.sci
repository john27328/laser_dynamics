clear
stacksize('max')
//n3=y(3)
//n2=y(2)
//n1=y(1)
//q=y(4)
N0=4.1e20; //см^-3
K=.87e-17; // см^3*c
A31=360; // cm^-1
A32=40; // cm^-1
A21=67; // cm^-1
W32=0*32; // cm^-1
W21=0;// cm^-1 //неизвестно
sigmaIzl=.25e-24;//.25e20 //cm^2
sigmaPogl=.04e-24;//.035e20 //cm^2
c=3e8; // м/с
L=5;// см //длина резонатора
La=2// см //длина активной области
v=%pi*0.08^2*L; //cm^3 //объем моды в резонаторе
Pnak=80e3;
h=6.62e-34;
R=Pnak/v/(h*3e8/808e-9)/N0;
Va=%pi*0.08^2*La;//под вопросом

Z=0*0.001; //внутренние потери
T1=0;
T2=0.15;
rho=-c/L*(log(1-Z) + (log(1-T1)+log(1-T2))/2);

Bizl=sigmaIzl*c/v;
Bpog=sigmaPogl*c/v;

function dy=balans(t,y)
    dy=zeros(4,1);
    dy(3)=R*y(1)-N0*K*y(1)*y(3)-(A31+A32)*y(3)-W32*y(3);
    dy(2)=2*N0*K*y(1)*y(3)+A32*y(3)+W32*y(3)-W21*y(2)-A21*y(2)-y(4)*(Bizl*y(2)-Bpog*y(1));
    dy(1)=-N0*K*y(1)*y(3)-R*y(1)+A31*y(3)+W21*y(2)+A21*y(2)+y(4)*(Bizl*y(2)-Bpog*y(1));
    dy(4)=N0*Va*(y(4))*(Bizl*y(2)-Bpog*y(1))-y(4)*rho;
endfunction

x0=[1;0;0;1];
t0=0;
t=0:0.0000001:0.06;
y=ode(x0,t0,t,balans);
y4=1-y(1,:)-y(2,:)-y(3,:);
//plot(t,y,leg="n1@n2@n3@q");
clf;
xtitle('населенности','t, с', 'n');
plot(t,y);xgrid;
legend('n1', 'n2', 'n3', 'q', 'n4')
//plot(t,y(2,:));
//n1+n2+n3=1
//R=P_nak/(s*l*h*nu_nak*n1)

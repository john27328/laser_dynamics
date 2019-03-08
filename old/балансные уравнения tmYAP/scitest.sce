clear
a=2*3;b=5.6;
//комментарии
clear b
c=%i*%pi-%e
// %inf - бесконечность %NaN %eps=2,220e-16
printf("%1.22f\n!!!",%pi)
d=....
5*10
//создание функций deff(’[имя1,...,имяN] = имя_функции(переменная_1,...,переменная_M)’,’имя1=выражение1;...;имяN=выражениеN’)
deff('z=fun1(x,y)','z=sqrt(abs(sin(x/y)))*exp(x^y)')
x=1.2;y=0.3;z=fun1(x,y);
deff('[x1,x2]=korni(a,b,c)','d=b^2-4*a*c;x1=(-b+sqrt(d))/2/a;x2=(-b-sqrt(d))/2/a');
[x1,x2]=korni(-2,-3,5)
//созжание функция
//function[имя1,...,имяN]=имя_функции(переменная_1,...,переменная_M)
//тело функции
//endfunction
function [x1,x2,x3]=cub(a,b,c,d)
r=b/a;
s=c/a;
t=d/a;
p=(3*s-r^2)/3;
q=2*r^3/27-r*s/3+t;
D=(p/3)^3+(q/2)^2;
u=(-q/2+sqrt(D))^(1/3);
v=(-q/2-sqrt(D))^(1/3);
y1=u+v;
y2=-(u+v)/2+(u-v)/2*%i*sqrt(3);
y3=-(u+v)/2-(u-v)/2*%i*sqrt(3);
x1=y1-r/3;
x2=y2-r/3;
x3=y3-r/3;
endfunction
[y1,y2,y3]=cub(3,-20,-3,4)

//Функция, описывающая систему дифференциальных уравнений
function dy=syst(t,y)
dy=zeros(2,1);
dy(1)=cos(y(1)*y(2));
dy(2)=sin(y(1)+y(2)*t);
endfunction
//Решение системы дифференциальных уравнений
x0=[0;0];
t0=0;
t=0:0.1:10;
y=ode(x0,t0,t,syst);
//Формирование графического решения
//plot(t,y)

//решение жесткой системы ду
function dx2=syst2(t,x) //Функция задающая систему ОДУ
dx2=zeros(3,1);
dx2(1)=-7*x(1)+7*x(2);
dx2(2)=157*x(1)+x(2)-1.15*x(1)*x(3);
dx2(3)=0.96*x(1)*x(2)-8.36*x(3);
endfunction
//Решение ОДУ
x02=[-1;0;1];t02=0; t2=0:0.01:2;y2=ode("stiff",x02,t02,t2,syst2);
plot(t2,y2); xgrid();

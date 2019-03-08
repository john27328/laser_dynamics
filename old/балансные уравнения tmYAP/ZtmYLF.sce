clear;
E1k=[0 34 65 274 307 325 350 362 415];
E2k=[5600 5751 5827 5944 5967 5977];
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

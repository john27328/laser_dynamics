#include <math.h>
#include <string.h>
#include <stdio.h>

const double eps=.01,x_k=5;
const double H =.0001;
const  double He=H/2.;
const int Nm=10000000;
int N=x_k/H,Ne=x_k/He;
double Nd=N;
double yk[3+1],y[Nm+1],yke[3+1],ye[2*Nm+1];
double k1[3+1],k2[3+1],k3[3+1],k4[3+1];
int k=0;

double f(int k, double x,double y1,double y2,double y3)
{	//printf("f, k=%i\n",k);
        double f0;
        if (k==1) f0=y2;
        if (k==2) f0=y3;
        if (k==3) f0=x*exp(x)-3*y3-3*y2-y1;
        return f0;
};

int kk (int j)
{	//printf("kk\n");
        int i;
        double x,h=H;
        x=h*j;
        for (i=1;i<=3;i++) k1[i]=f(i,x,yk[1],yk[2],yk[3]);
        for (i=1;i<=3;i++) k2[i]=f(i,x+.5*h,yk[1]+.5*h*k1[1],yk[2]+.5*h*k1[2],yk[3]+.5*h*k1[3]);
        for (i=1;i<=3;i++) k3[i]=f(i,x+.5*h,yk[1]+.5*h*k2[1],yk[2]+.5*h*k2[2],yk[3]+.5*h*k2[3]);
        for (i=1;i<=3;i++) k4[i]=f(i,x+h,yk[1]+h*k1[1],yk[2]+h*k1[2],yk[3]+h*k1[3]);
        return 0;
};

int kke (int j)
{	//printf("kk\n");
        int i;
        double x,h=He;
        x=h*j;
        for (i=1;i<=3;i++) k1[i]=f(i,x,yke[1],yke[2],yke[3]);
        for (i=1;i<=3;i++) k2[i]=f(i,x+.5*h,yke[1]+.5*h*k1[1],yke[2]+.5*h*k1[2],yke[3]+.5*h*k1[3]);
        for (i=1;i<=3;i++) k3[i]=f(i,x+.5*h,yke[1]+.5*h*k2[1],yke[2]+.5*h*k2[2],yke[3]+.5*h*k2[3]);
        for (i=1;i<=3;i++) k4[i]=f(i,x+h,yke[1]+h*k1[1],yke[2]+h*k1[2],yke[3]+h*k1[3]);
        return 0;
};

int fyk()
{	//printf("fyk\n");
        int I,J;
        y[0]=yk[1];
        for (J=1;J<=N;J++)
        {
                kk(J);
                for (I=1;I<=3;I++)
                        yk[I]+=H/6.*(k1[I]+2*k2[I]+2*k3[I]+k4[I]);
                y[J]=yk[1];
        }
        ye[0]=yke[1];
        for (J=1;J<=Ne;J++)
        {
                kke(J);
                for (I=1;I<=3;I++)
                       yke[I]+=He/6.*(k1[I]+2*k2[I]+2*k3[I]+k4[I]);
                ye[J]=yke[1];
        }
        for (I=0;I<N;I++) if(fabs(y[I]-ye[2*I])>eps){k=1;printf("|%e-%e|=%e\n",y[I],ye[2*I],fabs(y[I]-ye[2*I]));}
        return 0;
};

int main()
{	FILE *fp;
        fp=fopen("pr3.txt","w");
        int j;
            printf("N=%i",N);
        if(Nm<N) printf("o_O Nm<N!?\n");
        else {
        yk[1]=1;yke[1]=1;
        yk[2]=0;yke[2]=0;
        yk[3]=0;yke[3]=0;
        fyk();  
        if(k<1)
            for(j=0;j<=N;j++)
                fprintf(fp,"%E  %E \n",H*j,y[j]);
        else printf("reduce the step\n");}
        if(fp) fclose(fp);
        return 0;
}

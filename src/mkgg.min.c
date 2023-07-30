#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#define M 1024
#define W 65
#define k long long
#define X(a,e)C(a)P--;P[-1][0]=e;P[-1][1]|=1[*P]
#define N(a,e)X(a,P[-1][0]e**P)
#define x }else{puts("err");return 1;}
#define C(s);}else if(!strcmp(#s,B)){
typedef int(*D)(char(*)[W],k(*)[M][2],k(**)[2],k(*)[M][2],k(**)[2],char(*)[M][W+8+1],k*,char*(*)[M],char***,char(*)[M],void(*)(void));D L[W]={0};k S[M][2],(*P)[2]=S,(*J)[2],A[M][2],(*R)[2]=A,X,t;char B[W]={0},*b,E,F[M][W+8+1]={0},*Q[M]={0},**H=Q,G[M]={0},u[M],*c,**e;FILE*Y;void g(){*B=0;if(H>Q){H--;if(*H&&**H){strcpy(B,*H);*H+=W;if(**H)H++;}}else{b=B;do*b=fgetc(Y);while(*b>32&&*b^';'&&++b-B<W-1);if(*b==59)for(;*b!=10&&!feof(Y);*b=fgetc(Y));E=*b;*b=0;}}char*T(int c,int d){char O[W];do g();while(!*B);d+=*B==91;if(d-=*B==93){strcpy(O,B);return strcpy(T(c+1,d)-W,O);}t=W*c;b=malloc(t+1);b[t]=0;return b+t;}int i(int U){e=H;while(U?e<=H:H>Q||!feof(Y)){g();if(*B==91){**P=T(0,1);1[*P++]=1;}else if(*B==34)printf("%s",B+1);else if(*B==33)*H++ = **--P;else if(*B==58)if(1[*--P]){for(t=0;t<X&&strcmp(F[t],B+1);t++);strcpy(F[t],B+1);memcpy(F[t]+66,*P,8);if(t==X)X++;x else if(*B==37){if(B[1]^47)sprintf(u,"%s\\lib\\%s.dll",G,B+1);else sprintf(u,"%s.dll",B+2);HMODULE l=LoadLibrary(u);if(l){D*q=L;for(;*q;q++);*q=(D)GetProcAddress(l,"mkgg_step");x}else if(*B==35){if(B[1]^47)sprintf(u,"%s\\lib\\%s.mkgg",G,B+1);else sprintf(u,"%s.mkgg",B+2);FILE*j=Y=fopen(u,"r");i(0);fclose(j);Y=stdin;}else if(47<*B&*B<58|*B==45){**P=atoll(B);1[*P++]=0;C(give)memcpy(*R++,*--P,16);C(take)memcpy(*P++,*--R,16);N(add,+)N(and,&)N(or,|)N(mul,*)N(div,/)N(mod,%)X(cmp,(P[-1][0]>**P)-(P[-1][0]<**P))C(roll)t=**--P;for(k i=0;i<t;i++)memcpy(P+i,P-2*t+**(P-t+i),16);memcpy(P-2*t,P,t*16);P-=t;}else if(!strcmp("while",B)|!strcmp("if",B))for(char*c=**--P,*b=**--P,t=*B&8;;){*H++=c;i(1);if(!**--P)break;*H++=b;i(1);if(t)break;}else{t=0;for(D*q=L;*q&&!t;q++)t=(*q)(&B,&S,&P,&A,&R,&F,&X,&Q,&H,&u,&g);for(k s=0;!t&&s<X;s++)if(!strcmp(F[s],B))*H++=*(char**)(F[s]+W+1);}}return 0;}int main(int a,char**b){Y=stdin;strncpy(G,*b,(c=strrchr(*b,'\\'))?c-*b:0);if(!*G)*G='.';return i(0);}
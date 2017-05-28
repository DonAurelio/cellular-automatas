#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>      
#include <iostream>
#include <omp.h>
#include "evo_cell.h"

#define Nx 960
#define Ny 960
#define Nc 48
#define Ns 8
#define PI 3.14159265358979323846
#define Cores 24
#define Xl 122
#define Xs 606
#define Ts 100
/*
0 S
1 SE
2 NE
3 N
4 NW
5 SW
6 Static
7 Wall
 */

//n=s+x*Ns+y*Nx

using namespace std;



void Init_State(bool* Aut_Cel){
  FILE *dskrb;
  // cout<<"Opening border file!\n";
  for(int i=1;i<=4;i++){
    char FileN[50];
    int s;
    s=sprintf(FileN,"1150_%d.dat",i);
    s++;
    dskrb=fopen(FileN,"r+");
    //cout<<FileN<<endl;
    while(!feof(dskrb)){
      char s1[10]="",s2[10]="";
      int err=fscanf(dskrb,"%s %s\n",s1,s2);
      err++;
      int x=atoi(s1);
      int y=atoi(s2);
      //cout<<"Reading the points x="<<x<<" y="<<y<<endl;
      Aut_Cel[7+x*Ns+y*Nx*Ns]=1;
    }
    fclose(dskrb);    
  }
  for(int y=0;y<Ny;y++){
    int x=Nx-1;
    while(!Aut_Cel[7+x*Ns+y*Nx*Ns]){
      for(int s=0;s<7;s++)
	if(rand()%1000/1000.<=2./7.)
	  Aut_Cel[rand()%7+x*Ns+y*Nx*Ns]=1;      
      x--;
      // cout<<"x="<<x<<" y="<<y<<endl;
    }
  }
}

void Time_Step(bool* Aut_Cel_I,bool* Aut_Cel_O){
  int Thr_Num=Cores;
  for(int bloque=0;bloque<Nx*Ny/Cores;bloque++){
    omp_set_dynamic(0);   
#pragma omp parallel num_threads(Cores)
    {
      unsigned long int ID = omp_get_thread_num()+Cores*bloque;   
      //   //cout<<ID<<endl;
      int x = ID%Nx;
      int y = ID/Ny;
      unsigned long int xy=x*Ns+y*Ns*Nx;      
      short int Index_xy_I=0;
      short int Index_xy_O=0;
      bool State_xy_I[8];
      //cout<<"In-Loop variables defined for x="<<x<<" and y="<<y<<endl;  
      for(int s=0;s<Ns;s++){
	//	Aut_Cel_O=[s+xy]=0;
	//cout<<"State Loop. s="<<s<<" xy="<<xy<<endl;
	State_xy_I[s]=Aut_Cel_I[s+xy];
	//cout<<"Mini state = "<<State_xy_I[s]<<endl;
	if(s<7)
	  Index_xy_I+=State_xy_I[s]*(1<<s);
      }
      //cout<<"Index_xy defided = "<<Index_xy_I<<endl;
      if(State_xy_I[7]){
	//cout<<"There is a wall!\n";
	for(int s=0;s<6;s++)
	  Index_xy_O+=State_xy_I[(s+3)%6]*(1<<s);
	Index_xy_O+=State_xy_I[6]*(1<<6);
	Index_xy_O+=128;
      }else
	Index_xy_O=Evo_Cell(Index_xy_I);
      int x_O[Ns],y_O[Ns];	
      x_O[0]=x;
      x_O[1]=(x+1)%Nx;
      x_O[2]=(x+1)%Nx;
      x_O[3]=x;
      x_O[4]=(x-1+Nx)%Nx;
      x_O[5]=(x-1+Nx)%Nx;
      x_O[6]=x;
      x_O[7]=x;
      y_O[0]=(y-1+Ny)%Ny;
      y_O[1]=(y-1+Ny+x%2)%Ny;
      y_O[2]=(y+x%2)%Ny;
      y_O[3]=(y+1)%Ny;
      y_O[4]=(y+x%2)%Ny;
      y_O[5]=(y-1+Ny+x%2)%Ny;
      y_O[6]=y;
      y_O[7]=y;
      //cout<<"States:  Initial="<<Index_xy_I<<" Final="<<Index_xy_O<<endl;
      for(int s=0;s<Ns;s++){
	bool sxy =(Index_xy_O&(1<<s))>>s;
	Aut_Cel_O[s+x_O[s]*Ns+y_O[s]*Ns*Nx]=sxy;
	//cout<<"Address = "<<s+x_O[s]*Ns+y_O[s]*Ns*Nx<<" < "<<Nx*Ny*Ns<<endl; 
	//cout<<"New binary state = "<<sxy<<" for s = "<<s<<endl;
	//cout<<"New coordinates  : x = "<<x_O[s]<<" y = "<<y_O[s]<<endl;
      }
    }
  }
}

int randir(void){
  //  return (rand()%4+3)%6;
  /*int r=rand()%12;
   if(r<7)
    return r;
  else
    switch(r){
    case 7: 
      return 2;
      break;
    case 8:
      return 4;
      break;
    defaul:
      return 3;
      break;
      }*/
  int r=rand()%4;
  switch(r){
  case 0:
    return 3;
    break;
  case 1:
    return 3;
    break;
  case 2:
    return 4;
    break;
  defaut:
    return 4;
    break;
  }
}


int main(int argc, char *argv[]){
  omp_set_dynamic(0);
  bool *Aut_Cel_0;
  bool *Aut_Cel_1;
  FILE *dskw;
  //cout<<"Starting...\n";
  srand (time(NULL));
  Aut_Cel_0=(bool*)calloc(Nx*Ny*Ns,sizeof(bool));
  Aut_Cel_1=(bool*)calloc(Nx*Ny*Ns,sizeof(bool));
  //cout<<"Memory allocated!\n";
  Init_State(Aut_Cel_0);
  //cout<<"Initial state built!\n";
  int Cx=Nc,Cy=Nc;
  int Ntot=0;
  int Ntot_0=0;
  float Vx[Cx][Cy][Ts],Vy[Cx][Cy][Ts];
  int Nm[Cx][Cy][Ts];
  for(int t=0;t<10*Ny;t++){
    //cout<<"Time evolution t="<<t<<endl;
    char FileName[50];
    int err;    
    bool *Aut_Cel_p;
    //    Aut_Cel_p=(bool*)calloc(Nx*Ny*Ns,sizeof(bool));
    //cout<<"Printer memory allocated!\n";    
    //cout<<"Data file created!\n";
    if(t%2){      
      //cout<<"These are odd times!\n";
      Time_Step(Aut_Cel_1,Aut_Cel_0);
    //cout<<"Printer memory allocated!\n";    
      Aut_Cel_p=Aut_Cel_0;
    }else{
      //cout<<"These are even times!\n";
      Time_Step(Aut_Cel_0,Aut_Cel_1);
    //cout<<"Printer memory allocated!\n";    
      Aut_Cel_p=Aut_Cel_1;
    }
    //cout<<"Time step performed!\n";
    //Aut_Cel_p=Aut_Cel_0;
    int dx=Nx/Nc;
    int dy=Ny/Nc;
    float Max_Size=Ts*dx*dy*0.285/(6.);
    int DelP=5;
    //    if(t%100==99)    DelP-=8;
    for(int z=0;z<Nx;z++){
      for(int s=0;s<Ns-1;s++){
	if(Aut_Cel_p[s+z*Ns+0*Nx*Ns])
	  DelP++;
       	Aut_Cel_p[s+z*Ns+0*Nx*Ns]=0;
       	if(Aut_Cel_p[s+z*Ns+(Ny-1)*Nx*Ns])
	  DelP++;
       	Aut_Cel_p[s+z*Ns+(Ny-1)*Nx*Ns]=0;
       	if(Aut_Cel_p[s+0*Ns+z*Nx*Ns])
	  DelP++;
       	Aut_Cel_p[s+0*Ns+z*Nx*Ns]=0;
       	if(Aut_Cel_p[s+(Nx-1)*Ns+z*Nx*Ns])
	  DelP++;
       	Aut_Cel_p[s+(Nx-1)*Ns+z*Nx*Ns]=0;
      }
    }
    float FreeC=Ny+Nx-Xl+Nx-Xs;
    if(t>1)
      for(int j=0;j<Ntot_0-Ntot;j++){
	//Aut_Cel_p[(rand()%3+2)%6+(rand()%(Nx-Xl)+Xl)*Ns+0*Nx*Ns]=1; 
	if(rand()%((int)FreeC)>Nx-Xl){
	  if(rand()%((int)FreeC)>2*Nx-Xs-Xl){
	    int s=(rand()%5+3)%6;
	    if(s==1)
	      s==6;
	    Aut_Cel_p[(rand()%4+3)%6+(Nx-1)*Ns+(rand()%Ny)*Nx*Ns]=1;	  
	  }else{
	    int s=(rand()%4+5)%6;
	    if(s==2)
	      s=6;
	    Aut_Cel_p[+(rand()%(Nx-Xs)+Xs)*Ns+(Ny-1)*Nx*Ns]=1;    
	  }
	}else{
	  int s=(rand()%4+2)%6;
	  if(s==5)
	    s=6;
	  Aut_Cel_p[s+(rand()%(Nx-Xl)+Xl)*Ns+0*Nx*Ns]=1;
	}
      }
    
    //cout<<"Velocities Matrix created!\n";
    if(t==1)
      Ntot_0=Ntot;
    Ntot=0;
    if(t>=Ts){
      err=sprintf(FileName,"./Data/FHP_III_t%06d.dat",t-Ts);
      err++;
      dskw=fopen(FileName,"w+");    	  
      cout<<"Data File Created!\n";
    }
    for(int x_v=0;x_v<Cx;x_v++){
      for(int y_v=0;y_v<Cy;y_v++){
	int Npc=0;
	Vy[x_v][y_v][t%Ts]=0.;
	Vx[x_v][y_v][t%Ts]=0.;			
	Nm[x_v][y_v][t%Ts]=0;
	//cout<<"Velocities cell loop!\n";
	//cout<<"Cell: x="<<x_v<<" y="<<y_v<<endl;
	for(int x=0;x<dx;x++){
	  for(int y=0;y<dy;y++){
	    //cout<<"Full address:";
	    //cout<<"x="<<(x+x_v*dx);
	    //cout<<" y="<<(y+y_v*dy)<<endl;
	    //cout<<"Maximum size ="<<Max_Size<<endl;
	    for(int s=0;s<6;s++){
	      /*	      cout<<"State index = "
			      <<s+(x+x_v*dx)*Ns+(y+y_v*dy)*Ns*Nx<<"<"
			      <<Nx*Ny*Ns<<endl;*/
	      if(Aut_Cel_p[s+(x+x_v*dx)*Ns+(y+y_v*dy)*Ns*Nx]){
		Vy[x_v][y_v][t%Ts]-=cos(s*Xl*PI/180.); 
		Vx[x_v][y_v][t%Ts]+=sin(s*Xl*PI/180.);		
		Nm[x_v][y_v][t%Ts]++;
	      }
	    }
	    if(Aut_Cel_p[6+(x+x_v*dx)*Ns+(y+y_v*dy)*Ns*Nx])
	      Nm[x_v][y_v][t%Ts]++;
	  }
	}
	if(t>=Ts){
	  float Vxa=0.;
	  float Vya=0.;
	  Npc=0;
	  for(int i=0;i<Ts;i++){
	    Vxa+=Vx[x_v][y_v][i];
	    Vya+=Vy[x_v][y_v][i];
	    Npc+=Nm[x_v][y_v][i];
	  }
	  fprintf(dskw, "%d %d %f %f %d %f\n",
		  x_v,y_v,Vxa/Ts,Vya/Ts,
		  Npc,atan2(Vya,Vxa));    
	}
	Ntot+=Nm[x_v][y_v][t%Ts];;
      }
    }
    cout<<t<<" "<<Ntot<<" "<<DelP<<endl;
    //cout<<"File saved!\n";    
    if(t>=Ts){
      fflush(dskw);
      fclose(dskw);
    }
  }
  free(Aut_Cel_0);
  free(Aut_Cel_1);
  return 0;  
}

/*
clear
g++ -fopenmp -O2 FHP_III_v1.0.C -o FHP_III_v1.0
rm ./Data/FHP*.dat 
time ./FHP_III_v1.0 > Ntot.dat
ls ./Data/*.dat > arg-file
rm ./Data/*.png
time parallel -v -j+0 A={} ./Data/parallel_plotter :::: arg-file > images.log
rm *.mp4
time avconv -i ./Data/FHP_III_t%06d.dat.png FHP_III.mp4 > video.log
*/


/*	if(Aut_Cel_p[7+x*Ns+y*Ns*Nx])
	fprintf(dskw,"%d %f %d\n",x,y+(x%2)/2.,2);
	else{
	bool life=0;
	for(int s=0;s<7;s++)
	life=(life^Aut_Cel_p[s+x*Ns+y*Ns*Nx]);
	if(life)
	fprintf(dskw,"%d %f %d\n",x,y+(x%2)/2.,1);*/

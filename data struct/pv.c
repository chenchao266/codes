
#include <stdio.h>
#include<stdlib.h>
#define  TRUE 1
#define  FALSE 0
#define  MAXPRI 100    /* definition of the maxmum priority */
#define  MINPRI 0
#define  NIL -1
struct                      /* define structure "PCB" */
{  int  id;
   char status;
   int  nextwr;
   int  priority;
} pcb[3];
struct                     /* define structure "semaphore" */
{
   int value;
   int firstwr;
} sem[2];
char savearea[3][4],addr;   /* define global variables */
int i,s1,s2,seed,exe=NIL;
FILE *in,*out;              /* define two points of file type */

void init(void);   
int p(int se,char addr);
float random();
int timeint (char addr);
int find();     
int scheduler(); 
int p(int se,char addr);
void blockp(int se);   
int v(int se,char addr);
void wkup(int se);
void process1();
void process2();
void process3();
void extp(int );


void init()                      /* initialization */
{  int j;
   s1=0; 
   s2=0;
   if((in=fopen("Input.txt","r"))==NULL)  /* open file "Input" */
     {  fputs("cannot open file\n",out);
        exit(0);
     }
  for(j=0;j<3;j++)
   {
     pcb[j].id=j;
     pcb[j].status='r';
     pcb[j].nextwr=NIL;
     fprintf(out,"\n process%d priority?   ",j+1);  /* writing data into output file */
     fscanf(in,"%d",&i);             /* reading data from file "Input" */
     fprintf(out,"%d",i);            /* writing priority into output file */
     pcb[j].priority=i;
   }
 fclose(in);                      /* close file "Input" */
 sem[0].value=1; sem[0].firstwr=NIL;
 sem[1].value=1; sem[1].firstwr=NIL;
 for(i=1;i<3;i++)
	for(j=0;j<4;j++)
 		savearea[i][j]='0';
}
float random()                     /* produce function of random data */
{   int m;
    if(seed<0)  m=-seed;
    else  m=seed;
    seed=(25173*seed+13849)%65536;
    return (m/3277.0);
}
int timeint (char addr)               /* time slice interrupt */
{   float x;
    x=random();                     /* call to random */
    if((x<0.33)&&(exe==0)) return(FALSE);
    if((x<0.66)&&(exe==1)) return(FALSE);
    if((x<1.0)&&(exe==2))  return(FALSE);
       /* time slice is end and protect present process state */
   savearea[exe][0]=i;
   savearea[exe][1]=addr;
   pcb[exe].status='t';
   fprintf(out,"Times slice interrupt'\n'process%d enter ready.\n", exe+1);
   exe=NIL;                /* No executiver at present */
   return(TRUE);
}
int find()                     /* define function of find */
{   int j,pd=NIL,w=MAXPRI,h=MINPRI;
    for(j=0;j<3;j++)
     if(pcb[j].status=='r')
       if(pcb[j].priority<w && pcb[j].priority>h)
      {
       w=pcb[j].priority;   pd=j;
      }
    if(pd==NIL)
    for(j=0;j<3;j++)
      if (pcb [j].status=='t')
      if (pcb [j].priority<w && pcb[j].priority>h)
       {
           w=pcb [j].priority;   pd=j;
       }
 return (pd);
 }

int scheduler()               /* difine function of scheduler */
{   int pd;
    if((pd=find())==NIL && exe==NIL)     /* call to find*/
    return(NIL);            /* quit system */
    if(pd!=NIL)             /* there is a ready process */
  {
    if(exe==NIL)
    {
      pcb[pd].status='e';
      exe=pd;
      fprintf(out,"process%d is executing.\n",exe+1);
     }
    /* executing minmum priority of process */
else if(pcb[pd].priority<pcb[exe].priority)  
    {
       pcb[exe].status='r';
       fprintf(out,"process%d enter ready.\n", exe+1);
       pcb[pd].status='e';
       exe=pd;
       fprintf(out,"process%d is executing.\n",exe+1);
    }
  }
  i=savearea[exe][0];         /* recover process scene */
  addr=savearea[exe][1];
  return (exe);
}

int p(int se,char addr)    /* define function of source language "p" */
{
  if (--sem[se].value>=0) return (FALSE);    /* can enter critical section */
  blockp(se);                     /* call to blockp */
  savearea[exe][0] =i;
  savearea[exe][1] =addr;
  exe=NIL;
  return (TRUE);                 /* shift process scheduler */
}
void blockp(int se)                   /* define a funtion of blockp */
{
   int w;
   fprintf (out,"procfess%d is blocked\n",exe+1);
   pcb[exe].status='w';
   pcb[exe].nextwr=NIL;
   if((w=sem[se].firstwr) ==NIL)
       sem[se].firstwr=exe;      /* place it into the first writing process */
   else 
    {     
	      while(pcb[w].nextwr!=NIL)   	 
        w=pcb[w].nextwr;
         pcb[w].nextwr=exe;
    }
}
int v(int se,char addr)      /* define function of source language "V" */
{
  if(++sem[se].value>0)  return (FALSE);   /* no blocked process */
  wkup(se);                              /* call the function of wkup */
  savearea[exe][1]=addr;
  savearea[exe][0]=i;
  return (TRUE);                  /* return or shift process scheduler */
}
void wkup(int se)                     /* define function of wkup */
 {
    int w;
    w=sem[se].firstwr;
    if (w!=NIL)
    {
       sem[se].firstwr=pcb[w].nextwr;
       pcb[w].status='r';            /* the first waiting process is wake up */
       fprintf (out,"process%d is waken up\n",w+1);
    }
 }
void process1()
{
		    /* enter interrupt point */
      if (addr=='a') goto a1;
      if (addr=='b') goto b1;
      if (addr=='c') goto c1;
      if (addr=='d') goto d1;
      if (addr=='e') goto e1;
      if (addr=='f') goto f1;
      for(i=1; i<6; i++)
      {
       fprintf(out,"process1 calls P on the semaphore1\n");
        if(p(0,'a'))    break;        /* process1 is blocked */
    a1:   fputs("process1 is executing in the critical section1\n",out);
        if(timeint('b')) break;        /* time slice interrupt */
    b1:  fprintf(out,"sl=%d\n",++s1);
 	fputs("processl calls V on semaphorel and quit critical section 1,\n",out);
		if(v(0,'c'))  break;        /* wake up a blocked process */
    c1:  fputs("process1 calls P on semaphorel 2.\n",out);
        if(p(1,'d'))  break;
    d1:  fputs("process1 is executing critical section 2.\n",out);
         if(timeint ('e'))  break;
    e1:  fprintf(out,"s2=%d\n",++s2);
        fputs("process1 calls V on semaphore2 and critical section2.\n",out);
        if (v(1,'f'))  break;         /* wake up a blockp process */
    f1:  fprintf(out,"process1 cycle count =%d\n",i);
   }
   if (i<6) return;
      extp(0);
}
void process2()
{
   if (addr=='a')goto a2;
   if (addr=='b')goto b2;
   if (addr=='c')goto c2;
   if (addr=='d')goto d2;
   if (addr=='e')goto e2;
   if (addr=='f')goto f2;
 for(i=1;i<6;++i)
 {
    fputs("process2 calls P on semaphore2\n",out);
       if(p(1,'a'))  break;         /* process2 is blocked */
  a2:  fputs("process2 is executing on the cretical section2\n",out);
       if (timeint ('b'))  break;
  b2:  fprintf(out,"s2=%d\n",++s2);
       fputs("process2 calls V on semaphore2 and quit critical section2.\n",out);
       if (v(1,'c'))  break;       /* wake up a blocked process */
  c2:  fputs("process2 calls P on semaphore1.\n",out);
       if (p(0,'d'))  break;        /* process2 is blocked */
  d2:  fputs("process2 is execting cretical section1.\n",out);
       if (timeint ('e'))  break;
  e2:  fprintf(out,"s1=%d\n",++s1);
       fputs("process2 calls V on semaphore 1 and quit cretical section1.\n",out);
       if (v(0,'f'))  break;        /* wake up a blockp process */
  f2:  fprintf(out,"process2 cycle count=%d\n",i);
  }
     if(i<6)return;
     extp(1);
}
void process3()
{
   if(addr=='a')goto a3;
   if(addr=='b')goto b3;
   if(addr=='c')goto c3;
   for(i=1;i<6;++i)
 {
     fputs("process3 calls P on semaphore2\n",out);
     if (p (1,'a'))  break;           /* process3 is blocked */
 a3: fputs("process3 is executing on its critical section\n",out);
     if (timeint('b')) break;
 b3: fprintf(out,"s2=%d\n",++s2);
     fputs("process3 calls V on semaphore2 and quit critical section.\n",out);
     if (v(1,'c'))  break;            /* wake up a blocked process */
 c3: fprintf(out,"process3 cyclen count=%d\n",i);
  }
  if (i<6)return;
  extp(2);
}
void extp(int n)
{
  pcb[n].status='c';
  fprintf(out,"process%d is completed !\n",n+1);
  exe=NIL;
}
void main()
{
   int k;
   char filename[20];
		    /* set up a file which stores the results */
   fprintf(out,"Input the filename:");
   scanf("%s",filename);
   if((out=fopen(filename,"w"))==NULL)
     {  fputs("cannot open file\n",out);
        exit(0);
     }
   fputs("*************process management **************\n\n",out);
  init();
  fprintf(out,"\ns1=%d,s2=%d\n",s1,s2); /* output the first state of critical section */
  fputs("process1,process2,process3 are all in ready! \n",out);
   for(; ;)
  if((k=scheduler())!=NIL)
  switch(k)
  {
    case 0:  process1();   
            break;
    case 1:  process2();
            break;
    case 2:  process3();
            break;
    default: fputs("process identifer error\n",out);
            break;
    }
  else break;      /* finish process scheduler */
  fprintf(out,"s1=%d,s2=%d\n",s1,s2);  /* output the final state of critical section */
  fputs("\n***********END**************\n",out);
  fclose(out);        /* close file "Output" */
}

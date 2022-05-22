#include <stdio.h>
#include<stdbool.h>
#include<string.h>
#include <stdlib.h>

//show page table 
int showptable(int ptable[16][4]) {
   int i;
for(i=0;i<16;i++){   
   printf("%d:%d:%d:%d\n",ptable[i][0],ptable[i][1],ptable[i][2],ptable[i][3]);      
}  
  return 0;
}

//show main memory page
int showmain(int main[4][8][2] , int page) {
   int i;
for(i=0;i<8;i++){   
   printf("%d:%d\n",main[page][i][0],main[page][i][1]);      
}  
  return 0;
}

//cheking page fault
int checkpfault(int ptable[16][4],int v_address){
   int m =v_address/8;
   // printf("%d",m);

   if(ptable[m][1]==1){      
      return 0;
   }
   else{
      printf("A Page Fault Has Occurred \n");
      return 1;
   }  
}

 //Find LRU Posssion
int findLRU(int time[], int n){
int i, minimum = time[0], pos = 0;
//  printf("kskks");
for(i = 1; i < n; ++i){
if(time[i] < minimum){
minimum = time[i];
pos = i;
}
}
return pos;
}

//LRU Algorithum
void LRU(int pages[],int no_of_pages,int* result){
    int no_of_frames, frames[10], counter = 0, time[10], flag1, flag2, i, j, pos,val, faults = 0;
no_of_frames=4;

    
for(i = 0; i < no_of_frames; ++i){
     frames[i] = -1;
    }
    
    for(i = 0; i < no_of_pages; ++i){
     flag1 = flag2 = 0;
    
     for(j = 0; j < no_of_frames; ++j){
     if(frames[j] == pages[i]){
     counter++;
     time[j] = counter;
   flag1 = flag2 = 1;
   pos=j;
   val=pages[i];
   break;
   }
     }
    
     if(flag1 == 0){
for(j = 0; j < no_of_frames; ++j){
     if(frames[j] == -1){
     counter++;
     faults++;
     frames[j] = pages[i];
     time[j] = counter;
     flag2 = 1;
     pos=j;
     val=-1;
     break;
     }
     }
     }
    
  if(flag2 == 0){
      
     pos = findLRU(time, no_of_frames);
     counter++;
     faults++;
     val=frames[pos] ;
     frames[pos] = pages[i];
     time[pos] = counter;
     }      
     
}  

 result[0]=pos;
 result[1]=val;
 
}

//FIFO Algorithum
void FIFO(int referenceString[],int pages,int* result){

int pageFaults = 0, m, n, s, pos,val,frames;
frames=4;

int temp[frames];
for(m = 0; m < frames; m++)
{
  temp[m] = -1;
}

for(m = 0; m < pages; m++)
{
  s = 0;
  for(n = 0; n < frames; n++)
   {
      if(referenceString[m] == temp[n])
         {
            s++;
            pageFaults--;

            val=temp[n];
            pos=n;
         }
   }    

   pageFaults++;
   if((pageFaults <= frames) && (s == 0))
      {
           val=temp[m];
        temp[m] = referenceString[m];
       
            pos=m;
      }   
   else if(s == 0)
      {
          val=temp[(pageFaults - 1) % frames];
        temp[(pageFaults - 1) % frames] = referenceString[m];
        
            pos=(pageFaults - 1) % frames;
      }
  

} 
   result[0]=pos;
   result[1]=val;

}



// main function 
#include <stdio.h>

int main( int argc, char *argv[] )  {   

const char *page_repl_algo="FIFO";
int input_v_address[30];
int no_input=0;
   if( argc == 2 ) {
      if(strcmp( argv[1], "FIFO") == 0){
         page_repl_algo=argv[1];      
      }
      else if (strcmp( argv[1], "LRU") == 0){
          page_repl_algo=argv[1];       
      }
   }
  
bool end=false;
int ptable[16][4]={{0,0,0,0},{1,0,0,1},{2,0,0,2},{3,0,0,3},{4,0,0,4},{5,0,0,5},{6,0,0,6},{7,0,0,7},{8,0,0,8},{9,0,0,9},{10,0,0,10},{11,0,0,11},{12,0,0,12},{13,0,0,13},{14,0,0,14},{15,0,0,15}};  

int main[4][8][2]={{{0,-1},{1,-1},{2,-1},{3,-1},{4,-1},{5,-1},{6,-1},{7,-1}},{{8,-1},{9,-1},{10,-1},{11,-1},{12,-1},{13,-1},{14,-1},{15,-1}},{{16,-1},{17,-1},{18,-1},{19,-1},{20,-1},{21,-1},{22,-1},{23,-1}},{{24,-1},{25,-1},{26,-1},{27,-1},{28,-1},{29,-1},{30,-1},{31,-1}}};
char command[255];

while(!end){
   
   printf("> ");
   scanf(" %[^\n]s", command);   
   
    if (strcmp(command, "showptable") == 0)
    {
	   showptable(ptable);   
   
    }    
    else if ( strcmp(command, "quit") == 0)
    {	       
        return 0;        
    }
    else{
  
      char * token = strtok(command, " ");
   
      if(strcmp( page_repl_algo, "FIFO") == 0){       
  
      if(strcmp(token, "write") == 0){         


         // printf("write \n");
         int virtual_add = atoi(strtok(NULL, " "));
         // printf( "address %d\n", virtual_add );

         input_v_address[no_input]=virtual_add/8;
         

         int value = atoi(strtok(NULL, " "));
         // printf( "value %d\n", value );

         //cheking page fault
         int check=checkpfault(ptable,virtual_add);

         no_input=no_input+1;
         //update ptable and main memory
         if(check==1){
            int x[2];
            FIFO(input_v_address,no_input,x);
            // ptable[virtual_add/8][1]=1;
            ptable[virtual_add/8][2]=0;
            ptable[virtual_add/8][3]=x[0];

            for(int i=0;i<8;i++){  
            main[x[0]][i][1]=-1;   
            }  

            main[ptable[virtual_add/8][3]][(virtual_add%8)][1]=value;
            ptable[virtual_add/8][1]=1;
            ptable[virtual_add/8][2]=1;

            if(x[1]!=-1){           
            ptable[x[1]][1]=0;
            ptable[x[1]][3]=x[1];
            ptable[x[1]][2]=0;
         }

         }
         else{           
            ptable[virtual_add/8][2]=1;
            main[ptable[virtual_add/8][3]][(virtual_add%8)][1]=value;
            ptable[virtual_add/8][1]=1;
         }        

      }
      else if (strcmp(token, "read") == 0){
         
          
         // printf("read \n");
         int virtual_add = atoi(strtok(NULL, " "));
         // printf( "address %d\n", virtual_add );

         //cheking page fault
         int check=checkpfault(ptable,virtual_add);

         input_v_address[no_input]=virtual_add/8;
         no_input=no_input+1;

         //print address vale
         if(check==1){
            int x[2];
            FIFO(input_v_address,no_input,x);
            printf("-1\n");

             //update ptable          
            ptable[virtual_add/8][3]=x[0];
           
            for(int i=0;i<8;i++){   
               main[x[0]][i][1]=-1;     
            }  

            ptable[virtual_add/8][1]=1;

            if(x[1]!=-1){
               ptable[x[1]][1]=0;
               ptable[x[1]][3]=x[1];
            }
         }
         else{
            printf("%d\n",main[ptable[virtual_add/8][3]][(virtual_add%8)][1]);
         }

      }
       else if (strcmp(token, "showmain") == 0){
         // printf("showmain \n");

         int ppn = atoi(strtok(NULL, " "));
         // printf( "ppn %d\n", ppn );
         showmain(main,ppn);
      }

      }
      else if (strcmp( page_repl_algo, "LRU") == 0){      

  
      if(strcmp(token, "write") == 0){       


         // printf("write \n");
         int virtual_add = atoi(strtok(NULL, " "));
         // printf( "address %d\n", virtual_add );

         input_v_address[no_input]=virtual_add/8;
         

         int value = atoi(strtok(NULL, " "));
         // printf( "value %d\n", value );

         //cheking page fault
         int check=checkpfault(ptable,virtual_add);

         no_input=no_input+1;
         //update ptable and main memory
         if(check==1){
            int x[2];
            LRU(input_v_address,no_input,x);
         
            ptable[virtual_add/8][2]=0;
            ptable[virtual_add/8][3]=x[0];

            for(int i=0;i<8;i++){  
               main[x[0]][i][1]=-1;     

            }

            main[ptable[virtual_add/8][3]][(virtual_add%8)][1]=value;
            ptable[virtual_add/8][1]=1;
            ptable[virtual_add/8][2]=1;

            if(x[1]!=-1){             
            ptable[x[1]][1]=0;
            ptable[x[1]][3]=x[1];
            ptable[x[1]][2]=0;
         }
         }
         else{          
            ptable[virtual_add/8][2]=1;
            main[ptable[virtual_add/8][3]][(virtual_add%8)][1]=value;
            ptable[virtual_add/8][1]=1;
         }        

      }
      else if (strcmp(token, "read") == 0){
         
          
         // printf("read \n");

         int virtual_add = atoi(strtok(NULL, " "));
         // printf( "address %d\n", virtual_add );

         //cheking page fault
         int check=checkpfault(ptable,virtual_add);

          input_v_address[no_input]=virtual_add/8;
         no_input=no_input+1;

         //print address vale
         if(check==1){
            int x[2];
            LRU(input_v_address,no_input,x);
            printf("-1\n");

             //update ptable          
            ptable[virtual_add/8][3]=x[0];

            for(int i=0;i<8;i++){   
               main[x[0]][i][1]=-1;     
            }  

            ptable[virtual_add/8][1]=1;

            if(x[1]!=-1){
               ptable[x[1]][1]=0;
               ptable[x[1]][3]=x[1];
            }
         }
         else{
            printf("%d\n",main[ptable[virtual_add/8][3]][(virtual_add%8)][1]);
         }

      }
       else if (strcmp(token, "showmain") == 0){
         // printf("showmain \n");

         int ppn = atoi(strtok(NULL, " "));
         // printf( "ppn %d\n", ppn );

         showmain(main,ppn);
      }
    
      }

}  
};
}


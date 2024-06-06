// bimodal and gshare
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
using namespace std;

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
vector< long int > bimod;
vector< long int > gshare; 
vector< long int > hybrid;
class predictor{
    public:
   // int M1,N,K;
    unsigned long int index,M1,N,K;
    bool flag;
    int prediction=0,mispredictions=0,max=3,min=0,taccess=0,bhr=0;
   

    void print_vector(){  
        printf("OUTPUT\n");
        printf("number of predictions:\t\t%d\n",taccess);
        printf("number of mispredictions:\t%d\n",mispredictions);
        printf("misprediction rate:\t\t%.2f%%\n", (float)mispredictions / taccess* 100);
    }
    void print_bimod(){
        print_vector();
        printf("FINAL BIMODAL CONTENTS\n");
	//sort(bimod.begin(),bimod.end(),sortcol);
        //cout<<"Size of vector: "<<bimod.size()<<endl;
          
        for(int i=0;i<bimod.size();i++)
        {
            cout<<i<<"\t"<<bimod[i];
            cout<<"\n";
        }
}   
    void print_gshare(){
       print_vector();
       printf("FINAL GSHARE CONTENTS\n");
	//cout<<"\n"<<gshare.size()<<endl;
       	//sort(gshare.begin(),gshare.end(),sortcol);
          
        for(int i=0;i<gshare.size();i++)
        {
            cout<<i<<"\t"<<gshare[i];
            cout<<"\n";
        }
} 
void print_hybrid(){
       print_vector();
       printf("FINAL CHOOSER CONTENTS\n");
	//cout<<"\n"<<gshare.size()<<endl;
       	//sort(gshare.begin(),gshare.end(),sortcol);
          
        for(int i=0;i<hybrid.size();i++)
        {
            cout<<i<<"\t"<<hybrid[i];
            cout<<"\n";
        }

	printf("FINAL GSHARE CONTENTS\n");
	//cout<<"\n"<<gshare.size()<<endl;
       	//sort(gshare.begin(),gshare.end(),sortcol);
          
        for(int i=0;i<gshare.size();i++)
        {
            cout<<i<<"\t"<<gshare[i];
            cout<<"\n";
        }

	 printf("FINAL BIMODAL CONTENTS\n");
	//sort(bimod.begin(),bimod.end(),sortcol);
        //cout<<"Size of vector: "<<bimod.size()<<endl;
          
        for(int i=0;i<bimod.size();i++)
        {
            cout<<i<<"\t"<<bimod[i];
            cout<<"\n";
        }
}
    void share(unsigned long int addr,unsigned long int indexbits,unsigned long int n ,char outcome){        //Gshare
	unsigned long int gindex;
	if(n==0){
   unsigned long int temp_1 = pow(2,indexbits)-1;
   gindex=(addr=addr>>2)&(temp_1);//Mi
   gshare.resize((pow(2,indexbits)),2);
      }
	else{
    unsigned long int temp_1 = pow(2,indexbits)-1;
    index=(addr=addr>>2)&(temp_1);//M
    unsigned long int temp=(index>>(indexbits-n));
    gindex=bhr^temp;
    unsigned long int var = (pow(2,indexbits-n)-1);
    var = index & var;
    gindex=(gindex<<(indexbits-n))|var;
    gshare.resize((pow(2,indexbits)),2);
}
	
     if(gshare[gindex]>=2){
                prediction=1;}
            else{
                prediction=0;
            }
        if(outcome=='t'){                       //Branch taken
              if(gshare[gindex]<max){
                gshare[gindex]++;
            }               
            if(prediction==1){
                //correct
            }
            else{
                mispredictions++;
		}
        }
        else{                                   //Branch not taken
            if(gshare[gindex]>min){
                gshare[gindex]--;
            }          
            if(prediction==0){
                //correct
            }            
            else{
                mispredictions++;
            }  
        }
         taccess++;
         if(outcome=='t'){
            bhr=((bhr>>1)|(1<<(n-1)));
         }
         else{
            bhr=((bhr>>1)|0);
         }
         return;

    }
void bimodal(unsigned long int addr,unsigned long int indexbits,char outcome){                  //Bimod
   unsigned long int temp_1 = pow(2,indexbits)-1;
   index=(addr=addr>>2)&(temp_1);//Mi
   bimod.resize((pow(2,indexbits)),2);
     if(bimod[index]>=2){
                prediction=1;}
            else{
                prediction=0;
            }
        if(outcome=='t'){                       //Branch taken
              if(bimod[index]<max){
                bimod[index]++;
            }               
            if(prediction==1){
                //correct
            }
            else{
                mispredictions++;
		}
        }
        else{                                   //Branch not taken
            if(bimod[index]>min){
                bimod[index]--;
            }          
            if(prediction==0){
                //correct
            }            
            else{
                mispredictions++;
            }  
        }
         taccess++;
         return;
}
void chooser(unsigned long int addr,unsigned long int k,unsigned long int m1,unsigned long int n ,unsigned long int m2,char outcome){        //hybrid
    
    int max=3,min=0; 
    int bpredict =0, gpredict =0, hpredict =0;
    unsigned long int temp_b=0,temp_h=0;
    
            unsigned long int address = 0;
            address = addr >> 2;
            long bindex = address % (long)pow(2, m2); //M

            unsigned long int index; 
            addr = addr >> 2;
            unsigned long int temp = (1 << m1) - 1; 
            index = addr & temp;
            int num_bits = (1 << (m1 - n)) - 1;
            unsigned long int lbits=(index)&(num_bits);
            unsigned long int ubits=(index)>>(m1-n);
            long gindex = ((bhr^ubits)<<(m1-n))|lbits;

            unsigned long int hybridaddress = 0;
            hybridaddress = addr >> 2;
            long hindex = address % (long)pow(2, k);    
    if(bimod[bindex] >= 2){
	bpredict = 1;
    }
    else{
	bpredict = 0;
    }
    if(gshare[gindex] >= 2){
	gpredict = 1;
    }
    else{
	gpredict = 0;
    }


    if(hybrid[hindex] >= 2){
            if((outcome == 't') && (gshare[gindex] < max)){
		gshare[gindex]++;
		}     
	   else if ((outcome =='n') && (gshare[gindex] > min)){
		gshare[gindex]--;		
		}
	hpredict = gpredict;
    }
    else{
          if((outcome == 't') && (bimod[bindex] < max)){
		bimod[bindex]++;
		}     
	   else if ((outcome =='n') && (bimod[bindex] > min)){
		bimod[bindex]--;		
		}
	hpredict = bpredict;
    }
         
    if(outcome=='t'){
        bhr=((bhr>>1)|(1<<(n-1)));
    }
    else if(outcome=='n'){
        bhr=(bhr>>1);
    }
    bhr = (bhr) & ((1 << n) - 1);
    if(outcome =='t'){
	if(bpredict == 0 && gpredict == 1){
		if(hybrid[hindex] < max)
			hybrid[hindex]++;
	}
	else if(bpredict == 1 && gpredict == 0){
		if(hybrid[hindex] > min)
			hybrid[hindex]--;
	}
     }
    else if (outcome == 'n'){
	if(bpredict == 1  && gpredict == 0){
		if(hybrid[hindex] < max)
			hybrid[hindex]++;
	}
	else if(bpredict == 0 && gpredict == 1){
		if(hybrid[hindex] > min)
			hybrid[hindex]--;
	}
    }
    
    if((hpredict == 0 && outcome =='t') || (hpredict == 1 && outcome == 'n')){
	mispredictions++;
	}
    taccess++;
    
}
};
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);

    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);

    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    char str[2];
     predictor p ;
//	bhr=(bhr)&(params.N-1);
	int status=0;
 bimod.resize((pow(2,params.M2)),2);
    	     gshare.resize((pow(2,params.M1)),2);
             hybrid.resize((pow(2,params.K)),1);

    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        
        outcome = str[0];
        //if (outcome == 't')
            //printf("%lx %s\n", addr, "t");           // Print and test if file is read correctly
        //else if (outcome == 'n')
           // printf("%lx %s\n", addr, "n");          // Print and test if file is read correctly
        /*************************************
            Add branch predictor code here
        **************************************/
       if(strcmp(params.bp_name,"bimodal")==0){
            p.bimodal(addr,params.M2,outcome);
		    status=1;
        }
       else if(strcmp(params.bp_name,"gshare")==0){
            p.share(addr,params.M1,params.N,outcome);
	        status=2;
        }
       else if(strcmp(params.bp_name,"hybrid")==0){	
	    

            p.chooser(addr,params.K,params.M1,params.N,params.M2,outcome);
            status = 3;
        }

    }
	if(status==1){
   // cout<<"Size of vector: "<<bimod.size()<<endl;
	p.print_bimod();}
	else if(status==2){
        p.print_gshare();}
    else if(status ==3){
        p.print_hybrid();
    }
	else
	cout<<"Incorrect"<<endl;
    return 0;
}


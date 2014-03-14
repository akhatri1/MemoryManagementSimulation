#include<iostream>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<math.h>
#include<stdlib.h>
using namespace std;

class memoryManagementAlgorithms{
private: 
	int logicalSpace, physicalSpace, sizeOfSeq;
	char pageRef[100];
	char *physicalFrames;
	public:
	memoryManagementAlgorithms(int ls, int ps, char refString[], int k)
	{ 
		logicalSpace = ls; 
		physicalSpace = ps;
	   	strcpy(pageRef, refString); 
		sizeOfSeq = k;
		physicalFrames = new char[physicalSpace];
		
		for(int i=0; i < physicalSpace; i++)
		{
			physicalFrames[i] = 'x';
		}
	}
	void display()
	{
		cout<< "logical space is " <<logicalSpace<<endl;
		cout<< "physical space is " <<physicalSpace<<endl;
		cout<< "size of sequence is " <<sizeOfSeq<<endl;
		for(int i=0; i < strlen(pageRef); i++)
		{
			cout<<pageRef[i]<<"\t";
		}
	}

   	int LogicFIFO(void);
	int LogicLRU(void);
	int LogicOPTIMAL(void);
	int LogicNFU(void);
};

int memoryManagementAlgorithms::LogicFIFO(){
	
	int noPageFault=0, hit=0;
	int r=-1, f=-1;
	bool found = false;

	for(int i=0; i < sizeOfSeq;  i++)
	{
		char page = pageRef[i];
		found = false;
		for(int j=0; j < physicalSpace; j++)
		{
			if(physicalFrames[j] == page)
			{
				cout<<page<<": page found at frame  "<<j<<endl;
				found = true;
				noPageFault++;
			}
		}

		if(!found)
		{
			if(r==-1 && f==-1)
			{
				physicalFrames[0] = page;
				r = f = 0;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<r<<endl;
				cout<<page<<": page found at frame  "<<r<<endl;
			}
			else if( (r+1)%physicalSpace == f)
			{
				f= (f+1) % physicalSpace;
				r= (r+1) % physicalSpace;
				physicalFrames[r] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<r<<endl;
				cout<<page<<": page found at frame  "<<r<<endl;
			}
			else
			{
				r= (r+1) % physicalSpace;
				physicalFrames[r] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<r<<endl;
				cout<<page<<": page found at frame  "<<r<<endl;
			}
		}

	}
	cout<<"The FIFO Output:"<<sizeOfSeq<<" "<<noPageFault << endl;
        return noPageFault;
}


int memoryManagementAlgorithms::LogicLRU(){
	
	int noPageFault=0, hit=0;
	int top = -1;
	bool found = false;
	char * stack = new char[physicalSpace]();
	for(int i=0; i < physicalSpace; i++)
	{
		stack[i] = 'x';
	}

	for(int i=0; i < sizeOfSeq;  i++)
	{

			/*for(int p = 0 ; p < physicalSpace; p++)
			{
				cout<<"data   "<<physicalFrames[p]<<"  "<<stack[p]<<endl; 	
			}*/

		char page = pageRef[i];
		found = false;
		for(int j=0; j < physicalSpace; j++)
		{

			if(physicalFrames[j] == page)
			{
				cout<<page<<": page found at frame  "<<j<<endl;
				found = true;
				noPageFault++;
				
				int v = 0;
				for(int k=0; k < physicalSpace; k++)
				{
					if(stack[k] == page)
					{
						v=k;
					}
				}
				if(v < (physicalSpace-1))
				{
					char temp = stack[v];
					int k = 0;
					for(k=v; k <(physicalSpace-1); k++)
					{
						if(stack[k+1] == 'x')
								break;
						
						stack[k] = stack[k+1];
					}
					stack[k] = temp;
					//top = k;
				}
			}
		}

		if(!found)
		{
			if(top < (physicalSpace-1))
			{
				++top;
				physicalFrames[top] = page;
				stack[top] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<top<<endl;
				cout<<page<<": page found at frame  "<<top<<endl;
			}
			else
			{
				for(int k=0; k < physicalSpace; k++)
				{
					if(physicalFrames[k] == stack[0])
					{
						physicalFrames[k] = page;
						cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<k<<endl;
						cout<<page<<": page found at frame  "<<k<<endl;

					}
				}
				for(int k=0; k <(physicalSpace-1); k++)
				{
					stack[k] = stack[k+1];
				}
				stack[physicalSpace-1] = page;
				

			}
			
		}

	}
	cout<<"The LRU Output:"<<sizeOfSeq<<" "<<noPageFault<<endl;
	return noPageFault;
}


int memoryManagementAlgorithms::LogicOPTIMAL(){
	
	int noPageFault=0, hit=0;
	int top = -1;
	bool found = false;
	int * nextUse = new int[physicalSpace];
	for(int i=0; i < sizeOfSeq;  i++)
	{

			/*for(int p = 0 ; p < physicalSpace; p++)
			{
				cout<<"data   "<<physicalFrames[p]<<endl; 	
			}*/

		char page = pageRef[i];
		found = false;
		for(int j=0; j < physicalSpace; j++)
		{

			if(physicalFrames[j] == page)
			{
				cout<<page<<": page found at frame  "<<j<<endl;
				found = true;
				noPageFault++;
			}
		}

		if(!found)
		{
			if( top < physicalSpace - 1)
			{
				physicalFrames[++top] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<top<<endl;
				cout<<page<<": page found at frame  "<<top<<endl;
			}
			else
			{
				for(int j=0; j< physicalSpace; j++)
				{
					int k = 0;
					for(k=i+1; k < sizeOfSeq; k++)
					{
						if(physicalFrames[j] == pageRef[k])
						{
							nextUse[j] = k;
							
							break;
						}
					}
					if(k == sizeOfSeq)
						nextUse[j] = 1111; // using this as infinity
				}
				
				int max = -1111, maxIndex = -1;
				for(int j=0; j< physicalSpace; j++)
				{
					if(nextUse[j] >= max)
					{
						max = nextUse[j];
						maxIndex = j;
					}
				}
				//cout<< "*** Page " << physicalFrames[maxIndex] << " will be used most late at "<< nextUse[maxIndex]<<endl;
				physicalFrames[maxIndex] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<maxIndex<<endl;
				cout<<page<<": page found at frame  "<<maxIndex<<endl;
			}
			
		}

	}
	cout<<"The Optimal Output:"<<sizeOfSeq<<" "<<noPageFault<<endl;
	return noPageFault;
}

int memoryManagementAlgorithms::LogicNFU()
{

	int noPageFault=0, hit=0;
	int top = -1, value,min = 50,minIndex;
	bool found = false;
	int * counterLogicalSpace = new int[logicalSpace]();
	int * counterPhysicalFrames = new int[physicalSpace]();
	

	for(int i=0; i < sizeOfSeq;  i++)
	{
		char page = pageRef[i];
		found = false;
		for(int j=0;j<physicalSpace;j++)
		{
				if(physicalFrames[j] == page)
				{
					cout<<page<<": page found at frame  "<<j<<endl;
					found = true;
					noPageFault++;
                                	counterLogicalSpace[page] += 1;
					/*cout << "In found" <<  counterLogicalSpace[page] << endl;
					cout << endl;*/
				}
		}
		if(!found)
		{
			
			if( top < physicalSpace - 1)
			{
				physicalFrames[++top] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<top<<endl;
				cout<<page<<": page found at frame  "<<top<<endl;
                                counterLogicalSpace[page] += 1;
                                /*cout << "In adding elements" << counterLogicalSpace[page]<< endl;
				cout << endl;*/
			}
			else
			{	
				for(int k=0; k < physicalSpace; k++)
				{
					value = physicalFrames[k];
					counterPhysicalFrames[k] = counterLogicalSpace[value];
                                        //cout << counterPhysicalFrames[k] << endl;
				}
				// Find the minimum counterPhysicalFrames value 
				min = 100;
				for(int j=0; j< physicalSpace; j++)
				{
					if(counterPhysicalFrames[j] < min)
					{
						min = counterPhysicalFrames[j];
						minIndex = j;
					}
				}
				//cout << "This is minIndex" << minIndex << endl;
				physicalFrames[minIndex] = page;
				cout<<page<<": PAGE FAULT -- inserting page "<< page <<" into frame "<<minIndex<<endl;
				cout<<page<<": page found at frame  "<<minIndex<<endl;
				counterLogicalSpace[page] += 1;
                                /*cout << "In replacing the element" << counterLogicalSpace[page] << endl;
				cout << endl;*/
			}
		}
	}
        cout <<"The NRU Output:"<< sizeOfSeq << " " << noPageFault << endl;
	return noPageFault;
}
                                
	



int main(){

  int algoChoice, pages_logicalMem, pageFrames_physicalMem, intD = -1, i, k=0,flag=0;
  char pageRefString[501], delim = (char)intD, *pageRefStringTok;
  char  pageRefStringInt[501];
  int value;

   cout << "Choose the replacement Algorithm you want to simulate" << endl;
   cout << "\t1.FIFO" << endl;
   cout << "\t2.LRU" << endl;
   cout << "\t3.Optimal" << endl;
   cout << "\t4.NFU" << endl;
   cout << "\t5.Comparision" << endl;
   cin >> algoChoice;
   
   cout << "Enter the number of Pages in the logical memory" << endl;
   cin >> pages_logicalMem;
   cout << "Enter the number of the Page frames in the Physical Memory" << endl;
   cin >> pageFrames_physicalMem;
   getchar();
   cout << "Enter the page reference string:" << endl;
   cin.getline(pageRefString,200);
   
   pageRefStringTok = strtok(pageRefString," ");
   while(pageRefStringTok != NULL)
   {				
	   pageRefStringInt[k] = (char) *pageRefStringTok;
	   k++;
	   pageRefStringTok = strtok(NULL," ");
   }
    
   //pageRefStringInt[k] = (char)'\0';

   for(i=0;i<k;i++)
   {
	 
	if(pageRefStringInt[i] == '-')
	{
		pageRefStringInt[i] = (char)'\0';
		flag =1;
	}
	
   }
   if(!flag)
   {
	pageRefStringInt[k] = (char)'\0';
   }
   k = strlen(pageRefStringInt);
   for(i=0;i<k;i++)
   {
          
          value =(int)pageRefStringInt[i] - 48;
          if(pages_logicalMem <= value)
	{
		printf("\n Enter the input page reference string correctly\n");
		exit(0);
	}
   }
   if(algoChoice == 1)
   {
   	memoryManagementAlgorithms objFIFO(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	objFIFO.LogicFIFO();
   } 
   if(algoChoice == 2)
   {
   	memoryManagementAlgorithms objLRU(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	objLRU.LogicLRU();
   }
   if(algoChoice == 3)
   {
   	memoryManagementAlgorithms objOPTIMAL(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	objOPTIMAL.LogicOPTIMAL();
   }
   if(algoChoice == 4)
   {
   	memoryManagementAlgorithms objNFU(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	objNFU.LogicNFU();
   }
   if(algoChoice == 5)
   {
	memoryManagementAlgorithms objFIFO(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	int hit_FIFO = objFIFO.LogicFIFO();
	memoryManagementAlgorithms objLRU(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	int hit_LRU = objLRU.LogicLRU();
	memoryManagementAlgorithms objOPTIMAL(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	int hit_Optimal = objOPTIMAL.LogicOPTIMAL();
	memoryManagementAlgorithms objNFU(pages_logicalMem, pageFrames_physicalMem, pageRefStringInt, k); 
	int hit_NFU = objNFU.LogicNFU();
        cout << (float)hit_FIFO/k <<" "<<(float)hit_LRU/k<<" "<< (float)hit_Optimal/k<<" "<< (float)hit_NFU/k << endl;
   }
	

   return 0;
}

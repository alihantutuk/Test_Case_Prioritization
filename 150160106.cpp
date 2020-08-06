// Mehmet Ali Han Tutuk
// 150160106
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <bits/stdc++.h> 
#include <vector>
#include <sstream>

# define MAX_DATA 50
# define DATA_ARRAY_SIZE 100
using namespace std;

int max_running_time;
int max(int a, int b) { return (a > b)? a : b; } // finding max value
int min(int x, int y, int z) { return (((x<y) ? x:y) < z )? ((x<y) ? x:y): z ; } // finding min value

class Data{
private:
string testSuiteId;
int bugsDetected;
int runningTime;
public:
static int values[DATA_ARRAY_SIZE];  //decleration
static int weights[DATA_ARRAY_SIZE];  //decleration
static int result;  //decleration
static int index; //decleration
static int totalRunningTime;  //decleration
static string points[DATA_ARRAY_SIZE];  //decleration
static int hold_result_index[DATA_ARRAY_SIZE]; //decleration
int** testCaseFreqProfiles;
int** sortedFreqProfileIndices;
int* freqProfileTotalNumber;
int ** freqProfileIndices;
int totalMaxNumberIndice;
int freq_x; //  number of testCaseFreqProfiles x-axis
int freq_y; //  number of testCaseFreqProfiles y-axis
int same;
int SolveDistance(int* X, int* Y, int n);
void printResult();
void createFreqProfileIndices();
void sortIndices();
int findMaxIndice();
void insertionSort(int arr[],int arr2[], int n); //Ascending order Augmented Insertion Sort
void insertionSortReverse(int arr[],int arr2[], int n); // Descending order Augmented Reverse Insertion Sort

Data(){ // constructor
	testCaseFreqProfiles= new int*[MAX_DATA];
	freqProfileTotalNumber= new int[MAX_DATA];
	for(int i=0;i<MAX_DATA;i++){
		testCaseFreqProfiles[i]= new int[MAX_DATA];
	}
	freq_x=0; 
	freq_y=0;
	same=0;
	totalMaxNumberIndice=0;
}
~Data(){ // Destructor
	for(int i=0;i<MAX_DATA;i++){
		delete []testCaseFreqProfiles[i];
	}
	delete [] testCaseFreqProfiles;
	delete [] freqProfileTotalNumber;
}

string get_testSuiteId(){ return testSuiteId;}
void set_testSuiteId(string value){testSuiteId.assign(value);}
int get_bugsDetected(){ return bugsDetected; }
void set_bugsDetected(int value){bugsDetected=value;}
int get_runningTime(){ return runningTime;}
void set_runningTime(int value){runningTime=value;}

};
int Data::result=0;   //definition
int Data::index=0;   // definition
int Data::totalRunningTime=0; //definition
string Data::points[]; // definitiom
int Data::values[]={0};  //assign static member array 0
int Data::weights[]={0}; //assign static member array 0
int Data::hold_result_index[]={0}; //assign static member array 0


void Solve(int W, int wt[], int val[], int n, Data *); // Finding test suites  and maximum bugs number

int main(int argc,char ** argv){
	if(argc>0){  // For -Wall -Werror section
    int i=0;
    i++;
}
	Data * array = new Data[DATA_ARRAY_SIZE];
	int total_object=0;
	fstream file;
    string file_name=argv[1];
	file.open(file_name);
	string str_input;
	int counter=0,counter2=0,counter3=-1;
	while(!file.eof()){ //reading text file and save the values to spesific appropriate variables
		counter++;
		file >> str_input;
		if (counter==5) max_running_time=stoi(str_input);
		
		if (counter2==1){
			Data::values[counter3]=stoi(str_input);
			array[counter3].set_bugsDetected(stoi(str_input));	
			counter2++;
		} 
		else if(counter2==2) {
			Data::weights[counter3]=stoi(str_input);
			array[counter3].set_runningTime(stoi(str_input));
			counter2++;
		}
		else if(counter2>2 && str_input[0] !='T' ){
			vector <string> tokens; 
			stringstream check1(str_input); 	
			string intermediate; 
			
			while(getline(check1, intermediate, '-')){ 
				tokens.push_back(intermediate); 
			}
			array[counter3].freq_y=tokens.size();
			for(unsigned int i = 0; i < tokens.size(); i++) {
				array[counter3].testCaseFreqProfiles[array[counter3].freq_x][i]=stoi(tokens[i]);
				if(stoi(tokens[i])!=0){
					array[counter3].freqProfileTotalNumber[array[counter3].freq_x]++;
				}
				//array[counter3].freqProfileTotalNumber[array[counter3].freq_x]+=stoi(tokens[i]);
			}
			array[counter3].freq_x++;	
		}
		if(str_input[0]=='T' && str_input[1]=='S') {
			counter2=1; counter3++;
			array[counter3].set_testSuiteId(str_input);
			total_object++;
			}	
	}

	Solve(max_running_time,Data::weights,Data::values,total_object,array); //For Part 1
	//All of above for part1
	for(int i=0;i<total_object;i++){array[i].createFreqProfileIndices();} //Create index array corresponding to freq values
	for(int i=0;i<total_object;i++) {array[i].sortIndices();} //Sort index with insertion sort 
	for(int i=0;i<total_object;i++){array[i].totalMaxNumberIndice=array[i].findMaxIndice();} //Find index that maximum number and store it
	//Printing section below

	cout<<"Total amount of running time: "<<Data::totalRunningTime<<endl<<endl;
	
	for(int i=Data::index-1; i>=0;i-- ){
		cout<<Data::points[i]<<" ";
		array[Data::hold_result_index[i]].printResult(); //print result
	}

	delete [] array;
	return 0;
}/////////////////Main end


void Solve(int W, int weights[], int values[], int n, Data* array){ //Dynamic approach

	int K[n + 1][W + 1]; // initialize the array for memoization
    for (int i = 0; i <= n; i++) { 
        for (int w = 0; w <= W; w++) { 
            if (i == 0 || w == 0) // if any index is zoro fill the location with zero
                K[i][w] = 0; 
            else if (weights[i - 1] <= w){
                K[i][w] = max(values[i - 1] +  
                    K[i - 1][w - weights[i - 1]], K[i - 1][w]); 
			}
            else
                K[i][w] = K[i - 1][w]; 
        } 
    }
	int result =  K[n][W];
	Data::result=result; // store the result into a Data class static variable

	for (int i = n; i > 0 ; i--) {   
      	if(result<=0) break;	
		if (result != K[i - 1][W]) {	
			for(int j=0;j<n;j++){
				if(weights[i - 1]==array[j].get_runningTime()){
					Data::points[Data::index]=array[j].get_testSuiteId(); // store the points into a Data class static array
					Data::totalRunningTime+=array[j].get_runningTime();
					Data::hold_result_index[Data::index++]=i-1;
				}
			}
			result -=values[i - 1]; 
			W = W - weights[i - 1]; 
		} 
	} 
}

void Data::createFreqProfileIndices(){

	freqProfileIndices = new int*[this->freq_x];
	sortedFreqProfileIndices= new int*[this->freq_x];
	int i,j;
	for(i=0;i<this->freq_x;i++){
		freqProfileIndices[i] = new int[this->freq_y];
		sortedFreqProfileIndices[i]= new int[this->freq_y];
	}
	for(i=0;i<this->freq_x;i++){
		for(j=0;j<this->freq_y;j++){
			freqProfileIndices[i][j]=j+1;
		}
	}
}

void Data::sortIndices(){
	
	int* temp = new int[this->freq_y];
	int* temp2= new int[this->freq_y];
	for(int i=0;i<this->freq_x;i++){
		for(int k=0;k<this->freq_y;k++){
			temp[k]=this->testCaseFreqProfiles[i][k];
			temp2[k]=this->freqProfileIndices[i][k];
		}		
		insertionSort(temp,temp2,this->freq_y);
		for(int j=0;j<this->freq_y;j++){
			sortedFreqProfileIndices[i][j]=temp2[j];
		}	
	}
	delete[] temp;
	delete[] temp2;

}


void Data::insertionSort(int arr[],int arr2[], int n){   //Sort ascending 
    int i, key,key2 ,j;  
    for (i = 1; i < n; i++){  
        key = arr[i];
		key2=arr2[i] ; 
        j = i - 1;  
        while (j >= 0 && arr[j] > key){  
            arr[j + 1] = arr[j];
			arr2[j+1]=arr2[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;
		arr2[j+1]=key2;  
    }  
}

void Data::insertionSortReverse(int arr[],int arr2[], int n){ //Sort descending	
    int i, key,key2 ,j;  
    for (i = 1; i < n; i++) {  
        key = arr[i];
		key2=arr2[i] ; 
        j = i - 1;  
        while (j >= 0 && arr[j] < key) {  
            arr[j + 1] = arr[j];
			arr2[j+1]=arr2[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;
		arr2[j+1]=key2;  
    }  
}

int Data::findMaxIndice(){

	int max=-1;
	int index=0;
	for(int i=0;i<this->freq_x;i++){
		if(max<freqProfileTotalNumber[i]){
			max=freqProfileTotalNumber[i];
			index=i;
		}
	}
	for(int i=0;i<this->freq_x;i++){
		if(max==freqProfileTotalNumber[i]){
			this->same++;
		}
	}
	return index;
}

int Data::SolveDistance(int* X, int* Y, int n){

	int array[n + 1][n + 1];
	for (int i = 0; i <= n; i++) { 
		for (int j = 0; j <= n; j++) { 
			if (i == 0) {
				array[i][j] = j; 
			}		
			else if (j == 0) {
				array[i][j] = i; 
			}			
			else if (X[i - 1] == Y[j - 1]) {
				array[i][j] = array[i - 1][j - 1]; 
			}
			else{
				array[i][j] = 1 + min(array[i][j - 1], array[i - 1][j],array[i - 1][j - 1]); 
			}				
		} 
	} 
	return array[n][n];
}
void Data::printResult(){
	int* temp;
	int* temp2;	
	int * temp3=new int [this->freq_x];
	int * temp4=new int [this->freq_x];
	int * temp5=new int [Data::same];
	for(int i=0;i<this->freq_x;i++){
		temp3[i]=freqProfileTotalNumber[i];
		temp4[i]=i;
	}
	insertionSortReverse(temp3,temp4,this->freq_x);
	for(int i=0;i<this->freq_x;i++){
		if(i<this->same){
			temp5[i]=temp4[i];
		}		
	}
	if(this->freq_x==1){
		temp = new int[1];
		temp2= new int[1];
	}else{
		temp = new int[this->freq_x-this->same];
		temp2= new int[this->freq_x-this->same];
	}
	int index=0;
	for(int i=0;i<this->freq_x;i++){
		bool check =true;
		for(int j=0;j<this->same;j++){
			if(i==temp5[j]){
				check=false;
			}
		}
			if(check || this->freq_x==1) {
				temp[index]=SolveDistance(sortedFreqProfileIndices[this->totalMaxNumberIndice],sortedFreqProfileIndices[i],this->freq_y);
				temp2[index]=i;
				index++;
			}
	}	
	insertionSortReverse(temp,temp2,this->freq_x-this->same); //sort array and indice array descending order
	for(int i=0;i<this->same;i++){ //printing max covarage test cases
		cout<<temp5[i]+1<<" ";
	}	
	for(int i=0;i<this->freq_x-this->same;i++) cout<<temp2[i]+1<<" "; //printing descending order test case which is similar to total max number of indice
	cout<<endl;

	delete [] temp;
	delete [] temp2;
	delete [] temp3;
	delete [] temp4;
	delete [] temp5;

}


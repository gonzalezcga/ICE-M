#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/command-line.h"
#include "ns3/random-variable-stream.h"
#include "ns3/core-module.h"
#include "ns3/rng-seed-manager.h"
#include <iostream>
#include <cstdlib>

using namespace ns3;
using namespace std;

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

int i;
int CarrierPool [] = {1,2,3,4,5,6};
int sizeArray = ARRAY_SIZE(CarrierPool);
int RequestedCarrier = 6; //Is this going to be associated to an specific Service, Pipe? Ask to Chris
int n; int j = 0; int temp = 0;
int TTL = 16;
float ErrorModelValueGolden = 0.5;
float ErrorModelValueTransponder = 0.5;
int CounterOn;
int TempIteration = 0;

//Variables for transponder scanning
double long freqTarget = 1.4; //Expressed in GHz
double long startFreq = 0.0; 
double long stopFreq = 2.0;
double long stepFreq = 0.1;
double long tempStartFreq = 0.0;
int flagTransponderScanEnabled;
float t,w;
int y=0;
float firstErrorMod,SecondErrorMod,TranspErrorMod;

float Random()
{    
	y++;
 	srand(y);
	w = ((double)rand()/(RAND_MAX)+((rand())%(1)));
	return w;   
}

int main (int argc, char *argv[])
{
for (i=0;i<1000;i++){
   
	for (n=0; n<sizeArray; n++)
		{
		//Check Fading Value (This is the threshold)
			cout << "Carrier Number: " << CarrierPool[n] << endl;
			firstErrorMod = Random();
			//cout << "first: " << firstErrorMod << endl;
			if (ErrorModelValueGolden > firstErrorMod){
				cout << "Carrier Detected Number: " << CarrierPool[n] << endl;
				if (CarrierPool[n] == RequestedCarrier) {
					//temp = 0; 
					cout << "Carrier Acquired Number: " << CarrierPool[n] << endl;
					cout << "Acquisition processed is finished" << endl;
					goto FinishAcq;
					}
				else {
					cout << "Continue tunning process" << endl;
				}
			}
			else {
				for (j=temp;j<TTL;j++){
				cout << "Deep fading detected at Carrier Number: " << CarrierPool[n] << endl;
				cout << "Terminal going to sleep" << endl;
				cout << "TTL values is: " << j << endl;
				goto Sleep;
				}
			}
		}
	
	StartAcq:
		if (sizeArray < n+1){
			cout << "\n" << endl;
			cout << "Starting Transponder Scanning Process" << endl;
			cout << "\n" << endl;
			goto TransponderScan;}
		else{
				if (n<=sizeArray){
						cout << "Carrier Number: " << CarrierPool[n] << endl;
						SecondErrorMod = Random();
						//cout << "second " << SecondErrorMod << endl;
						if (ErrorModelValueGolden > SecondErrorMod){
							cout << "Carrier Detected Number: " << CarrierPool[n] << endl;
							if (CarrierPool[n] == RequestedCarrier) {
								temp = 0; 
								cout << "Carrier Acquired Number: " << CarrierPool[n] << endl;
								cout << "Acquisition processed is finished" << endl;
								goto FinishAcq;
								}
							else {
								temp = 0;
								cout << "Continue tunning process" << endl;
								n++;
								goto StartAcq;
							}
						}
						else {
						for (j=temp;j<TTL;j++){
							cout << "Deep fading detected at Carrier Number: " << CarrierPool[n] << endl;
							cout << "Terminal going to sleep" << endl;
							cout << "TTL values is: " << j << endl;
							goto Sleep;}
						}
						}
				else {cout << "Acquisition Process Failed across all Carriers" << endl;
				 	goto FinishAcq;
				 	}
				
			}
	//In case deep fading is detected
	Sleep:
		for (CounterOn = 0; CounterOn<10; CounterOn++){
			cout << "Waiting to wake up: T+" << CounterOn << endl;
		}
		if (flagTransponderScanEnabled == 1){
			goto TransponderScan;
			}
		else {
			if (CounterOn == 10 && n < sizeArray) {
				CounterOn = 0;
				TempIteration = TempIteration+1;
				if (temp == TTL-1){
					temp = 0;
					n = n+1;
				}
				else {
					if (n>=0){
						temp = temp + 1;
					}
					else{
						temp = 0;
						n = 0; //Return to the same carrier (Infinite loop ??) -> Add TTL
					}
					}
				goto StartAcq;
				}
			else {
				goto FinishAcq;
				//break;
			}
			}
	TransponderScan:
		flagTransponderScanEnabled = 1;
		if ((startFreq+stepFreq)>=stopFreq){
			cout << "Acquisition Process Failed across all Carriers" << endl;
				 	goto FinishAcq;
				 	}
		else {
			for (startFreq=tempStartFreq; startFreq<stopFreq; startFreq+=stepFreq)
			{
			//Check Fading Value (This is the threshold)
				cout << "Carrier Frequency: " << startFreq << endl;
				TranspErrorMod = Random();
				//cout << "transp " << TranspErrorMod << endl;
				if (ErrorModelValueTransponder > TranspErrorMod){
					if (abs(startFreq - freqTarget)<0.0000001) { 
						cout << "Acquisition processed is finished" << endl;
						goto FinishAcq;
						}
					else {
					
						cout << "Continue tunning process" << endl;
					}
				}
				else {
					cout << "Deep fading detected" << endl;
					cout << "Terminal going to sleep" << endl;
					tempStartFreq = startFreq+stepFreq;
					goto Sleep;
				}
				}
			}
FinishAcq:
	continue;
	//cout << "Acquisition processed is finished" << endl;
	}
	return 0;		
}

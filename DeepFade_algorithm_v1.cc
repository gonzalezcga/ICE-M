/*This code is to test the deep fading algorithm in ICEM
11Dec17
Gustavo Gonzalez*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <iostream>

using namespace std;
using namespace ns3;

int i;
int coherenceTime = 10;
float terminalPower = 5;
float threshold = 2;

int main(){
	//Check terminal power vs threshold
	for (i=0;i<coherenceTime;i++){
		if (terminalPower < threshold){
			if (i == (coherenceTime-1)){
				cout << "Terminal is in deep fading" << endl;
				break;
				}
			else{
				cout << "Wait for next coherence time" << endl;
				}
			}
		else{
			cout << "No deep fading detected" << endl;
			break;
			}
		
	}
return 0;
}

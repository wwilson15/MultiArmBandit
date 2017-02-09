//
//  main.cpp
//  Project Alpha
//
//  Created by Wade Wilson on 2/2/17.
//  Copyright © 2017 Wade Wilson. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <fstream>

#define LYRAND (double)rand()/RAND_MAX
using namespace std;

class arm{
public:
    double mu;
    double sigma;
    double winnings = 0;
    int count;
};

class agent{
public:
    double avgw = 0; //averavge winnings
    double pavg = 0; //saving previous average for future calculations
};

void setup(vector<arm>* pmab, vector<agent>* pagents, int n){ /// sets up agent and MaB vectors
    for (int i = 0; i < n; i++){          arm tempa;
        agent agentt;
        tempa.mu=rand()%30 + 1;
        tempa.sigma=LYRAND;
        agentt.avgw=0; //  starting values
        agentt.pavg=0;
        pmab->push_back(tempa);
        pagents->push_back(agentt);
        }
}

double generateGaussianNoise(double mu, double sigma){ // Box Muller transformation from wikipedia
    const double epsilon = std::numeric_limits<double>::min();
    const double two_pi = 2.0*3.14159265358979323846;
    
    static double z0, z1;
    static bool generate;
    generate = !generate;
    
    if (!generate)
        return z1 * sigma + mu;
    
    double u1, u2;
    do
    {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    }
    while ( u1 <= epsilon );
    
    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
    return z0 * sigma + mu;
}
int choice(vector<agent>* pagents, int n, double epsilon){  // Greedy choice with small exploration
    int temp = 0;
    int sto = rand()%n;
    if( LYRAND > epsilon){
        sto = 0;
        for(int i=0; i<n; i++){
        
            if(pagents->at(i).avgw > temp){
                temp = pagents->at(i).avgw;
                sto=i;
            }
        }
    }
    return sto;
}

int inputchoice(){ //user input choice
    int input;
    //deciding which arm to pull
    cout << "Which arm do you want to choose?\n>";
    cin >> input;
    return input;
}
void armpull(vector<arm>* pmab, int input){  // pulls specified arm
    pmab->at(input).winnings = generateGaussianNoise(pmab->at(input).mu, pmab->at(input).sigma);
}
void update(vector<agent>* pagents, vector<arm>* pmab, int input, double alpha){ //updates action values
    pagents->at(input).avgw = pmab->at(input).winnings * alpha + pagents->at(input).pavg * (1-alpha);
    pagents->at(input).pavg = pagents->at(input).avgw;
    
}

void show(vector<arm>* pmab, vector<agent>* pagents, int n){
    //shows action values and mu for each arm and agent
    for(int i=0; i<n; i++){
    cout << "Arm " << i << " Mu " << pmab->at(i).mu  << endl;
    cout << "Agent " << i << " Avgw " << pagents->at(i).avgw << endl;
    }
}
void learningcurve(double alpha, double epsilon){
    int n = 5;
    int iterations = 3000;
    int stat_runs = 2;
    int input = 0;
    vector<arm> mab;
    vector<agent> agents;
    vector<arm>* pmab = &mab;
    vector<agent>* pagents = &agents;
    setup(pmab, pagents,n);
    ofstream outputFile;
    outputFile.open("vout.txt");
    
    if(outputFile.is_open()){
        for(int i=0; i<stat_runs; i++){
            for(int j=0; j<iterations; j++){
                input = choice(pagents, n, epsilon);
                outputFile << pagents->at(input).avgw << "\t" << endl;
                armpull(pmab, input);
                update(pagents, pmab, input, alpha);
            }
        }
    }
    outputFile.close();
    //show(pmab,pagents, n); // shows each mu and average winnings for each arm
}

void testA(double alpha){
    
    int input=0;
    int n=1;
    
    vector<arm> mab;
    vector<agent> agents;
    vector<arm>* pmab = &mab;
    vector<agent>* pagents = &agents;
    setup(pmab, pagents,n);
    
    int mu=10;
    int sigma = 1;
    
    pmab->at(input).mu = mu;
    pmab->at(input).sigma = sigma;
    
    int iterations = 100000;
    for(int i=0; i<iterations; i++){
        armpull(pmab, input);
        update(pagents, pmab, input, alpha);
    }
    assert(pagents->at(input).avgw=mu+sigma/2);
    assert(pagents->at(input).avgw=mu-sigma/2);
    cout << "Test A passed" << endl;
    
}

void testB(double epsilon){
    int n=2;
    int iterations=100000;
    vector<arm> mab;
    vector<agent> agents;
    vector<arm>* pmab = &mab;
    vector<agent>* pagents = &agents;
    setup(pmab, pagents,n);

    
    int mu1=10;
    int mu2=-10;
    int sigma = 1;
    pmab->at(0).mu = mu1;
    pmab->at(1).mu = mu2;
    pmab->at(0).sigma = sigma;
    pmab->at(1).sigma = sigma;
    
    for(int i=0;i<iterations; i++){
        int input = choice(pagents, n, epsilon);
        armpull(pmab, input);
        pmab->at(input).count ++;
    }
    assert(pmab->at(0).count > pmab->at(1).count);
    
    cout << "Test B Passed" << endl;
    /// "Reset"
}


int main() {
    
    //initial values
    srand(time(NULL));
    double alpha = .1;
    double epsilon = .05;
    testA(alpha);
    testB(epsilon);
    learningcurve(alpha, epsilon);
    
}

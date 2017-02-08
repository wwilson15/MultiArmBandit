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

#define LYRAND (double)rand()/RAND_MAX
using namespace std;

class arm{
public:
    double mu;
    double sigma;
    double avgw;  // avergae winnings
    double pavg;  // saving previus average winnings for future iterations
    
    void armpull();
    
};

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
int choice(vector<arm>* pmab, int n, double epsilon){  // Greedy choice with small exploration
    int temp = 0;
    int sto = rand()%n;
    if( LYRAND > epsilon){
        sto = 0;
    for(int i=0; i<n; i++){
        
        if(pmab->at(i).avgw > temp){
            temp = pmab->at(i).avgw;
            sto=i;
    }
    }
    }
    return sto;
}

double inputchoice(){ //user input choice
    int input;
    //deciding which arm to pull
    cout << "Which arm do you want to choose?\n>";
    cin >> input;
    return input;
}
void armpull(vector<arm>* pmab, double alpha, double n, double iterations, int input){  // pulls specified lever
    double winnings = 0;
    double avg;
    avg = pmab->at(input).pavg;
    winnings = generateGaussianNoise(pmab->at(input).mu, pmab->at(input).sigma);
    pmab->at(input).avgw = winnings*alpha + avg*(1-alpha);
    pmab->at(input).pavg = pmab->at(input).avgw;
    // cout << winnings << endl;
    // cout << pmab->at(input).avgw << endl;
    // cout << "Weighted average of arm " << input << "\t" << "is \t" << pmab->at(input).avgw << "\n" << endl;
}

void show(vector<arm>* pmab, int n){
    for(int i=0; i<n; i++){
    cout << "Arm " << i << " Mu " << pmab->at(i).mu  << endl;
    cout << "Arm " << i << " Avgw " << pmab->at(i).avgw << endl;
    }
}


int main() {
    srand(time(NULL));
    int n = 3; // number of arms
    double alpha = .1;
    double epsilon = .05;
    double iterations = 5000;
    int input;
    vector<arm> mab;
    for (int i =0; i< n; i++){  // creating n number of arms with random mu and sigma
        arm tempa;
        tempa.mu=rand()%20 + 1;
        tempa.sigma=LYRAND;
        tempa.avgw=100; // "optimistic " starting values 
        mab.push_back(tempa);
    }
    vector<arm>* pmab = &mab;  // pointer to vector of arms
   
    for(int j=0; j<iterations; j++){  //going through iterations
        input = choice(pmab, n, epsilon);      // telling armpull function which arm to pull
    armpull(pmab, alpha, n, iterations, input);
    }
    show(pmab, n); // shows each mu and average winnings for each arm
}

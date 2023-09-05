#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

#include "SportsLayout.h"

using namespace std;

int main(int argc, char** argv )
{

     // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    
    SportsLayout *s  = new SportsLayout( inputfilename, outputfilename );
    s->compute_allocation();

    long long cost = s->give_cost();
    cout<< "cost: "<<cost<<endl;


    return 0;

}
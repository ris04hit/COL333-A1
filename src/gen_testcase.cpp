#include <random>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char** argv){
    if (argc<7){
        cout << "Missing parameters" << endl;
        exit(0);
    }

    int  time = stof(argv[1]), z = stoi(argv[2]), l = stoi(argv[3]), range_min = stoi(argv[4]), range_max = stoi(argv[5]);
    string filename (argv[6]);

    // Creating random number generator engine
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(range_min, range_max);

    ofstream file (filename);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    file << time << endl << z << endl << l << endl;
    for (int i = 0; i<z; i++){
        for (int j = 0; j<z; j++){
            file << dist(gen) << " ";
        }
        file << endl;
    }
    int arr[l][l];
    for (int i = 0; i<l; i++){
        for (int j = 0; j<l; j++){
            if (i==j) file << 0;
            else if (i<j){
                arr[i][j] = dist(gen);
                file << arr[i][j];
            }
            else {
                file << arr[j][i];
            }
            file << " ";
        }
        file << endl;
    }

    return 0;
}
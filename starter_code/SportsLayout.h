
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <random>
#include <chrono>
using namespace std;

class SportsLayout{

    private:
        int z,l;
        int** T;
        int** N;
        int time_limit, cost;
        int* mapping;

    public:
        SportsLayout(string inputfilename){
            readInInputFile(inputfilename);
            mapping= new int[z];
            for (int i = 0; i<z; i++) mapping[i] = i+1;
            cost = cost_fn(mapping);
        }

        bool check_output_format(){
            vector<bool> visited(l,false);
            for(int i=0;i<z;i++){
                if((mapping[i]>=1 && mapping[i]<=l)){
                    if(!visited[mapping[i]-1])
                        visited[mapping[i]-1]=true;
                    else{
                        cout<<"Repeated locations, check format\n";
                        return false;
                    }
                }
                else{
                    cout<<"Invalid location, check format\n";
                    return false;
                }
            }
            return true;
        }

        void readOutputFile(string output_filename){
            fstream ipfile;
            ipfile.open(output_filename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else{
                vector<int> ip;
                while (1) {
                    int t;
                    ipfile >> t;
                    ip.push_back(t);
                    if (ipfile.eof())
                        break;
                }
                if(ip.size()!=z){
                    cout<<"number of values not equal to number of zones, check output format\n";
                    exit(0);
                }
                for(int i=0;i<z;i++)
                    mapping[i]=ip[i];
                ipfile.close();

                if(!check_output_format())
                    exit(0);
                cout<<"Read output file, format OK"<<endl;
            }
        }

        long long cost_fn(int* mapZtoL){
            long long cost=0;
            for(int i=0;i<z;i++){
                for(int j=0;j<z;j++) {
                    cost+=(long long)N[i][j]*(long long)T[mapZtoL[i]-1][mapZtoL[j]-1];
                }
            }
            return cost;
        }

        long long new_cost_fn(int* mapZtoL, int ind1, int ind2, long long cost_curr){
            // Computes the new cost when ind1 and ind2 of mapZtoL are swapped
            long long cost_new = cost_curr;
            // Removing earlier contribution of ind1
            for (int i = 0; i<z; i++) cost_new -= (long long)N[i][ind1]*(long long)T[mapZtoL[i]-1][mapZtoL[ind1]-1];
            for (int i = 0; i<z; i++) cost_new -= (long long)N[ind1][i]*(long long)T[mapZtoL[ind1]-1][mapZtoL[i]-1];
            // Modifying array
            int old1 = mapZtoL[ind1];
            mapZtoL[ind1] = mapZtoL[ind2];
            // Adding new contribution of ind1
            for (int i = 0; i<z; i++) cost_new += (long long)N[i][ind1]*(long long)T[mapZtoL[i]-1][mapZtoL[ind1]-1];
            for (int i = 0; i<z; i++) cost_new += (long long)N[ind1][i]*(long long)T[mapZtoL[ind1]-1][mapZtoL[i]-1];
            // Removing earlier contribution of ind2
            for (int i = 0; i<z; i++) cost_new -= (long long)N[i][ind2]*(long long)T[mapZtoL[i]-1][mapZtoL[ind2]-1];
            for (int i = 0; i<z; i++) cost_new -= (long long)N[ind2][i]*(long long)T[mapZtoL[ind2]-1][mapZtoL[i]-1];
            // Modifying array
            int old2 = mapZtoL[ind2];
            mapZtoL[ind2] = old1;
            // Adding new contribution of ind2
            for (int i = 0; i<z; i++) cost_new += (long long)N[i][ind2]*(long long)T[mapZtoL[i]-1][mapZtoL[ind2]-1];
            for (int i = 0; i<z; i++) cost_new += (long long)N[ind2][i]*(long long)T[mapZtoL[ind2]-1][mapZtoL[i]-1];
            // Reversing changes to array
            mapZtoL[ind1] = old1;
            mapZtoL[ind2] = old2;

            return cost_new;
        }

        void readInInputFile(string inputfilename){
            fstream ipfile;
            ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                ipfile>> time_limit;
                time_limit*= 60;
                ipfile >> z;
                ipfile >> l;
                if(z>l){
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }
                int **tempT;
                int **tempN;
                tempT = new int*[l];
                for (int i = 0; i < l; ++i)
                    tempT[i] = new int[l];
                tempN = new int*[z];
                for (int i = 0; i < z; ++i)
                    tempN[i] = new int[z];
                for(int i=0;i<z;i++){
                    for(int j=0;j<z;j++)
                        ipfile>>tempN[i][j];
                }
                for(int i=0;i<l;i++){
                    for(int j=0;j<l;j++)
                        ipfile>>tempT[i][j];
                }
                ipfile.close();
                T= tempT;
                N= tempN;
            }
        }

        void write_to_file(string outputfilename){
            // Open the file for writing
            ofstream outputFile(outputfilename);
            // Check if the file is opened successfully
            if (!outputFile.is_open()) {
                cerr << "Failed to open the file for writing." << std::endl;
                exit(0);
            }
            for(int i=0;i<z;i++)
                outputFile<<mapping[i]<<" ";
            // Close the file
            outputFile.close();
            cout << "Allocation written to the file successfully." << endl;
        }

        int* random_state_generator(int* random_arr){
            // Generates a random state
            int* mapZtoL = new int[z];

            // Creating random number generator engine
            random_device rd;
            mt19937 gen(rd());

            // Generating distinct random numbers using Fisher-Yates algorithm
            for (int i = l-1; i>l-z-1; --i){
                uniform_int_distribution<> dist(0,i);
                int j = dist(gen);
                swap(random_arr[i], random_arr[j]);
                mapZtoL[l-i-1] = random_arr[i];
            }

            return mapZtoL;
        }

        int** create_beam(int beam_size, int* random_arr){
            int** beam = new int*[beam_size];
            for (int i = 0; i<beam_size; i++){
                beam[i] = random_state_generator(random_arr);
            }
            return beam;
        }

        long long* calculate_beam_cost(int** beam, int beam_size){
            // returns an array whose each element is the cost corresponding to mapping in beam
            long long* beam_cost = new long long[beam_size];
            for (int i = 0; i<beam_size; i++){
                beam_cost[i] = cost_fn(beam[i]);
            }
            return beam_cost;
        }

        void neighbour_state(int* state, int beam_size, int** beam, long long* beam_cost, long long cur_cost){
            // updates neighbor state in beam array. Neighbour is defined by swapping elements in mapping
            long long new_cost;
            for (int i = 0; i<z; i++){
                for (int j = i+1; j<z; j++){
                    new_cost = new_cost_fn(state, i, j, cur_cost);
                    int index = beam_size;
                    // inserting new state into beam at its correct position
                    while(index--){
                        if (beam_cost[index] > new_cost){
                            if (index!=beam_size-1){
                                beam_cost[index+1] = beam_cost[index];
                                beam[index+1] = beam[index];
                            }
                        }
                        else break;
                    }
                    if (index+1<beam_size){
                        beam_cost[index+1] = new_cost;
                        int new_state[z];
                        for (int k = 0; k<z; k++) new_state[k] = state[k];
                        swap(new_state[i], new_state[j]);
                        delete[] beam[index+1];
                        beam[index+1] = new_state;
                    }
                }
            }
        }

        void beam_search(int** beam, int stopping_num, long long* beam_cost, int beam_size){
            // performs beam_search with stopping condition that cost of top <stopping_num> state remain same in two consecutive search
            bool condition = 1;
            while (condition){
                long long sum_cost_initial = 0, sum_cost_final = 0;
                for (int i = 0; i<stopping_num; i++) sum_cost_initial += beam_cost[i];
                // performing one step of beam search
                for (int i = 0; i<beam_size; i++){
                    neighbour_state(beam[i], beam_size, beam, beam_cost, beam_cost[i]);
                }
                for (int i = 0; i<stopping_num; i++) sum_cost_final += beam_cost[i];
                if (sum_cost_final == sum_cost_initial) condition = 0;
            }
        }

        void compute_allocation(int beam_size, int stopping_num){
            // Beam Search with random restart
            auto start_time = chrono::high_resolution_clock::now();

            // Creating a array of first l natural numbers to facilitate random_state_generator
            int random_arr[l];
            for (int i=0; i<l; i++) random_arr[i] = 1+i;

            auto curr_time = chrono::high_resolution_clock::now();
            while ((curr_time-start_time).count() < time_limit){
                // Random Restart
                int** beam = create_beam(beam_size, random_arr);
                long long* beam_cost = calculate_beam_cost(beam, beam_size);
                // Performing beam search
                beam_search(beam, stopping_num, beam_cost, beam_size);
                if (beam_cost[0] < cost){                                           // Assume beam size to be greater than zero
                    cost = beam_cost[0];
                    for (int i = 0; i<z; i++) mapping[i] = beam[0][i];
                }
                delete[] beam;
                delete[] beam_cost;
                curr_time = chrono::high_resolution_clock::now();
            }
        }
};


#endif

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
        int time_limit;
        long long cost;
        int* mapping;

    public:
        SportsLayout(string inputfilename){
            readInInputFile(inputfilename);
            mapping= new int[l];
            for (int i = 0; i<l; i++) mapping[i] = i+1;
            cost = cost_fn(mapping);
        }

        long long give_cost(){
            return cost;
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
            long long cost_val=0;
            for(int i=0;i<z;i++){
                for(int j=0;j<z;j++) {
                    cost_val+=(long long)N[i][j]*(long long)T[mapZtoL[i]-1][mapZtoL[j]-1];
                }
            }
            return cost_val;
        }

        long long new_cost_fn(int* mapZtoL, int ind1, int ind2, long long cost_curr){
            // Computes the new cost when ind1 and ind2 of mapZtoL are swapped
            long long cost_new = cost_curr;
            // Removing earlier contribution of ind1
            for (int i = 0; i<z; i++) cost_new -= (long long)N[i][ind1]*(long long)T[mapZtoL[i]-1][mapZtoL[ind1]-1];
            for (int i = 0; i<z; i++) cost_new -= (long long)N[ind1][i]*(long long)T[mapZtoL[ind1]-1][mapZtoL[i]-1];
            // Modifying array
            int old1 = mapZtoL[ind1];
            int old2 = mapZtoL[ind2];
            mapZtoL[ind1] = mapZtoL[ind2];
            // Adding new contribution of ind1
            for (int i = 0; i<z; i++) cost_new += (long long)N[i][ind1]*(long long)T[mapZtoL[i]-1][mapZtoL[ind1]-1];
            for (int i = 0; i<z; i++) cost_new += (long long)N[ind1][i]*(long long)T[mapZtoL[ind1]-1][mapZtoL[i]-1];
            if (ind2<z){
                // Removing earlier contribution of ind2
                for (int i = 0; i<z; i++) cost_new -= (long long)N[i][ind2]*(long long)T[mapZtoL[i]-1][mapZtoL[ind2]-1];
                for (int i = 0; i<z; i++) cost_new -= (long long)N[ind2][i]*(long long)T[mapZtoL[ind2]-1][mapZtoL[i]-1];
                // Modifying array
                mapZtoL[ind2] = old1;
                // Adding new contribution of ind2
                for (int i = 0; i<z; i++) cost_new += (long long)N[i][ind2]*(long long)T[mapZtoL[i]-1][mapZtoL[ind2]-1];
                for (int i = 0; i<z; i++) cost_new += (long long)N[ind2][i]*(long long)T[mapZtoL[ind2]-1][mapZtoL[i]-1];
            }
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
            for(int i=0;i<z-1;i++)
                outputFile<<mapping[i]<<" ";
            outputFile<<mapping[z-1];
            // Close the file
            outputFile.close();
            cout << "Allocation written to the file successfully." << endl;
        }

        int* random_state_generator(int* random_arr){
            // Generates a random state
            int* mapZtoL = new int[l];

            // Creating random number generator engine
            random_device rd;
            mt19937 gen(rd());

            // Generating distinct random numbers using Fisher-Yates algorithm
            for (int i = l-1; i>=0; --i){
                uniform_int_distribution<> dist(0,i);
                int j = dist(gen);
                swap(random_arr[i], random_arr[j]);
                mapZtoL[l-i-1] = random_arr[i];
            }

            return mapZtoL;
        }

        pair<int**, long long*> create_beam(int beam_size, int* random_arr){
            // Creates a random pair of beam and its cost for given beam_size
            int** beam = new int*[beam_size];
            long long* beam_cost = new long long[beam_size];
            for (int i = 0; i<beam_size; i++){
                beam[i] = random_state_generator(random_arr);
                beam_cost[i] = cost_fn(beam[i]);
                int ind = i;
                for (int j = i-1; j>=0; j--){
                    if (beam_cost[j] > beam_cost[ind]){
                        swap(beam_cost[j], beam_cost[ind]);
                        swap(beam[j], beam[ind]);
                        ind = j;
                    }
                    else break;
                }
            }
            return make_pair(beam, beam_cost);
        }

        void print_beam(int** beam, int beam_size, long long* beam_cost){
            for (int i = 0; i<beam_size; i++){
                for (int j = 0; j<z; j++) cout << beam[i][j] << " ";
                cout << '\t' << beam_cost[i] << endl;
            }
        }

        void neighbour_state(int* state, int beam_size, int** beam, long long* beam_cost, long long cur_cost){
            // updates neighbor state in beam array. Neighbour is defined by swapping elements in mapping
            long long new_cost;
            for (int i = 0; i<z; i++){
                for (int j = i+1; j<l; j++){
                    new_cost = new_cost_fn(state, i, j, cur_cost);
                    int index = beam_size;
                    int* trash_state = beam[beam_size-1];
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
                        beam[index+1] = trash_state;
                        for (int k = 0; k<l; k++) beam[index+1][k] = state[k];
                        swap(beam[index+1][i], beam[index+1][j]);
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
                // creating new beam
                int new_beam[beam_size][l];
                long long new_beam_cost[beam_size];
                for (int i = 0; i<beam_size; i++){
                    for (int j = 0; j<l; j++){
                        new_beam[i][j] = beam[i][j];
                    }
                    new_beam_cost[i] = beam_cost[i];
                }
                // performing one step of beam search
                for (int i = 0; i<beam_size; i++){
                    neighbour_state(new_beam[i], beam_size, beam, beam_cost, new_beam_cost[i]);
                }
                for (int i = 0; i<stopping_num; i++) sum_cost_final += beam_cost[i];
                if (sum_cost_final == sum_cost_initial) condition = 0;
            }
        }

        pair<int,int> beam_param(){
            // calculates optimal beam size according to some manual analysis
            int beam_size = min(max((500000 * (long long) time_limit)/((long long) (l*l) * (long long) (l*z)), (long long) 1), (long long) 10000);
            int stopping_num = max(beam_size/10, min(5, beam_size));
            return {beam_size, stopping_num};
        }

        void compute_allocation(){
            int beam_size = beam_param().first;
            int stopping_num = beam_param().second;
            // cout << "beam size = " << beam_size << ", with stopping factor = " << stopping_num << endl;

            // Beam Search with random restart
            auto start_time = chrono::high_resolution_clock::now();

            // Creating a array of first l natural numbers to facilitate random_state_generator
            int random_arr[l];
            for (int i=0; i<l; i++) random_arr[i] = 1+i;

            auto curr_time = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = curr_time-start_time;
            int i = 0;
            while (diff.count() < time_limit){
                i++;
                // cout << "iteration: " << i << ", time = " << diff.count() << ", cost = " << cost << endl;
                // Random Restart
                auto pair = create_beam(beam_size, random_arr);
                int** beam = pair.first;
                long long* beam_cost = pair.second;
                // Performing beam search
                beam_search(beam, stopping_num, beam_cost, beam_size);
                if (beam_cost[0] < cost){                                           // Assume beam size to be greater than zero
                    cost = beam_cost[0];
                    for (int i = 0; i<l; i++) mapping[i] = beam[0][i];
                }
                delete[] beam;
                delete[] beam_cost;
                curr_time = chrono::high_resolution_clock::now();
                diff = curr_time - start_time;
            }
        }
};


#endif
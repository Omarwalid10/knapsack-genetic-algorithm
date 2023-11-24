#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

const int Solutions = 100, Generations = 20, N = 5000 ,K=2;

vector<vector<bool> > chromosomes, offspring;
int numOfItems, weight, itemsWeight[N], itemsValue[N], fitness[Solutions];
map<int, int> fitnessRepeated;

void initiPopulation() {
    chromosomes.resize(Solutions);
    for (int i = 0; i < Solutions; ++i) {
        chromosomes[i].resize(numOfItems);
        for (int j = 0; j < numOfItems; ++j) {
            int r = rand() % 2;
            chromosomes[i][j] = r;
        }
    }
}

void calculateFitness() {
    fitnessRepeated.clear();
    int totalFitness = 0,totalValue ,totalWeight ;
    for (int i = 0; i < Solutions; ++i) {
        totalValue = 0, totalWeight = 0;
        for (int j = 0; j < numOfItems; ++j) {
            if (chromosomes[i][j]) {
                totalWeight += itemsWeight[j];
                totalValue += itemsValue[j];
            }
        }
        if (totalWeight > weight) {
            while (totalWeight > weight) {
                int r = 0;
                while (chromosomes[i][r] == 0)
                    r++;
                chromosomes[i][r] = 0;
                totalWeight -= itemsWeight[r];
                totalValue -= itemsValue[r];
            }
        }
        fitness[i] = totalValue;
        ++fitnessRepeated[fitness[i]];
        totalFitness += fitness[i];
    }
}
int maxFitness() {
    int maxF = -1 , index = -1;
    for (int i = 0; i < Solutions; ++i) {
        if (maxF < fitness[i]) {
            maxF = fitness[i];
            index = i;
        }
    }
    return index;
}
//int percentageOfEqualFitness() {
//    int max_cnt = 0;
//    for (auto x: fitnessRepeated)
//        max_cnt = max(max_cnt, x.second);
//    return max_cnt * 100 / Solutions;
//}



//int get_second_fittest(int fittest_index) {
//    int max_second_fitness = INT_MIN, index = -1;
//    for (int i = 0; i < S; ++i) {
//        if (i != fittest_index && max_second_fitness < fitness[i]) {
//            max_second_fitness = fitness[i];
//            index = i;
//        }
//    }
//    return index;
//}

void elitism() {
    //store the value in fitness ---->its index to make sort by the fitness
    vector<pair<int,int>>indexFitness;
    for (int i = 0; i < Solutions; ++i) {
        indexFitness.push_back({fitness[i],i});
    }
    sort(indexFitness.begin(), indexFitness.end());
    reverse(indexFitness.begin(), indexFitness.end());
    for (int i = 0; i < K; ++i) {
        //this give the offspring the best K chromosomes fitness
        offspring.push_back(chromosomes[indexFitness[i].second]);
    }

//
//    int fittest_index = maxFitness();
//    offspring.push_back(chromosomes[fittest_index]);
//    int second_fittest_index = get_second_fittest(fittest_index);
//    offspring.push_back(chromosomes[second_fittest_index]);
}



int rankSelection() {
    //store the value in fitness ---->its index to make sort by the fitness
    multimap<int,int>indexFitness;
    for (int i = 0; i < Solutions; ++i) {
        indexFitness.insert({fitness[i],i});
    }
    double r = ((double) rand() / (RAND_MAX)), sum = 0;
    int i=1,sumOfAllRanks=(Solutions*(Solutions+1))/2;
    //form this we make the roulette_wheel
    for(auto it:indexFitness){
        sum += (double)i/sumOfAllRanks;
        if (sum > r)
            return it.second;
        i++;
    }
    return -1;
}
//int roulette_wheel_selection() {
//    int r = rand() % total_fitness, sum = 0;
//    for (int i = 0; i < S; ++i) {
//        sum += fitness[i];
//        if (sum > r)
//            return i;
//    }
//    return -1;
//}

void mutation(vector<bool> &chromosome) {
    for (int i = 0; i < numOfItems; ++i) {
        //mutation change with 0.005
        int r = rand() % 200;
        if (r < 1)
            chromosome[i] = !chromosome[i];
    }
    offspring.push_back(chromosome);
}

void crossover(int chromosomeIndex1, int chromosomeIndex2) {
    vector<bool> chromosome1 = chromosomes[chromosomeIndex1];
    vector<bool> chromosome2 = chromosomes[chromosomeIndex2];
    int r = rand() % numOfItems;
    for (int i = 0; i < r; ++i)
        swap(chromosome1[i], chromosome2[i]);
    mutation(chromosome1);
    mutation(chromosome2);
//    offspring.push_back(chromosome1);
//    offspring.push_back(chromosome2);
}
void selection(){
    while (offspring.size() < Solutions) {
        int chromosome_index1 = rankSelection();
        int chromosome_index2 = rankSelection();
        crossover(chromosome_index1, chromosome_index2);
    }
}
void replacement() {

    //this is important to store the best k fittness values in the next generation
    elitism();
    //to get the selected parents
    selection();

}

int main() {
//    freopen("input.txt", "rt", stdin);
//    freopen("output.txt", "wt", stdout);
    srand(time(NULL));
    int t,totalWeight=0,totalValue=0;
    cin >> t;
    vector<bool>selectedChromosome;
    for (int i = 1; i <= t; ++i) {
        int loops=0;
        offspring.clear();
        cin >> weight >> numOfItems;
        for (int i = 0; i < numOfItems; ++i)
            cin >> itemsWeight[i] >> itemsValue[i];
        int equalFitness = 0, generation_number = 0, max_cnt;
        initiPopulation();
        while (equalFitness < 100 && generation_number < Generations) {
            max_cnt = 0;
            if (generation_number) {
                chromosomes = offspring;
                offspring.clear();
            }
            calculateFitness();
            //to calculate if the all fitness of the solutions equal or not
            for (auto x: fitnessRepeated)
                max_cnt = max(max_cnt, x.second);
            equalFitness= max_cnt * 100 / Solutions;
            //cout<<loops<<" "<<percentage<<endl;
            ++generation_number;
            // think if the percentage will be lost !!
            if (equalFitness < 100 && generation_number < Generations)
                replacement();
            loops++;
        }
        int fittest_index = maxFitness();
        selectedChromosome=chromosomes[fittest_index];

        cout << "Test Case " << i << endl;
        cout<<"the weight and values: "<<endl;
        for (int j = 0; j < numOfItems; ++j) {
            if(selectedChromosome[j]){
                totalWeight+=itemsWeight[j];
                cout<<itemsWeight[j]<<" "<<itemsValue[j]<<endl;
            }
        }
        cout<<"total weight : "<<totalWeight<<" "<<"totel value : "<<fitness[fittest_index]<<endl;
        //cout<<loops<<" "<<equalFitness<<endl;
    }
    return 0;
}

#include <iostream> 
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <cmath>

std::vector<std::string> genInstances(int n= 250, int duplicatas=5){

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist4(0,3);
    std::uniform_int_distribution<std::mt19937::result_type> dist2030(19,29);
    std::vector<std::string> alphab = {"A","C","T","G"};
    std::string str = "";
    int j = 0; 

    while (j < n){
        str += alphab[dist4(rng)];
        ++j;
    }
    std::vector<std::string> vect;

    int cont = 0;
    while(cont < duplicatas){
        int pos = 0;
        do{
            auto random_number (dist2030(rng));
            std::string subs = str.substr(pos, random_number);
            vect.push_back(subs);
            pos += random_number;
        }while(pos+30 < 250);
        
        std::string subs = str.substr(pos);
        vect.push_back(subs);
        ++cont;
    }


    return vect;
}

int main(int argc, char const *argv[])
{
    auto v (genInstances());
    for (auto i: v)
    {
        std::cout << i << std::endl;
    }
    return 0;
}
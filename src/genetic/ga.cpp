#include <iostream> 
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>

static bool starts_with(const std::string str, const std::string prefix){
    return ((prefix.size() <= str.size()) && std::equal(prefix.begin(), prefix.end(), str.begin()));
}

int overlap(std::string a, std::string b, int min_length=5){
    /* Retorna o tamanho do maior sufixo ou prefixo que contenha pelo menos min_length.
        Se não existir sobreposição, returna 0. */
    int start = 0; 
    while (1){
        start = a.find(b.substr(0,min_length), start);
        if (start == std::string::npos)
            return 0;
        if (starts_with(b,a.substr(start,a.size()-1) ) )
            return (a.size())-start;
        start += 1;
    }
}

std::vector<double> fitness(std::vector<std::string> S,
                          std::vector<std::vector<bool>> P)
{

    std::vector<double> fitnessVector(P.size());
    int cont = 0;
    for (auto i : P)
    {
        size_t lastOne;
        std::string stringderived = "";
        for (size_t j = 0; j < i.size(); ++j)
        {
            if (i[j] == true){
                lastOne = j;
                stringderived += S[lastOne];
                break;
            }
        }
        
        for (auto j (lastOne+1); j < S.size()-1; ++j)
        {
            if (i[j] == true)
            {
                int over = overlap(S[lastOne], S[j], 1);
                if (over < S[j].size())
                    stringderived += S[j].substr(over);
            }
        }
        /*
        std::string stringNaoCoberta = "";
        for (auto j (lastOne+1); j < S.size()-1; ++j)
        {
            if (i[j] == false and stringderived.find(S[j]) == -1)
            {
                int over = overlap(stringNaoCoberta, S[j], 1);
                stringNaoCoberta += S[j].substr(over);
            }
        }*/
        int m = 0;
        for (auto b: i){
            if (!b)
                ++m;
        }
        fitnessVector[cont++] = 1.0/(std::pow((double)(stringderived.size()+m), 2));

        //int over = overlap(stringderived, stringNaoCoberta, 1);
        //std::string superstring = stringderived + stringNaoCoberta.substr(over);

        //std::cout << (double)1/(double)(superstring.size()*superstring.size())+1<< std::endl;

        //fitnessVector[cont++] = 1.0/((double)(superstring.size()*(double)superstring.size())+1);
    }

    return fitnessVector;
}

std::vector<std::vector<bool>> rouletteSelection(std::vector<double> fitness, 
                                                    std::vector<std::vector<bool>> P,
                                                    double percProb)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distSize(0,P.size()-1);

    std::vector<std::vector<bool>> newP;

    auto min (fitness[0]);
    for (auto i: fitness)
    {
        if (i < min)
        {
            min = i;
        }
    }
    //newP.push_back(P[min]);


    long double total = 0.0;
    for (auto fit : fitness)
    {
        total += fit;
    }
    while(newP.size() < P.size()){
        std::uniform_real_distribution<long double> fitnessSize(0,total);
        auto randomFit1(fitnessSize(rng));
        auto randomFit2(fitnessSize(rng));
        long double sumFit = 0.0;
        int index = 0;
        int index1 = 0;
        int index2 = 0;
        //std::cout << sumFit << "/" << randomFit1 << std::endl;
        while(sumFit <= randomFit1 && index != 499){
            //std::cout << sumFit << "|"<< randomFit1;
            sumFit += fitness[index];
            ++index;
        }
        index1 = index;
        index = 0;
        sumFit = 0.0;
        while(sumFit <= randomFit2 && index != 499){
            sumFit += fitness[index];
            ++index;
        }
        index2 = index;
        //std::cout << index1 << " "<< index2 <<std::endl;

        std::uniform_int_distribution<std::mt19937::result_type> distSizeBool(0,P[0].size()-1);
        auto pos1(distSizeBool(rng));
        auto pos2(distSizeBool(rng));
        while(pos1 == pos2){pos2 = distSizeBool(rng);}
        if (pos2 > pos1){
            auto temp(pos1);
            pos1 = pos2;
            pos2 = temp;
        }


        std::vector<bool> individ(P[0].size());
        std::vector<bool> individ2(P[0].size());

        for (int i = 0; i < pos1; ++i)
        {
            individ[i] = (P[index1][i]);
            individ2[i] = (P[index2][i]);
        }
        for (int i = pos1; i < pos2; ++i)
        {
            individ[i] = (P[index1][i]);
            individ2[i] = (P[index2][i]);
        }
        for (int i = pos2; i < P[0].size(); ++i)
        {
            individ[i] = (P[index1][i]);
            individ2[i] = (P[index2][i]);
        }
        std::default_random_engine generator;
        std::uniform_real_distribution<long double> distProb(0.0,1.0);
        auto prob(distProb(generator));
        if (prob < percProb)
        {
            auto randomPos(distSizeBool(rng));
            individ[randomPos] = not individ[randomPos];
        }
        prob = distProb(generator);
        if (prob < percProb)
        {
            auto randomPos(distSizeBool(rng));
            individ2[randomPos] = not individ2[randomPos];
        }
        newP.push_back(individ);
        newP.push_back(individ2);

    }

    return newP;

}
std::string getSuperstring(std::vector<bool> individ, std::vector<std::string> S)
{
    std::string super = "";
    for (int i = 0; i < S.size(); ++i)
    {
        if (individ[i])
        {
            int over = overlap(super, S[i], 1);
            if (super.find(S[i]) == std::string::npos)
                super += S[i].substr(over);
        }
    }
    for (int i = 0; i < S.size(); ++i)
    {
        if (!individ[i])
        {
            int over = overlap(super, S[i], 1);
            if (super.find(S[i]) == std::string::npos)
                super += S[i].substr(over);
        }
    }
    

    return super;
}
  
std::string GA(std::vector<std::string> S, int numOfGenarations, double pMut)
{
    int t = 0;

    auto qntdString(S.size());
    std::vector<bool> Pt(qntdString);
    std::vector<std::vector<bool>> P(500);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);
    std::string superstringFinal = "";

    for (int k = 0; k < 500; ++k)
    {
        for (int i = 0; i < qntdString; ++i)
        {
            Pt[i] = (bool) dist2(rng);
        }
        P[k] = (Pt);
    }

    auto fitnessVector (fitness(S, P));
    std::vector<std::vector<bool>> selectedIndividuals;

    while(t < numOfGenarations){
        selectedIndividuals = rouletteSelection(fitnessVector, P, pMut);

        fitnessVector = fitness(S,P);
        P = selectedIndividuals;
        ++t;
        std::string superstring = "";
        for (auto ind :P)
        {
            std::string aux = getSuperstring(ind, S);
            if (superstring == "" or aux.size() < superstring.size())
                superstring = aux;
        }
        if (superstringFinal == "" or superstring.size()< superstringFinal.size())
            superstringFinal = superstring;
        std::cout << t << " " << superstringFinal.size() << std::endl;
    }

    return superstringFinal;   

}


int main(int argc, char *argv[]) 
{
    std::fstream f(argv[1]);
    std::string line;    
    std::vector<std::string> vet;
    if (f.is_open())
    {
        while (!f.eof() )
        {
            getline (f,line);
            vet.push_back(line);
        }
        f.close();
        if (vet.empty())
        {
            std::cerr << "To run: ./bin/genetic <file> <numOfGenarations> <probMutations>" << std::endl;
        }
        else{
            int n = std::stoi(argv[2]);
            double pMut = std::stod(argv[3]);

            //std::vector<std::string> v = {"GGTTTCAAGGTCTGTTTCCGAGTTTGACT","AGTCCTTCCCCCTTAACTGA","ACCATACGCCGGGACCATCTTATACG","TAAGGGTTAATCGACTACACACCG","GTGAGTTGGGTCGACCAACC","TTCGGACTCTGCCTCCACAGGGA","CTCTCCGAGGCCGTTATAT","TTACGTGACCGAAAAAAGGTTAGCGTT","TACCTTGATCTTTTTGCGTAT","CCAACTAGGTTGATGCGTCGTGAAACGT","AAATTGGGAAGCT","GGTTTCAAGGTCTGTTTCCGAGTTT","GACTAGTCCTTCCCCCTTAAC","TGAACCATACGCCGGGACCAT","CTTATACGTAAGGGTTAATCGACTACA","CACCGGTGAGTTGGGTCGACCAACCTT","CGGACTCTGCCTCCACAGGGACTCTCC","GAGGCCGTTATATTTACGTG","ACCGAAAAAAGGTTAGCGTTT","ACCTTGATCTTTTTGCGTATCCA","ACTAGGTTGATGCGTCGTGAAA","CGTAAATTGGGAAGCT","GGTTTCAAGGTCTGTTTCCGAG","TTTGACTAGTCCTTCCCCCTTAACTG","AACCATACGCCGGGACCATCTTATACG","TAAGGGTTAATCGACTACACACC","GGTGAGTTGGGTCGACCAACCTTCGGACT","CTGCCTCCACAGGGACTCTCCGAGGCCG","TTATATTTACGTGACCGAAAAAAGGTTAG","CGTTTACCTTGATCTTTTTG","CGTATCCAACTAGGTTGATGCGTCGTGAA","ACGTAAATTGGGAAGCT","GGTTTCAAGGTCTGTTTCCGA","GTTTGACTAGTCCTTCCCCCTTAACTGA","ACCATACGCCGGGACCATCTTA","TACGTAAGGGTTAATCGACT","ACACACCGGTGAGTTGGGTCGA","CCAACCTTCGGACTCTGCC","TCCACAGGGACTCTCCGAG","GCCGTTATATTTACGTGACCGA","AAAAAGGTTAGCGTTTACCTTGAT","CTTTTTGCGTATCCAACTAGGTTGATGCG","TCGTGAAACGTAAATTGGGAAGCT","GGTTTCAAGGTCTGTTTCCG","AGTTTGACTAGTCCTTCCCCCTTAACT","GAACCATACGCCGGGACCATCTTATACGT","AAGGGTTAATCGACTACACACCGG","TGAGTTGGGTCGACCAACCTTCGGACT","CTGCCTCCACAGGGACTCTCCGAGGC","CGTTATATTTACGTGACCGAAA","AAAGGTTAGCGTTTACCTTG","ATCTTTTTGCGTATCCAACTAGG","TTGATGCGTCGTGAAACGTAA","ATTGGGAAGCT"};
            //std::random_shuffle (v.begin(), v.end());
            std::cout << GA(vet, n, pMut) << std::endl;
        }
    }
    return 0; 
}
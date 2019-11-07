#include <iostream> 
#include <string>
#include <vector>
#include <random>

static bool starts_with(const std::string str, const std::string prefix){
    return ((prefix.size() <= str.size()) && std::equal(prefix.begin(), prefix.end(), str.begin()));
}

int overlap(std::string a, std::string b, int min_length=5){
    /* Retorna o tamanho do maior sufixo ou prefixo que contenha pelo menos min_length.
        Se não existir sobreposição, returna 0. */
    int start = 0; 
    while (1){
        start = a.find(b.substr(0,min_length), start);
        if (start == -1)
            return 0;
        if (starts_with(b,a.substr(start,a.size()-1) ) )
            return (a.size())-start;
        start += 1;
    }
}

std::vector<double> fitness(std::vector<std::string> S,
                          std::vector<std::vector<bool>> P){

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
                stringderived += S[j].substr(over);
            }
        }

        std::string stringNaoCoberta = "";
        for (auto j (lastOne+1); j < S.size()-1; ++j)
        {
            if (i[j] == false and stringderived.find(S[j]) == -1)
            {
                int over = overlap(stringNaoCoberta, S[j], 1);
                stringNaoCoberta += S[j].substr(over);
            }
        }

        int over = overlap(stringderived, stringNaoCoberta, 1);
        std::string superstring = stringderived + stringNaoCoberta.substr(over);

        //std::cout << (double)1/(double)(superstring.size()*superstring.size())+1<< std::endl;

        fitnessVector[cont++] = 1.0/((double)(superstring.size()*(double)superstring.size())+1);
    }

    return fitnessVector;
}

std::vector<std::vector<bool>> tournamentSelection(std::vector<double> fitness, 
                                                    std::vector<std::vector<bool>> P,
                                                    double percProb){
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
    newP.push_back(P[min]);

    while(newP.size()!= P.size()){
        std::vector<int> randomInd(32);
        for (int i = 0; i < 32; ++i)
        {
            auto randomNumber(distSize(rng));
            for (int j = 0; j < i; ++j)
            {
                if (randomInd[j] == randomNumber)
                {
                    --i;
                    break;
                }
            }
            randomInd[i] = randomNumber;
        }
        std::vector<int> randomInd2(32);
        for (int i = 0; i < 32; ++i)
        {
            auto randomNumber(distSize(rng));
            for (int j = 0; j < i; ++j)
            {
                if (randomInd2[j] == randomNumber)
                {
                    --i;
                    break;
                }
            }
            randomInd2[i] = randomNumber;
        }
        auto min (fitness [randomInd[0]]);
        for (auto i: randomInd)
        {
            if (fitness[i] < min)
            {
                min = fitness[i];
            }
        }
        auto min2 (fitness[randomInd2[0]]);
        for (auto i: randomInd2)
        {
            if (fitness[i] < min2)
            {
                min2 = fitness[i];
            }
        }

        std::uniform_int_distribution<std::mt19937::result_type> distSizeBool(0,P[1].size()-1);
        auto pos1(distSizeBool(rng));
        auto pos2(distSizeBool(rng));
        while(pos1 == pos2){pos2 = distSizeBool(rng);}
        if (pos2 > pos1){
            auto temp(pos1);
            pos1 = pos2;
            pos2 = temp;
        }

        std::vector<bool> individ;
        for (int i = 0; i < pos1; ++i)
        {
            individ.push_back(randomInd[i]);
        }
        for (auto i (pos1); i < pos2; ++i)
        {
            individ.push_back(randomInd2[i]);
        }
        for (auto i (pos2); i < randomInd.size(); ++i)
        {
            individ.push_back(randomInd[i]);
        }
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distProb(0.0,1.0);
        auto prob(distProb(generator));
        if (prob < percProb)
        {
            auto randomPos(distSizeBool(rng));
            individ[randomPos] = not individ[randomPos];
        }
        newP.push_back(individ);
    }

    return newP;

}
  
std::string GA(std::vector<std::string> S, int numOfGenarations, double pMut){
    int t = 0;

    auto qntdString(S.size());
    std::vector<bool> Pt(qntdString);
    std::vector<std::vector<bool>> P(500);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);

    //std::cout << P.size()<< std::endl;

    for (int k = 0; k < 500; ++k)
    {
        for (int i = 0; i < qntdString; ++i)
        {
            Pt[i] = (bool) dist2(rng);
        }
        P[k] = (Pt);
        /*
        for (auto l : Pt)
        {
            std::cout <<l;
        }*/
    }

    auto fitnessVector (fitness(S, P));
    std::vector<std::vector<bool>> selectedIndividuals;

    while(t < numOfGenarations){
        selectedIndividuals = tournamentSelection(fitnessVector, P, pMut);

        fitnessVector = fitness(S,P);
        P = selectedIndividuals;
        ++t;
    }

    auto min (fitnessVector[0]);
    for (auto i: fitnessVector)
    {
        if (i < min)
        {
            min = i;
        }
    }

    size_t lastOne;
    std::string stringderived = "";
    for (size_t j = 0; j < P[min].size(); ++j)
    {
        if (P[min][j] == true){
            lastOne = j;
            stringderived += S[lastOne];
            break;
        }
    }
    
    for (auto j (lastOne+1); j < S.size()-1; ++j)
    {
        if (P[min][j] == true)
        {
            int over = overlap(S[lastOne], S[j], 1);
            stringderived += S[j].substr(over);
        }
    }

    std::string stringNaoCoberta = "";
    for (auto j (lastOne+1); j < S.size()-1; ++j)
    {
        if (P[min][j] == false and stringderived.find(S[j]) == -1)
        {
            //stringNaoCoberta += S[j];
            int over = overlap(stringNaoCoberta, S[j], 1);
            stringNaoCoberta += S[j].substr(over);
        }
    }

    int over = overlap(stringderived, stringNaoCoberta, 1);
    std::string superstring = stringderived + stringNaoCoberta.substr(over);

    return superstring;   

}


int main() 
{
    std::vector<std::string> v = {"ACTGTGTGCTATCTAGCTAGATAT","CGCTCGCATAGCTAGCTATATATA","GCGCGCTACGACTATCAGCATCAGCAT","CAGCAAAAAAATGTCAGCTAG","CATCGATAAACGACGGGCTAGCTAG","CTATATATAGCTAGTCAGTCGATGCTAG","AATTATATATATGCGCGCGATTCAGT","CAGCAGTCAGTGCGCGCTAG","ATCGTAGCATGCATACTACAGCTAG","CATCAGGGGAGGATTGAAACCCCCCCCTT","ACTGTGTGCTATCTAGCTAGATATCGC","TCGCATAGCTAGCTATATATAGCGCGCT","ACGACTATCAGCATCAGCATCA","GCAAAAAAATGTCAGCTAGCATCGATAAA","CGACGGGCTAGCTAGCTATATATAG","CTAGTCAGTCGATGCTAGAATTATA","TATATGCGCGCGATTCAGTCAGCAG","TCAGTGCGCGCTAGATCGTAGCA","TGCATACTACAGCTAGCATCA","GGGGAGGATTGAAACCCCCC","CCTTACTGTGTGCTATCTAGCTAGATATC","GCTCGCATAGCTAGCTATATATAG","CGCGCTACGACTATCAGCA","TCAGCATCAGCAAAAAAATGTCAGCT","AGCATCGATAAACGACGGGCTA","GCTAGCTATATATAGCTAGTCAGT","CGATGCTAGAATTATATATA","TGCGCGCGATTCAGTCAG","CAGTCAGTGCGCGCTAGATCGTAG","CATGCATACTACAGCTAGCAT","CAGGGGAGGATTGAAACCCCCCCCTTACTGTG","TGCTATCTAGCTAGATATCGCTC","GCATAGCTAGCTATATATAGC","GCGCTACGACTATCAGCATC","AGCATCAGCAAAAAAATGTCAGCTA","GCATCGATAAACGACGGGCT","AGCTAGCTATATATAGCTAGTCA","GTCGATGCTAGAATTATATATA","TGCGCGCGATTCAGTCAGCAGTCAG","TGCGCGCTAGATCGTAGCATGC","ATACTACAGCTAGCATCAGGGG","AGGATTGAAACCCCCCCCTT","ACTGTGTGCTATCTAGCTAGAT","ATCGCTCGCATAGCTAGCTAT","ATATAGCGCGCTACGACTATCAG","CATCAGCATCAGCAAAAAAATGTCAGCTA","GCATCGATAAACGACGGGCTAG","CTAGCTATATATAGCTAGTCAGTCGA","TGCTAGAATTATATATATGC","GCGCGATTCAGTCAGCAGTCAGTGCG","CGCTAGATCGTAGCATGCATA","CTACAGCTAGCATCAGGGG","AGGATTGAAACCCCCCCCTT"};
    std::cout << GA(v, 5000, 0.3).size() << std::endl;

    return 0; 
}
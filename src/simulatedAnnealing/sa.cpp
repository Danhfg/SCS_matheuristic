#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

std::vector<std::string>
randomPermutation(std::vector<std::string> strings) {

  std::random_device rd;
  std::mt19937 gen(rd());

  std::shuffle(strings.begin(), strings.end(), gen);
  return strings;
}

double randomNumber() {

  std::random_device rd;  
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  return dis(gen) ;
}

std::vector<std::string>
randomSwap(std::vector<std::string> palavras){

  std::random_device rd;
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dis(0, palavras.size()-1);

  std::random_device rd2;
  std::mt19937 gen2(rd2()); 
  std::uniform_int_distribution<> dis2(0, palavras.size()-1);

  int stringsSwap[2] = {-1, -1};
  stringsSwap[0] = dis(gen);

  while((stringsSwap[0] == stringsSwap[1])|| (stringsSwap[1] == -1))
       stringsSwap[1] = dis2(gen2);
  
  std::string aux = palavras[stringsSwap[0]];
  palavras[stringsSwap[0]] = palavras[stringsSwap[1]];
  palavras[stringsSwap[1]] = aux;

  return palavras;
}

static  bool  starts_with ( const std :: string str, const std :: string prefix) {
  return ((prefix. size () <= str. size ()) && std :: equal (prefix. begin (), prefix. end (), str. begin ()));
}

int  overlap (std :: string a, std :: string b, int min_length = 5 ){
    int start = 0 ;
    while ( 1 ) {
        start = a. find (b. substr ( 0 , min_length), start);
        if (start == - 1 )
            return  0 ;
        if ( starts_with (b, a. substr (start, a. size () - 1 )))
            return (a. size ()) - start;
        start += 1 ;
    }
}

std::string overlapjoin(std::string a, std::string b) {
    for (long long x = (b.length()<= a.length())? b.length(): a.length(); x > 0; --x)
        if (a.substr(a.length() - x, a.length() - 1) == b.substr(0, x))
            return(a.substr(0, a.length() - x) + b);
    return(a + b);
}

int fitness(std::vector<std::string> &palavras) {

  if (palavras.size() == 0)
    return 0;

  if (palavras.size() == 1)
   return palavras[0].length();

  std::string overlap = overlapjoin(palavras[0], palavras[1]);
  
  if (palavras.size() == 2)
    return overlap.length();

  for (int i = 2; i < palavras.size(); ++i) {
    overlap = overlapjoin(overlap, palavras[i]);
  }

  return overlap.length();
}


std::string 
simulatedAnelling(std::vector<std::string> palavras, double temperaturaMax, double temperaturaMin, double theta) {

  double temperaturaAtual = temperaturaMax;
  double lambda = randomNumber();
  std::vector<std::string> savedStrings = randomPermutation(palavras);
  int delta = 0;

  while (temperaturaAtual >= temperaturaMin) {
    std::vector<std::string> newStrings = randomSwap(savedStrings);
    delta = fitness(newStrings) - fitness(savedStrings);

    if (  delta <= 0 || 
          (delta > 0  && (randomNumber() <=  std::exp(((-1)*delta)/temperaturaAtual)) ) 
        ) {
      savedStrings.swap(newStrings);
    }  
  
    temperaturaAtual *= std::exp(((-1)*lambda*temperaturaAtual)/theta);
    
  }
  std::string minunSuperstringCommon = "";
  for (int i=0; i < savedStrings.size(); ++i) {
    minunSuperstringCommon = overlapjoin(minunSuperstringCommon, savedStrings[i]);
  }

  return minunSuperstringCommon;
}

int main() {
  std::vector<std::string> a =   
  {"ACTGTGTGCTATCTAGCTAGATAT","CGCTCGCATAGCTAGCTATATATA","GCGCGCTACGACTATCAGCATCAGCAT","CAGCAAAAAAATGTCAGCTAG","CATCGATAAACGACGGGCTAGCTAG","CTATATATAGCTAGTCAGTCGATGCTAG","AATTATATATATGCGCGCGATTCAGT","CAGCAGTCAGTGCGCGCTAG","ATCGTAGCATGCATACTACAGCTAG","CATCAGGGGAGGATTGAAACCCCCCCCTT","ACTGTGTGCTATCTAGCTAGATATCGC","TCGCATAGCTAGCTATATATAGCGCGCT","ACGACTATCAGCATCAGCATCA","GCAAAAAAATGTCAGCTAGCATCGATAAA","CGACGGGCTAGCTAGCTATATATAG","CTAGTCAGTCGATGCTAGAATTATA","TATATGCGCGCGATTCAGTCAGCAG","TCAGTGCGCGCTAGATCGTAGCA","TGCATACTACAGCTAGCATCA","GGGGAGGATTGAAACCCCCC","CCTTACTGTGTGCTATCTAGCTAGATATC","GCTCGCATAGCTAGCTATATATAG","CGCGCTACGACTATCAGCA","TCAGCATCAGCAAAAAAATGTCAGCT","AGCATCGATAAACGACGGGCTA","GCTAGCTATATATAGCTAGTCAGT","CGATGCTAGAATTATATATA","TGCGCGCGATTCAGTCAG","CAGTCAGTGCGCGCTAGATCGTAG","CATGCATACTACAGCTAGCAT","CAGGGGAGGATTGAAACCCCCCCCTTACTGTG","TGCTATCTAGCTAGATATCGCTC","GCATAGCTAGCTATATATAGC","GCGCTACGACTATCAGCATC","AGCATCAGCAAAAAAATGTCAGCTA","GCATCGATAAACGACGGGCT","AGCTAGCTATATATAGCTAGTCA","GTCGATGCTAGAATTATATATA","TGCGCGCGATTCAGTCAGCAGTCAG","TGCGCGCTAGATCGTAGCATGC","ATACTACAGCTAGCATCAGGGG","AGGATTGAAACCCCCCCCTT","ACTGTGTGCTATCTAGCTAGAT","ATCGCTCGCATAGCTAGCTAT","ATATAGCGCGCTACGACTATCAG","CATCAGCATCAGCAAAAAAATGTCAGCTA","GCATCGATAAACGACGGGCTAG","CTAGCTATATATAGCTAGTCAGTCGA","TGCTAGAATTATATATATGC","GCGCGATTCAGTCAGCAGTCAGTGCG","CGCTAGATCGTAGCATGCATA","CTACAGCTAGCATCAGGGG","AGGATTGAAACCCCCCCCTT"};
    
  std::cout << simulatedAnelling(a, 100, 0.0005, 5).length() << std::endl<< "OK" ;
}

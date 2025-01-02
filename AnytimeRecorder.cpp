#include "AnytimeRecorder.hpp"

void AnytimeRecorder::imprimirEnStream(std::ostream &os){

  clock_t start = fitnesses.front().first;

  for(int i=0; i<fitnesses.size(); i++){
    clock_t cl = fitnesses[i].first;
    double fit = fitnesses[i].second;
    double segundos = (double)(cl-start)/CLOCKS_PER_SEC;
    os << segundos << " " << fit << std::endl;
  }

}

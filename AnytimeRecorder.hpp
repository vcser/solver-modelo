#ifndef ANYTIME_RECORDER_HPP
#define ANYTIME_RECORDER_HPP

#include <vector>
#include <ctime>
#include <iostream>
class AnytimeRecorder{
public:
  static AnytimeRecorder& get(){
    static AnytimeRecorder instance;
    return instance;
  }

  AnytimeRecorder(AnytimeRecorder const &) = delete;
  void operator=(AnytimeRecorder const&) = delete;
  void agregarMedicion(clock_t t, double m ){ fitnesses.emplace_back(t,m);}
  void redefinirMedicion(clock_t t){
    fitnesses.clear();
    fitnesses.emplace_back(t,0.0);
  }
  unsigned getNumMediciones(){ return fitnesses.size();}
  std::pair<clock_t,double> getMedicion(unsigned pos){ return fitnesses[pos];}
  void imprimirEnStream(std::ostream&);
private:
  std::vector<std::pair<clock_t,double> > fitnesses;
  AnytimeRecorder(){}
};

#endif

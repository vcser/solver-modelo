#include "RedNeuronal.hpp"

const std::string RedNeuronal::_PerimetroDumped = "../NN/Perimetro/perimetro.dumped";
const std::string RedNeuronal::_AreaDumped = "../NN/Area/area.dumped";

void RedNeuronal::_procesaNuevoIncendio(const Incendio &i){
  std::vector<double> sample{ i.getPendiente(),i.getFactorVPL(),i.getHumedad(),
    i.getTemperatura(),i.getRapidezViento() };
    
  DataChunk *chunkSample = new DataChunkFlat();
  chunkSample->set_data(sample);

  double perimetroPorHora = PeriModel->compute_output(chunkSample)[0];
  double areaPorHora = AreaModel->compute_output(chunkSample)[0];
  
  _incendiosProcesados[&i] = std::make_pair(Perimetro(perimetroPorHora),Area(areaPorHora));
};

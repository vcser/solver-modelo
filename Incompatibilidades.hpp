#ifndef INCOMPATIBILIDADES_HPP
#define INCOMPATIBILIDADES_HPP

#include <vector>

class Incendio;
class Recurso;

class Incompatibilidades{
public:
  static Incompatibilidades& get(){
    static Incompatibilidades instance;
    return instance;
  }
  Incompatibilidades(Incompatibilidades const &) = delete;
  void operator=(Incompatibilidades const&) = delete;
  bool compatibleRR(const Recurso &, const Recurso &) const;
  bool compatibleRI(const Recurso &, const Incendio &) const;
  void setMatrizRecursoRecurso(const std::vector<std::vector<bool> >&);
  void setMatrizRecursoIncendio(const std::vector<std::vector<bool> >&);
private:

  Incompatibilidades(){}
  std::vector<std::vector<bool> > _matrizRecursoRecurso; // [tipoRecurso1][tipoRecurso2]
  std::vector<std::vector<bool> > _matrizIncendioRecurso; // [idIncendio][tipoRecurso]

};



#endif

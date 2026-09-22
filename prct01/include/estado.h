#include <iostream>
#include <memory>

class Estado {
 public:
  // Constructores
  Estado() = default;
  Estado(int fila, int columna, int coste_g, int coste_h) : 
        fila_(fila), columna_(columna), coste_g_(coste_g), coste_h_(coste_h) {}
  Estado(int fila, int columna, int coste_g, int coste_h, Estado estado) : 
        fila_(fila), columna_(columna), coste_g_(coste_g), coste_h_(coste_h), estado_anterior_(std::make_unique<Estado>(estado)) {}
  // Getters
  int GetFila() { return fila_; }
  int GetColumna() { return columna_; }
  int GetCosteG() { return coste_g_; }
  int GetCosteH() { return coste_h_; }
  int GetCosteF() { return coste_f_; }
  std::unique_ptr<Estado>& GetEstadoAnterior() { return estado_anterior_; }
  // Setters
  void SetFila(int fila) { fila_ = fila; }
  void SetColumna(int columna) { columna_ = columna; }
  void SetCosteG(int coste_g) { coste_g_ = coste_g; }
  void SetCosteH(int coste_h) { coste_h_ = coste_h; }
  void SetCosteF(int coste_f) { coste_f_ = coste_f; }
  void SetEstadoAnterior(Estado estado) { estado_anterior_ = std::make_unique<Estado>(estado); }
  // Inicializacion Funciones
  bool EsIgual(Estado estado); 
  bool operator<(Estado estado) const;

 private:
  int fila_;
  int columna_;
  int coste_g_; // Coste de llegada desde el nodo inicial
  int coste_h_; // Estimación del coste heurístico para alcanzar el nodo destino
  int coste_f_; 
  std::unique_ptr<Estado> estado_anterior_;
};
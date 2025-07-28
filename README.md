# Recomendador de despacho
Modelo de recomendacion basado en la metaheuristica GRASP y el modelo de prediccion kitral.

## Tipos de recursos
- Helitransportada
- Tipo I
- Tipo II
- Mecanizada
- Avion Cisterna
- Helicoptero Mediano
- Helicoptero Grande

## Tipos de modelos de combustible
| ID shapefile | ID ModCom | Descripcion                                         |
|--------------|-----------|-----------------------------------------------------|
| 0            |  -        | No combustible                                      |
| 1            |  PCH1     | Pastizales Mesomórficos Densos                      |
| 2            |  PCH2     | Pastizales Mesomórficos Ralos                       |
| 3            |  PCH3     | Pastizales Higromórficos Densos                     |
| 4            |  PCH4     | Pastizales Higromórficos Ralos                      |
| 5            |  PCH5     | Chacarería, Viñedos y Frutales                      |
| 6            |  MT01     | Matorrales y Arbustos Mesomórficos Densos           |
| 7            |  MT02     | Matorrales y Arbustos Mesomórficos Medios y Ralos   |
| 8            |  MT03     | Matorrales y Arbustos Higromórficos Densos          |
| 9            |  MT04     | Matorrales y Arbustos Higromórificos Medios y Ralos |
| 10           |  MT05     | Formaciones con predominancia de Chusquea spp       |
| 11           |  MT06     | Formaciones con predominancia de Ulex spp           |
| 12           |  MT07     | Renovales Nativos diferentes al Tipo Siempreverde   |
| 13           |  MT08     | Renovales Nativos del Tipo Siempreverde             |
| 14           |  BN01     | Formaciones con predominancia de Alerzales          |
| 15           |  BN02     | Formaciones con predominancia de Araucaria          |
| 16           |  BN03     | Arbolado Nativo Denso                               |
| 17           |  BN04     | Arbolado Nativo de Densidad Media                   |
| 18           |  BN05     | Arbolado Nativo de Densidad Baja                    |
| 19           |  PL01     | Plantaciones Coníferas Nuevas (0-3) sin Manejo      |
| 20           |  PL02     | Plantaciones Coníferas Jóvenes (4-11) sin Manejo    |
| 21           |  PL03     | Plantaciones Coníferas Adultas (12-17) sin Manejo   |
| 22           |  PL04     | Plantaciones Coníferas Mayores (>17) sin Manejo     |
| 23           |  PL05     | Plantaciones Coníferas Jóvenes (4-11) con Manejo    |
| 24           |  PL06     | Plantaciones Coníferas Adultas (12-17) con Manejo   |
| 25           |  PL07     | Plantaciones Coníferas Mayores (>17) con Manejo     |
| 26           |  PL08     | Plantaciones Eucaliptos Nuevas (0-3)                |
| 27           |  PL09     | Plantaciones Eucaliptos Jóvenes (4-10)              |
| 28           |  PL10     | Plantaciones Eucalipto Adultas (>10)                |
| 29           |  PL11     | Plantaciones Latifoliadas y Mixtas                  |
| 30           |  DX01     | Desechos Explotación a Tala Rasa de Plantaciones    |
| 31           |  DX02     | Desechos Explotación a Tala Rasa de Bosque Nativo   |
| 99           |  -        | No combustible                                      |

## Rendimientos recursos
| ModCom | Helitransportada | Tipo I | Tipo II | Mecanizada | Avion Cisterna | Helicoptero Mediano | Helicoptero Grande |
|--------|------------------|--------|---------|------------|----------------|---------------------|--------------------|
| PCH1   | 474              | 395    | 237     | 1185       | 593            | 198                 | 790                |
| PCH2   | 606              | 505    | 303     | 1515       | 758            | 253                 | 1010               |
| PCH3   | 714              | 595    | 357     | 1785       | 893            | 298                 | 1190               |
| PCH4   | 906              | 755    | 453     | 2265       | 1133           | 378                 | 1510               |
| PCH5   | 726              | 605    | 363     | 1815       | 908            | 303                 | 1210               |
| MT01   | 78               | 65     | 39      | 195        | 98             | 33                  | 130                |
| MT02   | 540              | 450    | 270     | 1350       | 675            | 225                 | 900                |
| MT03   | 72               | 60     | 36      | 180        | 90             | 30                  | 120                |
| MT04   | 546              | 455    | 273     | 1365       | 683            | 228                 | 910                |
| MT05   | 282              | 235    | 141     | 705        | 353            | 118                 | 470                |
| MT06   | 54               | 45     | 27      | 135        | 68             | 23                  | 90                 |
| MT07   | 120              | 100    | 60      | 300        | 150            | 50                  | 200                |
| MT08   | 414              | 345    | 207     | 1035       | 518            | 173                 | 690                |
| BN01   | 348              | 290    | 174     | 870        | 435            | 145                 | 580                |
| BN02   | 354              | 295    | 177     | 885        | 443            | 148                 | 590                |
| BN03   | 216              | 180    | 108     | 540        | 270            | 90                  | 360                |
| BN04   | 324              | 270    | 162     | 810        | 405            | 135                 | 540                |
| BN05   | 378              | 315    | 189     | 945        | 473            | 158                 | 630                |
| PL01   | 576              | 480    | 288     | 1440       | 720            | 240                 | 960                |
| PL02   | 270              | 225    | 135     | 675        | 338            | 113                 | 450                |
| PL03   | 180              | 150    | 90      | 450        | 225            | 75                  | 300                |
| PL04   | 150              | 125    | 75      | 375        | 188            | 63                  | 250                |
| PL05   | 294              | 245    | 147     | 735        | 368            | 123                 | 490                |
| PL06   | 366              | 305    | 183     | 915        | 458            | 153                 | 610                |
| PL07   | 396              | 330    | 198     | 990        | 495            | 165                 | 660                |
| PL08   | 510              | 425    | 255     | 1275       | 638            | 213                 | 850                |
| PL09   | 306              | 255    | 153     | 765        | 383            | 128                 | 510                |
| PL10   | 246              | 205    | 123     | 615        | 308            | 103                 | 410                |
| PL11   | 336              | 280    | 168     | 840        | 420            | 140                 | 560                |
| DX01   | 66               | 55     | 33      | 165        | 83             | 28                  | 110                |
| DX02   | 78               | 65     | 39      | 195        | 98             | 33                  | 130                |

## TODO
- [ ] Redefinir calculo de afinidad
- [ ] Corregir seleccion random de recursos
- [ ] Mejorar revision de vecindad
- [ ] Quitar costo por hora de recursos
- [ ] Apagar incendio iterativo: Si no se puede en 1hr probar con 2hr y asi sucesivamente hasta un limite como 6hr

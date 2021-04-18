# MINI TP SEMAFORO


## INTRODUCCIÓN
El programa se basa en que 4 equipos compiten por hacer un sándwich de milanesa lo más rápido posible, siguiendo una receta con la dificultad que solamente hay un salero, un sartén que cocina solamente una milanesa y un horno que cocina dos panes.


### Implementacion:
**Semaforos interno**
- s_mezclar
- s_salar
- s_agregarCarne
- s_empanar
- s_freir
- s_cortandoExtra
- s_panHorneado

**Semaforo compartido:**
- mutex 1
- mutex1 1

### Pseudocódigo
- Cortar()
  - V(s_mezclar)
- P(S_mezclar)
  - mezcla
  - V(s_salar)

- P(s_salar)
  - mutex() 
  - salar
  - mutex()
  - V(s_agregarCarne)

- P(s_agregarCarne)
  - agregarCarne
  - V(s_empanar)

- P(s_empanar)
  - empanar
  - V(s_freir)

- P(s_freir)
  - mutex()
  - freir
  - mutex()
  - V(s_hornear)

- P(s_hornear)
  - mutex1()
  - Hornear
  - mutex1()
  - mutex1()
  - V(s_panHorneado)

- Cortar Extra()
  - CortandoExtra
  - V(s_cortandoExtra) 

- Armar Milanesa()
  - P(s_hornear)
  - P(s_cortandoExtra)
  - P(s_panHorneado)
  - ArmandoMilanesa
  - Equipo Ganador


##

## DIFICULTADES
- Complicacion para usar los mutex.
- Problema en la concurrencia.
- Leer desde un archivo la receta.

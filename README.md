# MINI TP SEMAFORO

**Semaforos interno**
- s_mezclar
- s_salar
- s_agregarCarne
- s_empanar
- s_freir
- s_cortandoExtra
- s_panHorneado

**Semaforo compartido:**
- mutex
- mutex1

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
  - 
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

# MINI TP SEMAFORO

### Pseudocódigo

**Semaforos:**
- s_mezclar
- s_salar
- s_agregarCarne
- s_empanar
- s_freir
- s_mutex
- s_mutex1


Cortar()

V(s_mezclar)
 
P(S_mezclar)
- mezcla
V(s_salar)

P(s_salar)

- mutex() 
- salar
- mutex()
  
V(s_agregarCarne)

P(s_agregarCarne)
- agregarCarne
V(s_empanar)

P(s_empanar)
-empanar
V(s_freir)

P(s_freir)
- mutex()
- freir
- mutex()
  

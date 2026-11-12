% --- Razas ---
raza(link, hylian).
raza(zelda, hylian).
raza(ganondorf, gerudo).
raza(nabooru, gerudo).
raza(saria, kokiri).
raza(darunia, goron).
raza(ruto, zora).
raza(impa, sheikah).
raza(malon, hylian).

% --- Dónde vive cada uno ---
habita(link, bosque_kokiri).
habita(saria, bosque_kokiri).
habita(darunia, ciudad_goron).
habita(ruto, dominio_zora).
habita(impa, aldea_kakariko).
habita(nabooru, desierto_gerudo).
habita(ganondorf, desierto_gerudo).
habita(malon, rancho_lon_lon).
habita(zelda, castillo_hyrule).

% --- Sabios ---
sabio(saria, bosque).
sabio(darunia, fuego).
sabio(ruto, agua).
sabio(impa, sombra).
sabio(nabooru, espiritu).

% --- Mazmorras ---
mazmorra_nino(arbol_deku).
mazmorra_nino(cavernas_dodongo).
mazmorra_nino(vientre_jabu_jabu).

mazmorra_adulto(templo_bosque).
mazmorra_adulto(templo_fuego).
mazmorra_adulto(templo_agua).
mazmorra_adulto(templo_sombra).
mazmorra_adulto(templo_espiritu).

templo_de(bosque, templo_bosque).
templo_de(fuego, templo_fuego).
templo_de(agua, templo_agua).
templo_de(sombra, templo_sombra).
templo_de(espiritu, templo_espiritu).

% --- Jefes y objetos ---
jefe(arbol_deku, gohma).
jefe(cavernas_dodongo, rey_dodongo).
jefe(vientre_jabu_jabu, barinade).
jefe(templo_bosque, fantasma_ganon).
jefe(templo_fuego, volvagia).
jefe(templo_agua, morpha).
jefe(templo_sombra, bongo_bongo).
jefe(templo_espiritu, twinrova).

objeto_en(templo_bosque, arco_de_las_hadas).
objeto_en(templo_fuego, martillo_megaton).
objeto_en(templo_agua, gancho_largo).
objeto_en(templo_sombra, botas_voladoras).
objeto_en(templo_espiritu, guantes_de_plata).

debilidad(gohma, tirachinas).
debilidad(rey_dodongo, bombas).
debilidad(barinade, bumeran).
debilidad(volvagia, martillo_megaton).
debilidad(morpha, gancho_largo).
debilidad(bongo_bongo, arco_de_las_hadas).
debilidad(twinrova, escudo_espejo).

% --- Canciones ---
ensena(nana_de_zelda, zelda).
ensena(cancion_de_saria, saria).
ensena(cancion_de_epona, malon).
ensena(minueto_del_bosque, sheik).
ensena(bolero_del_fuego, sheik).
ensena(serenata_del_agua, sheik).
ensena(requiem_del_espiritu, sheik).
ensena(nocturno_de_la_sombra, sheik).

identidad(sheik, zelda).

% --- Orden de las mazmorras de adulto ---
siguiente(templo_bosque, templo_fuego).
siguiente(templo_fuego, templo_agua).
siguiente(templo_agua, templo_sombra).
siguiente(templo_sombra, templo_espiritu).

% --- Reglas ---
arma_vence_jefe(Mazmorra, Arma) :- jefe(Mazmorra, Jefe), debilidad(Jefe, Arma).

mismo(X, X).
mismo(X, Y) :- identidad(X, Y).
cancion_zelda(Cancion) :- ensena(Cancion, Q), mismo(Q, zelda).

pares_misma_raza(A, B, Raza) :- raza(A, Raza), raza(B, Raza), A @< B.

antes(TemploB, TemploA) :- siguiente(TemploB, TemploA).
antes(TemploB, TemploA) :- siguiente(TemploB, Z), antes(Z, TemploA).

mazmorras_anteriores(X, Mazmorra1) :- antes(X, Mazmorra1).

sabio_templo_jefe(X, Jefe) :- jefe(Templo, Jefe), templo_de(T, Templo), sabio(X, T).



% --- Hechos con listas ---
inventario(link, [tirachinas, bumeran, bombas, arco_de_las_hadas, martillo_megaton, gancho_largo]).

mazmorras_adulto([templo_bosque, templo_fuego, templo_agua, templo_sombra, templo_espiritu]).

sabios([saria, darunia, ruto, impa, nabooru, rauru]).

cancion_notas(nana_de_zelda,      [izquierda, arriba, derecha, izquierda, arriba, derecha]).
cancion_notas(cancion_de_saria,   [abajo, derecha, izquierda, abajo, derecha, izquierda]).
cancion_notas(cancion_de_epona,   [arriba, izquierda, derecha, arriba, izquierda, derecha]).
cancion_notas(minueto_del_bosque, [a, arriba, izquierda, derecha, izquierda, derecha]).
cancion_notas(serenata_del_agua,  [a, abajo, derecha, derecha, izquierda]).
cancion_notas(cancion_del_sol,    [derecha, abajo, arriba, derecha, abajo, arriba]).

% --- Reglas con listas ---

    % Con predicados predefinidos
n_inventario(X) :- inventario(link, Lista), length(Lista, X). % ¿Cuántos objetos tiene Link en su inventario?
tiene_objeto(X) :- inventario(link, Lista), member(X, Lista). % ¿Tiene Link las bombas? ¿Y la espada maestra?
objeto_tercera_pos(X) :- inventario(link, Lista), nth1(3, Lista, X). % ¿Qué objeto está en la 3ª posición del inventario?
ultimo_objeto(X) :- inventario(link, Lista), last(Lista, X). % ¿Cuál es el último objeto del inventario?
todas_mazmorras(X) :- findall(M, mazmorra_nino(M), Ninos), mazmorras_adulto(Adultos), append(Ninos, Adultos, X). % Lista todas las mazmorras de niño y de adulto juntas en una sola lista.

    % Escribiendo tú la recursión
mi_longitud([], 0).
mi_longitud([_|Resto], N) :- mi_longitud(Resto, M), N is M + 1.

mi_pertenece(X, [X|_]).
mi_pertenece(X, [_|Resto]) :- mi_pertenece(X, Resto).

cuantas_aparece(X, [], 0).
cuantas_aparece(X, [C|Resto], N) :- X \= C, cuantas_aparece(X, Resto, N).
cuantas_aparece(X, [X|Resto], N) :- cuantas_aparece(X, Resto, M), N is M + 1.

ultimo_elemento([X], X).
ultimo_elemento([_|Resto], X) :- ultimo_elemento(Resto, X).

eliminar(X, [], []).
eliminar(X, [X|Resto], Resto).
eliminar(X, [C|Resto], [C|R]) :- X \= C, eliminar(X, Resto, R).

invertir([], []).
invertir([C|Resto], R) :- invertir(Resto, RR), append(RR, [C], R).
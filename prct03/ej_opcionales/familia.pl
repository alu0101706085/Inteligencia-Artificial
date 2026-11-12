% Ejemplo de base de datos de hechos

femenino(petra).
femenino(carmen).
femenino(maria).
femenino(rosa).
femenino(ana).
femenino(cande).
masculino(angel).
masculino(pepe).
masculino(juan).
masculino(pedro).
masculino(enrique).
masculino(raul).
masculino(alfonso).
progenitor(petra, juan).
progenitor(petra, rosa).
progenitor(carmen, pedro).
progenitor(maria, ana).
progenitor(maria, enrique).
progenitor(rosa, raul).
progenitor(rosa, alfonso).
progenitor(rosa, cande).
progenitor(angel, juan).
progenitor(angel, rosa).
progenitor(pepe, pedro).
progenitor(juan, ana).
progenitor(juan, enrique).
progenitor(pedro, raul).
progenitor(pedro, alfonso).
progenitor(pedro, cande).

% Ejemplos de reglas
nieto_a(X, Y) :- progenitor(V, X), progenitor(Y, V).

antepasado(X, Z) :- progenitor(X, Z).
antepasado(X, Z) :- progenitor(X, Y), antepasado(Y, Z).

% Ejercicio 1
hijo(X, Y) :- progenitor(Y, X), masculino(X).
hija(X, Y) :- progenitor(Y, X), femenino(X).
nieto(X, Y) :- progenitor(Z, X), progenitor(Y, Z), masculino(X).
nieta(X, Y):- progenitor(Z, X), progenitor(Y, Z), femenino(X).

hermano(X, Y) :- progenitor(Z, X), progenitor(Z, Y), X \= Y, masculino(X).
hermana(X, Y) :- progenitor(Z, X), progenitor(Z, Y), X \= Y, femenino(X).
sobrino(X, Y) :- progenitor(Z, X), (hermano(Z, Y); hermana(Z, Y)), masculino(X).
sobrina(X, Y) :- progenitor(Z, X), (hermano(Z, Y); hermana(Z, Y)), femenino(X).
primo(X, Y) :- progenitor(Z, X), progenitor(V, Y), (hermano(Z, V); hermana(Z, V)), masculino(X).
prima(X, Y) :- progenitor(Z, X), progenitor(V, Y), (hermano(Z, V); hermana(Z, V)), femenino(X).
Partie I
Avec if 0 -> rien de doit d'afficher puis Halt est executé
Avec if 1 -> il doit s'afficher a0 a1 a2 a3 puis Halt

Partie II
* Si on lit le caractère avant d'être averti qu'il est dispo : on le lit rien (car il est indispo)
* Si on écrit un caractère avant que l'écriture précédente ne soit terminée -> l'écriture précédente ne se fait pas et on passe a l'écriture courante.

Action II.1
./nachos -c lance la console qui est en attente de caractère
Cela appelle la fonction Consoletest dans userprog/progtest.cc avec en parmaètre NULL, NULL

Action II.4
ATTENTION Penser a écrire un 'q' à la fin du fichier d'entrée, sinon boucle infinie

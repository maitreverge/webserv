#!/bin/bash

# this bah script should be names with extention .sh
# this is bad practice, all because of 42 guidelines...
# Définir le motif ASCII
pattern=(
'░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░▒▓████████▓▒░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░' 
'░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░' 
'░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒▒▓█▓▒░ ' 
'░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓█▓▒▒▓█▓▒░ ' 
'░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▓█▓▒░  ' 
'░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▓█▓▒░  ' 
' ░▒▓█████████████▓▒░░▒▓████████▓▒░▒▓███████▓▒░░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓██▓▒░   '                                                                                                                                                           
)

# Dégradé de couleur du haut (vert foncé) vers le bas (vert clair)
start_r=0
start_g=128
start_b=0
end_r=144
end_g=238
end_b=144

lines=${#pattern[@]}
color_step_r=$(( (end_r - start_r) / (lines - 1) ))
color_step_g=$(( (end_g - start_g) / (lines - 1) ))
color_step_b=$(( (end_b - start_b) / (lines - 1) ))

display_pattern() {
local delay=0.012  # Délai en secondes entre les lignes
local padding_x=5  # Espacement horizontal
local padding_y=2  # Espacement vertical

# Afficher l'espacement vertical
for _ in $(seq 1 $padding_y); do
printf "\n"
done

for i in "${!pattern[@]}"; do
# Afficher l'espacement horizontal avant chaque ligne du motif
printf "%${padding_x}s" ""  # Ajouter des espaces horizontaux

r=$((start_r + i * color_step_r))
g=$((start_g + i * color_step_g))
b=$((start_b + i * color_step_b))
printf "%s\n" "${pattern[$i]}"

sleep "$delay"
done
}
display_pattern
printf "\n\n\n";
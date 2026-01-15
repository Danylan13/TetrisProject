# 🎮 Tetris C++ (Raylib)

Implémentation moderne du jeu classique **Tetris**, développée en **C++** en utilisant la bibliothèque graphique **Raylib**.  
Le projet propose plusieurs modes de jeu, incluant **solo**, **multijoueur local**, ainsi qu’un mode contre une **intelligence artificielle (IA)**.

---

## 🖼️ Aperçu

<img width="1014" height="746" alt="image" src="https://github.com/user-attachments/assets/d6d1138c-63f6-4762-a98d-b2402fbc0ee7" />

---

## ✨ Fonctionnalités

### 🎯 Modes de jeu
- **Joueur Unique (Solo)** : Tetris classique pour un seul joueur.
- **Multijoueur Local** : jeu à deux sur le même clavier (écran partagé).
- **Jouer contre l’IA** : affrontez un algorithme qui tente d’optimiser le placement des pièces.

### 🧩 Mécaniques modernes de Tetris
- **Ghost Block (Bloc fantôme)** : aperçu de la position d’atterrissage de la pièce.
- **Hold** : possibilité de stocker une pièce pour l’utiliser plus tard.
- **Hard Drop** : chute instantanée de la pièce.
- **Next Piece Preview** : affichage de la prochaine pièce.
- **Système de niveaux** : vitesse augmente avec la progression (toutes les 10 lignes).
- **High Score** : sauvegarde et suivi du meilleur score.

### 🔊 Audio & UI
- **Musique de fond** + effets sonores.
- **Menu**, **pause**, et interface claire.

---

## 🎮 Commandes

| Action | Joueur 1 (ou Solo) | Joueur 2 (Multijoueur) |
|------|----------------------|------------------------|
| Déplacer Gauche/Droite | Flèches Gauche/Droite | A / D |
| Rotation | Flèche Haut | W |
| Chute Accélérée | Flèche Bas | S |
| Chute Instantanée (Hard Drop) | Espace | F |
| Maintien (Hold) | C | R |
| Pause | P | P |
| Menu Principal | ESC | ESC |

---

## 🗂️ Structure du Projet

| Fichier / Dossier | Description |
|------------------|------------|
| `main.cpp` | Point d’entrée, boucle principale et gestion des états du jeu |
| `Game.h / Game.cpp` | Logique principale, entrées joueur, mode IA |
| `Grid.h / Grid.cpp` | Plateau (20x10), détection lignes/collisions |
| `Block.h / Block.cpp` | Définition des tétriminos |
| `Blocks.h / Blocks.cpp` | Gestion des différentes formes |
| `Position.h` | Structure utilitaire pour les coordonnées |
| `Sounds/` | Ressources audio (`music.mp3`, `rotate.mp3`, ...) |
| `highscore.txt` | Sauvegarde du meilleur score |

---

## 🤖 À propos de l’IA

L’IA utilise une **évaluation heuristique** du plateau pour choisir le meilleur placement possible.  
Elle analyse notamment :

- **Hauteur agrégée** : somme des hauteurs des colonnes
- **Trous (holes)** : cellules vides sous des blocs
- **Bossellement (bumpiness)** : différence de hauteur entre colonnes voisines
- **Lignes complètes** : maximisation du nombre de lignes supprimées

Objectif : **minimiser les trous et la hauteur**, tout en **maximisant les lignes complètes**.

---

## 🚀 Comment lancer le jeu

### ✅ Prérequis
- Un compilateur **C++** (MinGW / GCC / Clang)
- La bibliothèque **Raylib** installée

### 🛠️ Compilation manuelle (exemple GCC sous Windows)
```bash
g++ *.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o Tetris.exe
Note : Les drapeaux de liaison peuvent varier selon votre système d'exploitation.

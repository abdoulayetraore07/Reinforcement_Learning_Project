# 🤖 Reinforcement Learning Project: Maze & Morpion

## 📌 Description
Ce dépôt contient un projet d'apprentissage par renforcement réalisé dans le cadre d'un cours à l'ENSTA Paris. Nous avons implémenté plusieurs algorithmes d'apprentissage par renforcement pour résoudre deux problèmes distincts :
1. Trouver la sortie d'un labyrinthe
2. Créer une IA capable de jouer au morpion (tic-tac-toe)

Ce projet démontre l'application de techniques d'apprentissage par renforcement comme Q-learning et SARSA à des problèmes concrets, en utilisant différentes stratégies d'exploration.

---

## 🛠️ Fonctionnalités
✅ **Algorithmes d'apprentissage par renforcement** - Q-learning et SARSA  
✅ **Stratégies d'exploration** - Epsilon-greedy et Exploration de Boltzmann  
✅ **Résolution de labyrinthe** - Trouver le chemin optimal dans un environnement complexe  
✅ **IA pour jeu de morpion** - Simple Q-learning et Double Q-learning  
✅ **Chargement/Sauvegarde de modèles** - Persistance des tables Q pour réutilisation  

---

## 📂 Structure du Projet

```
/Reinforcement_Learning_Project
│── README.md                      # Ce fichier
│── presentation.pptx              # Présentation du projet
│── Makefile                       # Fichier de compilation
│
├── Include/                       # Fichiers d'en-tête
│   │── ...                        # Headers pour les algorithmes et environnements
│
├── src/                           # Code source
│   │── Q_learning.c               # Implémentation de Q-learning
│   │── Sarsa.c                    # Implémentation de SARSA
│   │── Morpion_Q.c                # Q-learning pour le jeu de morpion
│   │── Morpion_2Q.c               # Double Q-learning pour le jeu de morpion
│   │── ...                        # Autres fichiers source et environnements
│
└── Data/                          # Données pour les environnements
│── maze.txt                   # Labyrinthe au format texte
│── ...                        # Fichiers pour chargement/sauvegarde des tables Q
```

## 🧠 Algorithmes Implémentés

### Q-learning
Q-learning est un algorithme d'apprentissage par renforcement qui apprend la valeur des actions dans des états spécifiques en utilisant une table de valeurs Q, sans nécessiter un modèle de l'environnement. Il met à jour les valeurs Q en maximisant les récompenses futures attendues.

### SARSA
SARSA (State-Action-Reward-State-Action) est un algorithme similaire qui met à jour les valeurs Q en se basant sur la politique suivie. Contrairement au Q-learning qui utilise la meilleure action possible, SARSA utilise l'action effectivement choisie selon la politique actuelle.

### Stratégies d'exploration
Deux fonctions sont disponibles pour la politique d'exploration :
- **Epsilon-greedy** : Rapide et efficace
- **Exploration de Boltzmann** : Plus sophistiquée mais plus lente en exécution

### Morpion (Tic-tac-toe)
Deux approches sont implémentées pour le jeu de morpion :
- **Q-learning** avec exploration epsilon-greedy
- **Double Q-learning** pour expérimentation

Nos tests ont montré que le Q-learning avec epsilon-greedy est plus efficace pour ce projet spécifique.

## 🚀 Exécution du Projet
Pour exécuter le projet, il suffit de taper `make` dans votre terminal. Vous obtiendrez les exécutables suivants :
- `Q_learning` : Q-learning pour le labyrinthe
- `Sarsa` : SARSA pour le labyrinthe
- `Morpion_Q` : Q-learning pour le morpion
- `Morpion_2Q` : Double Q-learning pour le morpion

Des descriptions détaillées peuvent être trouvées dans les commentaires des algorithmes.

---

Merci pour votre lecture et bon apprentissage par renforcement !

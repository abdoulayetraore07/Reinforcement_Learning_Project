# Reinforcement Learning Project: Maze & Tic-Tac-Toe

## 📌 Description
This repository contains a reinforcement learning project carried out as part of a course at ENSTA Paris. We implemented several reinforcement learning algorithms to solve two distinct problems:
1. Finding the exit of a maze
2. Creating an AI capable of playing tic-tac-toe

This project demonstrates the application of reinforcement learning techniques such as Q-learning and SARSA to concrete problems, using different exploration strategies.

---

## Features
✅ **Reinforcement learning algorithms** - Q-learning and SARSA  
✅ **Exploration strategies** - Epsilon-greedy and Boltzmann exploration  
✅ **Maze solving** - Find the optimal path in a complex environment  
✅ **Tic-tac-toe AI** - Simple Q-learning and Double Q-learning  
✅ **Model loading/saving** - Q-table persistence for reuse  

---

## Project Structure

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


## Implemented Algorithms

### Q-learning
Q-learning is a reinforcement learning algorithm that learns the value of actions in specific states using a Q-value table, without needing a model of the environment. It updates Q-values by maximizing expected future rewards.

### SARSA
SARSA (State-Action-Reward-State-Action) is a similar algorithm that updates Q-values based on the policy being followed. Unlike Q-learning which uses the best possible action, SARSA uses the action actually chosen according to the current policy.

### Exploration Strategies
Two functions are available for the exploration policy:
- **Epsilon-greedy**: Fast and efficient
- **Boltzmann exploration**: More sophisticated but slower in execution

### Tic-tac-toe
Two approaches are implemented for the tic-tac-toe game:
- **Q-learning** with epsilon-greedy exploration
- **Double Q-learning** for experimentation

Our tests showed that Q-learning with epsilon-greedy is more effective for this specific project.

## Running the Project
To run the project, simply type `make` in your terminal. You will obtain the following executables:
- `Q_learning`: Q-learning for the maze
- `Sarsa`: SARSA for the maze
- `Morpion_Q`: Q-learning for tic-tac-toe
- `Morpion_2Q`: Double Q-learning for tic-tac-toe

Detailed descriptions can be found in the comments of the algorithms.

---

Thanks for reading and enjoy your reinforcement learning journey!


# Reinforcement Learning Project: Maze 

Welcome to our git :)

Here's our repository dedicated to Reinforcement Learning. A small project will be conducted and used as a course project at ENSTA Paris.

We aim here to implement different Reinforcement Learning algorithms to exit a maze first and do our project which consists of creating an IA able to play a morpion game.

You'll find here several folders :

/Include is for the headers

/src is for the algorithms and the environments by themselves, it contains the different parts and functions provided by the teacher and our functions. 

/Data is for the maze in format txt and different files txt for loading Q at each execution for the morpion game. 
There is also a file pptx for the presentation.

/Our algorithms for the final evaluation are named : Q_learning.c ; Sarsa.c ; Morpion_Q.c ; Morpion_2Q.c

Q-learning is a reinforcement learning algorithm that aims to learn the value of actions in specific states using a Q-value table, without needing a model of the environment. It updates Q-values by maximizing the expected future rewards.

SARSA (State-Action-Reward-State-Action) is a similar algorithm that updates Q-values based on the policy being followed. Unlike Q-learning, which uses the best possible action, SARSA uses the action actually taken according to the current policy.

It's possible to choose for the moment between two functions for the policy: epsilon_greedy and  Boltzmann's exploration.
We notice that the first policy takes less time for execution than the second.

For the morpion game, you have implemented two algorithms: the first uses double Q_learning and the last one uses Q_learning with epsilon_greedy 
We have done these two algorithms to experiment with double Q_learning first and observe the difference and the efficacity.
We finally chose Q_learning with epsilon_greedy because we think it's more efficient for our project.

To run the project, you have just to type 'make' in your terminal and you'll get the executables named: Morpion_Q, Morpion_2Q, Q_learning, Sarsa

Detailed descriptions can be found in the algorithm.

Thank you for your reading!

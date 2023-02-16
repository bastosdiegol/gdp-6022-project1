# Artificial Intelligence Project 1 - Basic Steering Behaviours

This solution was created using Visual Studio Community 2022 Version 17 Toolset v143.

The project is currently configured to be build in Debug and Release mode, both on x64 architecture.

Currently you can only run this project through Visual Studio Solution.

There are 3 enemies (collored Balls) that follows the Player.
- Red Ball   : Enemy Type A : Seeks and Flees
- Green Ball : Enemy Type B : Pursues and Evades
- Blue Ball  : Enemy Type C : Approaches

The user inputs expected when using the application are:
- W moves the Player foward
- S moves the Player backward
- D turns the Player to the right
- A turns the Player to the left

The main loop of the scene is detailed on the file: AIProjOneCommons.h
Each Enemy has its on class that defines its Behaviours.
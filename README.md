# INFO-6022 - Physics & Simulation II Project 2 - 3rd Party Physics Library

Group Project Members:
- Bruno Berti
- Diego Bastos

Third Party Physics Library chosen: Bullet Phyhsics 

This solution was created using Visual Studio Community 2022 Version 17 Toolset v143.

The project is currently configured to be build in Debug and Release mode, both on x64 architecture.

Currently you can only run this project through Visual Studio Solution.

Our main application inicialization, loop and shutdown is defined on the file: PhysicsProjTwoCommons.h
Scene Variables that you can change: min and max scale/mass for generated meshes (Line: 19 n 20); numOfStaticBoxes (Line: 158);

The user inputs expected when using the application are:
- F1 resets the scene (or New Game on Imgui)

- Space Bar spawns a random sphere
- E controls the next sphere
- Q controls the previous sphere

- W moves the controlled Ball foward
- S moves the controlled Ball backward
- D turns the controlled Ball to the right
- A turns the controlled Ball to the left

- Arrow Key Up zooms the camera in
- Arrow Key Down zooms the camera out
- Arrow Key Left rotates the camera to the left
- Arrow Key Right rotates the camera to the right

- Numpad Arrow Key Up (8) moves the camera upward
- Numpad Arrow Key Down (2) moves the camera downward
- Numpad Arrow Key Left (6) moves the camera leftward
- Numpad Arrow Key Right (4) moves the camera rightward

- ESC closes the application
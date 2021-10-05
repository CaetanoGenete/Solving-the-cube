# Solving the cube
<h2>Description</h2>
Solving the cube is a windowed application inspired by Kociemba's algorithm (https://www.jaapsch.net/puzzles/compcube.htm#kocal). The program uses OpenGL as the backbone to renderer a fully rotate-able rubik's cube, and the ability to turn its faces. This algorithm provides, not only a remarkly close approximation to the shortest solutions but also a method of scrambling that is more random than a naive move-set random generator.

<h2>Instructions</h2>
The algorithm will first generate a set of tables to improve performace, this make take a while (1-5 minutes). Once complete, the cube can be moved by dragging the area surrounding it. The infomation panel controls the turning of the faces; U: White, R: Red, F: Green, D: Yellow, L: Orange and B: Blue.

<h2>Developer Notes</h2>

I made this for my A-Level computer science project from September 2018 to April 2019, over the years I have done almost nothing to it. This was the largest and most ambitious project I had undertaken at the time, where my knowledge of OpenGL was minute. I am proud of what I have achieved but be warned that it has only been tested on Windows, support for other platforms is not guaranteed. Moreover, for reasons I have failed to deduce. OpenGL refuses to render the program in Debug mode, perhaps due to some weird allocations. In the future, I might ammend this; for now, if you still wish to build and run the program, ensure that symbols are turned off.

<h2>Dependencies</h2>
<ul>
  <li>OpenGL 4.0.0+</li> 
  Rendering specification used for this project
  <li>GLEW (http://glew.sourceforge.net/)</li>
  C/C++ extension loading library of choice, used because it is the most popular and best documented.
  <li>GLFW (https://www.glfw.org/)</li>
  Light weight OpenGL API for rendering windows.
</ul>


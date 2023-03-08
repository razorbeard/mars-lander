# Mars Lander

Mars Lander is a CodingGame challenge to safely land a space shuttle.

Based on this [article](https://www.codingame.com/blog/genetic-algorithm-mars-lander/) from the CodingGame blog, the challenge can be
solved by using a genetic algorithm. This project is just a graphical tool developped in C++ with the SFML library, that helps for
visualising the evolution of an initial population.

## Rules

The zone of interest is **7000m wide** and **3000m high**. \
There is always a unique area of flat ground on the surface of Mars, which is at least **1000m wide**.

Every turn, depending on the current flight parameters (location, speed, fuel ...), the user must provide the new desired tilt angle and thrust power of Mars Lander.

The game simulates a free fall without atmosphere. Gravity on Mars is **3.711 m/s²**. \
For a thrust power of X, a push force equivalent to X m/s² is generated and X liters of fuel are consumed. As such, a thrust power of 4 in an almost vertical position is needed to compensate for the gravity on Mars. \
Angle goes **from -90° to 90°**. Thrust power goes **from 0 to 4**.

A successful landing is made when the shuttle :
* land on the flat ground (y coordinates are the same, given the consecutive points)
* land in a vertical position (tilt angle = 0°)
* has a vertical speed must be limited (≤ 40 m/s in absolute value)
* has a horizontal speed must be limited (≤ 20 m/s in absolute value)

**Note** : Response time per turn ≤ 100 ms

## Building

CMake version >= 3.2 is needed for building this project.

Open a Git bash at the root of the project, and follow the classic CMake build procedure :
```
~/mars-lander $ mkdir build
~/mars-lander $ cd build
~/mars-lander/build $ cmake ..
~/mars-lander/build $ cmake --build .
```

You can also directly use cmake-gui.

## Usage

In the folder `resources/data`, you will find text files representing each level. \
The first line represents the inital state of the lander :
`xPosition yPosition xVelocity yVelocity fuel angle thrust`

The next lines are the coordinates of a polyline, which represents the surface of Mars.

Considering the rules, you can create your own level by adding a new text file. In the method *Application::createButtons*,
you can add a new button by appending the level name in the string vector.
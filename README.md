# Boids

That is my implementation of the boids code.

## Compilation

To compile this project, you need [CMake](https://cmake.org), [Boost](http://boost.org), and the [SFML](sfml-dev.org) library.

## Simulations

You can use the keyboard to interact with the simulation
key|effect
:-:|:-:
`B` | Add a boid
`O` | Add an obstacle
`Ctrl`+`S` | Toggle the 'Separation' rule
`Ctrl`+`A` | Toggle the 'Alignment' rule
`Ctrl`+`C` | Toggle the 'Cohesion' rule
`Ctrl`+`O` | Toggle the 'Obstacles' rule
`Ctrl`+`B` | Toggle the 'Borders' rule
Pressing `F` | Follow the boid closest to the mouse (when pressed)
Releasing `F` | Return to the main view

## TODO

- [x] Add obstacles
- [x] Correct boids behaviour around obstacles
  - [ ] Allow boids to adopt a tear-drop shape around the obstacles
- [x] Takes arguments from the command line
- [x] Allow saving output to file
- [ ] Allow inputs from the keyboard
  - [x] add objects
  - [x] change rules
  - [x] follow boid
- [x] Speed up the algorithms
- [ ] Allow adding new rules easily
- [ ] Make the code easier to read
- [ ] Go 3D

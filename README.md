# Jigsaw Puzzle #

This repository contains my implementation for the jigsaw puzzle. Given an image as input, an `NxN` puzzle is generated from it, with `N` between 3 and 6, inclusively.
The user can shuffle the pieces at any time, by clicking on `Shuffle` or can check the correct solution by clicking on `Solve`.

## Building ##

Make sure you have Qt installed. Then, from the main directory, run:
```
mkdir build && cd build
qmake ../src && make
```

## Running ##

Go to the `build` directory and run `./jigsawpuzzle`. Have fun playing!

## Screenshots ##

![jigsaw](https://github.com/mihaimorariu/jigsaw-puzzle/blob/master/jigsaw.jpg)

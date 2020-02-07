# Jigsaw Puzzle #

This repository contains my implementation for the jigsaw puzzle. Given an image as input, an `NxN` puzzle is generated from it, with `N` between 3 and 6, inclusively.
The user can shuffle the pieces at any time, by clicking on `Shuffle` or can check the correct solution by clicking on `Solve`.

## Build ##

Make sure you have Qt installed. Then, from the main directory, run:
```
mkdir build && cd build
qmake .. && make
```

### Run ###

Run `./JigsawPuzzle` from the `build` directory and have fun playing!

Circle Collision Sandbox
========================

This is a short representation of my circle/circle collision algorithm.

Main properties are:

1. no penetration due to exact fixed point arithmetic (credits to CNL)
2. expected linear complexity for finding islands due to fixed parameters

The expected linear complexity is archived by limiting max velocity and max radius.
This limits the max extend an aabb can have which allows to create a closely meshed grid data structure that limits the
broad phase comparisons to circles in neighbour cells.
In this sample the limits are:

- `min radius = 2`
- `max radius = 6`
- `max velocity = 2`
- `max aabb extend = 16`

As no circle penetration is allowed/possible we can at most have `(max aabb extend)^2 / (2 * min radius)^2 = 16` circles
per cell.
This results in at most `72` (likely a lot less) broad phase comparisons per circle. This is not a small number but its
constant and the broad phase check is cheap.

Build
-----

```shell
python bootstrap.py
mkdir cmake-build-release
cmake -S . -B ./cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build ./cmake-build-release --config Release
cmake-build-release/bin/CircleCollisionSandbox
```

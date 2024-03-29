# CPU Ray tracer

<img src="output_samples/big.png" width="250" height="250">

## Build

``make``

## Run
Usage:

 ``./raytracer file.scene outputfile.ppm width height aliasing_level reflection_max_depth``

* **file.scene**: the input file (must have the .scene format)
* **output.ppm**: the output file (ppm format)
* **width**: width of the resolution of the output frame
* **height**: height of the resolution of the output frame
* **aliasing level**: level of aliasing by side. (i.e if aliasing level equals to 2, compute the mean of 2x2=4 pixels)
* **reflection_max_depth**: The maximum depth for the reflection

## Features
### Translatable Geometric figures

* Plan
* Sphere
* Raybox
* Triangle
* Metaball

### Aliasing

According to the aliasing level (supposed n here), compute the mean color of nxn subpixels of the current pixel 

### Reflection

The return of light. The reflection is restricted to the maximum reflection depth given as arguments.

## Scenes

The raytracer reads a specific file format ``.scene``.

A scene consists of a camera, a collection of lights and a collection of intersectable objects (geometric figures).

### Sample

A scene file ``scenes/sample.scene`` describes how to write a scene file for each geometric figures, camera and lights.

### Examples

Several more samples can be found in ``scenes/*``

## Output

<img src="output_samples/multiple-spheres.png" width="250" height="250">
<img src="output_samples/1metaball.png" width="250" height="250">
<img src="output_samples/1raybox.png" width="250" height="250">
<img src="output_samples/1sphere.png" width="250" height="250">
<img src="output_samples/1triangle.png" width="250" height="250">

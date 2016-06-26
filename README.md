# Z-Buffer and Raytracing

This code was created for the 4th project of PUC-Rio's INF2608 (Fundamentals of Computer Graphics).

The project's objectives were to implement a Raytracing and compare it to OpenGL's Z-Buffer algorithm.
First we do Z-Buffer's rendering and then we draw pixel by pixel from the Raytracing's point of view.

The Z-Buffer code does not use the Stencil Buffer, therefore the Raytracing scene has shadows while Z-Buffer's doesn't. Other differences are due to the use of Gouraud shading by Z-Buffer, while the Raytracing code uses Phong shading.

The main code is contained in Raytracing.cpp.

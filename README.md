# BlueLeafRenderer #

This repository will (hopefully) eventually become a full fledged 3D rendering program that should be able to take .obj files as input with specular, roughness, normal, maps as input and use metal gpu acceleration to render the models.

The Xcode project only builds for macOS as of now, in the future I might extend compatibility to iPasOS and maybe iOS (the latter will likely be pretty slow in performance).

Currently, the rendering engine is heavily based off of https://raytracing.github.io/books/RayTracingInOneWeekend.html by Peter Shirley and I will be implementing the features discussed in the subsquent two books as well before branching off to learn more advanced techniques to accomplish the above goal. While the math and design decisions I used are heavily influenced by Peter Shirley I do make various deviations in design which I believe based on my own design conclusions.

## Progress ##
RaytracingInOneWeekend Complete 11/28/2022 including:
- Sphere 3D objects can be rendered
- Lambertian, Metal, and Dielectric materials
- Abstraction to bridge C++ code to swiftui
- Custom functionality to view render samples in realtime

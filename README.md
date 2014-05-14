Cloth-Simulation
================

Implemented a Cloth Simulation using C++ and OpenGL. Cloth was made up of a number of particles.Particles, which have mass, were connected to each other by massless springy-links, which in turn were connected to form triangles.The common edges that the triangles share had torsional springs associated with them.The springy-links between particles provide spring forces that tend to keep the particles at a specific rest-distance from each other. The torsional springs between triangles provide spring forces that keep the triangles at a specific rest-angle with respect to each other. Both the link and torsional springs had some damping associated with them so that vibrations would die out over time

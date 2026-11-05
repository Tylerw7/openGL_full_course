In OpenGL everything is in 3D space, but the screen or window is a 2D array of pixels so a large part
of OpenGL’s work is about transforming all 3D coordinates to 2D pixels that fit on your screen. The
process of transforming 3D coordinates to 2D pixels is managed by the graphics pipeline of OpenGL.
The graphics pipeline can be divided into two large parts: the first transforms your 3D coordinates
into 2D coordinates and the second part transforms the 2D coordinates into actual colored pixels. In
this chapter we’ll briefly discuss the graphics pipeline and how we can use it to our advantage to
create fancy pixels.

The graphics pipeline takes as input a set of 3D coordinates and transforms these to colored
2D pixels on your screen. The graphics pipeline can be divided into several steps where each step
requires the output of the previous step as its input. All of these steps are highly specialized (they
have one specific function) and can easily be executed in parallel. Because of their parallel nature,
graphics cards of today have thousands of small processing cores to quickly process your data within
the graphics pipeline. The processing cores run small programs on the GPU for each step of the
pipeline. These small programs are called shaders.

Some of these shaders are configurable by the developer which allows us to write our own
shaders to replace the existing default shaders. This gives us much more fine-grained control over
specific parts of the pipeline and because they run on the GPU, they can also save us valuable CPU
time. Shaders are written in the OpenGL Shading Language (GLSL)
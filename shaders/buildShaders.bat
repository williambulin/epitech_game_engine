@echo off
cd /D "%~dp0"
del triangle.vert.spv
del triangle.frag.spv
glslc triangle.vert -o triangle.vert.spv
glslc triangle.frag -o triangle.frag.spv
pause

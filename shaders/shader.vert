#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec2 uResolution;
uniform vec2 uGridSize;
void main()
{
    float pointWidth = uResolution.x / uGridSize.x;
    float pointHeight = uResolution.y / uGridSize.y;
    gl_Position = vec4(aPos, 0.0, 1.0);
    gl_PointSize = min(pointWidth, pointHeight);
}
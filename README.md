# ShaderDebugger
Shader debugger, analyses parsetree and generates inspect shader by using ANGLE

This is a proof of concept demo of a shader debugger which uses ANGLE to generate the parse tree for the shader.
It searches for the target symbol in the parse tree and generates an 'inspect shader' which is then run with the same setup 
as the original shader. The output is rendered to a floating point texture and displayed in the inspect window. 
The application is currently a visual studio project but the goal is to make it work on multiple platforms and 
generate a javascript version with emscripten.


Usage

Select a test from the menu.
The source is displayed in the left source pane.
The shader is rendered in the middle gl window.
Select a symbol in the source window by clicking on a symbol.
The top line of the right debug window shows :
Inspect : 'symbol' (line n)
Under the top line is the parse tree displayed of the original shader.
Then is the modified parse tree displayed,
As last part is the generated glsl from the modified parse tree displayed.

This shader is then run and the output captured in a floating point texture.
By moving the mouse over the gl window you get the numeric value of the target symbol at that location.


Limitations

Currently the inspect functionality is very limited.
Only fragment shaders can be inspected which consist of only a main function.
Only floating point variables of type scalar and vecx can be inspected.
The value of a variable in a loop can only be inspected for the first iteration.
There is no way to change the value of the attributes or uniforms.


Issues 

If the target symbol is followed by a selector then this is the data stored in the inspect image.
But this is not stated in the first line.
(You can see it in the generated glsl shader)
If the target symbol is foo.xz then this is stored in gl_FragColor.xy (values are stored from left to right in 
gl_FragColor. If the shader also stores a value in gl_FragColor.w then this is not set to zero.
This can cause some confusion, again check the generated glsl code.


Building

Currently the project is only a visual studio 2013 edition solution.
The project needs several projects from the angle library.
Loading the solution will show which.
The following include paths needs to be added to the vc include dirs :
	path_to\angle\include
	path_to\angle\src
	path_to\angle\util

It also needs wxWidgets for the gui part and the following include paths 
needs to be added to the vc include dirs :
	path_to\wxWidgets-3.0.2\include
	path_to\wxWidgets-3.0.2\include\msvc


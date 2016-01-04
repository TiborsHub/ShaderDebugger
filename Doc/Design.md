Introduction

	This document describes the design of a software shader debugger.
	By software shader debugger is meant that the debugger does not need
	hardware support to retrieve the state. The debugger uses the
	abstract syntax tree (ast) to analyse the structure of the shader code
	and adds extra instructions to the shader (instrumentation) to retrieve the
	value of intermediate variables (expressions) for inspection.
	This method is also used to retrieve the value of conditional expressions
	and loop index to follow the flow of the current shader state when 
	simulating single stepping through the shader code.
	The next section describes some examples to clarify how the instrumentation works.

	At least two issues could interfere with this method to debug shaders :
		The first time an intermediate expression is evaluated the shader needs to be
		generated and executed : This will introduce a delay. If this delay is too long
		it will hamper the debug process. By caching the generated inspect shaders, at least the
		generation delay is skipped the next time the inspect shader is used.

		The instrumentation code will increase the amount of branches in the shader.
		Most shader implementations are sensitive for the total branch count in the code.
		This could fail the compilation.

	Initially the project will be build in C++ with wxWidgets 
	(https://www.wxwidgets.org/) as UI framework. 
	The interface with the UI framework will be small so it should be easy to implement
	the debugger functionality in another UI framework.
	The parsing of the shaders is done by the compiler from the ANGLE library
	(https://chromium.googlesource.com/angle/angle)
	
	LibShaderDebugger containst the functionality.
	LibShaderDebugger\Interface is the directory to include if this library is added to a project.
	LibShaderDebugger\Implementation contains the implementation.

	The first concrete gpu language to support is the GLSL dialect as defined in WebGL 1.0.
	The architecture will be done in a way that adding support for other gpu languages like
	WebCL, Compute shaders, or a geometry shader step should be easy to incorperate.
	
	If this turns out to be of some use it will	be converted to Javascript with emscripten.
	So the debugger can be integrated in web based development tools as well.

		
Examples of inspection implementation

Terms used

	inspect expression : The variable or intermediate result which needs to be outputted for inspection.
	
	inspect output variable : This is a variable of the same type as the target expression. It is used to
		temporarily store the value of the inspect expression and store it to the shader output variable.
		
	shader output variable : For a fragment shader this is gl_FragColor. For a vertex shader this is a varying 
		which is used to output from the vertex shader to the fragment shader. In the latter case a simple pass 
		through fragment shader is used to pass the varying on to gl_FragColor;

	Note the examples use pseudo code, declarations are not shown as are swizzle operations to match source 
	and destination component count.
	
	
Lineair statement sequence in main function

	Target is inspect expression in main() 
	Modified shader is assignment of inspect expression to gl_FragColor.
	Rest of statement sequence can be deleted.

	foobar is inspect expression
	
	main()
	{
		...
		foobar = exp;
		...
		
		gl_FragColor = ...;
	}
	
	is modified to  :
	
	main()
	{
		...
		foobar = exp;
		gl_FragColor = foobar;
	}
		
	
Fragment shader with loop

	The added complexity is that the target expression is called multiple times.
	So there needs to be a test which computes if the current index of the loop is the correct one.

	foo is inspect expression
	
	bar()
	{
		...
		foo = ...
		...
	}
	
	main()
	{
		for ix = 0; ix < N; ++ix)
		{
			...
			xyz = bar();
			...
		}
		
		gl_FragColor = ...;
	}
	
	is modified to :

	inspect_out;
	inspect_loop_count;
	inspect_loop_count_target;
	
	bar()
	{
		...
		foo = ...
		if (inspect_loop_count == inspect_loop_count_target)
		{
			inspect_out = foo;
		}
		...
	}
	
	main()
	{
		inspect_loop_count = -1;
		for ix = 0; ix < N; ++ix)
		{
			++inspect_loop_count;
			...
			xyz = bar();
			...
		}
		
		gl_FragColor = inspect_out
	}
	
	
Fragment shader with nested function reachable through multiple code paths

	Multiple code paths to reach a target expression result from a branch or loop which 
	is executed multiple times. In this case some extra info is needed to indicate which 
	invocation of the target expression is the correct one.
	For a branch a boolean is needed to store the result of the conditional expression.
	For shader languages which support switch case constructs an integer is needed to keep track of the state.

	foo is inspect expression

	bar()
	{
		...
		foo = ...
		...
	}
	
	main()
	{
		...
		if (cond)
		{
			bar();
		}
		else
		{
			for ix = 0; ix < N; ++ix)
			{
				bar();
			}	
		}
	}
	
	is modified to :
	
	inspect_out;
	inspect_cond;
	inspect_loop_count;
	inspect_cond_target;
	inspect_loop_count_target;
	
	bar()
	{
		...
		foo = ...
		if (inspect_cond == inspect_cond_target && inspect_loop_count == inspect_loop_count_target)
		{
			inspect_out = foo;
		}
		...
	}
	
	main()
	{
		...
		inspect_cond = false
		if (cond)
		{
			inspect_cond = true;
			bar();
		}
		else
		{
			inspect_loop_count = -1;		
			for ix = 0; ix < N; ++ix)
			{
				++inspect_loop_count;
			
				bar();
			}	
		}
		
		gl_FragColor = inspect_out
	}
	
	
Generalization 
	
	Shader code exists of three different code structures :
		instruction sequence : linear list of instructions
		loop : repeated sequence based on a loop count
		branch : selects from different sequences based on a condition
		
	To keep track of the state of a shader it is sufficient to store the 
	static instruction index (= line nr if one instruction per line)
	and the value of every loop index and branch condition.

	Because recursion is not allowed the maximum depth of the call graph 
	is fixed and can be derived from the static analysis of the ast.
	And although the same instruction can be reached by different paths
	(because it is inside a loop or it is inside a function which 
	has multiple invocation paths). These different paths always have a unique state
	of the branch conditions and/or loop counters higher up in the call graph.
	
	The generalization of the above examples is this:
	All possible unique paths through the shader code can be enumerated.
	This can be done by adding a boolean variable for every binary branch 
	and a execution instance counter for loops. In some cases there is a variable
	in the original shader which can be used (for instance the loop index variable).
	These state variables should be at global scope.
	If this proves to be problematic they could be defined in main() and passed on as extra variables.
	
	To retrieve the correct value of the target expression the state of all these variables should be tested
	in the condition before the expression is stored in the output variable.
	So the pseudo code fragment to store the target expression is like :
	
	if (shader-state-set == target-shader-state-set)
	{
		inspect-output = target-expression;
	}
	
	
	In the main() function the assignment is to the shader output variable instead of the inspect output
	variable.
	
	Note: if a shader contains a discard statement there is no output generated if the discard statement is reached.
	So these need to be removed.
	
	Note: the assignment of the inspect-output to gl_FragColor should be the last statement in main
	to avoid the overwriting of gl_FragColor by some other assignment;

	Not: If there are a lot of boolean branch state variables these can be compacted into a bitfield.
	(This works fine with floats as long as you stay inside the representable integer range of the float type).
	
	Due to that most shaders are relative small and recursion is not allowed, this method to keep track 
	of the shader state seems possible to implement. The increased branch count can be a problem.
	
	
Large sized inspect expressions

	If the inspect expression is larger than 4 floats, (ie a 4x4 matrix), the 
	complete value of the inspect expression must be returned in multiple calls of the inspect shader.
	A part index as uniform is added to the set of inputs to the shader and a
	cascaded if is used to return part of the inspect expression :
	if (part_ix == 0)
	{
		inspect_out = mat4[0];
	}
	else if (part_ix == 1)
	{
		inspect_out = mat4[1];
	}
	
	If multiple render targets are available 
	...
	
	
	If the extra introduced branching becomes problematic, another method would be to multiply the diferent parts
	of the expression with a weight factor (0 or 1) and sum the result of the multiplications.
	The weight factors will be input through a uniforms (packing in a float as bitfield is possible).
	
	
Debugging functionality

	The debugger can find the sequences, branches and loops in a shader from its ast.
	From this information it can compute the extra state variables needed to keep track of
	the values of conditions and loop indices.
	Using these state variables it can present a single stepping experience by advancing the line nr
	and if the line contains a state variable, retrieve the value of the condition and determine 
	if the line nr needs to be advanced to the next line or the shader jumped to another line.
	If the user wants to inspect a expression, the debugger can retrieve the value by generating
	an inspect shader as described in the previous section using the set of state variables.
	
	This debugging experience is for one pixel with all inputs (attributes, uniforms and textures) equal to 
	the ones used in the original shader.


Source elements the user can select 

	A variable like foobar of type float, vecV, matN, user defined structure
	Result of built in function
	Result of user defined function
	Sub expression : 
		Ex : a = 10 * (b + 5);
		If user clicks on one of the left parenthese the subexpression (b + 5) should be the
		target of the inspection
		
		
Implementation details

Internal structure of ast

	The intermediate nodes of the ast are defined in:
	angle/src/compiler/translator/IntermNode.h

	TIntermNode	
	TIntermAggregate;
	TIntermBinary;
	TIntermUnary;
	TIntermConstantUnion;
	TIntermSelection;
	TIntermSwitch;
	TIntermCase;
	TIntermTyped;
	TIntermSymbol;
	TIntermLoop;
	TIntermRaw;

	TIntermNode is the base class, the other classes represent concrete code constructs.
	
	What is missing is a node type id. Currently you have to call a sequence of getAsXyz() functions 
	until one of them returns not nullptr.
	This can be fixed by implementing a GetNodeType(TIntermNode*) utility function.
	
	Tokens are defined in:
	angle/src/compiler/translator/Operator.h
	enum TOperator;
	
	Note: all buildin functions are defined as tokens and not as symbols.
	
	TIntermTraverser is the base class to derive from to implement
	
	Note: The compiler uses a pool allocator to allocate the intermediate nodes and some supporting structures
	this allows for fast allocation and deallocation. It is used through two global functions:
		GetGlobalPoolAllocator() 
		SetGlobalPoolAllocator()

	When the pool allocator is popped, all allocated nodes become invalid.
	See TCompiler::compile().	
	
	Examples of relevant tree parts 
	
	Assignment statement
	Function call
	Return statement
	Conditional expression
	Member selection


Software architecture
	
	The target is to provide a simple interface to the inspect/debug functionality.
	It should be a nice enhancement if the debugger works for other gpu related 
	programming environments as well. Like WebCL, Compute shaders and geometry shaders.
	But the first concrete target is WebCL 1.0.
	
Main components

InspectContextInterface
    This interface provides the interface to the running of the inspected program
    It can run the target program and runs the generated inspect programs to retrieve
    the target information.
    Concrete instances are :
    InspectContextWebGL
    InspectContextCompute    
    InspectVulkan

ProgramInspectorInterface
    This interface provides the methods to inspect the target program from the
    InspectContext.
    ProgramInspectorWebGL
    ProgramInspectorCompute
    ProgramInspectorVulkan

InspectResult
    This object contains the result of an inspect request
	
	For testing that the gl state is not changed by the debugger code to store and retrieve 
	the complete gl state is needed.

	
Other ideas

	By analysing the ast it should be possible to compute an expression for the color for every pixel.
	
	
Code style

	For the initial concept demo I stick to my own style guide.
	If this ever becomes part of ANGLE I will rewrite the code in the ANGLE (Google) code style.
	And (for the c++ implementation) swith to a Google preferred ui framework if needed.


	

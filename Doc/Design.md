Introduction

	This document describes the design of a software shader debugger.
	By software shader debugger is meant that the debugger does not need
	hardware support to retrieve the state. The debugger uses the
	abstract syntax tree (ast) to analyze the structure of the shader code
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
	
	LibShaderDebugger contains the functionality.
	LibShaderDebugger\Interface is the directory to include if this library is added to a project.
	LibShaderDebugger\Implementation contains the implementation.

	The first concrete gpu language to support is the GLSL dialect as defined in WebGL 1.0.
	The architecture will be done in a way that adding support for other gpu languages like
	WebCL, Compute shaders, or a geometry shader step should be easy to incorporate.
	
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
	
	
Linear statement sequence in main function

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
	uniform inspect_loop_count_target;
	
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
	
	inspect_loop_count_target is a uniform which is added to the shader and set to the correct value
	by the debugger.
	
	
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
	uniform inspect_cond_target;
	uniform inspect_loop_count_target;
	
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

	
Fragment shader with multiple invocations of a function
	
	Multiple code paths to reach a target expression result from different invocations of a function
	To identify the correct invocation of a function a global unique invocation id is introduced and set
	just before the function is called.
	This invocation id is tested inside the function so the assignment to the inspect output variable is
	done for the right invocation.
	
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
		bar();
		...
		bar();
	}
	
	is modified to :
	
	inspect_out;
	uniform inspect_invocation_id;
	uniform inspect_invocation_id_target;
	
	bar()
	{
		...
		foo = ...
		if (inspect_invocation_id == inspect_invocation_id_target)
		{
			inspect_out = foo;
		}
		...
	}
	
	main()
	{
		...
		inspect_invocation_id = 1;
		bar();
		...
		inspect_invocation_id = 2;
		bar();
		
		gl_FragColor = inspect_out
	}
	
	
Generalization 
	
	Shader code exists of four different code structures types :
		instruction sequence : linear list of instructions
		loop : repeated sequence based on a loop count
		function : code section called from different locations
		conditional branch : a selection between multiple code sections based on a condition
		These code structures types can recursively contain each other.
		
	A way to look at the shader state is this :
	In a shader with no loops, function calls or branches the execution of the shader visits every node only once.
	So every state of the shader corresponds to a unique location of a node in the tree.
	
	If the tree contains a loop construct, an equivalent single node visiting tree can be constructed by 
	inserting the loop body multiple times (inlining the loop). In this tree every statement corresponds with a unique 
	location in the tree.
	To keep track of the unique state in the original tree the tree location is not enough. It needs to be extended
	with the loop index for the nodes in the loop body.

	If the tree contains a function and multiple invocations of this function, an equivalent single node visiting tree 
	can be constructed by inserting the function body at every call site (inlining the function). In this tree every 
	statement corresponds with a unique location in the tree.
	To keep track of the unique state in the original tree, the tree location is not enough. It needs to be extended
	with the invocation id for the nodes in the function body.
	
	To keep track of the state of a shader it is sufficient to store the 
	static instruction index (= line nr if one instruction per line)
	and the value of every loop index and function invocation id for the code sections where these loop indices and / or invocation ids
	are active.
	
	These state variables should be at global scope.
	If this proves to be problematic they could be defined in main() and passed on as extra variables.
	
	To retrieve the correct value of the target expression the state of all the active variables should be tested
	in the condition before the expression is stored in the output variable.
	So the pseudo code fragment to store the target expression is like :
	
	if (active-shader-state-set == target-shader-state-set)
	{
		inspect-output = target-expression;
	}
	
	In the main() function the assignment is to the shader output variable instead of the inspect output
	variable.
	
	Note: if a shader contains a discard statement there is no output generated if the discard statement is reached.
	So these need to be removed.
	
	Note: the assignment of the inspect-output to gl_FragColor should be the last statement in main
	to avoid the overwriting of gl_FragColor by some other assignment;

	Not: If there are a lot of boolean branch state variables these can be compacted into a bit field.
	(This works fine with floats as long as you stay inside the representable integer range of the float type).
	
	Due to that most shaders are relative small and recursion is not allowed, this method to keep track 
	of the shader state seems possible to implement. The increased branch count can be a problem.
	
	
Determine the next state of a shader from a given shader state  	
	
	A stack is needed to keep track of function invocations.
	
	For a function invocation the next statement is the first statement in the function body. And the location of the function invocation
	is put on the stack.
	For the last statement inside a function : the next statement after the function invocation from the stack is the target. 
	The stack is popped.
	For a selection : the value of the condition needs to be inspected to compute which branch is taken.
	The next statement is the first statement of the selected code section.
	For a loop : the loop condition needs to be inspected to compute if the loop body is entered or the first statement
	after the loop body is the next statement.
	For a sequence the next statement is just the next statement from the ast in 'tree' order.
	This is typically a dept first traversal.
	

Generation of inspect shaders for computing state

	Although it is possible to create one shader which can return the value of all state variables (loop indices, function invocation ids, branch conditions),
	I think it is safer to generate one shader per state variable. I encountered several cases where too complex logical expressions caused a shader
	to fail compilation. Particular inside a loop if that condition is used as a pre condition for a break statement.
	
	So for a shader with 2 loops (A and B) : two shaders are generated : one to retrieve the loop condition of loop A and one to retrieve 
	the loop condition of loop B.
	If loop B happens to be inside loop A, the shader for loop B needs the value for loop A as input.
	
	Inspect shaders for intermediate expressions selected by the user needs to be generated separately.
	They need to contain the union of the set of active shader state variables as input.
	Example : An inspect shader of an expression of a variable inside a function which can be called from two locations :
	Inside a loop 
	Inside main
	
	If one shader is te be generated, the inspect shader needs the invocation id and the loop index as input.
	The precondition to store the selected expression in the inspect output variable contains both the target invocation id and the target loop index.
	For the call site outside the loop the target loop index variable and the loop index inspect variable should be set to a not active value (-1). 

	inspect_out;
	uniform inspect_invocation_id;
	uniform inspect_invocation_id_target;
	uniform inspect_loop_ix;
	uniform inspect_loop_ix_target;
	
	foo()
	{
		if (inspect_invocation_id == target_invocation_id && inspect_loop_ix == inspect_loop_ix_target)
		{
			inspect_out = ...
		}
	}
	
	main()
	{
		inspect_loop_ix = -1;
		...
		inspect_invocation_id = 1;
		foo()
		...
		
		for (loop_ix = 1; loop_ix < N; ++loop_ix)
		{
			++inspect_loop_ix;
			...
			inspect_invocation_id = 2;
			foo()
			...
		}
		inspect_loop_ix = -1;
		
		gl_FragColor = inspect_out;
	}
	
	By setting inspect_invocation_id_target to 1 and inspect_loop_ix_target to -1 an intermediate expression from inside the function at the call site
	outside the loop can be outputted.
	By setting inspect_invocation_id_target to 2 and inspect_loop_ix_target to n an intermediate expression from inside the function at the call site
	inside the loop for loop iteration n can be outputted.
	
	In potential this can result in a lot of shaders. If the maximum count of shaders in the gl context is reached, shaders can be destroyed
	and regenerated when needed. This wil result in a delay to regenerate the shader.
	
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
	
	
	If the extra introduced branching becomes problematic, another method would be to multiply the different parts
	of the expression with a weight factor (0 or 1) and sum the result of the multiplications.
	The weight factors will be input through uniforms (packing in a float as bit field is possible).
	
	
Debugging functionality

	The debugger can find the sequences, branches and loops in a shader from its ast.
	From this information it can compute the extra state variables needed to keep track of
	the values of conditions and loop indices.
	Using these state variables it can present a single stepping experience by advancing the line nr
	and if the line contains a state variable, retrieve the value of the condition and determine 
	if the line nr needs to be advanced to the next line or the shader jumped to another line.
	If the user wants to inspect an expression, the debugger can retrieve the value by generating
	an inspect shader as described in the previous section using the set of state variables.
	
	This debugging experience is for one pixel with all inputs (attributes, uniforms and textures) equal to 
	the ones used in the original shader. But the debugger can allow the user to set these to specific values.
	

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


	

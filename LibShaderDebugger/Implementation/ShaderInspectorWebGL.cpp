/*******************************************************************************\
*                                                                               *
*           Class   : ShaderInspectorWebGL                                      *
*           Purpose : Inspector of a shader using the ANGLE library             *
*           File    : ShaderInspectorWebGL.cpp                                  *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Tranlation unit header
#include "ShaderInspectorWebGL.h"


// Library headers
#include "UtilGLSL.h"
#include "InspectResult.h"
#include "ProgramInspectorI.h"
#include "InspectContextI.h"
#include "ASTSymbolFinder.h"
#include "ASTQueries.h"
#include "ASTPrinter.h"
#include "ASTManipulations.h"
#include "DataFramebuffer.h"
#include "ShaderCompilerESSL.h"
#include "GlUtil.h"
#include "ASTNodeLocationConversion.h"
#include "ShaderStructureNodes.h"


// ANGLE headers
#include <compiler/translator/Intermediate.h>
#include <compiler/translator/ParseContext.h>
#include <compiler/translator/Initialize.h>
#include <shader_utils.h>


// Standard headers
#include <assert.h>


// Constructor
ShaderInspectorWebGL::ShaderInspectorWebGL(
    int                inShaderIx,
    ProgramInspectorI* inProgramInspector,
    sh::GLenum         inShaderType,
    ShShaderSpec       inShaderSpec,
    ShShaderOutput     inOutputLanguage,
    ShCompileOptions   inCompileOptions)
:
    ShaderInspectorI (inShaderIx, inProgramInspector),
    mShaderType      (inShaderType),
    mShaderSpec      (inShaderSpec),
    mOutputLanguage  (inOutputLanguage),
    mCompileOptions  (inCompileOptions),
    mNodeFactory     (mInfoSink)
{

}


// Initialize the inspector
// virtual
bool
ShaderInspectorWebGL::Initialize()
{
    // Construct compiler
    ShInitBuiltInResources(&mResources);

    if (mShaderSpec == SH_GLES3_SPEC || mShaderSpec == SH_WEBGL2_SPEC)
    {
        mResources.FragmentPrecisionHigh = 1;
    }

    mCompiler.reset(new ShaderCompilerESSL(mShaderType, mShaderSpec));
    if (mCompiler.get() == nullptr)
    {
        return false;
    }

    if (!mCompiler->Init(mResources))
    {
        mCompiler.release();
        return false;
    }

    // Create parse context
    TExtensionBehavior ext_behavior;
    InitExtensionBehavior(mResources, ext_behavior);
    mParseContext.reset(new TParseContext(
        mCompiler->getSymbolTable(),
        ext_behavior,
        mNodeFactory,
        mShaderType,
        mCompiler->getShaderSpec(),
        mCompileOptions,
        true,
        mInfoSink,
        mResources
        ));


    if (mParseContext.get() == nullptr)
    {
        return false;
    }

    mDataReader.reset(new DataFramebuffer);

    return (mDataReader.get() != nullptr);
}


// Compute all ast nodes where the shader can take different paths through the code
void
ShaderInspectorWebGL::ComputeDecisionNodes()
{
    std::string source(GetInspectContext()->GetShaderSource(mShaderIx));
    TIntermNode* ast(mCompiler->CompileToAST(source, mCompileOptions));


}


/*
// Returns ast node of first statement to execute
// virtual
TIntermNode*
ShaderInspectorWebGL::GetFirstStatement()
{
    // Find function main()
    // AGGREGATE : EOpFunction main(


    return nullptr;
}
*/

// Inspect a token at the given source position
void
ShaderInspectorWebGL::Inspect(
    const std::string& inSymbol,
    size_t             inLine,
    size_t             inColumn,
    InspectResult&     outResult)
{
    std::string source(GetInspectContext()->GetShaderSource(mShaderIx));

    TIntermNode* ast(mCompiler->CompileToAST(source, mCompileOptions));

    outResult.mInfoText =
        "Inspect : '" + inSymbol +
        "' (line " + std::to_string(inLine) + ")\n";


    // To understand the tree structure
    ASTPrinter::sPrint(ast, outResult.mInfoText);

    ASTSymbolFinder symbol_finder(inSymbol, inLine, inColumn);
    ast->traverse(&symbol_finder);
    if (!symbol_finder.GetPathToSymbolNode().empty())
    {

#ifdef _DEBUG
        // Test conversion node path from/to location by index
        std::vector<TIntermNode*> path_to_symbol(symbol_finder.GetPathToSymbolNode());
        std::vector<int> node_indices;
        GetNodeIndexPath(path_to_symbol, node_indices);
        // Convert back to node path and verify
        std::vector<TIntermNode*> shadow_node_path;
        GetNodePath(path_to_symbol[0], node_indices, shadow_node_path);
        assert(path_to_symbol == shadow_node_path);
#endif

        // Transform node tree to generate inspect shader
        TransformAST(
            symbol_finder.GetPathToSymbolNode(),
            outResult);

        // To understand the tree structure
        outResult.mInfoText += "\n Modified ast:\n";
        ASTPrinter::sPrint(ast, outResult.mInfoText);

        mCompiler->CompileFromAST(ast, mCompileOptions);

        // Append generated GLSL
        std::string inspect_src(mCompiler->getInfoSink().obj.str());
        outResult.mInfoText += "\nGLSL:\n";
        outResult.mInfoText += inspect_src;

        RunInspectProgram(inspect_src, outResult);
    }
    else
    {
        outResult.mResultCode = INSPECT_RESULT_SYMBOL_NOT_FOUND;
    }
}


// Transform AST to return target symbol
void
ShaderInspectorWebGL::TransformAST(
    const std::vector<TIntermNode*>& inTargetSymbolPath,
    InspectResult&                   outResult
)
{
    // Determine type of symbol

    TBasicType target_type(inTargetSymbolPath.front()->getAsTyped()->getBasicType());

    if (target_type == EbtFloat)
    {
        // Simple implementation
        // Assign value of symbol to gl_FragColor
        // And return
        // Will only work in fragment shader and in code which is directly contained in main()

        // If parent is declaration there is nothing to inspect
        if (IsInDeclaration(inTargetSymbolPath))
        {
            outResult.mInfoText += "\nDeclaration, no data to return\n";
            outResult.mResultCode = INSPECT_RESULT_TOKEN_NO_INFO;
        }
        else
        {
            // Insert return direct after assignment in parent sequence
            // Find sequence which contains the
            int seq_node_ix(FindDirectParentSequence(inTargetSymbolPath));
            if (seq_node_ix != -1)
            {
                TIntermAggregate* seq_node(inTargetSymbolPath[seq_node_ix]->getAsAggregate());
                assert(seq_node != nullptr);
                assert(seq_node_ix < int(inTargetSymbolPath.size()) - 1);

                // The node direct below the node containing the sequence is the one
                // which contains the symbol selected by the user
                int seq_ix(FindNodeinSequence(inTargetSymbolPath[seq_node_ix + 1], seq_node));
                if (seq_ix != -1)
                {
                    // Change AST

                    // Remove all statements from sequence after seq_ix
                    seq_node->getSequence()->resize(seq_ix + 1);

                    TIntermTyped* target_node(FindValueExpressionNode(inTargetSymbolPath)->getAsTyped());
                    TIntermTyped* output_node(CreateOutputVariableNode());
                    TIntermTyped* assign_stmt(CreateAssignmentVecN(
                        output_node,
                        target_node,
                        *mParseContext));

                    seq_node->getSequence()->push_back(assign_stmt);

                    // Add return statement to parent of sequence
                    TIntermBranch* return_stmt(CreateReturnStatement());
                    seq_node->getSequence()->push_back(return_stmt);
                }
            }
        }
    }
    else
    {
        outResult.mInfoText += "\nType not handled\n";
        outResult.mResultCode = INSPECT_RESULT_NOT_YET_IMPLEMENTED;
    }
}


// Returns node which receives the output value
TIntermTyped*
ShaderInspectorWebGL::CreateOutputVariableNode()
{
    switch(mShaderType)
    {
        case GL_FRAGMENT_SHADER:
            // Create receiving
            TInfoSink info_sink;
            TIntermediate node_factory(info_sink);

            TSourceLoc    src_loc = {0, 0, 0, 0};
            TType         type_vec4(EbtFloat, EbpMedium, EvqFragColor, 4, 1);
            return node_factory.addSymbol(
                -1,
                "gl_FragColor",
                type_vec4,
                src_loc);
    }

    assert(false);
    return nullptr;
}


// Run modified shader in context of original program
void
ShaderInspectorWebGL::RunInspectProgram(
    const std::string& inInspectShaderSrc,
    InspectResult&     outResult)
{
    GLint prev_prg;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_prg);

    // Create program
    GLuint prg(glCreateProgram());
    assert(prg != GL_ZERO);
    GLenum gl_error(glGetError());
    assert(gl_error == NO_ERROR);

    SpInspectContextI inspect_ctx(GetInspectContext());

    // For all shader which have an index lower compared to this shader
    // Set original shader
    for (size_t sh_ix = 0; sh_ix < mShaderIx; ++sh_ix)
    {
        GLuint handle(GLuint(inspect_ctx->GetHandle(sh_ix)));
        glAttachShader(prg, handle);
    }

    // Attach inspect shader
    GLuint h_inspect(CompileShader(mShaderType, inInspectShaderSrc));
    glAttachShader(prg, h_inspect);

    // TODO
    // For all shader which have an index higher compared to this shader
    // Set passthrough shader

    glLinkProgram(prg);
    GLint prg_linked;
    glGetProgramiv(prg, GL_LINK_STATUS, &prg_linked);
    glValidateProgram(prg);
    GLint prg_valid;
    glGetProgramiv(prg, GL_VALIDATE_STATUS, &prg_valid);
    if (prg_linked == GL_TRUE && prg_valid == GL_TRUE)
    {
        glUseProgram(prg);

        // x, y, width, height
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        mDataReader->Bind(viewport[2], viewport[3]);

        // Produce inspect result
        inspect_ctx->RunTargetProgram();

        // Read data from texture
        outResult.mInspectData = mDataReader->ReadData();
        mDataReader->Unbind();
    }
    else
    {
        outResult.mInfoText += "Program link failed: ";
        AppendProgramInfo(prg, outResult.mInfoText);
    }

    // Restore previous shader program
    glUseProgram(prev_prg);

    // Destroy inspect shader program
    glDetachShader(prg, h_inspect);
    glDeleteShader(h_inspect);
    glDeleteProgram(prg);

    assert(glGetError() == NO_ERROR);
}

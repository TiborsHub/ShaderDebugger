/*******************************************************************************\
*                                                                               *
*           Class   : ProgramInspectorWebGL                                     *
*           Purpose : Inspector for WebGL shader programs                       *
*           File    : ProgramInspectorWebGL.cpp                                 *
*           Author  : Tibor den Ouden                                           *
*                                                                               *
\*******************************************************************************/


// Project headers
#include "ProgramInspectorWebGL.h"


// Library headers
#include "UtilGLSL.h"
#include "InspectResult.h"
#include "InspectContextI.h"
#include "ShaderInspectorI.h"
#include "CreateShaderInspectorInstance.h"
#include "UtilText.h"


// Standard headers
#include <assert.h>


// Constructor
ProgramInspectorWebGL::ProgramInspectorWebGL(SpInspectContextI& inInspectContext, size_t inShaderIx) :
    ProgramInspectorI(inInspectContext, inShaderIx)
{

}


// Returns ast node of first statement to execute
// virtual
TIntermNode*
ProgramInspectorWebGL::GetFirstStatement()
{
    // Find function main()
    // AGGREGATE : EOpFunction main(


    return nullptr;
}


// Inspect a token in a shader at the given source position
// virtual
void
ProgramInspectorWebGL::Inspect(size_t inSourceIx, InspectResult& outResult)
{
    std::string shader_source(mContext->GetShaderSource(mShaderIx));

    // Find token in ast
    if (inSourceIx >=0 && inSourceIx < shader_source.size())
    {
        if (IsSeparatorGLSL(shader_source[inSourceIx]))
        {
            outResult.mResultCode = INSPECT_RESULT_TOKEN_NO_INFO;
        }
        else
        {
            // Potential value returning token selected
            size_t first_ch_ix(FindFirstCharOfSymbolGLSL(shader_source, inSourceIx));
            tInspectKey inspect_key(mShaderIx, first_ch_ix);
            tTokenInspectorMap::iterator inspect_it(mTokenInspectorMap.find(inspect_key));
            if (inspect_it == mTokenInspectorMap.end())
            {
                // Insert new inspector
                uint64_t shader_type(mContext->GetShaderType(mShaderIx));
                SpShaderInspectorI inspector(CreateShaderInspectorInstance(
                    mShaderIx,
                    this,
                    shader_type));

                tTokenInspectorMap::value_type entry(inspect_key, std::move(inspector));
                std::pair<tTokenInspectorMap::iterator, bool> insert_result(mTokenInspectorMap.insert(entry));

                if (insert_result.first->second->Initialize())
                {
                    inspect_it = insert_result.first;
                }
                else
                {
                    outResult.mResultCode = INSPECT_RESULT_SHADER_INSPECTOR_INITIALIZATION_FAILED;
                }
            }

            if (outResult.mResultCode == INSPECT_RESULT_UNKNOWN)
            {
                // Get target symbol
                size_t last_ch_ix(FindLastCharOfSymbolGLSL(shader_source, inSourceIx));
                std::string symbol(shader_source.substr(first_ch_ix, last_ch_ix - first_ch_ix + 1));

                // Convert offset to (line nr, column)
                size_t line_nr;
                size_t column;
                FindLineColumnForOffset(
                    shader_source,
                    first_ch_ix,
                    line_nr,
                    column);

                inspect_it->second->Inspect(symbol, line_nr, column, outResult);
            }
        }
    }
    else
    {
        outResult.mResultCode = INSPECT_RESULT_INVALID_SOURCE_POS;
    }
}

/*******************************************************************************\
*																				*
*			Class    : ASTPrinter  									            *
*			Purpose  : Prints the ANGLE AST to a stream                         *
*                      Based on \angle\src\compiler\translator\intermOut.cpp    *
*			File     : ASTPrinter.cpp										    *
*			Author   : Tibor den Ouden											*
*																				*
\*******************************************************************************/


// Translation unit header
#include "ASTPrinter.h"


// Library headers
#include "ASTQueries.h"


// Standard headers
#include <iomanip>


// Definitions local to translation unit
namespace
{

// Utility macro to create object name mapping
#define OBJECT_NAME_ENTRY(inObject) { inObject, #inObject }

// GLSL operator tokens and their names
// The operator id is stored (but not used) to discover if all values are valid TOperator enum values
struct
{
    TOperator                   mId;
    const char*                 mName;
}
OperatorNames[] =
{
    OBJECT_NAME_ENTRY(EOpNull),
    OBJECT_NAME_ENTRY(EOpSequence),        // denotes a list of statements, or parameters, etc.
    OBJECT_NAME_ENTRY(EOpFunctionCall),
    OBJECT_NAME_ENTRY(EOpFunction),        // For function definition
    OBJECT_NAME_ENTRY(EOpParameters),      // an aggregate listing the parameters to a function

    OBJECT_NAME_ENTRY(EOpDeclaration),
    OBJECT_NAME_ENTRY(EOpInvariantDeclaration), // Specialized declarations for attributing invariance
    OBJECT_NAME_ENTRY(EOpPrototype),


    // Unary operators
    OBJECT_NAME_ENTRY(EOpNegative),
    OBJECT_NAME_ENTRY(EOpPositive),
    OBJECT_NAME_ENTRY(EOpLogicalNot),
    OBJECT_NAME_ENTRY(EOpVectorLogicalNot),
    OBJECT_NAME_ENTRY(EOpBitwiseNot),

    OBJECT_NAME_ENTRY(EOpPostIncrement),
    OBJECT_NAME_ENTRY(EOpPostDecrement),
    OBJECT_NAME_ENTRY(EOpPreIncrement),
    OBJECT_NAME_ENTRY(EOpPreDecrement),

    // Binary operators
    OBJECT_NAME_ENTRY(EOpAdd),
    OBJECT_NAME_ENTRY(EOpSub),
    OBJECT_NAME_ENTRY(EOpMul),
    OBJECT_NAME_ENTRY(EOpDiv),
    OBJECT_NAME_ENTRY(EOpIMod),
    OBJECT_NAME_ENTRY(EOpEqual),
    OBJECT_NAME_ENTRY(EOpNotEqual),
    OBJECT_NAME_ENTRY(EOpVectorEqual),
    OBJECT_NAME_ENTRY(EOpVectorNotEqual),
    OBJECT_NAME_ENTRY(EOpLessThan),
    OBJECT_NAME_ENTRY(EOpGreaterThan),
    OBJECT_NAME_ENTRY(EOpLessThanEqual),
    OBJECT_NAME_ENTRY(EOpGreaterThanEqual),
    OBJECT_NAME_ENTRY(EOpComma),

    OBJECT_NAME_ENTRY(EOpVectorTimesScalar),
    OBJECT_NAME_ENTRY(EOpVectorTimesMatrix),
    OBJECT_NAME_ENTRY(EOpMatrixTimesVector),
    OBJECT_NAME_ENTRY(EOpMatrixTimesScalar),

    OBJECT_NAME_ENTRY(EOpLogicalOr),
    OBJECT_NAME_ENTRY(EOpLogicalXor),
    OBJECT_NAME_ENTRY(EOpLogicalAnd),

    OBJECT_NAME_ENTRY(EOpBitShiftLeft),
    OBJECT_NAME_ENTRY(EOpBitShiftRight),

    OBJECT_NAME_ENTRY(EOpBitwiseAnd),
    OBJECT_NAME_ENTRY(EOpBitwiseXor),
    OBJECT_NAME_ENTRY(EOpBitwiseOr),

    OBJECT_NAME_ENTRY(EOpIndexDirect),
    OBJECT_NAME_ENTRY(EOpIndexIndirect),
    OBJECT_NAME_ENTRY(EOpIndexDirectStruct),
    OBJECT_NAME_ENTRY(EOpIndexDirectInterfaceBlock),

    OBJECT_NAME_ENTRY(EOpVectorSwizzle),

    // Built-in functions potentially mapped to operators
    OBJECT_NAME_ENTRY(EOpRadians),
    OBJECT_NAME_ENTRY(EOpDegrees),
    OBJECT_NAME_ENTRY(EOpSin),
    OBJECT_NAME_ENTRY(EOpCos),
    OBJECT_NAME_ENTRY(EOpTan),
    OBJECT_NAME_ENTRY(EOpAsin),
    OBJECT_NAME_ENTRY(EOpAcos),
    OBJECT_NAME_ENTRY(EOpAtan),

    OBJECT_NAME_ENTRY(EOpSinh),
    OBJECT_NAME_ENTRY(EOpCosh),
    OBJECT_NAME_ENTRY(EOpTanh),
    OBJECT_NAME_ENTRY(EOpAsinh),
    OBJECT_NAME_ENTRY(EOpAcosh),
    OBJECT_NAME_ENTRY(EOpAtanh),

    OBJECT_NAME_ENTRY(EOpPow),
    OBJECT_NAME_ENTRY(EOpExp),
    OBJECT_NAME_ENTRY(EOpLog),
    OBJECT_NAME_ENTRY(EOpExp2),
    OBJECT_NAME_ENTRY(EOpLog2),
    OBJECT_NAME_ENTRY(EOpSqrt),
    OBJECT_NAME_ENTRY(EOpInverseSqrt),

    OBJECT_NAME_ENTRY(EOpAbs),
    OBJECT_NAME_ENTRY(EOpSign),
    OBJECT_NAME_ENTRY(EOpFloor),
    OBJECT_NAME_ENTRY(EOpTrunc),
    OBJECT_NAME_ENTRY(EOpRound),
    OBJECT_NAME_ENTRY(EOpRoundEven),
    OBJECT_NAME_ENTRY(EOpCeil),
    OBJECT_NAME_ENTRY(EOpFract),
    OBJECT_NAME_ENTRY(EOpMod),
    OBJECT_NAME_ENTRY(EOpModf),
    OBJECT_NAME_ENTRY(EOpMin),
    OBJECT_NAME_ENTRY(EOpMax),
    OBJECT_NAME_ENTRY(EOpClamp),
    OBJECT_NAME_ENTRY(EOpMix),
    OBJECT_NAME_ENTRY(EOpStep),
    OBJECT_NAME_ENTRY(EOpSmoothStep),
    OBJECT_NAME_ENTRY(EOpIsNan),
    OBJECT_NAME_ENTRY(EOpIsInf),

    OBJECT_NAME_ENTRY(EOpFloatBitsToInt),
    OBJECT_NAME_ENTRY(EOpFloatBitsToUint),
    OBJECT_NAME_ENTRY(EOpIntBitsToFloat),
    OBJECT_NAME_ENTRY(EOpUintBitsToFloat),

    OBJECT_NAME_ENTRY(EOpPackSnorm2x16),
    OBJECT_NAME_ENTRY(EOpPackUnorm2x16),
    OBJECT_NAME_ENTRY(EOpPackHalf2x16),
    OBJECT_NAME_ENTRY(EOpUnpackSnorm2x16),
    OBJECT_NAME_ENTRY(EOpUnpackUnorm2x16),
    OBJECT_NAME_ENTRY(EOpUnpackHalf2x16),

    OBJECT_NAME_ENTRY(EOpLength),
    OBJECT_NAME_ENTRY(EOpDistance),
    OBJECT_NAME_ENTRY(EOpDot),
    OBJECT_NAME_ENTRY(EOpCross),
    OBJECT_NAME_ENTRY(EOpNormalize),
    OBJECT_NAME_ENTRY(EOpFaceForward),
    OBJECT_NAME_ENTRY(EOpReflect),
    OBJECT_NAME_ENTRY(EOpRefract),

    OBJECT_NAME_ENTRY(EOpDFdx),            // Fragment only, OES_standard_derivatives extension
    OBJECT_NAME_ENTRY(EOpDFdy),            // Fragment only, OES_standard_derivatives extension
    OBJECT_NAME_ENTRY(EOpFwidth),          // Fragment only, OES_standard_derivatives extension

    OBJECT_NAME_ENTRY(EOpMatrixTimesMatrix),

    OBJECT_NAME_ENTRY(EOpOuterProduct),
    OBJECT_NAME_ENTRY(EOpTranspose),
    OBJECT_NAME_ENTRY(EOpDeterminant),
    OBJECT_NAME_ENTRY(EOpInverse),

    OBJECT_NAME_ENTRY(EOpAny),
    OBJECT_NAME_ENTRY(EOpAll),

    // Branch operators
    OBJECT_NAME_ENTRY(EOpKill),            // Fragment only
    OBJECT_NAME_ENTRY(EOpReturn),
    OBJECT_NAME_ENTRY(EOpBreak),
    OBJECT_NAME_ENTRY(EOpContinue),

    // Constructors
    OBJECT_NAME_ENTRY(EOpConstructInt),
    OBJECT_NAME_ENTRY(EOpConstructUInt),
    OBJECT_NAME_ENTRY(EOpConstructBool),
    OBJECT_NAME_ENTRY(EOpConstructFloat),
    OBJECT_NAME_ENTRY(EOpConstructVec2),
    OBJECT_NAME_ENTRY(EOpConstructVec3),
    OBJECT_NAME_ENTRY(EOpConstructVec4),
    OBJECT_NAME_ENTRY(EOpConstructBVec2),
    OBJECT_NAME_ENTRY(EOpConstructBVec3),
    OBJECT_NAME_ENTRY(EOpConstructBVec4),
    OBJECT_NAME_ENTRY(EOpConstructIVec2),
    OBJECT_NAME_ENTRY(EOpConstructIVec3),
    OBJECT_NAME_ENTRY(EOpConstructIVec4),
    OBJECT_NAME_ENTRY(EOpConstructUVec2),
    OBJECT_NAME_ENTRY(EOpConstructUVec3),
    OBJECT_NAME_ENTRY(EOpConstructUVec4),
    OBJECT_NAME_ENTRY(EOpConstructMat2),
    OBJECT_NAME_ENTRY(EOpConstructMat2x3),
    OBJECT_NAME_ENTRY(EOpConstructMat2x4),
    OBJECT_NAME_ENTRY(EOpConstructMat3x2),
    OBJECT_NAME_ENTRY(EOpConstructMat3),
    OBJECT_NAME_ENTRY(EOpConstructMat3x4),
    OBJECT_NAME_ENTRY(EOpConstructMat4x2),
    OBJECT_NAME_ENTRY(EOpConstructMat4x3),
    OBJECT_NAME_ENTRY(EOpConstructMat4),
    OBJECT_NAME_ENTRY(EOpConstructStruct),

    // Copy operators
    OBJECT_NAME_ENTRY(EOpAssign),
    OBJECT_NAME_ENTRY(EOpInitialize),
    OBJECT_NAME_ENTRY(EOpAddAssign),
    OBJECT_NAME_ENTRY(EOpSubAssign),

    OBJECT_NAME_ENTRY(EOpMulAssign),
    OBJECT_NAME_ENTRY(EOpVectorTimesMatrixAssign),
    OBJECT_NAME_ENTRY(EOpVectorTimesScalarAssign),
    OBJECT_NAME_ENTRY(EOpMatrixTimesScalarAssign),
    OBJECT_NAME_ENTRY(EOpMatrixTimesMatrixAssign),

    OBJECT_NAME_ENTRY(EOpDivAssign),
    OBJECT_NAME_ENTRY(EOpIModAssign),
    OBJECT_NAME_ENTRY(EOpBitShiftLeftAssign),
    OBJECT_NAME_ENTRY(EOpBitShiftRightAssign),
    OBJECT_NAME_ENTRY(EOpBitwiseAndAssign),
    OBJECT_NAME_ENTRY(EOpBitwiseXorAssign),
    OBJECT_NAME_ENTRY(EOpBitwiseOrAssign)
};

#undef OBJECT_NAME_ENTRY


// Consistency checks
// Will not detect if a new value is added after the current last one (EOpBitwiseOrAssign)
static_assert(EOpNull == 0, "EOpNull is no longer first entry");
static_assert( (EOpBitwiseOrAssign - EOpNull + 1) == sizeof(OperatorNames) / sizeof(OperatorNames[0]), "Not all operator ids in table");


// Return the name of the operator token
const char*
GetOperatorName(TOperator inOperator)
{
    if (inOperator >= EOpNull && inOperator <= EOpBitwiseOrAssign)
    {
        return OperatorNames[inOperator].mName;
    }
    else
    {
        assert(false);
        return "Unknown operator value";
    }
}


const char*
GetNodeName(TIntermNode* inNode)
{
    switch(GetNodeType(inNode))
    {
        case AST_NODE_TYPE_TYPED:          return "TYPED";
        case AST_NODE_TYPE_CONSTANT_UNION: return "CONSTANT UNION";
        case AST_NODE_TYPE_AGGREGATE:      return "AGGREGATE";
        case AST_NODE_TYPE_BINARY:         return "BINARY";
        case AST_NODE_TYPE_UNARY:          return "UNARY";
        case AST_NODE_TYPE_SELECTION:      return "SELECTION";
        case AST_NODE_TYPE_SWITCH:         return "SWITCH";
        case AST_NODE_TYPE_CASE:           return "CASE";
        case AST_NODE_TYPE_SYMBOL:         return "SYMBOL";
        case AST_NODE_TYPE_LOOP:           return "LOOP";
        case AST_NODE_TYPE_RAW:            return "RAW";

        default:
            assert(false);
            return "";
    }
}


} // namespace



// Constructor
ASTPrinter::ASTPrinter(
    std::ostream& outStream,
    int           inLineNrWidth,
    int           inIndentWidth)
:
    TIntermTraverser (true, false, false),
    mStream          (outStream),
    mLineNrWidth     (inLineNrWidth),
    mIndentation     (inIndentWidth, ' ')
{

}


// Print ast to stream
// static
void
ASTPrinter::sPrint(
    TIntermNode*  inRoot,
    std::ostream& outStream,
    int           inLineNrWidth,
    int           inIndentWidth)
{
    ASTPrinter printer(outStream);
    inRoot->traverse(&printer);
}


// Print ast to string
// static
void
ASTPrinter::sPrint(
    TIntermNode* inRoot,
    std::string& outString,
    int          inLineNrWidth,
    int          inIndentWidth)
{
    std::stringstream stream_out;
    sPrint(inRoot, stream_out);
    outString += stream_out.str();
}


// Output prefix for every line to show hierarchy
void
ASTPrinter::OutputLinePrefix(TIntermNode* inNode)
{
    mStream << std::setw(mLineNrWidth) <<inNode->getLine().first_line << ' ';
    for (int ch_ix = 0; ch_ix < mDepth; ++ch_ix)
    {
        mStream << mIndentation;
    }
}


// Return true from interior nodes to have the external traversal
// continue on to children.  If you process children yourself,
// return false.


void
ASTPrinter::visitSymbol(TIntermSymbol* inSymbolNode)
{
    OutputLinePrefix(inSymbolNode);

    mStream << "'" << inSymbolNode->getSymbol() << "' ";
    mStream << "(" << inSymbolNode->getCompleteString() << ")\n";
}


bool
ASTPrinter::visitBinary(Visit inVisit, TIntermBinary *inBinaryNode)
{
    (void)inVisit;

    OutputLinePrefix(inBinaryNode);
    TOperator oper(inBinaryNode->getOp());
    switch (oper)
    {
      case EOpAssign:
      case EOpInitialize:
      case EOpAddAssign:
      case EOpSubAssign:
      case EOpMulAssign:
      case EOpVectorTimesMatrixAssign:
      case EOpVectorTimesScalarAssign:
      case EOpMatrixTimesScalarAssign:
      case EOpMatrixTimesMatrixAssign:
      case EOpDivAssign:
      case EOpIModAssign:
      case EOpBitShiftLeftAssign:
      case EOpBitShiftRightAssign:
      case EOpBitwiseAndAssign:
      case EOpBitwiseXorAssign:
      case EOpBitwiseOrAssign:
      case EOpIndexDirect:
      case EOpIndexIndirect:
      case EOpIndexDirectStruct:
      case EOpIndexDirectInterfaceBlock:
      case EOpVectorSwizzle:
      case EOpAdd:
      case EOpSub:
      case EOpMul:
      case EOpDiv:
      case EOpIMod:
      case EOpBitShiftLeft:
      case EOpBitShiftRight:
      case EOpBitwiseAnd:
      case EOpBitwiseXor:
      case EOpBitwiseOr:
      case EOpEqual:
      case EOpNotEqual:
      case EOpLessThan:
      case EOpGreaterThan:
      case EOpLessThanEqual:
      case EOpGreaterThanEqual:

      case EOpVectorTimesScalar:
      case EOpVectorTimesMatrix:
      case EOpMatrixTimesVector:
      case EOpMatrixTimesScalar:
      case EOpMatrixTimesMatrix:

      case EOpLogicalOr:
      case EOpLogicalXor:
      case EOpLogicalAnd:
        mStream << GetOperatorName(oper);
        break;

      default:
        mStream << "<unknown op>";
        break;
    }

    mStream << " (" << inBinaryNode->getCompleteString() << ")\n";


    // Special handling for direct indexes. Because constant
    // unions are not aware they are struct indexes, treat them
    // here where we have that contextual knowledge.
    if (oper == EOpIndexDirectStruct ||
        oper == EOpIndexDirectInterfaceBlock)
    {
        ++mDepth;
        inBinaryNode->getLeft()->traverse(this);
        --mDepth;

        TIntermConstantUnion *intermConstantUnion = inBinaryNode->getRight()->getAsConstantUnion();
        ASSERT(intermConstantUnion);

        ++mDepth;
        OutputLinePrefix(intermConstantUnion);

        // The following code finds the field name from the constant union
        const TConstantUnion *constantUnion = intermConstantUnion->getUnionArrayPointer();
        const TStructure *structure = inBinaryNode->getLeft()->getType().getStruct();
        const TInterfaceBlock *interfaceBlock = inBinaryNode->getLeft()->getType().getInterfaceBlock();
        ASSERT(structure || interfaceBlock);

        const TFieldList &fields = structure ? structure->fields() : interfaceBlock->fields();

        const TField *field = fields[constantUnion->getIConst()];

        mStream << constantUnion->getIConst() << " (field '" << field->name() << "')";

        --mDepth;

        return false;
    }

    return true;
}


bool
ASTPrinter::visitUnary(Visit inVisit, TIntermUnary *inUnaryNode)
{
    (void)inVisit;

    OutputLinePrefix(inUnaryNode);

    TOperator oper(inUnaryNode->getOp());
    switch (oper)
    {
        case EOpNegative:
        case EOpPositive:
        case EOpVectorLogicalNot:
        case EOpLogicalNot:
        case EOpBitwiseNot:

        case EOpPostIncrement:
        case EOpPostDecrement:
        case EOpPreIncrement:
        case EOpPreDecrement:

        case EOpRadians:
        case EOpDegrees:
        case EOpSin:
        case EOpCos:
        case EOpTan:
        case EOpAsin:
        case EOpAcos:
        case EOpAtan:

        case EOpSinh:
        case EOpCosh:
        case EOpTanh:
        case EOpAsinh:
        case EOpAcosh:
        case EOpAtanh:

        case EOpExp:
        case EOpLog:
        case EOpExp2:
        case EOpLog2:
        case EOpSqrt:
        case EOpInverseSqrt:

        case EOpAbs:
        case EOpSign:
        case EOpFloor:
        case EOpTrunc:
        case EOpRound:
        case EOpRoundEven:
        case EOpCeil:
        case EOpFract:
        case EOpIsNan:
        case EOpIsInf:

        case EOpFloatBitsToInt:
        case EOpFloatBitsToUint:
        case EOpIntBitsToFloat:
        case EOpUintBitsToFloat:

        case EOpPackSnorm2x16:
        case EOpPackUnorm2x16:
        case EOpPackHalf2x16:

        case EOpUnpackSnorm2x16:
        case EOpUnpackUnorm2x16:
        case EOpUnpackHalf2x16:

        case EOpLength:
        case EOpNormalize:
        // case EOpDPdx:
        // case EOpDPdy:
        // case EOpFwidth:

        case EOpDeterminant:
        case EOpTranspose:
        case EOpInverse:

        case EOpAny:
        case EOpAll:
            mStream << GetOperatorName(inUnaryNode->getOp());
            break;

        default:
            mStream << "ERROR: Bad unary op";
            break;
    }

    mStream << " (" << inUnaryNode->getCompleteString() << ")\n";

    return true;
}


bool
ASTPrinter::visitAggregate(Visit inVisit, TIntermAggregate* inAggregateNode)
{
    (void)inVisit;

    TOperator oper(inAggregateNode->getOp());
    if (oper == EOpNull)
    {
        mStream << "ERROR: node is still EOpNull!";
        return true;
    }

    OutputLinePrefix(inAggregateNode);

    switch (oper)
    {
        case EOpSequence:
        case EOpComma:

        case EOpParameters:

        case EOpConstructFloat:
        case EOpConstructVec2:
        case EOpConstructVec3:
        case EOpConstructVec4:
        case EOpConstructBool:
        case EOpConstructBVec2:
        case EOpConstructBVec3:
        case EOpConstructBVec4:
        case EOpConstructInt:
        case EOpConstructIVec2:
        case EOpConstructIVec3:
        case EOpConstructIVec4:
        case EOpConstructUInt:
        case EOpConstructUVec2:
        case EOpConstructUVec3:
        case EOpConstructUVec4:
        case EOpConstructMat2:
        case EOpConstructMat2x3:
        case EOpConstructMat2x4:
        case EOpConstructMat3x2:
        case EOpConstructMat3:
        case EOpConstructMat3x4:
        case EOpConstructMat4x2:
        case EOpConstructMat4x3:
        case EOpConstructMat4:
        case EOpConstructStruct:

        case EOpLessThan:
        case EOpGreaterThan:
        case EOpLessThanEqual:
        case EOpGreaterThanEqual:
        case EOpVectorEqual:
        case EOpVectorNotEqual:

        case EOpMod:
        case EOpModf:
        case EOpPow:

        case EOpAtan:

        case EOpMin:
        case EOpMax:
        case EOpClamp:
        case EOpMix:
        case EOpStep:
        case EOpSmoothStep:

        case EOpDistance:
        case EOpDot:
        case EOpCross:
        case EOpFaceForward:
        case EOpReflect:
        case EOpRefract:
        case EOpMul:

        case EOpOuterProduct:

        case EOpDeclaration:
        case EOpInvariantDeclaration:
            mStream << GetOperatorName(oper);
            break;

        case EOpFunction:
        case EOpFunctionCall:
        case EOpPrototype:
            mStream << GetOperatorName(oper) << ' ' << inAggregateNode->getName();
            break;

        default:
            mStream << "ERROR: Bad aggregation op";
            break;
    }

    if (oper != EOpSequence && oper != EOpParameters)
    {
        mStream << " (" << inAggregateNode->getCompleteString() << ")";
    }

    mStream << "\n";

    return true;
}


bool
ASTPrinter::visitSelection(Visit inVisit, TIntermSelection *inSelectionNode)
{
    (void)inVisit;

    OutputLinePrefix(inSelectionNode);

    mStream << "Test condition and select";
    mStream << " (" << inSelectionNode->getCompleteString() << ")\n";

    ++mDepth;

    OutputLinePrefix(inSelectionNode);
    mStream << "Condition\n";
    inSelectionNode->getCondition()->traverse(this);

    OutputLinePrefix(inSelectionNode);
    if (inSelectionNode->getTrueBlock())
    {
        mStream << "true case\n";
        inSelectionNode->getTrueBlock()->traverse(this);
    }
    else
    {
        mStream << "true case is null\n";
    }

    if (inSelectionNode->getFalseBlock())
    {
        OutputLinePrefix(inSelectionNode);
        mStream << "false case\n";
        inSelectionNode->getFalseBlock()->traverse(this);
    }

    --mDepth;

    return false;
}


void
ASTPrinter::visitConstantUnion(TIntermConstantUnion *inConstUnionNode)
{
    size_t size = inConstUnionNode->getType().getObjectSize();

    for (size_t i = 0; i < size; i++)
    {
        OutputLinePrefix(inConstUnionNode);
        switch (inConstUnionNode->getUnionArrayPointer()[i].getType())
        {
            case EbtBool:
                if (inConstUnionNode->getUnionArrayPointer()[i].getBConst())
                    mStream << "true";
                else
                    mStream << "false";

                mStream << " (" << "const bool" << ")\n";
                break;

            case EbtFloat:
                mStream << inConstUnionNode->getUnionArrayPointer()[i].getFConst();
                mStream << " (const float)\n";
                break;

            case EbtInt:
                mStream << inConstUnionNode->getUnionArrayPointer()[i].getIConst();
                mStream << " (const int)\n";
                break;

            case EbtUInt:
                mStream << inConstUnionNode->getUnionArrayPointer()[i].getUConst();
                mStream << " (const uint)\n";
                break;

            default:
                mStream << "INTERNAL ERROR: " << inConstUnionNode->getLine().first_line << "Unknown constant";
                break;
        }
    }
}


bool
ASTPrinter::visitLoop(Visit inVisit, TIntermLoop* inLoopNode)
{
    (void)inVisit;

    OutputLinePrefix(inLoopNode);

    mStream << "Loop with condition ";
    if (inLoopNode->getType() == ELoopDoWhile)
    {
        mStream << "not ";
    }
    mStream << "tested first\n";

    ++mDepth;

    OutputLinePrefix(inLoopNode);
    if (inLoopNode->getCondition())
    {
        mStream << "Loop Condition\n";
        inLoopNode->getCondition()->traverse(this);
    }
    else
    {
        mStream << "No loop condition\n";
    }

    OutputLinePrefix(inLoopNode);
    if (inLoopNode->getBody())
    {
        mStream << "Loop Body\n";
        inLoopNode->getBody()->traverse(this);
    }
    else
    {
        mStream << "No loop body\n";
    }

    if (inLoopNode->getExpression())
    {
        OutputLinePrefix(inLoopNode);
        mStream << "Loop Terminal Expression\n";
        inLoopNode->getExpression()->traverse(this);
    }

    --mDepth;

    return false;
}


bool
ASTPrinter::visitBranch(Visit inVisit, TIntermBranch* inBranchNode)
{
    (void)inVisit;

    OutputLinePrefix(inBranchNode);
    TOperator flow_op(inBranchNode->getFlowOp());
    switch (flow_op)
    {
      case EOpKill:
      case EOpBreak:
      case EOpContinue:
      case EOpReturn:
        mStream << GetOperatorName(flow_op);
        break;

      default:
        mStream << "Branch: Unknown Branch";
        break;
    }

    if (inBranchNode->getExpression())
    {
        mStream << " with expression\n";
        ++mDepth;
        inBranchNode->getExpression()->traverse(this);
        --mDepth;
    }
    else
    {
        mStream << "\n";
    }

    return false;
}


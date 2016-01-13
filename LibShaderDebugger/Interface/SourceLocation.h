



// Indicates section of source code
// Interval of source characters (boundaries inclusive)
class SourceLocation
{
public:
                                // Constructor
                                SourceLocation(
                                    int inLineNrFirst,
                                    int inColIxFirst,
                                    int inLineNrLast,
                                    int inColIxLast);

    int                         mLineNrFirst;
    int                         mColIxFirst;
    int                         mLineNrLast;
    int                         mColIxLast;
};


// Inline definitions

// Constructor
inline
SourceLocation::SourceLocation(
    int inLineNrFirst,
    int inColIxFirst,
    int inLineNrLast,
    int inColIxLast)
:
    mLineNrFirst (inLineNrFirst),
    mColIxFirst  (inColIxFirst),
    mLineNrLast  (inLineNrLast),
    mColIxLast   (inColIxLast)
{

}

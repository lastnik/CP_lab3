#include "BigIntTest.h"
namespace details
{

TestCases casesAdd =
{
        {"1","2","3","A"}, //A
        {"1","2","3","1"}, //B
        {"2","4","6","b"}, //C
};

TestCases casesSub =
{
        {"1","2","3","A"}, //A
        {"-2","2","9","1"}, //B
        {"3","0","-6","9"}, //C
};

TestCases casesMul =
{
        {"1","3","3","A"}, //A
        {"-2","-4","9","-62"}, //B
        {"-2","-C","1B","-3d4"}, //C
};

}

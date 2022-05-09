#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

/*
We have used structs in order to shorten the lenth
of the arguments we pass in every function. In this way,
we read our code faster and we make it easier to understand.

sideValues has the index and count values of every substring we check.
Ultimately, they give the final solution to the problem.
*/

struct sideValues
{
    int index;
    int count;
};

/*
The arguments struct is passed as an argument in every call of the recursive function.
m is the number of concequtive characters we need to find.

'C' is the character we have to check for.
'subString' is left or right substring we pass in the recursion every time.
'subStartIndex' is the substring's index in the original String, needed for the final
solution.
*/

struct arguments
{
    int m;
    char C;
    string subString;
    int subStartIndex;
};

sideValues FindConcequtiveCharacters(arguments args);

int main()
{
    string str;
    char C;
    int m;

    cout << "Give string: " << endl;
    cin >> str;
    cout << "Give character for search: " << endl;
    cin >> C;
    while (true)
    {
        cout << "Give m. m should be less or equal than the string's length." << endl;
        cin >> m;
        if (m <= str.length())
            break;
        cout << "m is larger that string's length. Try again.";
    }

    sideValues solutions;
    arguments args = {m, C, str, 0};

    auto start = high_resolution_clock::now();
    solutions = FindConcequtiveCharacters(args);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "String given: " << str << endl
         << "Character to search: " << C << endl
         << "For " << m << " concequtive characters." << endl
         << "Solution." << endl
         << "Starting Index: " << solutions.index
         << endl
         << "Count: " << solutions.count << endl
         << endl
         << "Duration: " << duration.count() << "microseconds" << endl;
    return 0;
}

/*
With this function we count the number of times character C appears
in substring str.
*/

int CountCharactersInString(string str, char C)
{
    int count = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == C)
            count++;
    }
    return count;
}

/*
FindConcequtiveCharacters is the divide and conquer function that we use.

Example string: "abbbcdddbdddabadddbdbdaacd"
If the string's length equals the m variable, it counts how many times the character
C appears in the substring.

If not, the string is divided in two sides: left and right.
If, for example, m is 4 the string is divided in this way:

1)leftSide: "abbb"
  rightSide: "bbcddd...acd": => FindConcequtiveCharacters(...):
    2)leftSide: "bbcd"
      rightSide: "bcddd...acd" => FindConcequtiveCharacters(...):
        3)leftSide: "bcdd"
          rightSide: "cddd...acd" => FindConcequtiveCharacters(...):
              .
              .
*/

sideValues FindConcequtiveCharacters(arguments args)
{
    if (args.subString.length() == args.m)
    {
        return {
            args.subStartIndex,
            CountCharactersInString(args.subString, args.C)};
    }
    sideValues leftSide = {
        args.subStartIndex,
        CountCharactersInString(args.subString.substr(0, args.m), args.C),
    };

    sideValues rightSide = FindConcequtiveCharacters({args.m, args.C,
                                                      args.subString.substr(1, args.subString.length() - 1),
                                                      args.subStartIndex + 1});

    if (leftSide.count >= rightSide.count)
        return leftSide;
    return rightSide;
}
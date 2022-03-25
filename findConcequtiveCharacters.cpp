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

C is the character we have to check for.

The originalString is used when the substring's length is less than m, and we have to 
borrow m - substring's.length characters from the original string.

subString is left or right substring we pass in the recursion every time.

subStartIndex is the substring's index in the original String.

Example: User gives the string "acdabcdacc", m = 3, C= c
First call of FindConcequtiveCharacters:
    m= 3, C=c, originalString="acdabcdacc",subString="acdabcdacc",subStartIndex= 0.
Second call of FindConcequtiveCharacters:
    m=3, C=c, originalString="acdabcdacc", subString="acdab",subStartIndex = 0.
Third call of FindConcequtiveCharacters:
    m=3, C=c, originalString="acdabcdacc", subString="acd",subStartIndex = 0.
Fourth call of FindConcequtiveCharacters:
    m=3,C=c,orinalString="acdabcdacc",subString="ab",subStartIndex = 3.
Fifth call of FindConcequtiveCharacters:
    m=3,C=c,originalString="acdabcdacc", subString="cdacc", subStartIndex = 5.
.
.
.
*/

struct arguments {
    int m;
    char C;
    string originalString;
    string subString;
    int subStartIndex;
};

sideValues FindConcequtiveCharacters(arguments args);

int main() {
    string str;
    char C;
    int m;

    cout << "Give string: " << endl;
    cin >> str;
    cout << "Give character for search: " << endl;
    cin >> C;
    while(true){
        cout << "Give m. m should be less or equal than the string's length." << endl;
        cin >> m;
        if (m <= str.length()) break;
        cout << "m is larger that string's length. Try again.";
    }

    sideValues solutions;
    arguments args = {m,C,str,str,0};

    auto start = high_resolution_clock::now();
    solutions = FindConcequtiveCharacters(args);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "String given: " << str << endl
    << "Character to search: " << C << endl
    << "For " << m << " concequtive characters." << endl
    << "Solution." << endl << "Starting Index: " << solutions.index 
    << endl << "Count: " << solutions.count << endl
    << endl << "Duration: " << duration.count() << "microseconds" << endl;
    return 0;
}

/*
With this function we count the number of times character C appears
in substring str.
*/

int CountCharactersInString(string str,char C) {
    int count = 0;
    for (int i=0; i < str.length(); i++) {
        if(str[i] == C) count++;
    }
    return count;
}

/*
We use FindMiddle function to divide the string in two parts.
If the string has an even length, substrings will have substring.length / 2 length.
    Example: string is "abba". We get two substrings, "ab" and "ba".
If the sring has an odd length, the left substring will have an odd length, while
the right one will have an even.
    Example: string is "abcba". The Left substring is "abc" and the right one is "ba".
*/

int FindMiddle(int length) {
    div_t result = div(length,2);
    if(result.rem == 1) return result.quot;
    return result.quot - 1;
}

/*
We use the function FindCharactersFromCenter to check substrings that contain
the middle character in a substring. We take two cases into consideration. The one 
is when the substring's length and the m variable are even numbers. The other case is
when both variables are odd numbers.
    Example: Substring = "abba" and m=2. We check the substrings "ab" and "ba" with
    the divide and conquer logic and we use FindCharactersFromCenter to also check the
    substring "bb".

    Another Example: If the substring is "abcba" and m equals 3, we will check the 
    left substring "abc" and the right substring is "ba". As a result, for the right 
    substring, we borrow one character from the left and we check "cba". Finally, with
    FindCharactersFromCenter we check the substring "bcb".
*/

sideValues FindCharactersFromCenter(arguments args, int middle) {
    if(args.subString.length() % 2 == 0 && args.m % 2 == 0
    && (args.subString.length() - 2) == args.m) {
        //case is "abba" and we need to check "bb"
        return {
            args.subStartIndex + 1,
            CountCharactersInString(args.subString.substr(args.subStartIndex + 1,args.m),args.C),
        };
    }

    if(args.subString.length() % 2 == 1 && args.m % 2 == 1 && (args.subString.length() - 2) == args.m ) {
        return {
            args.subStartIndex - ( (args.m - 1) / 2 ),
            CountCharactersInString(args.subString.substr(middle - ( (args.m - 1) / 2 ),args.m),args.C)
        };
    }

    return {0,0};
}

/*
The Borrow function is used when we have split the string so much that eventually
the substring's length that is checked is smaller that variable m. For example, if 
the user's string is "abcba" and m equals 3, we split the string in substrings "abc" 
and "ba". The substring "abc" has 3 characters and equals variable m. As a result, we
search character C with the above-mentioned function and we return. However, the 
substring "ba" has only two characters and needs to borrow one character from the left.
Therefore, we check the substring "cba" and we return.

Example: the string is "abcbafffac", m = 4, C=f.
Substring "abc" must only borrow one character from the right.
Substring "fac" must only borrow one character from the left.

For substring "fff", we must check the substring "afff", the substring 
"fffa", compare the two counts and return the correct index and count variables.
leftBorrowCount is for the left borrow and rightBorrowCount is for the right one.
*/

sideValues Borrow(arguments args,int middle,int borrow) {
    if(args.subStartIndex == 0) 
        return {
            args.subStartIndex,
            CountCharactersInString(args.originalString.substr(0,args.m),args.C)
        };
    if(args.subStartIndex + args.m > args.originalString.length()) {
        return {
            args.subStartIndex - borrow,
            CountCharactersInString(args.originalString.substr(args.subStartIndex - borrow,args.m),args.C)
        };
    }
   int leftBorrowCount,rightBorrowCount;
   leftBorrowCount = CountCharactersInString(args.originalString.substr(args.subStartIndex-borrow,args.m),args.C);
   rightBorrowCount = CountCharactersInString(args.originalString.substr(args.subStartIndex,args.m),args.C);
   if (leftBorrowCount >= rightBorrowCount) 
        return {
        args.subStartIndex - borrow,
        leftBorrowCount
    };  
    return {
        args.subStartIndex,
        rightBorrowCount
    };
}

/*
FindConcequtiveCharacters is the divide and conquer function that we use.
stringLength equals the substring's length.
middle equals the middle index of the substring.

substr method takes two arguments: The first argument is the starting index of the new 
substring. The second arguments is for the length of the new substring.

We want the left substring to have middle + 1 characters. 
If the given string is "abba", middle will be 1 so the left substring starts 
from index 0, and has middle + 1 = 1 + 1 = 2 characters.
If the given string is "abcba", middle will equal 2. The left substring will then have
middle + 1 = 2 + 1 = 3 characters.

Right string starts from middle + 1 position and has stringLength - middle + 1 characters.
As a result, if the string is "abba", the right substring will have subStartIndex=2 and
stringLength - middle + 1 = 4 - 1 + 1 = 2 characters length.

Finally, we call FindCharactersFromCenter to check the center substring.

left,right and equal structs are for the left,right and center substring respectively.
We finally compare the sides' count and return the right substring values.

*/

sideValues FindConcequtiveCharacters(arguments args) {
    int stringLength = args.subString.length();
    int middle = FindMiddle(stringLength);

    if(stringLength == args.m) 
        return {
            args.subStartIndex,
            CountCharactersInString(args.subString,args.C)
        };

    if(stringLength < args.m) 
        return Borrow(args,middle,args.m - stringLength);
    
    sideValues left = {0,0};
    arguments newArgs = {
        args.m, args.C, args.originalString,
        args.subString.substr(0,middle + 1),
        args.subStartIndex
    };
    left = FindConcequtiveCharacters(newArgs);
    
    sideValues right = {0,0};
    newArgs.subString = args.subString.substr(middle + 1,stringLength - middle + 1);
    newArgs.subStartIndex = args.subStartIndex + middle + 1;
    right = FindConcequtiveCharacters(newArgs);

    sideValues center = FindCharactersFromCenter(args,middle);

    if(left.count >= right.count && left.count >= center.count) 
        return { left.index, left.count};
    
    if(right.count >= left.count && right.count >= center.count) {
        return {right.index,right.count};
    }
    
    return {center.index, center.count};
}
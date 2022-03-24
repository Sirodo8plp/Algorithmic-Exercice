#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;
struct sideValues
{
    int index;
    int count;
};

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
    solutions = FindConcequtiveCharacters(args);
    cout << "String given: " << str << endl
    << "Character to search: " << C << endl
    << "For " << m << " concequtive characters." << endl
    << "Solution." << endl << "Starting Index: " << solutions.index 
    << endl << "Count: " << solutions.count << endl;
    return 0;
}

// O(n)
int CountCharactersInString(string str,char C) {
    int count = 0;
    for (int i=0; i < str.length(); i++) {
        if(str[i] == C) count++;
    }
    return count;
}

//O(1)
int FindMiddle(int length) {
    div_t result = div(length,2);
    if(result.rem == 1) return result.quot;
    return result.quot - 1;
}

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

sideValues Borrow(arguments args,int middle,int borrow) {
    if(args.subStartIndex == 0) 
        //we need to borrow only from the right side;
        return {
            args.subStartIndex,
            CountCharactersInString(args.originalString.substr(0,args.m),args.C)
        };
    if(args.subStartIndex + args.m > args.originalString.length()) {
        //we need to borrow only from the left side;
        return {
            args.subStartIndex - borrow,
            CountCharactersInString(args.originalString.substr(args.subStartIndex - borrow,args.m),args.C)
        };
    }
    //we have to borrow both from left and right.
    /*
    Example case: The original string is cddabcdacc. m = 3
    cddabcdacc -> a)cddab | b) cdacc
    cddab -> a)cdd | b)ab
    ab needs to borrow 1 character
    We have to check the strings 'dab' and 'abc'
    Then compare the counts and return appropriate values.
    */
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
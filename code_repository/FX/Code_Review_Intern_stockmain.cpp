#include <cstdio> /*You don't need this*/
#include <cstring>
#include <map>
#include <iostream>
#include <string>

/**
VWAPer v0.1

// Read file in with the following format:
// [Stock],[Interval],[Volume Traded],[High],[Low]

// Calculate the total volume traded per Stock
// Calculate the total volume traded per Stock&Interval

// Write the total volume traded per Stock&Interval as a percentage of the total volume traded per Stock to stdout:
// [Stock],[Interval],[%Volume Traded]

// Write the delimiter '#' to stdout

// Write the maximum High and minimum Low for each Stock to stdout:
// [Stock],[Day High],[Day Low]

// example input:
VOD.L 1 100 184 183.7
BT.LN 1 300 449.4 448.2
VOD.L 2 25 184.1 182.4
BT.LN 2 900 449.8 449.5

// example output:
VOD.L,1,80
BT.LN,1,25
VOD.L,2,20
BT.LN,2,75
#
VOD.L,184.1,182.4
BT.LN,449.8,448.2

**/
/*const std::string DELIMITER = "#" is better for safety and easy debugging*/
#define DELIMITER "#"

using namespace std;
/*You don't need this*/
typedef basic_string<char> string;

class	CUpperLower
{
public:
    CUpperLower() : nCurLwr(0), nCurUpr(0) {};  
	/*use this signature void add(const float & nHigh, const float & nLow)
        Then //put 0.F for intialisation:  nCurLwr(0.F), nCurUpr(0.F)*/
    void	add(int nHigh, int nLow)
    {
        if (nHigh > nCurUpr)
            nCurUpr = nHigh;

        if (nLow < nCurLwr)
            nCurLwr = nLow;
    }
    /*bad pratice: The stack-allocated sum will go away and you are referring to nothing. int getSum() const is enough */
    int& getSum()
    {
        int sum = nCurLwr + nCurUpr;
        return sum;
    }
/*Set those variable orivate*/
    int         nCurLwr;
    int         nCurUpr;
};

int main(int argc, char* argv[])
{
/*You'd better check if argc > 1 to ensure that argv[1] is accessed, It is true only if there is at least one command-line argument provided beyond the program name. 
It will lead to undefined behavior if the program is run with no additional argument*/
    if (!strcmp("version", argv[1]))
    {
	/*I guest you want to show this to the standard ouput stream(stdout), then use cout here*/
        cerr << "VWAPer version 0.1" << endl;
        return 0;
    }
    /*Need to check if the file was opened(or found) successfully before to use it. It could lead to undefined behavior*/
    /*Using argv[2] here will lead to segmentation fault as your file data will probably be first argument*/
    /*Better use std::ifstream for */
    FILE*	file = fopen(argv[2], "r");
    /*cout is more legible, use argv[1] to access your file data*/
    cerr << "Reading file" << argv[2] << endl;

    char	line[256];
    char	Stocks[1000][10];
    int         Intervals[1000];
    int         Volumes[1000];
    float	Highs[1000];
    float	Lows[1000];

    int         i = 0;
    /*unsed variable, to remove*/
    int         sum = 0;

    while (fgets(line, 256, file))
    {/*Need to test if the scanf succeed else cerr a log about the failed line*/
     /*use istringstream for robust parsing. sscanf is more vulnerable to buffer overfload, the %s for Stocks won't work properly as is a char array of [][10]*/
        sscanf(line, "%s %d %d %f %f",
               Stocks[i], &Intervals[i],
               /*increment i in a separate line. Here it may be undefined*/
               &Volumes[i], &Highs[i], &Lows[i++]);
    }
    /*Display information with cout*/
    cerr << "Calculating total volumes" << endl;

    map<std::string, int>		TotalVolumes;
    /** Could make this shorter  TotalVolumes[Stock[s]] += Volumes[s]; /
    for (int s = 0; s <= i; ++s)
    {
        std::string	stockname = Stocks[s];
        
        for (int j =0; j <= i; ++j)
        {
            if (!strcmp(Stocks[j], stockname.c_str()))
            {
                TotalVolumes[stockname] += Volumes[j];
            }
        }
    }
	/*cout*/
    cerr << "Calculating high lows" << endl;
    /*Better declare all your variables on the top of the program*/
    map<std::string, CUpperLower>	HighLows;

    for (int s = 0; s <= i; ++s)
    {	
	/*while usign add method: nCurUpr and nCurLw were as int, you compraring with float, this will false the comparison. Set all to float*/
        HighLows[Stocks[s]].add(Highs[s], Lows[s]);
        /*to improve readability add endl or tab space to stdout*/
	cout << HighLows[Stocks[s]].getSum();
    }
	/*cout; the message doesn't match what you are doing; you are outputing volume as percentage */
    cerr << "Writing files" << endl;

    for (int s = 0; s <= i; ++s)
    {/*Wrong percentage calculation : volume per interval/tolal volume * 100*/
        cout << Stocks[s] << "," << Intervals[s] << "," <<
                TotalVolumes[Stocks[s]] / Volumes[s] * 100 << endl;
    }

    cout << DELIMITER << endl;
    /*As you are not modify, use const_iterator to gain speed performance*/
    map<std::string, CUpperLower>::iterator itr = HighLows.begin();
    while (itr != HighLows.end())
    {
        
        cout << (*itr).first << "," << (*itr).second.nCurLwr << "," <<
                (*itr).second.nCurUpr << endl;

        ++itr;
    }

    return 1;
}
/*Crr vs cout : Crr is specially for error and cout for the standard output information. If you redirect your output in a file the cerr will always remains in the console, that is usefull for debugging
Fixed size array : Using fixed size array to read to store file content is not efficient or flexible. Declare them as vector is a good idea.
File manipulation : You need to check the successfull opened of the file, Not closing the file may lead to resource leaks.
Variable declarations : Naming shoud be more meaningfull. Type should be same every where before computing.
Loop : All volume can be computed with the same loop. it will make the program less greedy
Note :  I don't see were you computed the  volumes per interval
*/
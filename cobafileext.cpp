#include<iostream>
#include<fstream>

using namespace std;

int main() {

    // string STRING;
    // ifstream infile;
    // infile.open ("coba.txt");
    // while(!infile.eof()) // To get you all the lines.
    // {
    //     getline(infile,STRING); // Saves the line in STRING.
    //     cout<<STRING; // Prints our STRING.
    // }
    // infile.close();

    ofstream File("output.txt");
    File << "Farah";
    File << "Ghezi";
    File.close();

    return 0;
}

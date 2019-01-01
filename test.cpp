#include<iostream>
#include<fstream>


using namespace std;

int main()
{
    ifstream p ("plain.txt", ifstream::in);
    ofstream q ("cipher", ofstream::out);
    
    char key[] = "https://www.youtube.com/watch?v=a01QQZyl-_I";
    
    char poi;
    char la;
    int i = 0;
    
    while(p.get(poi))
    {
        q << (char)(poi ^ key[i++]);
        
        if(i == sizeof(key) / sizeof(char) - 1)
            i = 0;
        
    }
    
    
    return 0;
}
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include "decode.hpp"

#define MAX_KEY_LEN 1024
#define C_CHAR_MAX 256

using namespace std;

char buf[100000];
float key_len_prob[MAX_KEY_LEN];
float key_len_val[MAX_KEY_LEN];
//int chars_count[C_CHAR_MAX];
int fsize = 0;
int most_poss_key_len = 0;


char poss_key_set[10000][MAX_KEY_LEN];
int magicshit_ori[65];
int found_keys = 0;


int TNUM = 4;

char goodSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
char probable_keys[MAX_KEY_LEN], key_char_used[MAX_KEY_LEN];

int calculate_fitnesses(int from, int to)
{
    
    float prev = 0;
    float pprev = 0;
    float asd = 0;
    float fitness = 0;
    
    for(int i = from; i < to; i++)
    {
        asd = float(count_equals(i));
        fitness = asd / (MAX_KEY_LEN + pow(i, 1.5));

        if( pprev < prev && prev > fitness)
        {
            key_len_prob[i - 1] = prev;
        }
        pprev = prev;
        prev = fitness;
    }

    if(to == MAX_KEY_LEN && pprev < prev)
        key_len_prob[MAX_KEY_LEN - 1] = prev;
    
    //print_fitnesses();

    return 0;
}

void print_fitnesses(void)
{
    float sum = 0, tmp, maxs = 0;
    for(int i = 0; i < MAX_KEY_LEN; i++)
    {
        if(key_len_prob[i] != 0)
            sum += key_len_prob[i];
    }

    cout << "The most probable key lengths:\n";
    for(int i = 0; i < MAX_KEY_LEN; i++)
    {
        tmp = (key_len_prob[i] * 100) / sum;
        if(key_len_prob[i] != 0 && tmp > 2.0 ) // dont do this if you're a good boy
        {
            cout << setw(3)  << i << ":   " << setprecision(3) << tmp << "%\n";
            if(maxs < tmp)
            {
                maxs = tmp;
                most_poss_key_len = i;
            }
            tmp = (key_len_prob[i] * 100) / sum;
        }
    }
}

int count_equals(int key_length)
{

    int equals_count = 0;
    if (key_length >= fsize)
        return 0;
    int i = 0, j = 0;

    for( i = 0; i < key_length; i++)
    {
        int maxc = chars_count_at_offset(key_length, i);
        equals_count += (maxc / 1000) - 1;

    }
    //cout << key_length << ": " << equals_count << '\n';

    return equals_count;

}

int chars_count_at_offset(int key_length, int offset)   // store result in chars_count[]
{
    int chars_count[C_CHAR_MAX];
    
    for(int i = 0;  i < C_CHAR_MAX; i++)
        chars_count[i] = -1;
    
    int c;
    
    for(int i = offset; i < fsize; i+= key_length)
    {
        c = buf[i];
        if(chars_count[c] == -1)
            chars_count[c] = 1;
        else
            chars_count[c]++;
    }
    int retmax = 0, retpos = 0;
    //cout << "before: " << *retmax << '\n';
    for(int j = 0; j < C_CHAR_MAX; j++)
        if(retmax < chars_count[j])
        {
            retmax = chars_count[j];
            retpos = j;
        }
    //cout << "  after: " << *retmax << '\n';
    return retmax * 1000 + retpos;
}

int guess_probable_keys_for_chars()
{
    //#pragma omp parallel for 
    for(int i = 0; i < 63; i++) // 26 + 26 + 10 + 1
    {
        guess_keys(goodSet[i], i);
    }
    
    return 0;
}

//============================== key ======================================//

char* guess_keys(char most_char, int offset)
{
    int key_len = most_poss_key_len, num;
    //char key_possible_bytes[most_poss_key_len];
    int j=0, i=0;


    for(int i = 0; i < key_len; i++)
    {
        int maxc = 0;
        int max = chars_count_at_offset(key_len, i);
        
        //key_possible_bytes[i] = (char) (max % 1000 ^ (int) most_char);
        poss_key_set[offset][i] = (char) (max % 1000 ^ (int) most_char);
    }
    
    

    //return all_keys(key_possible_bytes, none, 0);
    //all_keys(key_possible_bytes, NULL, 0);
    return 0;
}




void string_generator_OuO(char key_possible_bytes[], int key_no, int *magicshit)
{
    for(int i = 0; i < most_poss_key_len; i++)
        poss_key_set[key_no][i] = key_possible_bytes[i];
    //cout << found_keys << '\n';
    
}

void key_generator(char key_possible_bytes[10], int *magicshit, int offset)
{
    
    if(offset < most_poss_key_len-1)
        key_generator(key_possible_bytes, magicshit, offset+1);
    else
    {
        string_generator_OuO(key_possible_bytes, found_keys, magicshit);
        found_keys++;
    }
    
    
    for(int i = 1, j = magicshit[offset]; i <= j; i++)
    {
        magicshit[offset]--;
        key_generator(key_possible_bytes, magicshit, offset+1);
        //cout << magicshit[offset] << ' ' << j << '\n';
    }
    
    magicshit[offset] = magicshit_ori[offset];
}

// HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP
char* all_keys(char key_possible_bytes[], char* key_part, int offset)
{
    for(int i = 0; i < 10000; i++)
        for(int j = 0; j < MAX_KEY_LEN; j++)
            poss_key_set[i][j] = -1;
    
    int magicshit[most_poss_key_len];
    for(int i = 0; i < most_poss_key_len; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(key_possible_bytes[i] != -1)
            {
                magicshit_ori[i] = j;
                magicshit[i] = j;
            }
        }
    }
    
    key_generator(key_possible_bytes, magicshit, 0);
    
    /*
    for(int i = 0; i < most_poss_key_len; i++)
        if(key_possible_bytes[i][1] == -1)
            cout << ' ';
        else
            cout << key_possible_bytes[i][1];
    cout << '\n';
    */
    
    return 0;
}

//====================================== thread func ====================================//

void *test(void *in)
{
    int from, to;
    from = MAX_KEY_LEN / TNUM * (*(int*)in);
    if (*(int*)in != TNUM -1)
        to =  MAX_KEY_LEN / TNUM * (*(int*)in + 1);
    else
        to = MAX_KEY_LEN-1;

    calculate_fitnesses(from+1, to+1);
    
    pthread_exit(NULL);;
}

void *donothing(void *in)
{
    for(int i = *(int*)in; i < 10000; i+= TNUM)
        for(int j = 0; j < MAX_KEY_LEN; j++)
            poss_key_set[i][j] = -1;
    
    for(int i = *(int*)in; i < 63; i+= TNUM) // 26 + 26 + 10 + 1
    {
        //cout << "hi: " << i << '\n';
        
        if(i == 0)
            found_keys = 63;
        guess_keys(goodSet[i], i);
    }
    
    
    
    //return 0;
    
    
    pthread_exit(NULL);;
}

//========================================== main =======================================//
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cout << "usage:\n$ ./a.out [filename]\n";
        return 0;
    }

    ifstream file(argv[1], std::ios::binary | std::ios::ate);
    streamsize sizef = file.tellg();
    file.seekg(0, std::ios::beg);

    //fsize is the size of whole file
    fsize = sizef;
    if(!file.read(buf, sizef))
        return -1;

    for(int i = 0;  i < MAX_KEY_LEN; i++)
    {
        key_len_prob[i] = 0;
    }

    //most_poss_key_len = 1;
    
    
    pthread_t T[TNUM];   
    for(int i = 0; i < TNUM; i++)          // create thread 1 ~ 7
    {
        int *tmp = new int;
        *tmp = i;
        pthread_create(&T[i], NULL, test, (void *)tmp);
    }
    
    for(int i = 0; i < TNUM; i++)
    {
        pthread_join(T[i], NULL);
    }
    
    print_fitnesses();
    cout << "len: " << most_poss_key_len << '\n';
      
      //*
    for(int i = 0; i < TNUM; i++)          // create thread 1 ~ 7
    {
        int *tmp = new int;
        *tmp = i;
        pthread_create(&T[i], NULL, donothing, (void *)tmp);
    }
    
    for(int i = 0; i < TNUM; i++)
    {
        pthread_join(T[i], NULL);
    }
    //*/
    //guess_probable_keys_for_chars();
    //found_keys = 63;
    cout << "n: " << found_keys << '\n';
    produce_plaintexts();
    
    
    /*
    calculate_fitnesses(1, 8);
    calculate_fitnesses(9, 16);
    calculate_fitnesses(17, 24);
    calculate_fitnesses(25, 32);
    calculate_fitnesses(33, 40);
    calculate_fitnesses(41, 48);
    calculate_fitnesses(49, 56);
    calculate_fitnesses(57, 65);
    
    for(int i = 1; i < MAX_KEY_LEN-1; i++)
    {
        if(key_len_val[i-1] < key_len_val[i] && key_len_val[i] > key_len_val[i+1])
            key_len_prob[i] = key_len_val[i];
    }
    */
    
    //calculate_fitnesses(1, MAX_KEY_LEN);
    
    
    
    
    return 0;
}
//========================================== main =======================================//

// fuck
// i'll just print out the most possible key 
// by calc the percentage of printable chars 

void produce_plaintexts()   
{
    /*
    cout << "all:\n";
    for(int i = 0; i < found_keys; i++)
    {
        for(int j = 0; j < most_poss_key_len; j++)
            cout << poss_key_set[i][j];
        cout << '\n';
    }*/
    cout << found_keys << '\n';
    cout << "key that can produce 98.0\%+ printable characters: \n";
    for(int i = 0; i < found_keys; i++)
    {
        
        int k = 0;
        int cnt = 0;
        for(int j = 0; j < fsize; j++)
        {
            int poi = (int)poss_key_set[i][k++]  ^ (int)buf[j];
            if((poi >= ' ' && poi <= '~') || poi == '\n')
                cnt++;
                
            if(k == most_poss_key_len)
                k = 0;
            
        }
        
        if(cnt > (fsize * 98 / 100))
        {
            
            for(int j = 0; j < most_poss_key_len; j++)
                cout << poss_key_set[i][j];
            cout << '\n';
        }
    }
    
    return;
}



int guess_key_length()
{
    // if KEY_LENGTH is unknown
    calculate_fitnesses(1, MAX_KEY_LEN);
    // else
    //   return KEY_LENGTH
    //guess_and_print_divisors();
    return 0;
}

void guess_and_print_divisor()
{
   // cannot understand python code
   // this function is quite useless
}

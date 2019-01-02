
using namespace std;

// key length guessing

int guess_key_length();

void guess_and_print_divisors();

void print_fitnesses(void);

int calculate_fitnesses(int from, int to);

int count_equals(int);

int chars_count_at_offset(int key_length, int offset);


// key guessing

int guess_probable_keys_for_chars();

char *guess_keys(char,  int);

char *all_keys(char key_possible_bytes[], char* key_part, int offset);

// QAQ

void produce_plaintexts();



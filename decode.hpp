
using namespace std;

// key length guessing

int guess_key_length();

void guess_and_print_divisors();

void print_fitnesses(void);

int calculate_fitnesses(int from, int to);

int count_equals(int);

int *chars_count_at_offset(int, int);


// key guessing

int guess_probable_keys_for_chars();

char *guess_keys(char);

char *all_keys(char key_possible_bytes[][10], char* key_part, int offset);

int print_keys();

// QAQ

void produce_plaintexts();



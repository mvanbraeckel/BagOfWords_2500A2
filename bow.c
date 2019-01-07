/*
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * 06/02/2017
 * bow.c
    Links to "bow.h" header file
    Contains all the functions required for the bag-of-words
 */

#include "bow.h"
#include "mymalloc.h"

/* This function will return a pointer to a newly allocated bag_struct with a bag_size of zero,
   and total_words of zero (you will need to malloc the struct bag_struct). */
struct bag_struct *new_bag() {

  struct bag_struct *new = malloc(sizeof(struct bag_struct)); /* allocate memory and set it */

  /* set default values */
  new->bag = NULL;
  new->bag_size = 0;
  new->total_words = 0;

  return new;
}

/* This function will perform a binary search of the "bag" array contained in the struct bag_struct pointed to by the pointer bow.
   The binary search should look in the middle of the array for the "word".
   If the "word" is alphabetically before the middle word in the "bow", then the search should be repeated on the first "half" of the bow.
   This can be done by creating a new struct bag_struct with the same "bag" and "bag_size" equal to ~1/2 of the original bag_size and calling "bagsearch" recursively.
   If the "word" is alphabetically after the middle word in the "bow", then the search should be repeated on the second half of the bow.
   This can be done by creating a new struct bag_struct with "bag" pointing one position past the middle element, and "bag_size" equal to ~1/2 of the original bag_size.
   (HINT: do a few examples to figure out exactly how to calculate 1/2 (round up or round down) so that you search exactly the right part of the array, and no further.)
   If at any point the middle element matches "word", then the function should return the index of "word" in the bag_struct.
   If "word" is not located in the bag_struct, then the return index should refer to the first word that is alphabetically after "word"
    Note: created my own binary_bagsearch function that uses different parameters to make it easier and avoid unneccesary memory allocation to make it more efficient. */
int bagsearch( struct bag_struct *bow, char *word ){
  return binary_bagsearch( bow->bag, 0, bow->bag_size-1, word);
}

/* This function should loop over the characters in the string that is pointed to by string_ptr.
   It should start by reading and skipping any letters that are not uppercase or lowercase A-Z letters.
   Next it should read characters, convert them to lower case, and remember the lower case characters
   until it encounters something other than an uppercase or lowercase letter.
   It should store all characters encountered prior to the non-letter in a newly allocated
   string variable just large enough to hold the letters and a terminating '\0'.
   Increment the value pointed to by string_ptr so that the first character that it points to
   is the first non-alphabetic character after the last alphabetic character that you stored.
   Return the pointer to the newly allocated string variable, unless there are no alphabetic characters left in the string.
   In that case return NULL. */
char *get_word( char **string_ptr ) {
  char *word;
  int i;

  word = malloc( 919 );   /* 918 is max string length (+1 for '\0') */
  i = 0;

  /* loop until an alphabetical char or '\0' is reached */
  while(!isalpha(**string_ptr) && **string_ptr != '\0') {
    *string_ptr += 1;
  }

  /* read in a word */
  while(isalpha(**string_ptr)) {
    word[i] = tolower(**string_ptr);
    i++;
    *string_ptr += 1;
  }
  word[i] = '\0';

  /* checks why the loop was exited */
  if(**string_ptr == '\0' && isalpha(*(*string_ptr-1))) {
    word = realloc(word, strlen(word)+1); /* gives back extra memory */
    return word;

  } else if(**string_ptr == '\0') {
    free(word);
    return NULL;

  } else {
    word = realloc(word, strlen(word)+1); /* gives back extra memory */
    return word;
  }
}

/* This function should allocate a new struct word_count_struct and return a pointer to it.
   Additionally, it should set the "word" pointer to point to the argument. It should set the "count" integer to 1.
   It should not allocate memory for, or copy the word (we will assume that this was already done by get_word). */
struct word_count_struct *new_word_count( char *word ) {

  struct word_count_struct *new = malloc(sizeof(struct word_count_struct)); /* allocate memory and set it */

  new->word = word; /* points to same address */
  new->count = 1;

  return new;
}

/* This function should search the "bow" for the provided "word", using bagsearch.
   If "word" is found in the "bow", it should increment "count" within the appropriate element of "bag".
   If "word" is not found in the "bow", then "bag" of "bow" should be reallocated to hold one additional
   "struct word_count_struct", a new "struct word_count_struct" should be created (with "new_word_count").
   The words that belong after the argument "word" should be moved 1 position later,
   and the new "struct word_count_struct" inserted in its alphabetical location. */
void add_word( struct bag_struct *bow, char *word ) {

  struct word_count_struct *word_count = new_word_count(word);
  int pos;

  /* for realloc: "sizeof" a word_count_structure multiplied by the bag_size (after adding 1 to it) */
  bow->total_words++;
  bow->bag = realloc(bow->bag, sizeof(struct word_count_struct) * (bow->bag_size + 1));

  /* make sure that something exists in the bag */
  if(bow->bag_size > 0) {
    pos = bagsearch(bow, word);
  } else {
    pos = 0;
  }

  /* check if the word already exists in the bag, that the bag actuall has something in it,
     and the the pos is never > bag_size (never index too high out of bounds) */
  if(bow->bag_size > 0 && pos < bow->bag_size && bow->bag[pos].word != NULL && strcmp(bow->bag[pos].word, word) == 0 ) {
    bow->bag[pos].count++;  /* accumulate count for that word */

  } else {
    bow->bag_size++;
    /* copy-paste move all elements => index over one right */
    memcpy(bow->bag + pos + 1, bow->bag + pos, sizeof(struct word_count_struct) * (bow->bag_size - 1 - pos));
    /* actually copy in the word at the index*/
    memcpy(bow->bag + pos, word_count, sizeof(struct word_count_struct));
  }

  free(word_count);
}

/* This function should identify all the words in the string line (using the get word function), and add them to the bow. */
void add_text( struct bag_struct *bow, char *line ) {
  char *word;  /* pointer to a word */
  int i;
  i = 0;

  /* while sentence doesn't point to the '\0' character at the end of the string */
  while (*line) {
    word = get_word( &line );  /* this will allocate memory for a word */

    /* doesn't print if word is NULL (is empty)*/
    if(word != NULL) {
      add_word(bow, word);
    }

    i++;
  }
}

/* This function should open the file: SMSSpamCollection and read it one line at a time.
   Each line will consist of either the word "ham" or the word "spam" followed by a tab ('\t') character.
   You may assume that the maximum length of text message that you can send is 918 characters.
   And that each line is terminated by a single newline character ('\n').
   The variable label will always contain either "ham" or "spam".
   As you read the file, one line at a time, you should check if it starts with "ham" or "spam".
   If it does not match the provided "label", then you should ignore the line and move to the next line.
   If it matches the label, then you should add the line using the add_text function to the bow. */
void read_sms_data( struct bag_struct *bow, char *label ) {
  FILE *fdata;
  char* line = malloc(sizeof(char)*924);
  char *word;

  fdata = fopen("SMSSpamCollection", "r");
  /* checks if fopen messed up */
  if(fdata == NULL) {
    fprintf(stderr, "ERROR: File could not be opened\n");
  }

  /* loops until end of file is reached (loads in all the data) */
  while(!feof(fdata)) {
    /* label + '\t' + textMSG + '\n' = 4+1+918+1 = 924 */
    fgets(line, 924, fdata);

    word = strtok(line, "\t");  /* collects chars up to first \t ("ham" or "spam") */

    /* if label matches (and not end of file), add the line */
    if(strcmp(word, label)== 0 && !feof(fdata)){
      word = strtok(NULL, "\n");
      add_text(bow, word);
    }
  }

  fclose(fdata);
}

/* This function should go through the entries of ham and the spam.
   It will need to go through them asynchronously --- that is, both files at the same time, but not necessarily at the same speed.
   At each step through the loop it should look at the "current" word from each bag_struct.
    a) If the two words are the same then the program:
      --> should (floating point) divide their counts, by their respective total_words, thus computing two word frequencies
            (how often that word occurs divided by the total number of words that are in the bag).
          If one word comes alphabetically before the other, then the alphabetically first word's count should be divided by its respective total_words to compute the frequency,
            while the second frequency should be calculated as 1 divided by the other bag's total words (this is called a pseudo count).
    b.1) If neither of the two frequencies is above 0.005 (i.e. 0.5%), then we discard the word as too rare to be useful.
    b.2) Next we will calculate the ratio of the two frequencies (divide one by the other) to calculate how much more often one word occurs compared to the other in the "ham" vs "spam" bags.
        If the ratio is less than 1, we will invert it (divide 1 by it).
          Finally, if the ratio is greater than 2.0, use the following formatting string "%20s %8.6f %8.6f %10.6f %s" to print
            --> the word, the ham frequency, the spam frequency, the final ratio, and either the word "ham" or "spam" if the word occurs more frequently in "ham" or "spam" respectively.
  After processing one word, move on to the next work.
  NOTE:  advance your position in both bags if the two words matched; otherwise, only advance your position in the bag that had the alphabetically earlier word. */
void differences( struct bag_struct *ham, struct bag_struct *spam ) {
  int h = 0; /* ham bag index */
  int s = 0; /* spam bag index */

  float ham_frequency = 0;
  float spam_frequency = 0;
  float ratio = 0;
  char *more_frequent = malloc(5); /* spam+\0 = 5 chars */

  /* while it's not the end of both bags */
  while (h < ham->bag_size && s < spam->bag_size) {

    /* compare the current words in each bag */
    if( strcmp(ham->bag[h].word, spam->bag[s].word) == 0 ) {        /* ham and spam are the same word */
      /* how often that word occurs divided by the total number of words that are in the bag */
      ham_frequency =  ham->bag[h].count /  (float)ham->total_words;
      spam_frequency = spam->bag[s].count / (float)spam->total_words;

      /* if neither frequency is >0.005, discard word as too rare */
      if(ham_frequency > 0.005 || spam_frequency > 0.005) {

        ratio = ham_frequency / (float)spam_frequency;

        if(ratio < 1) {
          ratio = 1 / (float)ratio; /* invert it */
        }

        /* check which is more frequent */
        if(ham_frequency > spam_frequency) {
          strcpy(more_frequent, "ham");
        } else {
          strcpy(more_frequent, "spam");
        }

        /* only print the word if ratio > 2.0 && word from each bag is the same */
        if (ratio > 2.0) {
          /* since words are the same, doesn't matter which bag it's from */
          printf("%20s %8.6f %8.6f %10.6f %s\n", ham->bag[h].word, ham_frequency, spam_frequency, ratio, more_frequent);
        }
      }

      h++;
      s++;

    } else if( strcmp(ham->bag[h].word, spam->bag[s].word) > 0 ) {  /* spam word is alphabetically before ham word */
      spam_frequency = spam->bag[s].count / (float)spam->total_words;
      ham_frequency =  1 / (float)ham->total_words;   /* pseudo count */

      /* if neither frequency is >0.005, discard word as too rare */
      if(ham_frequency > 0.005 || spam_frequency > 0.005) {

        ratio = ham_frequency / (float)spam_frequency;

        if(ratio < 1) {
          ratio = 1 / (float)ratio; /* invert it */
        }

        /* check which is more frequent */
        if(ham_frequency > spam_frequency) {
          strcpy(more_frequent, "ham");
        } else {
          strcpy(more_frequent, "spam");
        }

        /* only print the word if ratio > 2.0 && word from each bag is the same */
        if (ratio > 2.0) {
          printf("%20s %8.6f %8.6f %10.6f %s\n", spam->bag[s].word, ham_frequency, spam_frequency, ratio, more_frequent);
        }
      }

      s++;

    } else {                                                      /* ham word is alphabetically before spam word */
      ham_frequency =  ham->bag[h].count /  (float)ham->total_words;
      spam_frequency = 1 / (float)spam->total_words;  /* pseudo count */

      /* if neither frequency is >0.005, discard word as too rare */
      if(ham_frequency > 0.005 || spam_frequency > 0.005) {

        ratio = ham_frequency / (float)spam_frequency;

        if(ratio < 1) {
          ratio = 1 / (float)ratio; /* invert it */
        }

        /* check which is more frequent */
        if(ham_frequency > spam_frequency) {
          strcpy(more_frequent, "ham");
        } else {
          strcpy(more_frequent, "spam");
        }

        /* only print the word if ratio > 2.0 && word from each bag is the same */
        if (ratio > 2.0) {
          printf("%20s %8.6f %8.6f %10.6f %s\n", ham->bag[h].word, ham_frequency, spam_frequency, ratio, more_frequent);
        }
      }

      h++;
    }
  } /* end while loop */
}

/* ========================================================================== */

/* frees the memory associated with the word string, and then frees the memory associated with the word_count_struct */
void free_word_count( struct word_count_struct *word_count ) {
  free(word_count->word);
  free(word_count);
}

/* print both the word and the count of the number of times that the word occurs */
void print_word_count( struct word_count_struct *word_count ) {
  printf("%s: %d\n", word_count->word, word_count->count);
}

/* loops over the bag's word_count structures, printing each in turn using the print_word_count function */
void print_bag(struct bag_struct *bow) {
  int i;

  for(i = 0; i < bow->bag_size; i++) {
    printf("%d ", i);
    print_word_count(bow->bag + i);
  }
  printf("Total words: %d\n", bow->total_words);
}

/* loops over the struct word_count elements in the array in the struct bag_struct, and for each element free the word attribute */
void free_bag(struct bag_struct *bow) {
  int i;

  for(i = 0; i < bow->bag_size; i++) {
    free(bow->bag[i].word);
  }
}

/* recursive function to search for a matching word in a bag. Returns the index of the found word,
   or the index of where it should be placed alphabetically if not found in the bag */
int binary_bagsearch( struct word_count_struct *bag, int left, int right, char* find ) {
  int mid;
  mid = (left + right) / 2;

  if(mid < 0) {
    return 0;

  } else if(left > right) {
   /* check if there is anything left to search
      i.e. search ended, so return the index of word alphabetically after find */
   return left;

  } else if (strcmp(bag[mid].word, find) == 0) { /* mid is a match */
   return mid;

  } else if( strcmp(bag[mid].word, find) > 0 ) { /* find is less than mid */
   return binary_bagsearch(bag, left, mid-1, find);   /* searches that half excluding mid */

  } else {                                  /* find is greater than mid */
   return binary_bagsearch(bag, mid+1, right, find);  /* searches that half excluding mid */
  }
}

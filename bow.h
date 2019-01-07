/*
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * 06/02/2018
 * bow.h
    Header file for bow.c
 */

/* Included Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef BOW_H
#define BOW_H

/* ----- Struct Prototypes ----- */

/* Represents a single word and the number of times that the word occurs in memory.
   Note that the structure does not include memory to store the word (only a pointer), so
   you will need to allocate the memory for the word and make the pointer point to it. */
struct word_count_struct {
  char *word;
  int   count;
};
/* Represents all the word and count pairs as well as the total number of such pairs.
   Note that the structure does not include memory to store the bag (only a pointer), so
   you will need to allocate the memory for the bag by allocating an array of
   word_count_structs and make the pointer point to it. */
struct bag_struct {
  struct word_count_struct *bag;
  int bag_size;
  int total_words;
};

/* Function Prototypes */
struct bag_struct *new_bag();
int bagsearch( struct bag_struct *bow, char *word );
char *get_word( char **string_ptr );
struct word_count_struct *new_word_count( char *word );
void add_word( struct bag_struct *bow, char *word );
void add_text( struct bag_struct *bow, char *line );
void read_sms_data( struct bag_struct *bow, char *label );
void differences( struct bag_struct *ham, struct bag_struct *spam );

void free_word_count( struct word_count_struct *word_count );
void print_word_count( struct word_count_struct *word_count );
void print_bag(struct bag_struct *bow);
void free_bag(struct bag_struct *bow);
int binary_bagsearch( struct word_count_struct *bag, int left, int right, char* find );

#endif

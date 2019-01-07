/*
 * Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * 06/02/2017
 * main.c
    for running the bow.c file functions
 */

#include "bow.h"
#include "mymalloc.h"

int main() {

  struct bag_struct *ham_bag = new_bag();
  struct bag_struct *spam_bag = new_bag();

  read_sms_data(ham_bag, "ham");
  read_sms_data(spam_bag, "spam");

  differences(ham_bag, spam_bag);

  free_bag(ham_bag);
  free_bag(spam_bag);

  free(ham_bag);
  free(spam_bag);

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patricia.h"


/* Chop s at a newline or carriage return */
static void chop(char *s) {
  int i;

  for (i = 0; s[i] != '\r' && s[i] != '\n' && s[i] != '\0'; i++);
  s[i] = '\0';
}

/* callback function used in patricia_enumerate */
static void print_it(unsigned char* s, void *ctx)
{
  /* simply print the string to the screen */
  printf("%s\n", s);
}

/**
 * Open the dictionary file "words.txt" (or the file specified on the
 * command line) and treat each line as an entry.  Read all the
 * entries into a patricia_tree and then read lines from stdin
 * reporting whether or not each line is an entry or a prefix of an
 * entry.
 */
int main(int argc, char *argv[]) 
{
  patricia_tree pt;
  char *filename;
  FILE *fp;
  char line[1000], *copy;
  void *data;

  if (argc == 1) {
    filename = "test/words.txt";
  } else if (argc == 2) {
    filename = argv[1];
  } else {
    fprintf(stderr, "Usage: %s <wordfile>\n", argv[0]);
  }

  /* open wordfile and insert all words */
  if ((fp = fopen(filename, "rt")) == NULL) {
    fprintf(stderr, "Unable to open input file %s\n", filename);
    exit(-1);
  }
  
  printf("Reading dictionary file %s...\n", filename);
  fflush(stdout);
  
  patricia_create(&pt, patricia_print_translator);
  line[sizeof(line)-1] = '\0';
  
  printf("Inserting words into patricia tree...\n");
  while (fgets(line, sizeof(line), fp) != NULL) {
    chop(line);
    if ((copy = malloc(strlen(line)+1)) == NULL) {
      fprintf(stderr, "Out of memory\n");
      exit(-1);
    }
    strcpy(copy, line);
    patricia_insert(&pt, copy, NULL);
  }
  printf("done\n");
  fclose(fp);

  /* search for words */
  if ((fp = fopen(filename, "rt")) == NULL) {
    fprintf(stderr, "Unable to open input file %s\n", filename);
    exit(-1);
  }
  
  printf("Reading dictionary file %s...\n", filename);
  fflush(stdout);
  line[sizeof(line)-1] = '\0';
  
  printf("Searching for words in patricia tree...\n");
  while (fgets(line, sizeof(line), fp) != NULL) {
    chop(line);
    if ((copy = malloc(strlen(line)+1)) == NULL) {
      fprintf(stderr, "Out of memory\n");
      exit(-1);
    }
    strcpy(copy, line);
    patricia_search(&pt, copy, data);
  }
  printf("done\n");
  fclose(fp);

  printf("Enumerating ...\n");
  patricia_enumerate(&pt, print_it);
  printf("done\n");
  
  /* open word file and delete all words */
  if ((fp = fopen(filename, "rt")) == NULL) {
    fprintf(stderr, "Unable to open input file %s\n", filename);
    exit(-1);
  }
  
  printf("Reading dictionary file %s...\n", filename);
  fflush(stdout);
  line[sizeof(line)-1] = '\0';
  
  printf("Deleting words from patricia tree...\n");
  while (fgets(line, sizeof(line), fp) != NULL) {
    chop(line);
    if ((copy = malloc(strlen(line)+1)) == NULL) {
      fprintf(stderr, "Out of memory\n");
      exit(-1);
    }
    strcpy(copy, line);
    patricia_delete(&pt, copy);
  }
  
  printf("done\n");
  fclose(fp);

  printf("Enumerating ...\n");
  patricia_enumerate(&pt, print_it);
  printf("done\n");

  printf("Cleaning up...");
  fflush(stdout);
  patricia_cleanup(&pt);
  printf("done\n");

  return 0;
}

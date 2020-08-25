#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Recursive power function
int power(int base, int exp)
{
  if (base == 0 && exp == 0)
    return INT_MIN;

  if (exp == 0)
    return 1;

  return base * power(base, exp - 1);
}

char* open_file(char *file_name)
{
  FILE *fptr;
  char buffer[1024], *file_text;
  char c;
  int size = 0;

  buffer[0] = '\0';

  // Copy every character in the file to a buffer of size 1024
  if (NULL != (fptr = fopen(file_name, "r")))
  {
    while (fscanf(fptr, "%c", &c) != EOF)
    {
      buffer[size] = c;

      size++;
      buffer[size] = '\0';
    }
  }

  // Copy the buffer into a string that's the exact size neccessary
  file_text = malloc((sizeof(char) * size) + 1);
  strcpy(file_text, buffer);
  file_text[size] = '\0';


  fclose(fptr);
  return(file_text);
}

void echo_file(char *file_text)
{
  int i = 0, j = 0;

  printf("\n");

  while (file_text[i] != '\0')
  {
    if (j == 80)
    {
      printf("\n");
      j = 1;
    }

    printf("%c", file_text[i]);

    if (file_text[i] != '\n')
      j++;

    i++;
  }
  printf("\n");
}

char* pad_plaintext(int checkSumSize, int characterCount, char *file_text)
{
  int i = 1;
  int add_size = checkSumSize / 8;
  int pad;

  if ((characterCount % add_size) != 0)
    pad = add_size - (characterCount % add_size);

  char *plaintext = malloc((sizeof(char) * characterCount + pad) + 1);

  strcpy(plaintext, file_text);

  while (pad != 0)
  {
    plaintext[(characterCount - 1) + i] = 'X';
    i++;
    plaintext[(characterCount - 1) + i] = '\0';
    pad--;
  }

  free(file_text);

  file_text = malloc((sizeof(char) * strlen(plaintext) + 1));
  strcpy(file_text, plaintext);

  free(plaintext);
  return file_text;
}

unsigned long int calc_checksum(int checkSumSize, int characterCount, char *plaintext)
{
  unsigned long int checksum = 0;
  int i = 0, j = 0, k = 0, m;
  int add_size = checkSumSize / 8, buff_cap;
  char *buffer = malloc((sizeof(char) * add_size) + 1);
  char *checksum_str;

  buffer[add_size] = '\0';
  buff_cap = add_size;
  characterCount = characterCount / (checkSumSize / 8);

  while (characterCount != 0)
  {
    while (buff_cap > 0)
    {
      buffer[j] = plaintext[i];
      j++;
      i++;
      buff_cap--;
    }

    j = 0;
    buff_cap = add_size;
    k = add_size - 1;
    m = 0;

    while (k >= 0)
    {
      checksum += buffer[k] * power(16, m);
      k--;
      m += 2;
    }

    characterCount--;
  }

  free(buffer);

  if (checkSumSize == 8)
    return checksum & 0xFF;
  if (checkSumSize == 16)
    return checksum & 0xFFFF;
  if (checkSumSize == 32)
    return checksum & 0xFFFFFFFF;

  else return INT_MIN;
}

void print_checksum(int checkSumSize, unsigned long checksum, int characterCount)
{
  printf("%2d bit checksum is %8lx for all %4d chars\n",
          checkSumSize, checksum, characterCount);
}

int main(int argc, char *argv[])
{
  // Declare and initilalize all variables
  char *file_name, *file_text, *plaintext;
  int checkSumSize = atoi(argv[2]), characterCount, i;
  unsigned long int checksum;

  if (checkSumSize != 8 && checkSumSize != 16 && checkSumSize != 32)
  {
    fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
    exit(0);
  }

  // Copies the file name into a string file_name
  file_name = malloc((sizeof(char) * strlen(argv[1])) + 1);
  strcpy(file_name, argv[1]);

  file_text = open_file(file_name);
  characterCount = strlen(file_text);

  plaintext = pad_plaintext(checkSumSize, characterCount, file_text);
  echo_file(plaintext);

  characterCount = strlen(plaintext);
  checksum = calc_checksum(checkSumSize, characterCount, plaintext);

  print_checksum(checkSumSize, checksum, characterCount);

  free(file_name);
  free(plaintext);
  return 0;
}

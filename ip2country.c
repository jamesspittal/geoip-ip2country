#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    //return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
     printf("Usage: %s IPAddress\n", argv[0]);
    }
    else {
     int s1, s2, s3, s4;
     double c1, c2, c3, c4;
     double integer_ip;
     sscanf(argv[1], "%d.%d.%d.%d", &s1, &s2, &s3, &s4);

     c1 = (double)16777216 * s1;
     c2 = (double)65536 * s2;
     c3 = (double)256 * s3;
     c4 = (double)s4;

     integer_ip = c1 + c2 + c3 + c4;

     //printf("%.0f\n", integer_ip);

     FILE* stream = fopen("GeoIPCountryWhois.csv", "r");
     int found_it = 0;

     char line[1024];
     while (fgets(line, 1024, stream))
     {
         char* tmp = strdup(line);
         char* tmp2 = strdup(line);
         char* tmp3 = strdup(line);

         const char* thirdField = getfield(tmp,3);
         const char* fourthField = getfield(tmp2,4);
         removeChar(thirdField, '"');
         removeChar(fourthField, '"');

         double lower = atof(thirdField);
         double upper = atof(fourthField);

         double m = integer_ip;

         //printf("Field 3: %f - compare to %f", n, m);

         if (m > lower && m < upper) {
          found_it = 1;
          const char* countryName = getfield(tmp3,6);
          removeChar(countryName, '"');
          printf("%s\n", countryName);
          free(tmp);
          exit(0);
         }

         if (m == lower || m == upper) {
          found_it = 1;
          const char* countryName = getfield(tmp3,6);
          removeChar(countryName, '"');
          printf("%s\n", countryName);
          free(tmp);
          exit(0);
         }

         // NOTE strtok clobbers tmp
         free(tmp);
     }

     if (found_it == 0) {
      printf("Unable to find country name from IP integer\n");
     }


    }
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

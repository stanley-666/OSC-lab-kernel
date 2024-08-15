int strcmp(const char* str1, const char* str2) {
  while (*str1 && *str2 && (*str1 == *str2)) {
    str1++;
    str2++;
  }
  return *str1 - *str2;
}

int strlen(const char* str) {
  int length = 0;
  while (*str) {
    length++;
    str++;
  }
  return length;
}

char *strcat(char *dest, const char *src) {
    char *ptr = dest;

    // Find the end of the destination string
    while (*ptr != '\0') {
        ptr++;
    }

    // Copy the source string to the end of the destination
    while (*src != '\0') {
        *ptr++ = *src++;
    }

    // Terminate the destination string
    *ptr = '\0';

    return dest;
}
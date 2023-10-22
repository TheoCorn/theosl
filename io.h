#include <stdio.h>
#include <theosl/vec.h>

#ifndef __THEOSL_IO__
#define __THEOSL_IO__

DEF_VEC(char)
typedef Vec(char) string;
DEF_VEC(string)
DEF_VEC(int)
DEF_VEC(float)

/// @returns 100 * allocation fail OR -2 for invalid character
int iscan(Vec(int) * iv) {
  char ch;
  bool spacer = true;
  string word;
  {
    int e = vec_char_init(&word);
    if (e) {
      return e * 100;
    }
  }

  while ((ch = getc(stdin)) != EOF) {
    switch (ch) {
    case ' ':
    case '\t':
    case ',':
      if (!spacer) {
        spacer = true;
        vec_char_rappend(&word, '\0');
        vec_int_rappend(iv, atoi(word.data));
        vec_char_clear(&word);
      }
      break;
    case '\n':
      goto rest;

    default:
      spacer = false;
      if ((ch >= '0' && ch <= '9') || ch == '-') {
        vec_char_rappend(&word, ch);
      } else {
        return -2;
      }
      break;
    }
  }

rest:

  if (word.len != 0) {
    // str_add(&word, '\0');
    // str_arr_add(&arr, word.start);
    vec_char_rappend(&word, '\0');
    vec_int_rappend(iv, atoi(word.data));
  }

  vec_char_free(&word);

  // for (size_t i = 0; i < arr.len; ++i) {
  //   // data[i] = atoi(arr.start[i]);
  //   // free(arr.start[i]);
  //   vec_int_rappend(iv, atoi(arr.data[i].data));
  //   vec_char_free(arr.data+i);
  // }
  // // free(arr.start);
  // vec_string_free(&arr);

  return 0;
}

int fscan(Vec(float) * fv) {
  char ch;
  bool spacer = true;
  Vec(char) word;
  {
    int e = vec_char_init(&word);
    if (e) {
      return e * 100;
    }
  }

  while ((ch = getc(stdin)) != EOF) {
    switch (ch) {
    case ' ':
    case '\t':
      if (!spacer) {
        spacer = true;
        vec_char_rappend(&word, '\0');
        vec_float_rappend(fv, atof(word.data));
        vec_char_clear(&word);
      }
      break;
    case '\n':
      goto rest;

    default:
      spacer = false;
      if ((ch >= '0' && ch <= '9') || ch == '.') {
        vec_char_rappend(&word, ch);
      } else {
        return -2;
      }
      break;
    }
  }

rest:

  if (word.len != 0) {
    vec_char_rappend(&word, '\0');
    vec_float_rappend(fv, atof(word.data));

  } else {
    vec_char_free(&word);
  }

  return 0;
}

#endif

#define TESTING
#include "../src/main.c"
#undef TESTING

#include <stdio.h>
#include <string.h>

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_EQ(expected, actual) do { \
  tests_run++; \
  if (strcmp(expected, actual) != 0) { \
    tests_failed++; \
    printf("FAIL %s:%d: expected \"%s\", got \"%s\"\n", __FILE__, __LINE__, expected, actual); \
  } else { \
    tests_passed++; \
  } \
} while(0)

#define ASSERT_NULL(ptr) do { \
  tests_run++; \
  if ((ptr) != NULL) { \
    tests_failed++; \
    printf("FAIL %s:%d: expected NULL, got non-NULL\n", __FILE__, __LINE__); \
  } else { \
    tests_passed++; \
  } \
} while(0)

// ======================== unquote tests ========================

static void test_no_quotes(void) {
  char* r = unquote("hello world");
  ASSERT_EQ("hello world", r);
  free(r);
}

static void test_single_quotes(void) {
  char* r = unquote("'hello world'");
  ASSERT_EQ("hello world", r);
  free(r);
}

static void test_single_quotes_literal_backslash(void) {
  char* r = unquote("'hello\\nworld'");
  ASSERT_EQ("hello\\nworld", r);
  free(r);
}

static void test_single_quotes_literal_doublequote(void) {
  char* r = unquote("'hello\"world'");
  ASSERT_EQ("hello\"world", r);
  free(r);
}

static void test_double_quotes_basic(void) {
  char* r = unquote("\"hello world\"");
  ASSERT_EQ("hello world", r);
  free(r);
}

static void test_double_quotes_singlequote_inside(void) {
  char* r = unquote("\"shell's\"");
  ASSERT_EQ("shell's", r);
  free(r);
}

static void test_double_quotes_backslash_escape_backslash(void) {
  char* r = unquote("\"a\\\\b\"");
  ASSERT_EQ("a\\b", r);
  free(r);
}

static void test_double_quotes_backslash_escape_quote(void) {
  char* r = unquote("\"a\\\"b\"");
  ASSERT_EQ("a\"b", r);
  free(r);
}

static void test_double_quotes_multiple_words(void) {
  char* r = unquote("\"hello\"  \"shell's\"  test\"\"script");
  ASSERT_EQ("hello shell's testscript", r);
  free(r);
}

static void test_consecutive_quoted_strings(void) {
  char* r = unquote("\"test  script\"  \"hello\"\"example\"");
  ASSERT_EQ("test  script helloexample", r);
  free(r);
}

static void test_empty_double_quotes(void) {
  char* r = unquote("\"\"");
  ASSERT_EQ("", r);
  free(r);
}

static void test_space_collapsing(void) {
  char* r = unquote("hello    world");
  ASSERT_EQ("hello world", r);
  free(r);
}

static void test_backslash_escape_space(void) {
  char* r = unquote("hello\\ \\ \\ world");
  ASSERT_EQ("hello   world", r);
  free(r);
}

static void test_backslash_escape_single_quote(void) {
  char* r = unquote("hello\\'world");
  ASSERT_EQ("hello'world", r);
  free(r);
}

static void test_backslash_escape_double_quote(void) {
  char* r = unquote("hello\\\"world");
  ASSERT_EQ("hello\"world", r);
  free(r);
}

static void test_backslash_escape_backslash(void) {
  char* r = unquote("hello\\\\world");
  ASSERT_EQ("hello\\world", r);
  free(r);
}

static void test_backslash_newline_becomes_n(void) {
  char* r = unquote("hello\\nworld");
  ASSERT_EQ("hellonworld", r);
  free(r);
}

static void test_quotes_within_unquoted_text(void) {
  char* r = unquote("pre\"mid\"post");
  ASSERT_EQ("premidpost", r);
  free(r);
}

static void test_mixed_quoting(void) {
  char* r = unquote("'single' \"double\" plain");
  ASSERT_EQ("single double plain", r);
  free(r);
}

// ======================== extract_command tests ========================

static void test_extract_unquoted(void) {
  char* r = extract_command("ls -la");
  ASSERT_EQ("ls", r);
  free(r);
}

static void test_extract_single_quoted(void) {
  char* r = extract_command("'exe  with  space' /tmp/dog/f1");
  ASSERT_EQ("'exe  with  space'", r);
  free(r);
}

static void test_extract_double_quoted(void) {
  char* r = extract_command("\"exe with \\\\ backslash\" /tmp/dog/f4");
  ASSERT_EQ("\"exe with \\\\ backslash\"", r);
  free(r);
}

static void test_extract_double_quoted_single_quote_inside(void) {
  char* r = extract_command("\"exe with 'single quotes'\" /tmp/dog/f3");
  ASSERT_EQ("\"exe with 'single quotes'\"", r);
  free(r);
}

static void test_extract_single_quoted_double_quote_inside(void) {
  char* r = extract_command("'exe with \"quotes\"' /tmp/dog/f2");
  ASSERT_EQ("'exe with \"quotes\"'", r);
  free(r);
}

static void test_extract_then_unquote(void) {
  char* q = extract_command("\"exe with \\\\ backslash\" /tmp/dog/f4");
  char* r = unquote(q);
  ASSERT_EQ("exe with \\ backslash", r);
  free(r);
  free(q);
}

// ======================== find_executable tests ========================

static void test_find_executable_not_found(void) {
  char* r = find_executable("nonexistent_cmd_xyzzy");
  ASSERT_NULL(r);
}

// ======================== main ========================

int main(void) {
  test_no_quotes();
  test_single_quotes();
  test_single_quotes_literal_backslash();
  test_single_quotes_literal_doublequote();
  test_double_quotes_basic();
  test_double_quotes_singlequote_inside();
  test_double_quotes_backslash_escape_backslash();
  test_double_quotes_backslash_escape_quote();
  test_double_quotes_multiple_words();
  test_consecutive_quoted_strings();
  test_empty_double_quotes();
  test_space_collapsing();
  test_backslash_escape_space();
  test_backslash_escape_single_quote();
  test_backslash_escape_double_quote();
  test_backslash_escape_backslash();
  test_backslash_newline_becomes_n();
  test_quotes_within_unquoted_text();
  test_mixed_quoting();
  test_extract_unquoted();
  test_extract_single_quoted();
  test_extract_double_quoted();
  test_extract_double_quoted_single_quote_inside();
  test_extract_single_quoted_double_quote_inside();
  test_extract_then_unquote();
  test_find_executable_not_found();

  printf("\n%d passed, %d failed, %d total\n",
         tests_passed, tests_failed, tests_run);
  return tests_failed > 0 ? 1 : 0;
}

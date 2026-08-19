#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // Opening the temp directory for temp files
  char* prefix_path = getenv("PREFIX");
  char temp_path[256];
  snprintf(temp_path, sizeof(temp_path), "%s/tmp/", prefix_path);
  printf("%s\n", temp_path);
  DIR* dr = opendir(prefix_path);
  if (dr == NULL) {
    perror("[Error] ");
    return -1;
  }
  closedir(dr);

  // Handling input and output file creation and its error Handling
  char input_txt_path[256];
  char output_txt_path[256];
  snprintf(input_txt_path, sizeof(input_txt_path), "%sInput.txt", temp_path);
  snprintf(output_txt_path, sizeof(output_txt_path), "%sOutput.txt", temp_path);

  printf("Input Path:%s\n", input_txt_path);
  printf("Output Path:%s\n", output_txt_path);

  FILE* input_fd = fopen(input_txt_path, "w");

  if (input_fd == NULL) {
    perror("[Error: Input file creation]");
    return -1;
  }
  // Writing a string list to the input_file
  char s[][50] = {"Aman", "Shivansh", "Astitva", "Shashank"};
  int arrlen = sizeof(s) / sizeof(s[0]);
  for (int i = 0; i < arrlen; i++) {
    fprintf(input_fd, "%d. %s\n", i + 1, s[i]);
  }
  fclose(input_fd);  // Closing input buffer before fzf prompt

  // Create Fzf prompt
  //

  char temp_buf[256];
  snprintf(temp_buf, sizeof(temp_buf), "cat %s|fzf > %s ", input_txt_path,
           output_txt_path);  // Building fzf prompt
  system(temp_buf);

  // Reading output
  FILE* output_fd = fopen(output_txt_path, "r");

  if (output_fd == NULL) {
    perror("[Error: Output file reading]");
    return -1;
  }
  char buf[256];
  if (fgets(buf, sizeof(buf), output_fd) == NULL) {
    printf("Error in reading file.");
    return 1;
  }
  int idx = atoi(buf) - 1;
  buf[strlen(buf) - 1] = '\0';
  printf("Output: %s at index:%d\n", buf, idx);
  fclose(output_fd);
  return 0;
}

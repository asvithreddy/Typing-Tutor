#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define maxlengthofwords 20
#define HISTORY_SIZE 100
typedef struct {
    int wpm;
    float accuracy;
    char errorPatterns[maxlengthofwords];
} UserProgress;
typedef struct {
    char word[maxlengthofwords];
} TypingExercise;
typedef struct {
    TypingExercise exercises[HISTORY_SIZE];
    int historyIndex;
} ExerciseHistory;
char words[][maxlengthofwords] = {
    "lisbon", "berlin", "tokyo", "helsinki", "nairobi", "professor", "denver", "oslon",
    "stockholm", "michaelscofield", "lincoln", "narcos", "pablo", "escobar", "jagananna"
};
void initUserProgress(UserProgress* progress) {
    progress->wpm = 0;
    progress->accuracy = 0;
    memset(progress->errorPatterns, 0, sizeof(progress->errorPatterns));
}
void initExerciseHistory(ExerciseHistory* history) {
    history->historyIndex = 0;
}
TypingExercise generateTypingExercise() {
    TypingExercise exercise;
    int randomIndex = rand() % (sizeof(words) / sizeof(words[0]));
    strcpy(exercise.word, words[randomIndex]);
    return exercise;
}
void evaluateTyping(UserProgress* progress, TypingExercise* exercise, char* userInput, double timetaken) {
  int correctChars = 0;
  int length = strlen(exercise->word);
  for (int i = 0; i < length; i++) {
    progress->errorPatterns[i] = '_';
  }
  for (int i = 0; i < length; i++) {
    if (exercise->word[i] == userInput[i]) {
      correctChars++;
    } else {
      progress->errorPatterns[i] = userInput[i];
    }
  }
  progress->errorPatterns[length] = '\0';
  progress->accuracy = ((float)correctChars / length) * 100;
  progress->wpm = (int)((correctChars / 5.0) / (timetaken / 60.0));
}
void displayResults(UserProgress* progress) {
    printf("Words per minute: %d\n", progress->wpm);
    printf("Accuracy: %.2f%%\n", progress->accuracy);
    printf("Error patterns: %s\n", progress->errorPatterns);
}
int main() {
    srand(time(NULL));
    UserProgress userProgress;
    ExerciseHistory exerciseHistory;
    initUserProgress(&userProgress);
    initExerciseHistory(&exerciseHistory);
    int numExercises = 5;
    int totalScore = 0;
    int level = 1;
    printf("Welcome to the Typing Game!\n");
    for (int i = 0; i < numExercises; i++) {
        TypingExercise exercise = generateTypingExercise();
        printf("Level %d: Type the following word: \"%s\"\n", level, exercise.word);
        char userInput[maxlengthofwords];
        clock_t startTime = clock();
        scanf("%s", userInput);
        clock_t endTime = clock();
        double timetaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
        evaluateTyping(&userProgress, &exercise, userInput, timetaken);
        displayResults(&userProgress);
        int exerciseScore = (int)userProgress.accuracy;
        totalScore += exerciseScore;
        printf("Score for this round: %d\n", exerciseScore);
        printf("Total Score: %d\n", totalScore);
        if (totalScore >= level * 100) {
            level++;
            printf("Congratulations! You've reached Level %d\n", level);
        }
        exerciseHistory.exercises[exerciseHistory.historyIndex] = exercise;
        exerciseHistory.historyIndex++;
    }
    printf("Game Over. Your final score: %d\n", totalScore);
    return 0;
}
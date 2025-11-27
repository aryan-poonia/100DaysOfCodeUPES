#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    struct tm date = {0};

    // Start date: 28-Aug-2025
    date.tm_year = 2025 - 1900;
    date.tm_mon = 7;   // August (0 = Jan)
    date.tm_mday = 28;

    time_t t = mktime(&date);

    // End date: 27-Nov-2025
    struct tm end = {0};
    end.tm_year = 2025 - 1900;
    end.tm_mon = 10;   // November
    end.tm_mday = 27;

    time_t end_t = mktime(&end);

    while (t <= end_t) {
        struct tm *current = localtime(&t);
        char date_str[20];

        // Format as YYYY-MM-DD
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", current);

        // Append to a file so git has something to commit
        char echo_cmd[200];
        snprintf(echo_cmd, sizeof(echo_cmd),
                 "echo \"Commit for %s\" >> history.txt", date_str);
        system(echo_cmd);

        // Create backdated commit
        char commit_cmd[500];
        snprintf(commit_cmd, sizeof(commit_cmd),
                 "GIT_AUTHOR_DATE=\"%s 12:00:00\" "
                 "GIT_COMMITTER_DATE=\"%s 12:00:00\" "
                 "git add history.txt && "
                 "GIT_AUTHOR_DATE=\"%s 12:00:00\" "
                 "GIT_COMMITTER_DATE=\"%s 12:00:00\" "
                 "git commit -m \"Backdated commit for %s\"",
                 date_str, date_str,
                 date_str, date_str,
                 date_str);

        system(commit_cmd);

        // Move to next day
        t += 24 * 60 * 60;
    }

    printf("Done! Push with: git push origin main\n");
    return 0;
}

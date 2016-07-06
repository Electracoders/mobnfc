/** @FileApi.c
 *  @brief File system related APIs
 *
 *  This file contains file operations
 *
 *  @author Ratnesh Zinzuwadia (ratnesh69)
 *  @author Sandeep Yenugola (sandy)
 *  @bug Testing require for detailed usage of it.
 */

#include "FileApi.h"

/**
 * To find checksum of file.
 *
 */
int CheckSum(char *string) {
	int sum = 0;
	int i = 0;
	while (string[i]) {
		sum += string[i];
		i++;
	}
	return sum;
}

/**
 * To read characters from file returned in char *
 *
 */
int ReadFile(char * path, char** string) {

	int bufferSize = 0, ret = 0;

	FILE *f1;

	f1 = fopen(path, "r");
	if (f1 == NULL) {
		perror("error in opening file\n");
		exit(EXIT_FAILURE);
	}

	fseek(f1, 0, 2);
	bufferSize = (int) ftell(f1);

	*string = malloc((bufferSize * sizeof(char)) + 1);

	rewind(f1);

	ret = fread(*string, 1, bufferSize, f1);

	if (bufferSize == ret) {
		//printf("string: %s \n", *string);
	}
	else {
		printf("ret : %d !=  buffersize : %d \n", ret, bufferSize);
	}
	fclose(f1);

	return ret;
}

/**
 * Find File Extension
 *
 */
int FindExt(char *fileName, char *str) {
	char *ptr;
	int i = 0;
	ptr = strchr(fileName, '.');
	if (ptr == NULL) {
		return -1;
	}
	ptr++;
	i = 0;
	while (*ptr) {
		if (i == MAX_EXT_SIZE - 1) {
			break;
		}
		str[i++] = *(ptr++);
	}
	str[i] = '\0';
	return 0;
}

/**
 * Check if it is fileor not
 *
 */
int IsFile(char *fileName) {
	DIR* directory = opendir(fileName);

	if (directory != NULL) {
		closedir(directory);
		return 0;
	}
	else {
		printf("errno : %x\n", errno);
		printf("%s is file.\n", fileName);
	}
}

/**
 * Move file to other place
 *
 */
int MoveFile(char *destPath, char *sourcePath) {
	int bufferSize = 0, ret = 0, sum = 0;
	FILE *f1;
	FILE *f2;
	char newFile[1024];
	char * string;
	struct tm *dateandtime;
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	dateandtime = localtime(&tv.tv_sec);

	f1 = fopen(sourcePath, "r");
	if (f1 == NULL) {
		perror("error in opening file\n");
		exit(EXIT_FAILURE);
	}

	fseek(f1, 0, 2);
	bufferSize = (int) ftell(f1);

	string = malloc((bufferSize * sizeof(char)) + 1);
	rewind(f1);

	ret = fread(string, 1, bufferSize, f1);
	if (bufferSize == ret) {
		printf("string: %s \n", string);
	}
	else {
		printf("ret : %d !=  buffersize : %d \n", ret, bufferSize);
	}
	fclose(f1);

	strcat(destPath, "/");

	snprintf(newFile, sizeof(newFile), "%d-%02d-%02d_%02d-%02d-%02d.txt",
	        (1900 + dateandtime->tm_year), dateandtime->tm_mon,
	        dateandtime->tm_mday, dateandtime->tm_hour, dateandtime->tm_min,
	        dateandtime->tm_sec);

	strcat(destPath, newFile);

	f2 = fopen(destPath, "w");
	if (f2 == NULL) {
		perror("error in opening file\n");
		exit(EXIT_FAILURE);
	}

	ret = fwrite(string, 1, bufferSize, f2);
	if (ret <= 0) {
		perror("error in write file\n");
		exit(EXIT_FAILURE);
	}
	fclose(f2);

	remove(sourcePath);

	return ret;
}

/**
 * create Directory
 *
 */
NfcBool createDirectory(const char* path, __mode_t mode) {
	mkdir(path, mode);
	return TRUE;
}

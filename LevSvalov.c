/*
Itroduction to proggramming 2.
Assignment 2.
Made by Svalov Lev, BS18-05.
DESCRIPTION.
To make my assignment as much clear and readable as possible, I leave here detailed despription of the structure of the assignment
and general ideas of each particular function(how it works and why it's needed).
*/

// eee boy
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h" boy
#define MAX_NUM_COURSES 50
#define MAX_NUM_PROFS 50
#define MAX_NUM_TA 50
#define MAX_NUM_STUDENTS 50
typedef struct Course Course;
typedef struct Prof Prof;
typedef struct TA TA;
typedef struct Student Student; 

// structs
struct Prof
{
	char* Proffesor_Name;
	int Trained_Courses[MAX_NUM_COURSES];
	int course_capacity;
	int max_work; // max number of courses that prof can be assigned - 2
};

struct TA
{
	char* TA_Name;
	int Trained_Labs[MAX_NUM_COURSES];
	int Lab_capacity;	
	int max_work; // max number of courses that prof can be assigned - 4
};

struct Student
{
	char* Student_Name;
	char* Student_ID;
	int Studying_Courses[MAX_NUM_COURSES];
	int Studying_Courses_capacity;
};

struct Course{
	char* Course_Name;
	int amt_students;
	int amt_labs;
	Prof assighned_prof;
	TA assigned_TAs[MAX_NUM_TA];
};

Course arr_courses[MAX_NUM_COURSES]; 
// an array with courses
int amt_courses;
// amount of courses in the input

Prof arr_profs[MAX_NUM_PROFS]; 
// an array with proffessors
int amt_profs; 
// amount of proffessors in the input

TA arr_ta[MAX_NUM_TA]; 
// an array with TAs
int amt_ta; 
//amount of TAs in the input

Student arr_stdnt[MAX_NUM_STUDENTS]; 
// an array with students
int amt_stdnt; 
// amount of students  in the input

int MIN_BADNESS; // variable that keep a minimal badness value


// implementation of functions below the main()

//auxillary functions
char* namefile(int i);
// function that returns the name of input file("input(i).txt") according to integer that we pass

char* namefileOutput(int i);
// function that returns the name of output file("LevSvalovOutput(i).txt") according to integer that we pass

void InvalidInput(int i);
// function that write - "Invalid input" to the particular outputfile

int compare_strings(char* a, char* b);
// function that compares 2 strings character by character and returns 0 if they are eqyal, -1 otherwise

int AnythingExceptLetters(char *s);
// function that finds something apart english letters in the string and returns 1 if there exists something and 0 if there are only letters

int AnythingExceptLetterAndDigits(char *s);
//  function that finds something apart english letters and digits in the string and returns 1 if there exists something and 0 if there are only letters and digits

int getNumber(char* tmp);
// function that takes a string and convert the value to integer type. idea to convert char by char, if it's not a number it returns -1

char* concat(const char *s1, const char *s2);
// function that concatinates 2 strings and returns new one 

int CourseIndex(char* tmp);
// function that takes the string and find the value of this string along the names of the courses(running in array of courses) and returns the index in the array of courses

char* substr( char *src, int m, int n);
// function that takes  a string and start and end points of interval and returns this interval of the string as an another string.


//major functions
int ReadCourses(FILE *inputfile);
// function that reads the part of the input from the start until the "P", it puts correspondind info to course truct with attention to validation of input.
// it returns -1 if here invalid input, 0 if everything is ok.
int ReadProfessors(FILE *inputfile);
// function that reads from "P" to "T" and check for invalid input and puts data to prof struct
// it returns -1 if here invalid input, 0 if everything is ok.
int ReadTAs(FILE *inputfile);
// function that reads from "T" to "S" and check for invalid input and puts data to TA struct
// it returns -1 if here invalid input, 0 if everything is ok.
int ReadStudents(FILE *inputfile);
// function that reads from "S" until the end of the input and check for invalid input and puts data to Student struct
// it returns -1 if here invalid input, 0 if everything is ok.
int ReadText(FILE *inputfile);
// function that sums all 4 previos parts of input, returns -1(means that invalid input here) is at least one of it returns -1.
void BruteProf(int k, int key);
// function that assigns proffesors from array of proffessors for each possible case(according to the rules)
// for the purpose to check each possible case, after assigning professor it calls BruteTA() that assigns ta and with checker() calculate badness points for this particular case

void BruteTA(int k,int m, int key);
// assigning TAs

void checker(int key);
// functions that calculates badness and write to the outputfile if it's smaller than current MIN badness. and it will overwrite each time if we find smaller one.

int main(int argc, char const *argv[])
{ 
	FILE *emailfile; // file for  writing email
	emailfile = fopen("LevSvalovEmail.txt","w");
	fprintf(emailfile, "%s","l.svalov@innopolis.ru" );
	fclose(emailfile);
	MIN_BADNESS = 10000;
	char *name = (char*)malloc(13*sizeof(char)); // string for name of inputfile
	int last_existed;
	int flag;
	// firstly, we check the last existed file
	for (int i = 1; i < 51; ++i)
	{
		FILE *inputfile;
		name = namefile(i);
		inputfile = fopen(name, "r");
		if (inputfile != NULL)
		{
			last_existed = i;
		}
		fclose(inputfile);
	}
	// now we run from 1 to last existed, and if we have particular file - we read, otherwise - call InvalidInput()
	for (int i = 1; i < last_existed + 1; ++i)
	{
		FILE *inputfile;
		name = namefile(i);
		inputfile = fopen(name, "r");
		if (inputfile == NULL)
		{	
			// printf("%s\n", name);
			fclose(inputfile);	
			InvalidInput(i);
		}else{
			// printf("|| %s ||\n",name);
			flag = ReadText(inputfile);
			fclose(inputfile);
			if (flag == -1) // input file consists mistakes in input 
			{
				InvalidInput(i);
				continue; // we may not check further this file
			}
			MIN_BADNESS = 10000000; // refresh global variable for new file.
			for (int i = 0; i < amt_courses; ++i) // for handling case when we don't have enough TA to run the course 
			 {
			 	for (int j = 0; j < arr_courses[i].amt_labs; ++j)
			 	{
			 		arr_courses[i].assigned_TAs[j].max_work = -1;
			 	}
			 } 
			BruteProf(0,i); // bruteforcing cases
			
		}
		
	}

	return 0;
}							

//Implementation
//auxillary functions
char* namefile(int i){
	char *input = (char*)malloc(11*sizeof(char));
	sprintf(input,"%s%d%s","input",i,".txt"); 
	return input;		
}
char* namefileOutput(int i){
	char *output = (char*)malloc(21*sizeof(char));
	sprintf(output,"%s%d%s","LevSvalovOutput",i,".txt"); 
	return output;		
}

void InvalidInput(int i){
	char *output = (char*)malloc(21*sizeof(char));
	sprintf(output,"%s%d%s","LevSvalovOutput",i,".txt"); 	
	FILE *file2;
	file2 = fopen(output,"w");
	fprintf(file2, "%s","Invalid input.");
	fclose(file2);
}

int compare_strings(char* a, char* b)
{
	int c = 0;

	while (a[c] == b[c]) {
  		if (a[c] == '\0' || b[c] == '\0') break;
  		c++;
	}

	if (a[c] == '\0' && b[c] == '\0'){
  		return 0;
	}else{
	  	return -1;
	}
}

int AnythingExceptLetters(char *s){
	for (int i = 0; i < strlen(s)-1; i++)
	{
		if (!((s[i] >='a' && s[i] <='z') || (s[i] >='A' && s[i] <='Z')))
		{
			return 1;
		}
	}
	return 0;
}

int getNumber(char* tmp){
	int len;
	if (tmp[strlen(tmp)-1] =='\n'){
		len = strlen(tmp) - 1;
	}else{
		len = strlen(tmp);
	}
	int ind = len-1;
	int mult = 1;
	int sum = 0; 
	while(ind!=-1){
		char q =  tmp[ind];
		if (isdigit(q) == 0)
		{		
			return -1;
		}
		int t = (int) q - 48; // here the shift in between ASCII codes and real values of numbers
		sum = sum + mult*t;
		mult = mult * 10;
		ind--;
	}
	return sum;
}
int AnythingExceptLetterAndDigits(char *s){
	for (int i = 0; i < strlen(s)-1; i++)
	{
		if ((!((s[i] >='a' && s[i] <='z')) && !((s[i] >='A' && s[i] <='Z')) && isdigit(s[i]) == 0) )
		{
			return 1;
		}
	}
	return 0;
}
char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); 
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); 
    return result;
}
int CourseIndex(char* tmp){
	for (int i = 0; i < amt_courses; ++i)
	{
		if(compare_strings(tmp,arr_courses[i].Course_Name) != -1){
			return i;
		}
	}
	return -1;
}
char* substr( char *src, int m, int n){
		int len = n - m;
		char *dest = (char*)malloc(sizeof(char) * (len + 1));
		for (int i = m; i < n && (*src != '\0'); i++)
		{
			*dest = *(src + i);
			dest++;
		}
		*dest = '\0';
		return dest - len;
}

//major funcs
int ReadText(FILE *inputfile){
	int flag;
	flag = ReadCourses(inputfile);
	if (flag == -1)
	{
		return -1;
	}
	flag = ReadProfessors(inputfile);
	if (flag == -1)
	{
		return -1;
	}
	flag = ReadTAs(inputfile);
	if (flag == -1)
	{	
		return -1;
	}
	flag = ReadStudents(inputfile);
	return flag;
}
// in all reading functions below, I read string by string from the text file using fgets()
int ReadCourses(FILE *inputfile){
	char string[100];
	int particular_lab;
	int particular_st;
	char *res = (char*)malloc(30*sizeof(char));
	fgets(string,100,inputfile);
	// printf("string - %s\n", string);
	int counter = 0;
	int pre_counter = 0;
	int index = 0;
	while(compare_strings(string, "P\n")!=0){
		Course *temporary_course = (Course*)malloc(sizeof(Course));
		temporary_course->Course_Name =(char*)malloc(30*sizeof(char));
		counter = 0;
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		res = substr(string, 0, counter);
		if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
		{
			// printf("ex1\n");
			return -1;
		}
		if (compare_strings(res,"P") == 0 || compare_strings(res,"T") == 0 || compare_strings(res,"S") == 0)
		{
			return -1;
		}
		strcpy(temporary_course->Course_Name,res); // name of the course
		free(res);
		
		//now labs
		counter ++;
		pre_counter = counter;	

		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		res = substr(string,pre_counter,counter);
		particular_lab = getNumber(res);
		if (particular_lab == -1)
		{
			// printf("ex2\n");
			return -1;
		}
		temporary_course->amt_labs = particular_lab;
		free(res);
		
		// now students
		counter++;
		pre_counter = counter;
		
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}

		res = substr(string, pre_counter, counter);
		particular_st = getNumber(res);

		if (particular_st == -1)
		{
			// printf("ex3\n");
			return -1;
		}	
		temporary_course->amt_students = particular_st;
		arr_courses[index] = *temporary_course;
		index++;
		if (feof(inputfile))
		{
			return -1;
		}
		// printf("%s,labs -  %d, students - %d\n",temporary_course->Course_Name,temporary_course->amt_labs,temporary_course->amt_students);
		fgets(string,100,inputfile);
		free(temporary_course);
		// printf("string - %s\n",string);
	}
	amt_courses = index;

	//now uniqnes of names of courses
	for (int i = 0; i < amt_courses-1; ++i)
	{
		for (int j = i+1; j < amt_courses; ++j)
		{
			if (compare_strings(arr_courses[i].Course_Name,arr_courses[j].Course_Name) == 0) // names are not unique --> Invalid input
			{
				return -1;
			}
		}
	}

	return 0;
}
int ReadProfessors(FILE *inputfile){
	char string[100];
	char *res = (char*)malloc(30*sizeof(char));
	fgets(string,100,inputfile);
	// printf("string - %s", string);
	int pre_counter = 0;	
	int counter = 0;
	int ind_for_arr = 0;
	int trained_course;
	int index = 0;
	while(compare_strings(string, "T\n")!=0){
		Prof *temporary_proffessor =(Prof*) malloc(sizeof(Prof));
		temporary_proffessor->Proffesor_Name = (char*)malloc(50*sizeof(char));
		counter = 0;
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		res = substr(string, 0, counter);
		if (compare_strings(res,"P") == 0 || compare_strings(res,"T") == 0 || compare_strings(res,"S") == 0)
		{
			return -1;
		}
		if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
		{
			// printf("ex4\n");
			return -1;
		}
		res = concat(res," ");
		counter ++;
		pre_counter = counter;	
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		if (compare_strings(substr(string,pre_counter,counter),"P") == 0 || compare_strings(substr(string,pre_counter,counter),"T") == 0 || compare_strings(substr(string,pre_counter,counter),"S") == 0)
		{
			return -1;
		}
		if (AnythingExceptLetters(substr(string,pre_counter,counter)) == 1 || strlen(substr(string,pre_counter,counter)) == 0)//Invalid input
		{
			// printf("ex5\n");
			return -1;
		}
		res = concat(res,substr(string,pre_counter,counter));
		strcpy(temporary_proffessor->Proffesor_Name,res); // Proffessor's name
		free(res);		
		// now subjects
		counter ++;
		pre_counter = counter;	
		ind_for_arr = 0;
		while(counter < strlen(string)-1){
			while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
				counter++;
			}
			res = substr(string,pre_counter,counter);
			if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
			{
				// printf("ex6\n");
				return -1;
			}
			trained_course = CourseIndex(res);
			if (trained_course == -1)
			{
				// printf("ex7\n");
				return -1;
			}
			temporary_proffessor->Trained_Courses[ind_for_arr] = trained_course;

			ind_for_arr++;
			counter++;
			pre_counter = counter;
			free(res);
		}
		temporary_proffessor->course_capacity = ind_for_arr;
		temporary_proffessor->max_work = 0;
		arr_profs[index] = *temporary_proffessor;
		index++;
		if (feof(inputfile))
		{
			// printf("ex8\n");
			return -1;
		}
		// printf("%s",temporary_proffessor->Proffesor_Name);
		// for (int i = 0; i < temporary_proffessor->course_capacity; ++i)
		// {
		// 	printf(" %d", temporary_proffessor->Trained_Courses[i]);	
		// }
		// printf("\n");
		free(temporary_proffessor);
		fgets(string,100,inputfile);
		// printf("string - %s", string);
	}
	amt_profs = index;

	//now uniqness of names of profs
	for (int i = 0; i < amt_profs-1; ++i)
	{
		for (int j = i+1; j < amt_profs; ++j)
		{
			if (compare_strings(arr_profs[i].Proffesor_Name, arr_profs[j].Proffesor_Name) == 0) // names are not uneque --> invalid input
			{
				return -1;
			}
		}
	} 
	// I should add a null proffessor, because can be case when we will not run a course, but technically this course have a proffesor(null proffesor)
	Prof *null_prof = (Prof*)malloc(sizeof(Prof));
	null_prof->Proffesor_Name = NULL;
	null_prof->max_work = 0;
	arr_profs[index] = *null_prof;
	amt_profs++;	
	return 0;
}
int ReadTAs(FILE *inputfile){
	char string[100];
	char *res = (char*)malloc(30*sizeof(char));
	fgets(string,100,inputfile);
	// printf("string - %s", string);
	int pre_counter = 0;	
	int counter = 0;
	int ind_for_arr = 0;
	int trained_labs;
	int index = 0;
	while(compare_strings(string, "S\n")!=0 && compare_strings(string, "S")!=0){
		TA *temporary_ta = (TA*)malloc(sizeof(TA));
		temporary_ta->TA_Name = (char*)malloc(50*sizeof(char));
		counter = 0;
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		res = substr(string, 0, counter);
		if (compare_strings(res,"P") == 0 || compare_strings(res,"T") == 0 || compare_strings(res,"S") == 0)
		{
			return -1;
		}
		if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
		{
			// printf("ex9\n");
			return -1;
		}
		res = concat(res," ");
		counter ++;
		pre_counter = counter;	
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		if (compare_strings(substr(string,pre_counter,counter),"P") == 0 || compare_strings(substr(string,pre_counter,counter),"T") == 0 || compare_strings(substr(string,pre_counter,counter),"S") == 0)
		{
			return -1;
		}
		if (AnythingExceptLetters(substr(string,pre_counter,counter)) == 1 || strlen(substr(string,pre_counter,counter)) == 0)//Invalid input
		{
			// printf("ex10\n");
			return -1;
		}
		res = concat(res,substr(string,pre_counter,counter));
		strcpy(temporary_ta->TA_Name,res); // TA name
		free(res);
		//now subjects
		counter ++;
		pre_counter = counter;	
		ind_for_arr = 0;
		while(counter < strlen(string)-1){
			while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
				counter++;
			}
			res = substr(string,pre_counter,counter);
			if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
			{
				// printf("ex11\n");
				return -1;
			}
			trained_labs = CourseIndex(res);
			if (trained_labs == -1)
			{
				// printf("ex12\n");
				return -1;
			}
			temporary_ta->Trained_Labs[ind_for_arr] = trained_labs;
			ind_for_arr++;
			counter++;
			pre_counter = counter;
			free(res);
		}
		temporary_ta->Lab_capacity = ind_for_arr;
		temporary_ta->max_work = 0;
		arr_ta[index] = *temporary_ta;
		index++;
		if (feof(inputfile))
		{
			// printf("ex13\n");
			return -1;
		}
		// printf("%s",temporary_ta->TA_Name);
		// for (int i = 0; i < temporary_ta->Lab_capacity; ++i)
		// {
		// 	printf(" %d",temporary_ta->Trained_Labs[i]);
		// }
		// printf("\n");
		free(temporary_ta);
		fgets(string,100,inputfile);
		// printf("string - %s", string);
	}
	amt_ta = index;

	// now uniqness of names of ta
	for (int i = 0; i < amt_ta-1; ++i)
	{
		for (int j = i+1; j < amt_ta; ++j)
		{
			if (compare_strings(arr_ta[i].TA_Name, arr_ta[j].TA_Name) == 0) // names are not unique --> invalid input
			{
				return -1;
			}
		}
	}

	
	return 0;
}
int ReadStudents(FILE *inputfile){
	char string[100];
	char *res = (char*)malloc(30*sizeof(char));
	if (feof(inputfile))
	{
		return 0;
	}
	fgets(string,100,inputfile);
	// printf("string - %s", string);
	int pre_counter = 0;	
	int counter = 0;
	int ind_for_arr = 0;
	int studying_course;
	int index = 0;
	while(inputfile!=NULL){
		Student *temporary_student = (Student*)malloc(sizeof(Student));
		temporary_student->Student_Name = (char*)malloc(50*sizeof(char));
		temporary_student->Student_ID = (char*)malloc(5*sizeof(char));
		counter = 0;
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		res = substr(string, 0, counter);
		if (compare_strings(res,"P") == 0 || compare_strings(res,"T") == 0 || compare_strings(res,"S") == 0)
		{
			return -1;
		}
		if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
		{
			// printf("ex14\n");
			return -1;
		}
		res = concat(res," ");
		counter++;
		pre_counter = counter;	
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		if (compare_strings(substr(string,pre_counter,counter),"P") == 0 || compare_strings(substr(string,pre_counter,counter),"T") == 0 || compare_strings(substr(string,pre_counter,counter),"S") == 0)
		{
			return -1;
		}
		if (AnythingExceptLetters(substr(string,pre_counter,counter)) == 1 || strlen(substr(string,pre_counter,counter)) == 0)//Invalid input
		{
			// printf("ex15\n");
			return -1;
		}
		res = concat(res,substr(string,pre_counter,counter));
		strcpy(temporary_student->Student_Name,res); // Student's name
		free(res);
		//now ID
		counter++;
		pre_counter = counter;
		while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
			counter++;
		}
		res = substr(string, pre_counter, counter);
		// ID should be length = 5 and contains only digits and letters
		if (AnythingExceptLetterAndDigits(res) == 1 || strlen(res) != 5)//Invalid input
		{
			// printf("ex16\n");
			return -1;
		}
		strcpy(temporary_student->Student_ID,res);
		free(res);

		//now subjects
		counter++;
		pre_counter = counter;	
		ind_for_arr = 0;
		while(counter < strlen(string)-1){
			while(string[counter]!=' ' && string[counter]!= '\0' && string[counter] != '\n'){
				counter++;
			}
			res = substr(string,pre_counter,counter);
			if (AnythingExceptLetters(res) == 1 || strlen(res) == 0)//Invalid input
			{
				// printf("ex17\n");
				return -1;
			}
			studying_course = CourseIndex(res);
			if (studying_course == -1)
			{
				// printf("ex18\n");
				return -1;
			}
			temporary_student->Studying_Courses[ind_for_arr] = studying_course;
			ind_for_arr++;
			counter++;
			pre_counter = counter;
			free(res);
		}
		temporary_student->Studying_Courses_capacity = ind_for_arr;
		arr_stdnt[index] = *temporary_student;
		index++;
		// printf("%s, id - %s,",temporary_student->Student_Name, temporary_student->Student_ID);
		// for (int i = 0; i < temporary_student->Studying_Courses_capacity; ++i)
		// {
		// 	printf(" %d",temporary_student->Studying_Courses[i]);
		// }
		// printf("\n");
		free(temporary_student);
		if (feof(inputfile))
		{
			break;
		}
		fgets(string,100,inputfile);
		// printf("string - %s", string);


	}
	amt_stdnt = index;
	return 0;
}

// here Brutforce part
void BruteProf(int k,int key){
	
  if (k < amt_courses) 
  {
    for (int i = 0; i < amt_profs; ++i)
    {

      if (arr_profs[i].max_work < 3) // max amount of courses that can take a prof = 2
      {	
      	arr_courses[k].assighned_prof = arr_profs[i];
      	arr_profs[i].max_work++;
      	BruteProf(k+1,key);
    	arr_profs[i].max_work--;  	
      }
    }

  }else{
    BruteTA(0,0,key);
  }
}
void BruteTA(int k,int m,int key){
	if (k < amt_courses )
	{	
		if (m < arr_courses[k].amt_labs )
		{
			if (arr_courses[k].assighned_prof.Proffesor_Name == NULL)
			{
				BruteTA(k+1,m,key);
			}
			for (int i = 0; i < amt_ta; ++i)
			{
				int flag = 0;
				for (int y = 0; y < arr_ta[i].Lab_capacity; ++y)// check if TA is trained for course[k]
				{
					if (arr_ta[i].Trained_Labs[y] == k)
					{
						flag = 1;
						break;
					}
				}
				if(arr_ta[i].max_work < 5 && flag == 1) // max amount of courses that can take a prof = 2
				{
					arr_courses[k].assigned_TAs[m] = arr_ta[i];
					arr_ta[i].max_work++;
					BruteTA(k,m+1,key);
					arr_ta[i].max_work--;	
				}	
			}
		}else{
			BruteTA(k+1,0,key);
		}
	}else{
		checker(key); 
	}
}

void checker(int key){
	int flagok = 0;
	int cur_badness = 0;
	for (int i = 0; i < amt_courses; ++i)
	{	
		
		flagok = 0;
		for (int j = 0; j < arr_courses[i].amt_labs; ++j)
		{
			if(arr_courses[i].assigned_TAs[j].max_work == -1){
			 	arr_courses[i].assighned_prof.Proffesor_Name = NULL;
			 	flagok = 1;
			 	break;
			 } 
		}
		if (flagok == 1)
		{
			for (int j = 0; j < arr_courses[i].amt_labs; ++j)
			{
				arr_courses[i].assigned_TAs[j].TA_Name = NULL; 
			}
		}
		
		if (arr_courses[i].assighned_prof.Proffesor_Name == NULL)
		{	
			cur_badness+=20; // without prof --> course can not be run
		}
	}
	int mark=0;
	for (int i = 0; i < amt_profs; ++i)
	{	
		if (arr_profs[i].Proffesor_Name  == NULL)
		{	
			continue;
		}
		int cnt = 0;
		mark = 0;
		for (int w = 0; w < amt_courses; ++w)
		{	
			if (arr_courses[w].assighned_prof.Proffesor_Name == NULL )//this course cannot be run we can not calculate badness for this
			{
				continue;
			}

			if (compare_strings(arr_courses[w].assighned_prof.Proffesor_Name,arr_profs[i].Proffesor_Name) == 0)
			{
				// check if it is trained for it
				int flag = 0;
				for (int y = 0; y < arr_profs[i].course_capacity; ++y)
				{
					if (arr_profs[i].Trained_Courses[y] == w) // this courses is trained for prof[i]
					{
						flag = 1;
					}
				}
				if (flag != 1)
				{
					
					cur_badness+=5;
					mark++;
					cnt++;
					 // he is assigned, but not to trained course
				}else{
					cnt++;	// he is trained and he is assighed for this course --> everything is okay 
				}
			}
		}
		if (cnt != 2 && mark == 0 )
		{
			if (cnt == 0)
			{
				//unsighned to any class
				cur_badness+=10; 
			}else{
				cur_badness+=5; // lacking course
			}
			
		}
	}


	//now calculate for ta
	for (int i = 0; i < amt_ta; ++i)
	{
		int cnt = 0;
		for (int w = 0; w < amt_courses; ++w)
		{	
			
			int flag = 0;
			for (int z = 0; z < arr_ta[i].Lab_capacity; ++z)
			{
				if (arr_ta[i].Trained_Labs[z] == w) // is this course trained for this TA
				{
					flag = 1; // yeap, he is trained for it
				}
			}
			if (flag == 1)
			{
				for (int e = 0; e < arr_courses[w].amt_labs; ++e)
				{
					if (arr_courses[w].assigned_TAs[e].TA_Name ==NULL || compare_strings(arr_ta[i].TA_Name,arr_courses[w].assigned_TAs[e].TA_Name) == -1) // he missed trained course
					{
						if (cnt<4)
						{
							cur_badness+=2;
						}
						cnt++;	
					}
				}
			}
		}

		//here output TA is lacking (cnt) labs
	}
	
	// now for students
	int student_needed = 0;
	for (int i = 0; i < amt_courses; ++i)
	{

		student_needed = arr_courses[i].amt_students;
		if (arr_courses[i].assighned_prof.Proffesor_Name == NULL)
		{
			student_needed = 0;
		}
		for (int j = 0; j < amt_stdnt; ++j)
		{
			int flag =0;
			for (int w = 0; w < arr_stdnt[j].Studying_Courses_capacity; ++w)
			{
				if (arr_stdnt[j].Studying_Courses[w] == i) //particular student took this course i
					{
						flag = 1;
					}
			}
			if (flag == 1 && student_needed>0 && arr_courses[i].assighned_prof.Proffesor_Name != NULL) 
				{
					student_needed--; // add this student to course
					
				}else{
					//there is no place for student +1 badness
					if(flag == 1){
						cur_badness++;
					}
					
				}
		}
	}
	


	if (cur_badness<MIN_BADNESS) // if after calculation this case better for us --> it should have smaller badness points
	{
		MIN_BADNESS = cur_badness;
		FILE *outputfile;
		char *name = (char*)malloc(21*sizeof(char));
		name = namefileOutput(key);
		outputfile = fopen(name,"w");


		for (int i = 0; i < amt_courses; ++i)
		{
			if (arr_courses[i].assighned_prof.Proffesor_Name == NULL)
			{	
				fprintf(outputfile, "%s cannot be run.\n",arr_courses[i].Course_Name );

			}else{
				fprintf(outputfile, "%s\n",arr_courses[i].Course_Name);
				fprintf(outputfile, "%s\n",arr_courses[i].assighned_prof.Proffesor_Name);
				for (int j = 0; j < arr_courses[i].amt_labs; ++j)
				{
					fprintf(outputfile, "%s\n",arr_courses[i].assigned_TAs[j].TA_Name);
				}
				int student_needed = arr_courses[i].amt_students;
				for (int j = 0; j < amt_stdnt; ++j)
				{	
					int flag = 0;
					for (int w = 0; w < arr_stdnt[j].Studying_Courses_capacity; ++w)
					{
						if (arr_stdnt[j].Studying_Courses[w] == i) //particular student took this course i
						{
							flag = 1;
						}
					}
					if (flag == 1 && student_needed>0) 
					{
						student_needed--; // add this student to course
						fprintf(outputfile, "%s %s\n",arr_stdnt[j].Student_Name,arr_stdnt[j].Student_ID );
					}
				}
				fprintf(outputfile, "\n" );
			}
		}
		

		for (int i = 0; i < amt_profs; ++i)
		{
			if (arr_profs[i].Proffesor_Name == NULL)
			{
				continue;
			}
			int cnt = 0;
			mark = 0;
			for (int w = 0; w < amt_courses; ++w)
			{
				if (arr_courses[w].assighned_prof.Proffesor_Name == NULL)//this course cannot be run we can not calculate badness for this
				{
					continue;
				}
				if ( compare_strings(arr_courses[w].assighned_prof.Proffesor_Name,arr_profs[i].Proffesor_Name) == 0)
				{
					int flag = 0;
					for (int y = 0; y < arr_profs[i].course_capacity; ++y)
					{
						if (arr_profs[i].Trained_Courses[y] == w)
						{
							flag = 1;
						}
					}
					if (flag != 1)
					{
						//prof[i] is untrained for course course[i]
						cnt++;
						mark++;
						fprintf(outputfile, "%s is not trained for %s.\n",arr_profs[i].Proffesor_Name, arr_courses[w].Course_Name);
					}else{
						cnt++; // trained and assigned --> ok
					}
				}

			}
			if (cnt != 2 && mark ==0)
			{
				if (cnt == 0)
				{
					fprintf(outputfile, "%s is unassigned.\n", arr_profs[i].Proffesor_Name);
				}else{
					fprintf(outputfile, "%s is lacking class.\n", arr_profs[i].Proffesor_Name );
				}
			}
		}


		for (int i = 0; i < amt_ta; ++i)
		{
			int cnt = 0;
			for (int w = 0; w < amt_courses; ++w)
			{
				int flag = 0;
				for (int z = 0; z < arr_ta[i].Lab_capacity; ++z)
				{
					if (arr_ta[i].Trained_Labs[z] == w) // is this course trained for this TA
					{
						flag = 1; // yeap, he is trained for it
					}
				}
				if (flag == 1)
				{
					for (int e = 0; e < arr_courses[w].amt_labs; ++e)
					{
						if (arr_courses[w].assigned_TAs[e].TA_Name ==NULL || compare_strings(arr_ta[i].TA_Name,arr_courses[w].assigned_TAs[e].TA_Name) == -1) // he missed trained course
						{
							
							cnt++;	
						}
					}
				}
			}
			if (cnt!=0)
			{
				//here output TA is lacking (cnt) labs
				if (cnt>4) cnt = 4;
				fprintf(outputfile, "%s is lacking %d lab(s).\n", arr_ta[i].TA_Name, cnt);
			}
			
		}

		for (int i = 0; i < amt_courses; ++i)
		{
			int student_needed = arr_courses[i].amt_students;
			for (int j = 0; j < amt_stdnt; ++j)
			{	
				int flag = 0;
				for (int w = 0; w < arr_stdnt[j].Studying_Courses_capacity; ++w)
				{
					if (arr_stdnt[j].Studying_Courses[w] == i) //particular student took this course i
					{
						flag = 1;
					}
				}
				if (flag == 1 && student_needed>0 && arr_courses[i].assighned_prof.Proffesor_Name != NULL) 
				{
					student_needed--; // add this student to course
					
				}else{
					//there is no place for student +1 badness
					if(flag == 1){
						fprintf(outputfile, "%s is lacking %s.\n",arr_stdnt[j].Student_Name,arr_courses[i].Course_Name );
					}
					
				}
			}
		}
		fprintf(outputfile, "Total score is %d.",MIN_BADNESS );

		// printf("||%s||\n",name );
		// printf("badness - %d\n",MIN_BADNESS );
		
		fclose(outputfile);
		free(name);
	}


}
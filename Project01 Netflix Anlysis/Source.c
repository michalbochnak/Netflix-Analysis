/*main.c*/

//
// Netflix movie analysis: top-10 by rating and # of reviews.
//
// << Michal Bochnak, mbochn2 >>
// Windows with Visual Studio
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #01
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Movie {
	int ID;
	char name[255];
	int year;
	int numOfRatings;				// number of reviews
	double totalRatingsPoints;		// sum of all points from user ratings
	double avgRating;
} Movie;


// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
char *getFileName()
{
	char filename[512];
	int  fnsize = sizeof(filename) / sizeof(filename[0]);

	// input filename from the keyboard:
	fgets(filename, fnsize, stdin);
	filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

												 // make sure filename exists and can be opened:
	FILE *infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("**Error: unable to open '%s'\n\n", filename);
		exit(-1);
	}

	fclose(infile);

	// duplicate and return filename:
	char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(s, filename);

	return s;
} // end getFileName


  //-----------------------------------------------------------------------------
  // Read in informations about movie and stores them in array of structs
void readInMoviesData(Movie *movies, int numOfMovies, char *pLine,
	int lineLength, char *pMoviesFile) {

	int actualMovie = 0;

	// Read in the line and parse the line, store information in struct
	for (actualMovie = 0; actualMovie < numOfMovies; actualMovie++) {
		fgets(pLine, lineLength, pMoviesFile);
		pLine[strcspn(pLine, "\r\n")] = '\0';	// avoid errors on different platforms
		movies[actualMovie].ID = atoi(strtok(pLine, ","));
		strcpy(movies[actualMovie].name, strtok(NULL, ","));
		movies[actualMovie].year = atoi(strtok(NULL, ","));
		// initialize the stats to 0
		movies[actualMovie].numOfRatings = 0;
		movies[actualMovie].totalRatingsPoints = 0;
		movies[actualMovie].avgRating = 0;
	}
};


//-----------------------------------------------------------------------------
// Read in additional information from reviews.csv file about movies
// and update the array of structs with movies
void UpdateMoviesData(Movie *movies, int numOfReviews, char *pLine,
	int lineLength, char *pReviewsFile) {
	int i = 0;		// loop counter
	int movieIndex = -1;

	// read in the line and update required informations
	for (i = 0; i < numOfReviews; i++) {
		fgets(pLine, lineLength, pReviewsFile);
		movieIndex = atoi(strtok(pLine, ",")) - 1;
		strtok(NULL, ",");		// skip user ID
		movies[movieIndex].numOfRatings++;		// update number of ratings by 1
		movies[movieIndex].totalRatingsPoints += atoi(strtok(NULL, ","));
	}
}


//-----------------------------------------------------------------------------
void SelectionSortByID(Movie *movies, int numOfMovies) {
	int minIndex = 0;
	Movie temp;			// stores movie struct while swapping them
	int i = 0;			// outer loop counter
	int j = 0;			// inner loop counter

	for (i = 0; i < numOfMovies; i++) {
		minIndex = i;
		
		for (j = i + 1; j < numOfMovies; j++) {
			if (movies[j].ID < movies[minIndex].ID) {
				minIndex = j;
			}
		} // for ( j = ...

		temp = movies[i];
		movies[i] = movies[minIndex];
		movies[minIndex] = temp;
	} // for (i = ...
}

//-----------------------------------------------------------------------------
void SelectionSortByRating(Movie *movies, int numOfMovies) {
	int index = 0;		// for sorting to store min / max element
	Movie temp;			// stores movie struct while swapping them
	int i = 0;			// outer loop counter
	int j = 0;			// inner loop counter

						// sort by Average Rating - primary sort
	for (i = 0; i < numOfMovies; i++) {
		index = i;
		for (j = i + 1; j < numOfMovies; j++) {
			if (movies[j].avgRating > movies[index].avgRating) {
				index = j;
			}
		} // for ( j = ...
		temp = movies[i];
		movies[i] = movies[index];
		movies[index] = temp;
	} // for (i = ...

	  // sort by Movie Name - secondary sort
	for (i = 0; i < numOfMovies; i++) {
		index = i;
		for (j = i + 1; j < numOfMovies; j++) {
			if ((movies[j].avgRating == movies[index].avgRating)
				&& (strcmp(movies[j].name, movies[index].name) < 0)) {
				index = j;
			}
		}
		temp = movies[i];
		movies[i] = movies[index];
		movies[index] = temp;
	}
}


//-----------------------------------------------------------------------------
void SelectionSortByNumOfReviews(Movie *movies, int numOfMovies) {
	int index = 0;
	Movie temp;			// stores movie struct while swapping them
	int i = 0;			// outer loop counter
	int j = 0;			// inner loop counter

	for (i = 0; i < numOfMovies; i++) {
		index = i;
		for (j = i + 1; j < numOfMovies; j++) {
			if (movies[j].numOfRatings > movies[index].numOfRatings) {
				index = j;
			}
		} // for ( j = ...
		temp = movies[i];
		movies[i] = movies[index];
		movies[index] = temp;
	} // for (i = ...

	  // sort by Movie Name - secondary sort
	for (i = 0; i < numOfMovies; i++) {
		index = i;
		for (j = i + 1; j < numOfMovies; j++) {
			if ((movies[j].numOfRatings == movies[index].numOfRatings)
				&& (strcmp(movies[j].name, movies[index].name) < 0)) {
				index = j;
			}
		}
		temp = movies[i];
		movies[i] = movies[index];
		movies[index] = temp;
	}
}


//-----------------------------------------------------------------------------
// calculate the averages for all the movies in the array
void CalculateAvg(Movie *movies, int numOfMovies) {
	int i = 0;		// loop counter

					// itterate thru array of movies and calsulate the average for each one
	for (i = 0; i < numOfMovies; i++) {
		if (movies[i].numOfRatings > 0) {
			movies[i].avgRating = movies[i].totalRatingsPoints / movies[i].numOfRatings;
		}
	}
}


int main()
{
	// get filenames from the user/stdin:
	char *moviesFileName = getFileName();
	char *reviewsFileName = getFileName();
	Movie *movies;										// holds movies data
	FILE *pMoviesFile = fopen(moviesFileName, "r");		// pointer to movies.csv
	FILE *pReviewsFile = fopen(reviewsFileName, "r");	// pointer to reviews.csv
	int numOfMovies = 0;								// number of movies in the file
	int numOfReviews = 0;								// number of reviews in the file
	int lineLength = 280;								// max line leghth
	char line[280];										// stores read in line
	char *pLine = line;									// pointer to line
	int i = 0;											// loop counter

														// read in first line to get info how many movies is in file
	fgets(pLine, lineLength, pMoviesFile);
	// eliminate the problems with new line character at different platforms
	pLine[strcspn(pLine, "\r\n")] = '\0';
	// update numOfMovies with actual number of movies in the file
	numOfMovies = atoi(strtok(pLine, ","));
	// allocate memory for movies
	movies = (Movie*)malloc(sizeof(Movie)*numOfMovies);
	// Read in movies data into struct
	readInMoviesData(movies, numOfMovies, pLine, lineLength, pMoviesFile);
	// sort movies by ID
	SelectionSortByID(movies, numOfMovies);
	// read in first line to get info how many reviews is in file
	fgets(pLine, lineLength, pReviewsFile);
	// eliminate the problems with new line character at different platforms
	pLine[strcspn(pLine, "\r\n")] = '\0';
	numOfReviews = atoi(strtok(pLine, ","));
	// update array with movies by informations from reviews.csv
	UpdateMoviesData(movies, numOfReviews, pLine, lineLength, pReviewsFile);
	// calculate the averages for reviews
	CalculateAvg(movies, numOfMovies);

	// display results
	printf("Movies: %d\n", numOfMovies);
	printf("Reviews %d\n", numOfReviews);

	// sort by rating (primary), and name (secondary)
	SelectionSortByRating(movies, numOfMovies);
	printf("**Top-10 by Rating**\n");

	// display 10 movies from the top of the list
	// sorted by (1) rating and (2) name
	for (i = 0; i < 10; i++) {
		printf("%d: %f,'%s'\n", i + 1, movies[i].avgRating, movies[i].name);
	}

	// sort by num of reviews (primary), and name (secondary)
	SelectionSortByNumOfReviews(movies, numOfMovies);
	printf("**Top-10 by Num Reviews**\n");

	// display 10 movies from the top of the list
	// sorted by (1) number of reviews and (2) name
	for (i = 0; i < 100; i++) {
		printf("%d: %d,'%s'\n", i + 1, movies[i].numOfRatings, movies[i].name);
	}

	return 0;
} // end of main


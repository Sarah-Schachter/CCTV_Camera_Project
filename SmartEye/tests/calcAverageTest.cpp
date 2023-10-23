#include "catch.hpp"
#include "../smart_eye/calcAvarage.h"
#include <opencv2/opencv.hpp>


// Test case for calcAverageColor function
TEST_CASE("calcAverageColor calculates the average color correctly", "[calcAverageColor]")
{
	cv::Mat image(3, 3, CV_8UC3, cv::Scalar(200, 150, 100));
	double avgR, avgG, avgB;

	calcAverageColor(image, avgR, avgG, avgB);

	REQUIRE(avgB == Approx(200));
	REQUIRE(avgG == Approx(150));
	REQUIRE(avgR == Approx(100));
}


// Test case for saveToDB function
TEST_CASE("saveToDB inserts data into the database", "[saveToDB]")
{
	// Create a test detection
	Detection detection;
	detection.box.y = 10;
	detection.box.x = 20;
	detection.box.width = 30;
	detection.box.height = 40;

	// Specify the test average color values
	double avgR = 100.5;
	double avgG = 150.5;
	double avgB = 200.5;

	// Call the saveToDB function
	int result = saveToDB(detection, avgR, avgG, avgB);

	// Check if the result is as expected
	REQUIRE(result == 1);

	// Verify that the data was correctly inserted into the database
	sqlite3* db;
	int rc = sqlite3_open_v2("ImagesDB.db", &db, SQLITE_OPEN_READWRITE, NULL);
	REQUIRE(rc == SQLITE_OK);

	// Prepare the SQL statement to retrieve the inserted data
	const char* sql = "SELECT * FROM images_tbl WHERE Top = ? AND Left = ? AND Width = ? AND Height = ? AND AvgR = ? AND AvgG = ? AND AvgB = ?;";
	sqlite3_stmt* statement;
	rc = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);
	REQUIRE(rc == SQLITE_OK);

	// Bind the parameters to the SQL statement
	sqlite3_bind_int(statement, 1, detection.box.y);
	sqlite3_bind_int(statement, 2, detection.box.x);
	sqlite3_bind_int(statement, 3, detection.box.width);
	sqlite3_bind_int(statement, 4, detection.box.height);
	sqlite3_bind_double(statement, 5, avgR);
	sqlite3_bind_double(statement, 6, avgG);
	sqlite3_bind_double(statement, 7, avgB);

	// Execute the SQL statement
	rc = sqlite3_step(statement);
	REQUIRE(rc == SQLITE_ROW);

	// Verify that the retrieved data is correct
	REQUIRE(strcmp((const char*)sqlite3_column_text(statement, 1), "") != 0); // Verify that the Time column is not empty
	REQUIRE(sqlite3_column_int(statement, 2) == detection.box.y);
	REQUIRE(sqlite3_column_int(statement, 3) == detection.box.x);
	REQUIRE(sqlite3_column_int(statement, 4) == detection.box.width);
	REQUIRE(sqlite3_column_int(statement, 5) == detection.box.height);
	REQUIRE(sqlite3_column_double(statement, 6) == Approx(avgR));
	REQUIRE(sqlite3_column_double(statement, 7) == Approx(avgG));
	REQUIRE(sqlite3_column_double(statement, 8) == Approx(avgB));

	// Finalize the statement
	sqlite3_finalize(statement);



	// Delete the dummy record from the database

	// Prepare the SQL statement to delete the record
	const char* deleteSql = "DELETE FROM images_tbl WHERE Top = ? AND Left = ? AND Width = ? AND Height = ? AND AvgR = ? AND AvgG = ? AND AvgB = ? AND Time = (SELECT MAX(Time) FROM images_tbl WHERE Top = ? AND Left = ? AND Width = ? AND Height = ? AND AvgR = ? AND AvgG = ? AND AvgB = ?);";
	sqlite3_stmt* deleteStatement;
	rc = sqlite3_prepare_v2(db, deleteSql, -1, &deleteStatement, nullptr);
	REQUIRE(rc == SQLITE_OK);

	// Bind the parameters to the SQL statement
	sqlite3_bind_int(deleteStatement, 1, detection.box.y);
	sqlite3_bind_int(deleteStatement, 2, detection.box.x);
	sqlite3_bind_int(deleteStatement, 3, detection.box.width);
	sqlite3_bind_int(deleteStatement, 4, detection.box.height);
	sqlite3_bind_double(deleteStatement, 5, avgR);
	sqlite3_bind_double(deleteStatement, 6, avgG);
	sqlite3_bind_double(deleteStatement, 7, avgB);
	sqlite3_bind_int(deleteStatement, 8, detection.box.y);
	sqlite3_bind_int(deleteStatement, 9, detection.box.x);
	sqlite3_bind_int(deleteStatement, 10, detection.box.width);
	sqlite3_bind_int(deleteStatement, 11, detection.box.height);
	sqlite3_bind_double(deleteStatement, 12, avgR);
	sqlite3_bind_double(deleteStatement, 13, avgG);
	sqlite3_bind_double(deleteStatement, 14, avgB);

	// Execute the SQL statement
	rc = sqlite3_step(deleteStatement);
	REQUIRE(rc == SQLITE_DONE);

	// Finalize the statement and close the database
	sqlite3_finalize(deleteStatement);
	sqlite3_close(db);
}


// Test case for calcSaveDetectoins function
TEST_CASE("calcSaveDetectoins calculates and saves detections to the database", "[calcSaveDetectoins]")
{
	// Create a test image with a detection area
	cv::Mat currentFrame(100, 100, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Rect detectionArea(10, 10, 20, 20);
	cv::rectangle(currentFrame, detectionArea, cv::Scalar(200, 150, 100), cv::FILLED);

	// Create a test detection
	Detection detection;
	detection.box = detectionArea;

	// Call the calcSaveDetectoins function
	calcSaveDetectoins(currentFrame, { detection });

	// Verify that the data was correctly inserted into the database
	sqlite3* db;
	int rc = sqlite3_open_v2("ImagesDB.db", &db, SQLITE_OPEN_READWRITE, NULL);
	REQUIRE(rc == SQLITE_OK);

	// Prepare the SQL statement to retrieve the inserted data
	const char* sql = "SELECT * FROM images_tbl WHERE Top = ? AND Left = ? AND Width = ? AND Height = ?;";
	sqlite3_stmt* statement;
	rc = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);
	REQUIRE(rc == SQLITE_OK);

	// Bind the parameters to the SQL statement
	sqlite3_bind_int(statement, 1, detection.box.y);
	sqlite3_bind_int(statement, 2, detection.box.x);
	sqlite3_bind_int(statement, 3, detection.box.width);
	sqlite3_bind_int(statement, 4, detection.box.height);

	// Execute the SQL statement
	rc = sqlite3_step(statement);
	REQUIRE(rc == SQLITE_ROW);

	// Verify that the retrieved data is correct
	REQUIRE(strcmp((const char*)sqlite3_column_text(statement, 1), "") != 0); // Verify that the Time column is not empty
	REQUIRE(sqlite3_column_int(statement, 2) == detection.box.y);
	REQUIRE(sqlite3_column_int(statement, 3) == detection.box.x);
	REQUIRE(sqlite3_column_int(statement, 4) == detection.box.width);
	REQUIRE(sqlite3_column_int(statement, 5) == detection.box.height);
	REQUIRE(sqlite3_column_double(statement, 6) == Approx(100)); // AvgR
	REQUIRE(sqlite3_column_double(statement, 7) == Approx(150)); // AvgG
	REQUIRE(sqlite3_column_double(statement, 8) == Approx(200)); // AvgB

	// Finalize the statement
	sqlite3_finalize(statement);


	// Delete the dummy record from the database

	// Prepare the SQL statement to delete the record
	const char* deleteSql = "DELETE FROM images_tbl WHERE Time = (SELECT MAX(Time) FROM images_tbl WHERE Top = ? AND Left = ? AND Width = ? AND Height = ?);";
	sqlite3_stmt* deleteStatement;
	rc = sqlite3_prepare_v2(db, deleteSql, -1, &deleteStatement, nullptr);
	REQUIRE(rc == SQLITE_OK);

	// Bind the parameters to the delete SQL statement
	sqlite3_bind_int(deleteStatement, 1, detection.box.y);
	sqlite3_bind_int(deleteStatement, 2, detection.box.x);
	sqlite3_bind_int(deleteStatement, 3, detection.box.width);
	sqlite3_bind_int(deleteStatement, 4, detection.box.height);

	// Execute the delete SQL statement
	rc = sqlite3_step(deleteStatement);
	REQUIRE(rc == SQLITE_DONE);

	// Finalize the delete statement
	sqlite3_finalize(deleteStatement);

	// Close the database
	sqlite3_close(db);
}
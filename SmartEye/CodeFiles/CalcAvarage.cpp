#include "CalcAvarage.h"

using namespace std;
using namespace cv;

#pragma region Milestone1 Task 4 calc and save to csv average color

void calcAverageColor(const cv::Mat& image, double& avgR, double& avgG, double& avgB)
{
	cv::Scalar avgColor = mean(image);
	avgR = avgColor[2]; // Red channel
	avgG = avgColor[1]; // Green channel
	avgB = avgColor[0]; // Blue channel
}

void saveToCSV(const std::string& filename, const Detection& detection, double avgR, double avgG, double avgB)
{
	std::ofstream file;
	file.open(filename, ios_base::app); // Open in append mode

	if (!file.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	// Write headers if the file is empty
	if (file.tellp() == 0) {
		file << "Time,Top,Left,Width,Height,AvgR,AvgG,AvgB" << endl;
	}

	// Get the current time
	time_t currentTime = time(nullptr);
	// Convert the time to a string
	char timeString[26];

#ifdef _WIN32
	ctime_s(timeString, sizeof(timeString), &currentTime);
#else
	time_t rawTime = currentTime;
	ctime_r(&rawTime, timeString);
#endif

	// Write detection details and average color values
	file << timeString << detection.box.y << ", " << detection.box.x << ", " << detection.box.width << ", " << detection.box.height
		<< ", " << avgR << ", " << avgG << ", " << avgB << endl;

	file.close();
}
//////----------------save to DB-------------/////////////////
int saveToDB(const Detection& detection, double avgR, double avgG, double avgB) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    const char* sql;

    rc = sqlite3_open_v2("ImagesDB.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    //Checking if the database was opened successfully.
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS images_tbl (id INTEGER PRIMARY KEY AUTOINCREMENT, Time TEXT, Top INTEGER, Left INTEGER , Width INTEGER, Height INTEGER, AvgR REAL, AvgG REAL, AvgB REAL);";

    rc = sqlite3_exec(db, sql, 0, 0, NULL);

    //Checking if the table was opened successfully.

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot create table: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Table created or already exists.\n");
    }

    time_t currentTime = time(nullptr);

    // Convert the time to a string
    char timeString[26];
    //ctime_s(timeString, sizeof(timeString), &currentTime);
#ifdef _WIN32
    ctime_s(timeString, sizeof(timeString), &currentTime);
#else
    time_t rawTime = currentTime;
    ctime_r(&rawTime, timeString);
#endif
    sql = "INSERT INTO images_tbl(Time, Top, Left, Width, Height, AvgR, AvgG, AvgB) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* statement;
    rc = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(statement, 1, timeString, -1, SQLITE_STATIC);
        sqlite3_bind_int(statement, 2, detection.box.y);
        sqlite3_bind_int(statement, 3, detection.box.x);
        sqlite3_bind_int(statement, 4, detection.box.width);
        sqlite3_bind_int(statement, 5, detection.box.height);
        sqlite3_bind_double(statement, 6, avgR);
        sqlite3_bind_double(statement, 7, avgG);
        sqlite3_bind_double(statement, 8, avgB);

        rc = sqlite3_step(statement);
        if (rc != SQLITE_DONE) {
            // Handle the error
            cout << " Couldnt insert data." << endl;
        }
    }
    sqlite3_finalize(statement);
    sqlite3_close(db);
    return(1);
}

//////--------------------------------------/////////////////



void calcSaveDetectoins(cv::Mat currentFrame, std::vector<Detection> detections)
{
	for (const Detection& detection : detections) {
		double avgRectR, avgRectG, avgRectB;
		cv::Mat detectionROI = currentFrame(detection.box); // get the small image of the detection area only
		calcAverageColor(detectionROI, avgRectR, avgRectG, avgRectB);
		saveToCSV("results.csv", detection, avgRectR, avgRectG, avgRectB);
        saveToDB(detection, avgRectR, avgRectG, avgRectB);

	}
}

#pragma endregion



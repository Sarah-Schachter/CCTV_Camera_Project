#include "pch.h"
#include "../../CameraLibrary/Program.cpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
int main() {
	camera(VIDEO_PATH);
	backend();
	return 1;
}
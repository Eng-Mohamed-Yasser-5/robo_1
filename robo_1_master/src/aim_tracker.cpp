#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// int camID = 0;
Mat myCanvas , img_HSV , mask , img_cropped;
// VideoCapture cam(camID);
Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));

vector<int> colorToDetect_HSV{ 100,116,40,137,255,255 };

int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax = 240, vmax = 255;

Rect getContours(Mat imgDil) {

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    double maxArea = 0;
    Rect bestRect;

    for (size_t i = 0; i < contours.size(); i++) {

        double area = contourArea(contours[i]);

        if (area > 10 && area > maxArea) {

            vector<Point> conPoly;
            double peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly, 0.02 * peri, true);

            Rect rect = boundingRect(conPoly);

            maxArea = area;
            bestRect = rect;
        }
    }

    return bestRect; // لو مفيش object هترجع Rect فاضي
}

Point getCenterPoint(Rect rect) {
	Point centerPoint;
	centerPoint.x = rect.x + rect.width / 2;
	centerPoint.y = rect.y + rect.height / 2;

	return centerPoint;
}

void drawOnObject(Mat img,cv::Rect rect) {

	Scalar aim_color(70,200,70);
	int thickness = 2;
	int crossH = 30;
	int rect_high = 50;
	int rect_width = 50;

	if (!rect.empty()) {

		Point centerPoint = getCenterPoint(rect);

		Point tlPoint = Point(centerPoint.x - rect_width / 2, centerPoint.y - rect_high / 2);
		Point brPoint = Point(centerPoint.x + rect_width / 2, centerPoint.y + rect_high / 2);


		int width = 200;     // Width of crop
		int height = 200;    // Height of crop

		// Ensure the crop is within image boundaries
		int x = std::max(0, centerPoint.x - width / 2);
		int y = std::max(0, centerPoint.y - height / 2);

		// Adjust width and height if the crop goes beyond image boundaries
		if (x + width > img.cols) x = img.cols - width;
		if (y + height > img.rows) y = img.rows - height;

		// Ensure x and y are still non-negative
		x = std::max(0, x);
		y = std::max(0, y);

		Rect objRect(x, y, width, height);
		img_cropped = img(objRect);

		imshow("Target", img_cropped);
		rectangle(img, tlPoint, brPoint,aim_color,thickness);


		line(img, Point(centerPoint.x - crossH / 2, centerPoint.y), Point(centerPoint.x + crossH / 2, centerPoint.y), aim_color, thickness);

		line(img, Point(centerPoint.x, centerPoint.y - crossH / 2), Point(centerPoint.x, centerPoint.y + crossH / 2), aim_color, thickness);

	}
	else {
		try{
			destroyWindow("Target");
		}
		catch (const std::exception& e) {
			std::cout << "Target window not visible." << std::endl;
		}
		
	}

}

Point findCanvasCenterPoint(Mat img) 
{
	Point canvas_center_point;
	canvas_center_point.x = img.size().width / 2;
	canvas_center_point.y = img.size().height / 2;
	return canvas_center_point;
}

Point trackObj(cv::Point objPoint , cv::Mat img) {
	Scalar aim_color(0, 200, 230);
	int thickness = 1;

	Point canvas_center_point = findCanvasCenterPoint(img);

	if (objPoint.x > Point(0, 0).x) {
		//std::cout << objPoint << endl;
		line(img, canvas_center_point, objPoint, aim_color, thickness,16);
		Point defranceInAxis;
		defranceInAxis = canvas_center_point - objPoint;
		defranceInAxis.x = -defranceInAxis.x;
		return defranceInAxis;
	}
	else {
		return Point(0, 0);
	}
}

Point findObj(Mat img) {

	cvtColor(img, img_HSV, COLOR_BGR2HSV);

	hmin = colorToDetect_HSV[0];
	smin = colorToDetect_HSV[1];
	vmin = colorToDetect_HSV[2];
	hmax = colorToDetect_HSV[3];
	smax = colorToDetect_HSV[4];
	vmax = colorToDetect_HSV[5];
	//Color Range
	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);

	inRange(img_HSV, lower, upper, mask);

	Rect myRect = getContours(mask);
	imshow("Mask", mask);

	drawOnObject(img, myRect);

	Point obj_point = getCenterPoint(myRect);
	
	Point point_to_move =  trackObj(obj_point , img);

	return point_to_move;
}


void slider(Mat img, string slider_name,int x, int y, int width, int height, Scalar empty_color, Scalar color, int value ,int max_value) {

	Rect slider_1(x, y, width, height);
	rectangle(img, slider_1.tl(), slider_1.br(), empty_color, FILLED);

	// convert value to 0 : 100 %
	float value_per = (float)value / (float)max_value;

	Rect slider_1_filled(x + (width / 2), y, value_per * (width / 2), height);
	rectangle(img, slider_1_filled.tl(), slider_1_filled.br(), color, FILLED);

	putText(img,slider_name,Point(x +5,y - 10) ,FONT_HERSHEY_COMPLEX, 0.7, Scalar(0,0,255),1);

}
void canvas(cv::Mat img ,Point point) 
{
	Scalar aim_color(70, 200, 70);
	int thickness = 2;
	int crossH = 50;

	Point canvas_center_point = findCanvasCenterPoint(img);


	line(img, Point(canvas_center_point.x - crossH / 2, canvas_center_point.y), Point(canvas_center_point.x + crossH / 2, canvas_center_point.y), aim_color, thickness);

	line(img, Point(canvas_center_point.x, canvas_center_point.y - crossH / 2), Point(canvas_center_point.x, canvas_center_point.y + crossH / 2), aim_color, thickness);

	//Slide 
	// int x_max = img.size().width / 2;

	// int y_max = img.size().height / 2;


	Scalar Slider_1_color_empty(80, 170, 180);
	Scalar Slider_1_filled_color(80, 230, 255);

	slider(img, "X Axis", 20, img.size().height - 60, 250, 30, Slider_1_color_empty, Slider_1_filled_color, point.x, img.size().width / 2);

	slider(img, "Y Axis", img.size().width / 2 + 20, img.size().height - 60, 250, 30, Slider_1_color_empty, Slider_1_filled_color, point.y, img.size().height / 2);

}


// void goToTarget(Point targetPoint) {

// 	// controle to attack target

// 	//cout << "Move => " << targetPoint << endl;

// }

int test()
{
	return 5;
}

void run(Mat img)
{
    //img = cv::imread()
    // cam.read(img);

    //Mat img_dilate = preProcessing(img);


    //Detect object using his color
    Point myPoint = findObj(img);

    // goToTarget(myPoint);
    

    canvas(img, myPoint);


    imshow("image", img);

    waitKey(1);

}


// int main() {

// 	while (false)
// 	{
// 		//img = cv::imread()
// 		cam.read(img);

// 		//Mat img_dilate = preProcessing(img);


// 		//Detect object using his color
// 		Point myPoint = findObj(img);

// 		// goToTarget(myPoint);
		

// 		canvas(img, myPoint);


// 		imshow("image", img);

// 		waitKey(1);
// 	}
// 	return 0;
// }
// 利用模板匹配， 对图像进行定位
void cv_MatchTemplate(Mat src, Mat temp)
{
	Mat result, display;
	src.copyTo(display);

	// 创建输出结果的矩阵
	int result_cols = src.cols - temp.cols + 1;
	int result_rows = src.rows - temp.rows + 1;
	result.create(Size(result.cols, result.rows), CV_8UC1);

	matchTemplate(src, temp, result, CV_TM_SQDIFF_NORMED);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal; double maxVal; Point minLoc; Point maxLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	Point matchLoc = minLoc;

	rectangle(display, matchLoc, Point(matchLoc.x + temp.cols, matchLoc.y + temp.rows), Scalar(0), 1, 8, 0);
	imshow("The result of the matchTemplate", display);
	waitKey(0);
}

int main()
{
	Mat src = imread("E:\\picture\\ocr.jpg", 1);
	Mat templ = imread("E:\\picture\\templ.jpg", 1);
	cv_MatchTemplate(src, templ);
	return 0;
}
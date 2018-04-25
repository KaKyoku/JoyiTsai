void cut_ocrimage(Mat Color){
	Mat Gray;
	cvtColor(Color, Gray, CV_BGR2GRAY);
	threshold(Gray, Gray, 40, 255, THRESH_OTSU);
	Mat hist(Gray.rows, Gray.cols, CV_8UC1, Scalar(0));
	int h[1000] = { 0 };

	for (int i = 0; i < Gray.rows; i++){
		for (int j = 0; j < Gray.cols; j++){
			// 对图像取反
			Gray.at<uchar>(i, j) = 255 - Gray.at<uchar>(i, j);
		}
	}

	for (int i = 0; i < Gray.rows; i++)
	{
		for (int j = 0; j < Gray.cols; j++)
		{
			if (Gray.at<uchar>(i, j) == 255)
			{
				h[i] ++;  // 统计每一行的高亮像素数
			}
		}
	}

	// 得到垂直方向上的投影
	for (int i = 0; i < Gray.rows; i++)
	{
		for (int j = 0; j < h[i]; j++)
		{
			hist.at<uchar>(i, j) = 255;
		}
	}
	imshow("hist", hist);
	imshow("binary image", Gray);

	bool inLine = false, Hinline = false;
	int start = 0, v = 0, x = 0;
	int vertical[100] = { 0 };
	int harizon[100] = { 0 };
	int horizonH[1000] = { 0 };
	Mat horizon_hist;
	for (int i = 0; i < sizeof(vertical); i++)
	{
		if (i % 2 == 1)
		{
			horizon_hist = Mat(vertical[i] - vertical[i - 1], Gray.cols, CV_8UC1, Scalar(0));
		}
	}
	Mat ROIimg;

	for (int i = 0; i < 1000; ++i)
	{
		if (i > Gray.rows)
			break;

		if (!inLine && h[i] > 10)
		{
			vertical[v] = i;
			v++;
			line(Color, Point(0, i), Point(Gray.cols, i), Scalar(200, 0, 0), 1, 8, 0);
			inLine = true;
			start = i;
		}
		else if ((i - start) > 5 && h[i] < 10 && inLine)
		{
			inLine = false;
			vertical[v] = i;
			v++;
			line(Color, Point(0, i), Point(Gray.cols, i), Scalar(200, 0, 0), 1, 8, 0);
		}
	}
	int Hstart = 0, mm = 0;
	char *name = "E:\\picture\\words\\";
	// 在这里v是我们存放到vertical中的分割线数目
	for (int m = 0; m < v; m++)
	{
		if (m % 2 == 1)
		{
			// vertical[i]表示存放分割线x坐标的列表，其中第i条分割线的x坐标
			ROIimg = Gray(Rect(0, vertical[m - 1], Gray.cols, vertical[m] - vertical[m - 1]));
			horizon_hist = Mat(vertical[m] - vertical[m - 1], Gray.cols, CV_8UC1, Scalar(0));


			// 现在对每一张ROIimg进行水平方向上的投影计算
			for (int i = 0; i < ROIimg.cols; i++)
			{
				// 必须在累加每一列高亮像素数之前，对该列进行置零，否则重复累加显然会超出ROIimg的图像高度。
				horizonH[i] = 0;
				for (int j = 0; j < ROIimg.rows; j++)
				{
					if (ROIimg.at<uchar>(j, i) == 255)
					{
						horizonH[i]++;
					}
				}
			} //水平方向上的投影直方图的计算
			for (int i = 0; i < ROIimg.cols; i++)
			{
				for (int j = 0; j < horizonH[i]; j++)
				{
					horizon_hist.at<uchar>(j, i) = 255;
				}
			}
			for (int i = 0; i < 1000; i++)
			{
				if (i>ROIimg.cols)
					break;
				if (!Hinline && horizonH[i]>4)
				{
					harizon[x] = i;
					x++;
					line(Color, Point(i, vertical[m - 1]), Point(i, vertical[m]), Scalar(0, 0, 200), 1, 8, 0);

					Hstart = i;
					Hinline = true;
				}
				else if ((i - Hstart) > 30 && horizonH[i] < 2 && Hinline)
				{

					mm++;
					harizon[x] = i;
					x++;
					Hinline = false;

					line(Color, Point(i, vertical[m - 1]), Point(i, vertical[m]), Scalar(0, 0, 200), 1, 8, 0);
					imshow("word", Gray(Rect(harizon[x - 2], vertical[m - 1], harizon[x - 1] - harizon[x - 2], vertical[m] - vertical[m - 1])));

					waitKey(0);
				}
			}
		}

	}
	imshow("Color", Color);
	waitKey(0);
}

int main()
{
	Mat src = imread("E:\\picture\\ocr.jpg", 1);
	//cv_calcHist(src);
	cut_ocrimage(src);
	return 0;
}

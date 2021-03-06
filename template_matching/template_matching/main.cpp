#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>

struct position {
	int x;
	int y;
};

void Template_matching(const std::string& img_name, const std::string& template_img_name);
int SSD(cv::Mat& img, cv::Mat& template_img, const int* x, const int* y);
void Indicate_Predicted_Position(cv::Mat& img, cv::Mat& template_img, const position& p);

int main() {

	Template_matching("lena2.png", "template_img2.png");

	return 0;
}

void Template_matching(const std::string& img_name, const std::string& template_img_name) {
	cv::Mat img = cv::imread(img_name);
	if (img.empty()) {
		std::cerr << "Can't read img." << std::endl;
		exit(1);
	}

	cv::Mat template_img = cv::imread(template_img_name);
	if (template_img.empty()) {
		std::cerr << "Can't read template_img." << std::endl;
		exit(1);
	}

	int min = 100000000;
	int r = 0;
	struct position p = { 0,0 };

	std::cout << "SSD" << std::endl;
	for (int y = 0; y <= img.rows - template_img.rows; ++y) {
		for (int x = 0; x <= img.cols - template_img.cols; ++x) {
			r = SSD(img, template_img, &x, &y);
			if (min > r) {
				min = r;
				p.x = x;
				p.y = y;
			}
		}
	}

	std::cout << "Indicate" << std::endl;
	cv::Mat copy_img;
	img.convertTo(copy_img, CV_8U);
	Indicate_Predicted_Position(copy_img, template_img, p);

	cv::imshow("img", img);
	cv::imshow("template_img", template_img);
	cv::imshow("matchimg_result", copy_img);
	cv::waitKey();
}

int SSD(cv::Mat& img, cv::Mat& template_img, const int* x, const int* y) {
	int r = 0;
	int temp = 0;
	int i = 0;
	int j = 0;
	int c = 0;

	for (i = 0; i < template_img.rows; ++i) {
		for (j = 0; j < template_img.cols; ++j) {
			for (c = 0; c < img.channels(); ++c) {
				temp = static_cast<int>(img.data[(i + *y)*img.step + (j + *x)*img.elemSize() + c]) - static_cast<int>(template_img.data[i*template_img.step + j*template_img.elemSize() + c]);
				r += temp*temp;
			}
		}
	}

	return r;
}

void Indicate_Predicted_Position(cv::Mat& img, cv::Mat& template_img, const position& p) {
	int y = 0;
	int x = 0;
	for (y = 0; y < img.rows; ++y) {
		for (x = 0; x < img.cols; ++x) {
			if (y == p.y || y == (p.y + template_img.rows)) {
				if (x > p.x && x < p.x + template_img.cols) {
						img.data[y*img.step + x*img.elemSize() + 0] = static_cast<uchar>(0);
						img.data[y*img.step + x*img.elemSize() + 1] = static_cast<uchar>(0);
						img.data[y*img.step + x*img.elemSize() + 2] = static_cast<uchar>(255);
				}
			}

			if (x == p.x  || x == (p.x + template_img.cols)) {
				if (y > p.y && y < p.y + template_img.rows) {
					img.data[y*img.step + x*img.elemSize() + 0] = static_cast<uchar>(0);
					img.data[y*img.step + x*img.elemSize() + 1] = static_cast<uchar>(0);
					img.data[y*img.step + x*img.elemSize() + 2] = static_cast<uchar>(255);
				}
			}
		}
	}
}
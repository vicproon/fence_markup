#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <array>
#include <utility>

#include "projections.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

struct Ainv
{
  double h;
  array<double, 9> mat;
};

struct Proj
{
  array<double, 9> proj;
  array<double, 12> aff;
};

vector<Ainv> read_ainv_matrices()
{
  ifstream f("../data/ainv", ios::binary);
  vector<Ainv> result;
  int n = 0;
  f >> n;
  cout << "reading " << n << " matrices" << endl;
  for (int k = 0; k < n; ++k)
  {
    Ainv res;
    f >> res.h;
    for (int i = 0; i < 9; ++i)
      f >> res.mat[i];
    result.push_back(move(res));
  }
  return result;
}

Proj read_proj_matrix()
{
  ifstream f("../data/proj", ios::binary);
  array<double, 9> a;
  for (int i = 0; i < 9; ++i)
    f >> a[i];
  array<double, 12> b;
  for (int i = 0; i < 12; ++i)
    f >> b[i];
  return {move(a), move(b)};
}

int main()
{
  auto ainv = read_ainv_matrices();
  auto proj = read_proj_matrix();

  cout << "Ainvs: \n";
  for (const auto &a : ainv)
  {
    cout << a.h << "\t";
    copy(begin(a.mat), end(a.mat), ostream_iterator<double>(cout, ", "));
    cout << endl;
  }
  cout << "proj: \n";
  copy(begin(proj.proj), end(proj.proj), ostream_iterator<double>(cout, ", "));
  cout << endl;
  copy(begin(proj.aff), end(proj.aff), ostream_iterator<double>(cout, ", "));
  cout << endl;
  cout << endl;
  
  cv::Mat img = cv::imread("../imgs/akn.200.001.left.000138.png");  

  for (const auto &a : ainv)
  {
    reproj::PointVR p[3] = {{0.0, 15.0, a.h},
                            {5.0, 10.0, a.h},
                            {-5.0, 10.0, a.h}};
    reproj::PointIMG pix[3];
    reproj::PointVR rp[3];

    cout << endl;
    cout << "height = " << a.h << endl;
    
    for (int i = 0; i < 3; ++i)
    {
      pix[i] = reproj::projectPoint(p[i], proj.proj, proj.aff);
      rp[i] = reproj::reprojectPoint(pix[i], a.mat, a.h);
      cout << "p" << i << "(" << p[i].x << ", " << p[i].y << ", " << p[i].z << ")\t";
      cout << "pix" << i << "(" << pix[i].x << ", " << pix[i].y << ")\t";
      cout << "rp" << i << "(" << rp[i].x << ", " << rp[i].y << ", " << rp[i].z << ")\t";
      cout << endl;

    }

    cv::line(img, {pix[0].x, pix[0].y}, {pix[1].x, pix[1].y},
      cv::Scalar(100, 100, 230), 1, cv::LINE_AA);
    cv::line(img, {pix[0].x, pix[0].y}, {pix[2].x, pix[2].y},
      cv::Scalar(230, 100, 100), 1, cv::LINE_AA);
  }

  cv::imshow("img", img);
  while(27 != cv::waitKey(1))
  {

  }
  return 0;
}

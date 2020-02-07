//! @brief проекции точек на pinhole camera и репроекции в модели плоского мира
//! @author prun
#include <array>

namespace reproj
{
//! @biref точка в трехмерной метровой с.к. (VR = Vehicle Related)
struct PointVR
{
  double x; //! ось Х смотрит вправо
  double y; //! ось Y смотрит вперед
  double z; //! ось Z смотрит вверх
};

//! @brief координата пикселя на изображении
struct PointIMG
{
  int x; //! ось X смотрит вправо
  int y; //! ось Y смотрит вних
};

static constexpr double eps = 1e-7; //! some numerical epsilon

//! @brief проекция точки на плоскость изображения по модели pinhole
//! @param p точка 3д-мира
//! @param proj матрица проекции, записанная в row-major порядке
//! @param aff  матрица афинного преобразования, записанная в row-major порядке
//! @retval PointIMG пиксель, в который попадает данная 3д-точка
inline PointIMG projectPoint(const PointVR &p,
  const std::array<double, 9> &proj, const std::array<double, 12> &aff);

//! @brief репроекция пикселя на заданной высоте по модели плоского мира 
//! @param pix пиксель на изображении
//! @param ainv матрица репроекции на заданной высоте
//! @param h высота на которой проводим репроекцию
//! @retval PointVR полученная в результате репроекции 3д-точка
inline PointVR reprojectPoint(const PointIMG &pix, const std::array<double, 9> &ainv, double h);
} // namespace reproj

#include <cmath>

inline reproj::PointIMG reproj::projectPoint(const reproj::PointVR &p, 
  const std::array<double, 9> &proj, const std::array<double, 12> &aff)
{

  double x = aff[0] * p.x + aff[1] * p.y + aff[2] * p.z + aff[3];
  double y = aff[4] * p.x + aff[5] * p.y + aff[6] * p.z + aff[7];
  double z = aff[8] * p.x + aff[9] * p.y + aff[10] * p.z + aff[11];


  double wx = proj[0] * x + proj[1] * y + proj[2] * z;
  double wy = proj[3] * x + proj[4] * y + proj[5] * z;
  double w  = proj[6] * x + proj[7] * y + proj[8] * z;

  if (std::abs(w) < reproj::eps)
    return {};
  return {static_cast<int> (wx / w),
          static_cast<int> (wy / w)};
}


inline reproj::PointVR reproj::reprojectPoint(const reproj::PointIMG &pix,
  const std::array<double, 9> &ainv, double h)
{
  double px = pix.x + 0.5;
  double py = pix.y + 0.5;

  double wx = ainv[0] * px + ainv[1] * py + ainv[2];
  double wy = ainv[3] * px + ainv[4] * py + ainv[5];
  double w  = ainv[6] * px + ainv[7] * py + ainv[8];

  if (std::abs(w) < reproj::eps)
    return {};
  return {wx / w, 
          wy / w,
          h};
}


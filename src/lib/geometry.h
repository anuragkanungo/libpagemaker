#pragma once
#include <vector>
#include "Units.h"
#include "yaml_utils.h"
namespace libpagemaker
{
template <typename Unit> class Point
{
public:
  Unit m_x;
  Unit m_y;

  Point(Unit x, Unit y) : m_x(x), m_y(y)
  { }

  void emitYaml(yaml_emitter_t *emitter) const
  {
    yamlBeginMap(emitter);
    yamlMapObject(emitter, "x", m_x);
    yamlMapObject(emitter, "y", m_y);
    yamlEndMap(emitter);
  }
};

typedef Point<PMDShapeUnit> PMDShapePoint;
typedef Point<double> InchPoint;

class PMDLineSet
{
public:
  virtual std::vector<PMDShapePoint> getPoints() const = 0;
  bool virtual getIsClosed() const = 0;
  virtual float getRotation() const = 0;
  PMDShapePoint virtual getRotatingPoint() const = 0;

  void emitYaml(yaml_emitter_t *emitter) const
  {
    yamlBeginMap(emitter);
    yamlMapEntry(emitter, "closed", getIsClosed());
    yamlForeach(emitter, "points", getPoints());
    yamlEndMap(emitter);
  }

  virtual ~PMDLineSet()
  {
  }
};

class PMDPolygon : public PMDLineSet
{
  std::vector<PMDShapePoint> m_points;
  bool m_isClosed;
  float m_rotation;
  PMDShapePoint m_rotatingPoint;

public:
  PMDPolygon(std::vector<PMDShapePoint> points, bool isClosed)
    : m_points(points), m_isClosed(isClosed), m_rotation(0), m_rotatingPoint(PMDShapePoint(0,0))
  { }

  float virtual getRotation() const
  {
    return m_rotation;
  }

  PMDShapePoint virtual getRotatingPoint() const
  {
    return m_rotatingPoint;
  }

  bool virtual getIsClosed() const
  {
    return m_isClosed;
  }

  virtual std::vector<PMDShapePoint> getPoints() const
  {
    return m_points;
  }

  virtual ~PMDPolygon()
  {
  }
};

class PMDRectangle : public PMDLineSet
{
  PMDShapePoint m_topLeft;
  PMDShapePoint m_botRight;
  float m_rotation;
  PMDShapePoint m_rotatingPoint;

public:
  PMDRectangle(const PMDShapePoint &topLeft, const PMDShapePoint &botRight, const float rotation, const PMDShapePoint rotatingPoint)
    : m_topLeft(topLeft), m_botRight(botRight), m_rotation(rotation), m_rotatingPoint(rotatingPoint)
  { }

  float virtual getRotation() const
  {
    return m_rotation;
  }

  PMDShapePoint virtual getRotatingPoint() const
  {
    return m_rotatingPoint;
  }

  bool virtual getIsClosed() const
  {
    return true;
  }

  virtual std::vector<PMDShapePoint> getPoints() const
  {
    std::vector<PMDShapePoint> points;

    points.push_back(m_topLeft);
    points.push_back(PMDShapePoint(m_botRight.m_x, m_topLeft.m_y));
    points.push_back(m_botRight);
    points.push_back(PMDShapePoint(m_topLeft.m_x, m_botRight.m_y));

    return points;
  }

  virtual ~PMDRectangle()
  {
  }
};

class TransformationMatrix
{
  double m_tl, m_tr, m_bl, m_br;

public:
  TransformationMatrix(double topLeft, double topRight, double bottomLeft, double bottomRight)
    : m_tl(topLeft), m_tr(topRight), m_bl(bottomLeft), m_br(bottomRight)
  { }

  template <typename Unit> InchPoint transform(const Point<Unit> &point) const
  {
    double xInches = point.m_x.toInches(),
           yInches = point.m_y.toInches();
    double newX = m_tl * xInches + m_tr * yInches,
           newY = m_bl * xInches + m_br * yInches;
    return InchPoint(newX, newY);
  }
};
std::pair<InchPoint, InchPoint>
getBoundingBox(const PMDLineSet &lineSet, const TransformationMatrix &matrix);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */

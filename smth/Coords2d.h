#pragma once
#include "Vector2d.h"
#include "Matrix2x2.h"
#include "Boat.h"


struct Coords2d
{
public:
  Coords2d()
  {
  }
  Coords2d(Vector2d pos, Matrix2d axes)//передаем сюда boat.pos, boat.direction, boat.direction.Getnormal 
  {
    this->position = pos;
    this->axes = axes;
  }
  Coords2d(Vector2d pos, Vector2d direction)//передаем сюда boat.pos, boat.direction, boat.direction.Getnormal 
  {
    this->position = pos;
    this->axes.column1 = direction;
    this->axes.column2 = direction.GetNormalTangent();
  }
  void SetAxes(Matrix2d axes)
  {
    this->axes = axes;
  }
  void SetDirection(Vector2d v)
  {
    if (v.GetLength() > 1e-15)
    {
      this->axes.column1 = v;
      this->axes.column2 = v.GetNormalTangent();
    }
  }
    
  void SetPosition(Vector2d pos)
  {
    this->position = pos;
  }
  Matrix2d GetAxes()
  {
    return axes;
  }
  Vector2d GetPosition()
  {
    return position;
  }
  Vector2d GetDirection()
  {
    return axes.column1;
  }
  Vector2d GetNormalDirection()
  {
    return axes.column2;
  }
  Vector2d GetLocalPoint(Vector2d worldPoint)
  {
    Matrix2d mat = Matrix2d(axes.column2 / axes.column2.GetLength(), axes.column1 / axes.column1.GetLength());
    mat.Transpose();
    Vector2d v = mat * worldPoint;
    //Vector2d v = Turn(worldPoint, GetAngle(axes.column1, Vector2d(0.0, 1.0)));
    return v - position;
  }
  Vector2d GetWorldPoint(Vector2d localPoint)
  {
    Matrix2d mat = Matrix2d(axes.column2 / axes.column2.GetLength(), axes.column1 / axes.column1.GetLength());
    Vector2d v = mat * localPoint;
    //Vector2d v = Turn(localPoint, -GetAngle(axes.column1, Vector2d(0.0, 1.0)));
    return v + position;
  }
  Vector2d GetLocalDirection(Vector2d worldVector)
  {
    if (worldVector.GetLength() > 1e-15)
      //   return Turn(worldVector, GetAngle(axes.column1, Vector2d(0.0, 1.0)));
    {
      Matrix2d mat = Matrix2d(axes.column2 / axes.column2.GetLength(), axes.column1 / axes.column1.GetLength());
      mat.Transpose();
      return mat * worldVector;
    }
    else
      return Vector2d(0.0, 0.0);
  }
  Vector2d GetWorldDirection(Vector2d localVector)
  {
    if (localVector.GetLength() > 1e-15)
      // return Turn(localVector, -GetAngle(axes.column1, Vector2d(0.0, 1.0)));
    {
      Matrix2d mat = Matrix2d(axes.column2/axes.column2.GetLength(), axes.column1/axes.column1.GetLength());
      return mat * localVector;
    }
    else
      return Vector2d(0.0, 0.0);
  }

private:
  Matrix2d axes;// 1 столбец - Xкоорд(направл лодки), 2 столбец - Yкоорд(нормаль к лодке)
  Vector2d position;// position лодки, он же начало координат 
};

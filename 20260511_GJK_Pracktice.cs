using System.Collections.Generic;
using UnityEngine;

namespace Practice
{
    public struct Point
    {
        public float x, y;
        public Point(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }
    public struct Edge
    {
        public Vector2 normal;   // 바깥 방향 (단위 벡터)
        public float distance;   // 원점에서 이 edge(직선)까지의 수직 거리
        public int index;        // polytope 리스트에서의 위치 (분할할 때 필요)
    }
    public class GJKPractice
    {
        List<Point> triangleList = new();

        public bool GJK_Flow(List<Point> shapeA, List<Point> shapeB)
        {
            //첫 두점 등록
            Vector3 dir = Vector3.right;
            Point p1 = Support(shapeA, shapeB, dir);
            Point p2 = Support(shapeA, shapeB, -dir);
            triangleList.Add(p1);
            triangleList.Add(p2);

            //다음 방향
            dir = Vertical_AOToLineAB(p1,p2);

            //충분한 수만큼 반복
            int Iter_value = 128;
            for(int i = 0; i < Iter_value; i++)
            {
                //dir 방향으로 점 P를 뽑는다.
                Point p = Support(shapeA, shapeB,dir);

                //종료 조건 : 충돌 안함
                //원점과 비교하니까 벡터op 기준
                //Dir방향으로 점 p를 투영->내적 사용
                //내적값이 0미만 -> 그 방향으론 원점 포함을 안함 -> 서로 충돌하지 않음
                Vector3 po = new Vector3(p.x, p.y, 0);
                if (po.x * dir.x + po.y * dir.y <= 0)
                {
                    return false;
                }

                //삼각형 ABP가 원점을 포함하는지 검사한다.
                if(IsInTriangle(p1,p2, p))
                {
                    //EPA 알고리즘
                    triangleList = new List<Point>{p1,p2,p};
                    return true;
                }
                else//p1,p2중 원점에서 먼 점을 버린다.
                {
                    //새로운 삼각형
                    Vector3 pp1 = new Vector3(p1.x - p.x, p1.y - p.y, 0);   // p → p1
                    Vector3 pp2 = new Vector3(p2.x - p.x, p2.y - p.y, 0);   // p → p2

                    //p1p, pp2를 dir에 투영
                    Vector3 ppNormal = Vector3.Cross(Vector3.Cross(pp2, pp1), pp1);
                    Vector3 poo = new Vector3(-p.x, -p.y, 0);
                    float ppDot = ppNormal.x*poo.x+ ppNormal.y*poo.y;
                    if (ppDot>0)//더 작은 값을 날림
                    {
                        p2 = p;
                        dir = Vertical_AOToLineAB(p, p1);
                    }
                    else
                    {
                        p1 = p;
                        dir = Vertical_AOToLineAB(p2, p);
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// 가장 멀리 있는 점
        /// </summary>
        /// <returns></returns>
        Point Support(List<Point> shapeA, List<Point> shapeB, Vector3 dir)
        {
            //도형 A,B에서 dir방향으로 가장 멀리 있는점
            Point FarAPoint = FarthestInDirection(shapeA, dir);
            Point FarBPoint = FarthestInDirection(shapeB, dir);

            //민코브스키 차, B-A
            Point mincovDiff = new Point(FarBPoint.x-FarAPoint.x,FarBPoint.y-FarAPoint.y);
            return mincovDiff;
        }

        /// <summary>
        /// 해당 방향에서 가장 멀리 있는 지점
        /// </summary>
        /// <returns></returns>
        Point FarthestInDirection(List<Point> shapeList, Vector3 dir)
        {
            Point farPoint = new Point();
            float dotValue = -float.MaxValue;

            foreach (Point p in shapeList)
            {
                //dir을 선분 AB라하면 나머지 점들을 Pn이라 하자
                //내적에의해 선분 APn들은 전부 선분 AB에 투영될터이다.
                //그렇다는 뜻은 선분 APn의 방향은 같고 투영된 길이만 달라지는데 
                //이 투영된 길이가 가장 긴 것이 dir방향으로부터 가장 멀리 떨어진 점이다.
                //즉 내적값이 가장 크게 나오는 선분 APn을 찾으면 점 Pn이 dir방향으로 가장 멀리 있는 점이 된다.
                float curDot = p.x * dir.x + p.y * dir.y;
                if (curDot > dotValue)
                {
                    dotValue = curDot;
                    farPoint = p;
                }
            }
            return farPoint;
        }

        /// <summary>
        /// 삼각형이 원점을 포함하는가?
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="c"></param>
        /// <returns></returns>
        bool IsInTriangle(Point a, Point b, Point c)
        {
            Vector2 oa = new Vector2(-a.x,-a.y);
            Vector2 ob = new Vector2(-b.x, -b.y);
            Vector2 oc = new Vector2(-c.x,-c.y); 

            //외적값이 전부 같은 부호이면 된다.
            if(CrossVec2D(oa, ob)>=0 && CrossVec2D(ob, oc) >= 0 && CrossVec2D(oc, oa) >= 0)
            {
                return true;
            }
            if (CrossVec2D(oa, ob) <= 0 && CrossVec2D(ob, oc) <= 0 && CrossVec2D(oc, oa) <= 0)
            {
                return true;
            }
            return false;
        }

        //2차원 벡터 외적
        float CrossVec2D(Vector2 a, Vector2 b)
        {
            return a.x * b.y - a.y * b.x;
        } 

        /// <summary>
        /// 선분 AB에서 O로 향하는 방향 찾기(입력은 점 A,B)
        /// 선분 AB, AO를 활용하면 된다.
        /// AB, AO를 외적하면 z축에 수직인 벡터(PZ)가 나온다.
        /// 이를 다시 AB와 외적하면 AB, PZ와 수직인 벡터를 구할 수 있다.
        /// 우리가 구하는건 방향이므로 방금 과정에서 방향 찾고 종료
        /// </summary>
        /// <returns></returns>
        Vector3 Vertical_AOToLineAB(Point a, Point b)
        {
            Vector2 ab = new Vector2(b.x - a.x, b.y - a.y);
            Vector2 ao = new Vector2(- a.x, - a.y);
            float pz = ab.x*ao.y- ab.y*ao.x;

            Vector3 crossVecAO = new Vector3(-ab.y * pz, ab.x * pz, 0);
            return crossVecAO;
        }
    }
}

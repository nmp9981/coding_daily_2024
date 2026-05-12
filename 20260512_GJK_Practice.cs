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
       
        public bool GJK_Flow(List<Point> shapeA, List<Point> shapeB)
        {
            //첫 두점 등록
            Vector3 dir = Vector3.right;
            Point p1 = Support(shapeA, shapeB, dir);//A
            Point p2 = Support(shapeA, shapeB, -dir);//B

            //다음 방향 탐색
            dir = Dir_LineAB_LineAO(p1,p2);

            //충분한 수 반복
            int Iter = 128;
            for(int i = 0; i < Iter; i++)
            {
                //가장 멀리 있는 점
                Point p = Support(shapeA, shapeB, dir);

                //충돌 X
                Vector2 op = new Vector2(p.x,p.y);
                float pDot = op.x * dir.x + op.y * dir.y;
                if (pDot <= 0)
                {
                    return false;
                }

                //삼각형이 원점 포함
                if (IsTriangleInOrigin(p1, p, p2))
                {
                    List<Point> triangleList = new List<Point> { p1, p2, p };
                    //EPA
                    return true;
                }
                else
                {
                    //필요 없는 점을 제외(점P가 어느변의 바깥에 있는지 알아야함)
                    Vector3 PA = new Vector3(p1.x-p.x,p1.y-p.y,0);
                    Vector3 PB = new Vector3(p2.x - p.x, p2.y - p.y,0);

                    //PB의 법선벡터(PA, PB 중 기준 선 하나 잡고 구함)
                    Vector3 normalPB = Vector3.Cross(Vector3.Cross(PA, PB), PB);
                    //PO와 내적
                    Vector3 PO = new Vector3(-p.x,-p.y);
                    float dotValue = PO.x*normalPB.x+PO.y*normalPB.y;

                    //점 제거하고 방향 다시 찾음
                    if(dotValue > 0)//A(p1)제거
                    {
                        p1 = p;
                        dir = Dir_LineAB_LineAO(p1, p2);
                    }
                    else//B(p2)제거
                    {
                        p2 = p;
                        dir = Dir_LineAB_LineAO(p2, p1);
                    }

                }
            }

            return false;
        }


        /// <summary>
        /// 해당 방향에서 가장 멀리 있는 점 찾기
        /// </summary>
        /// <param name="shapeA"></param>
        /// <param name="shapeB"></param>
        public Point Support(List<Point> shapeA, List<Point> shapeB, Vector3 dir)
        {
            Point A = FarestPointInShape(shapeA, dir);
            Point B = FarestPointInShape(shapeB, -dir);

            //민코브스키 차 : A(dir) - B(-dir)
            Point mincovDiff = new Point(A.x - B.x,A.y-B.y);
            return mincovDiff;
        }

        /// <summary>
        /// 가장 멀리 있는 점
        /// dir방향으로 투영하므로 벡터 크기가 결과를 좌우
        /// 내적값이 가장 큰 점을 고른다.
        /// </summary>
        /// <returns></returns>
        Point FarestPointInShape(List<Point> shapeList, Vector3 dir)
        {
            Point farPoint = new Point();
            float dotValue = -float.MaxValue;

            foreach (var p in shapeList)
            {
                float curDot = p.x * dir.x + p.y * dir.y;
                
                //더 큰 내적값이 오면 갱신
                if (curDot > dotValue)
                {
                    farPoint = p;
                    dotValue = curDot;
                }
            }
            return farPoint;
        }
        /// <summary>
        /// 선분 AB에서 원점으로 가는 방향 찾기
        /// (ABXAO)XAB
        /// </summary>
        /// <returns></returns>
        Vector2 Dir_LineAB_LineAO(Point A, Point B)
        {
            Vector2 AB = new Vector2(B.x - A.x, B.y - A.y);
            Vector2 AO = new Vector2(-A.x, -A.y);

            float zValue = AB.x * AO.y - AB.y * AO.x;
            Vector2 dirO = new Vector2(-zValue*AB.y, zValue*AB.x);
            return dirO;
        }

        /// <summary>
        /// 삼각형이 원점을 포함하는가?
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="c"></param>
        /// <returns></returns>
        bool IsTriangleInOrigin(Point a, Point b, Point c)
        {
            Vector2 OA = new Vector2(a.x,a.y);
            Vector2 OB = new Vector2(b.x, b.y);
            Vector2 OC = new Vector2(c.x, c.y);

            if (CrossVector2(OA,OB)>=0 && CrossVector2(OB, OC) >= 0 && CrossVector2(OC, OA) >= 0)
            {
                return true;
            }
            if (CrossVector2(OA, OB) <= 0 && CrossVector2(OB, OC) <= 0 && CrossVector2(OC, OA) <= 0)
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// 2차원 외적
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns></returns>
        float CrossVector2(Vector2 A, Vector2 B) {
            return A.x * B.y - A.y * B.x;
        }
    }
}

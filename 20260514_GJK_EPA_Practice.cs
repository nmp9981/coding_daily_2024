using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Unity.VisualScripting;
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
    public class GJKPractice : MonoBehaviour
    {
        private void Start()
        {
            // 1만큼 겹침
            var a1 = new List<Point> { new(0, 0), new(2, 0), new(2, 2), new(0, 2) };
            var b1 = new List<Point> { new(1, 0), new(3, 0), new(3, 2), new(1, 2) };
            //Debug.Log($"겹침: {new GJKPractice().GJK_Flow(a1, b1)}");  // true

            // 떨어짐
            var a2 = new List<Point> { new(0, 0), new(1, 0), new(1, 1), new(0, 1) };
            var b2 = new List<Point> { new(5, 5), new(6, 5), new(6, 6), new(5, 6) };
            //Debug.Log($"분리: {new GJKPractice().GJK_Flow(a2, b2)}");  // false

            // 살짝 겹침 (0.5만큼 위로)
            var a3 = new List<Point> { new(0, 0), new(2, 0), new(2, 2), new(0, 2) };
            var b3 = new List<Point> { new(0, 1.5f), new(2, 1.5f), new(2, 3.5f), new(0, 3.5f) };
            Debug.Log($"수직 겹침: {new GJKPractice().GJK_Flow(a3, b3)}");  // true

            // 삼각형 vs 사각형
            var a4 = new List<Point> { new(0, 0), new(3, 0), new(1.5f, 3) };
            var b4 = new List<Point> { new(1, 1), new(4, 1), new(4, 4), new(1, 4) };
            //Debug.Log($"비대칭 겹침: {new GJKPractice().GJK_Flow(a4, b4)}");  // true
        }

        public bool GJK_Flow(List<Point> shapeA, List<Point> shapeB)
        {
            //첫 두점 등록
            Vector2 dir = Vector2.right;
            Point a =Support(shapeA,shapeB,dir);
            Point b = Support(shapeA,shapeB,-dir);

            //첫 방향 구하기
            dir = Direction_ABLineToAO(a,b);

            //충분한 반복수
            int iter = 128;
            for(int i = 0; i < iter; i++)
            {
                //새로운 점
                Point p = Support(shapeA,shapeB,dir);

                //충돌 체크
                float pDot = p.x*dir.x+ p.y*dir.y;
                if (pDot < 0) return false;//충돌 안함

                //삼각형이 원점을 포함하는가?
                if (IsTriangleOrigin(a,p,b))
                {
                    List<Point> triangleList = new List<Point> { a, p, b };
                    (float depth, Vector2 direction) = EPA_Flow(shapeA, shapeB, triangleList);
                    Debug.Log("깊이 : " + depth + " 방향 " + direction.x + " " + direction.y);
                    return true;
                }
                else//점 교체 및 방향 변경
                {
                    Vector2 PA = new Vector2(a.x-p.x,a.y-p.y);
                    Vector2 PB = new Vector2(b.x - p.x, b.y - p.y);

                    float z = Cross_Vector2(PA,PB);
                    Vector2 PBnormal = new Vector2(-z*PB.y,z*PB.x);
          
                    Vector2 PO = new Vector2(-p.x,-p.y);
                    float dot = PBnormal.x*PO.x+ PBnormal.y*PO.y;
                    if (dot > 0)//A를 교체
                    {
                        a = p;
                        dir = Direction_ABLineToAO(a,b);
                    }
                    else//B를 교체
                    {
                        b = p;
                        dir = Direction_ABLineToAO(a, b);
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// EPA알고리즘
        /// 깊이, 방향 반환
        /// </summary>
        (float depth, Vector2 direction) EPA_Flow(List<Point> shapeA, List<Point> shapeB, List<Point> triangleList)
        {
            Debug.Log($"EPA 입력 삼각형: {triangleList[0].x},{triangleList[0].y} | {triangleList[1].x},{triangleList[1].y} | {triangleList[2].x},{triangleList[2].y}");
            //CCW 정렬
            SortCCW(triangleList);
            Debug.Log($"EPA 입력 삼각형: {triangleList[0].x},{triangleList[0].y} | {triangleList[1].x},{triangleList[1].y} | {triangleList[2].x},{triangleList[2].y}");

            const int MAX_ITER = 32;
            const float EPSILON = 0.0001f;
            for(int i = 0; i < MAX_ITER; i++)
            {
                //원점에서 가장 가까운 변
                Edge edge = ClosestEdgeInOrigin(triangleList);

                //해당 방향으로 부터 가장 먼 점
                Point p = Support(shapeA, shapeB, edge.normal);
                //점 p를 노말 방향으로 투영한 길이
                float dist = p.x* edge.normal.x+p.y* edge.normal.y;

                //거리 비교
                if (Mathf.Abs(edge.distance-dist)<EPSILON)//종료
                {
                    return (edge.distance, edge.normal);
                }
                else
                {
                    triangleList.Insert(edge.index+1, p);
                }
            }
            //최대 반복
            Edge side = ClosestEdgeInOrigin(triangleList);
            return (side.distance, side.normal);
        }

        /// <summary>
        /// 해당 방향으로 가장 멀리 있는 점 찾기
        /// </summary>
        /// <param name="shapeA"></param>
        /// <param name="shapeB"></param>
        /// <param name="dir"></param>
        /// <returns></returns>
        Point Support(List<Point> shapeA, List<Point> shapeB, Vector2 dir)
        {
            Point A = FarestPoint(shapeA, dir);
            Point B = FarestPoint(shapeB, -dir);
            Point mincovDiff = new Point(A.x-B.x,A.y-B.y);
            return mincovDiff;
        }

        /// <summary>
        /// 가장 멀리 있는 점
        /// 내적으로 판정
        /// </summary>
        /// <param name="shapeList"></param>
        /// <param name="dir"></param>
        /// <returns></returns>
        Point FarestPoint(List<Point> shapeList, Vector2 dir)
        {
            Point target = new Point();
            float dist = -float.MaxValue;

            foreach (Point p in shapeList)
            {
                float curDist = p.x*dir.x+ p.y*dir.y;
                if (curDist > dist)
                {
                    target = p;
                    dist = curDist;
                }
            }
            return target;
        }

        /// <summary>
        /// 선분 AB에서 원점으로 향하는 방향 구하기
        /// </summary>
        /// <param name="dir"></param>
        /// <returns></returns>
        Vector2 Direction_ABLineToAO(Point a, Point b)
        {
            Vector2 AB = new Vector2(b.x - a.x, b.y - a.y);
            Vector2 AO = new Vector2(-a.x,-a.y);
            float z = Cross_Vector2(AB,AO);
            return new Vector2(-z*AB.y, z*AB.x);
        }
        /// <summary>
        /// 2차원 벡터 외적
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        float Cross_Vector2(Vector2 a, Vector2 b)
        {
            return a.x * b.y - b.x * a.y;
        }
        /// <summary>
        /// 해당 삼각형이 원점을 포함하는가?
        /// </summary>
        /// <returns></returns>
        bool IsTriangleOrigin(Point a, Point b, Point c)
        {
            Vector2 OA = new Vector2(a.x,a.y);
            Vector2 OB = new Vector2(b.x, b.y);
            Vector2 OC = new Vector2(c.x, c.y);

            if(Cross_Vector2(OA, OB) >= 0 && Cross_Vector2(OB, OC) >= 0 && Cross_Vector2(OC, OA) >= 0)
            {
                return true;
            }
            if (Cross_Vector2(OA, OB) <= 0 && Cross_Vector2(OB, OC) <= 0 && Cross_Vector2(OC, OA) <= 0)
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// CCW 정렬
        /// </summary>
        /// <param name="triangleList"></param>
        void SortCCW(List<Point> triangleList)
        {
            Vector2 p1 = new Vector2(triangleList[1].x-triangleList[0].x, triangleList[1].y - triangleList[0].y);
            Vector2 p2 = new Vector2(triangleList[2].x - triangleList[0].x, triangleList[2].y - triangleList[0].y);

            float ccwValue = Cross_Vector2(p1,p2);
            if (ccwValue >= 0)//반시계
            {
            }
            else//시계
            {
                (triangleList[1], triangleList[2]) = (triangleList[2], triangleList[1]);
            }
        }
        /// <summary>
        /// 원점에서 가장 가까운 변 찾기
        /// </summary>
        /// <returns></returns>
        Edge ClosestEdgeInOrigin(List<Point> triangleList)
        {
            Edge edge = new Edge();
            float dist = float.MaxValue;

            int count = triangleList.Count;
            for(int i = 0; i < count; i++)
            {
                //변
                Point p1 = triangleList[i];
                Point p2 = triangleList[(i + 1)%count];

                //변을 90도 시계방향으로 회전하면 노말벡터가 나옴
                //우리는 해당 방향기준으로 원점까지의 거리를 구하면 된다.
                //방향은 이미 있으니 선분 OA, OB중 아무거나 해당 방향에 투영시킴
                //원점으로부터 선분 AB까지의 거리를 구할 수 있음
                Vector2 AB = new Vector2(p2.x-p1.x,p2.y-p1.y);
                Vector2 ABNormal = new Vector2(AB.y,-AB.x).normalized;

                //노말 방향 투영길이
                float curDist = ABNormal.x*p1.x+ ABNormal.y*p1.y;
                if(curDist < dist)//더 가까운 거리
                {
                    dist = curDist;

                    edge.distance = dist;
                    edge.normal = ABNormal;
                    edge.index = i;
                }
            }
            return edge;
        }
    }
}

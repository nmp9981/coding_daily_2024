using System.Collections.Generic;
using System.Drawing;
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
            Debug.Log($"겹침: {new GJKPractice().GJK_Flow(a1, b1)}");  // true

            // 떨어짐
            var a2 = new List<Point> { new(0, 0), new(1, 0), new(1, 1), new(0, 1) };
            var b2 = new List<Point> { new(5, 5), new(6, 5), new(6, 6), new(5, 6) };
            Debug.Log($"분리: {new GJKPractice().GJK_Flow(a2, b2)}");  // false

            // 살짝 겹침 (0.5만큼 위로)
            var a3 = new List<Point> { new(0, 0), new(2, 0), new(2, 2), new(0, 2) };
            var b3 = new List<Point> { new(0, 1.5f), new(2, 1.5f), new(2, 3.5f), new(0, 3.5f) };
            Debug.Log($"수직 겹침: {new GJKPractice().GJK_Flow(a3, b3)}");  // true

            // 삼각형 vs 사각형
            var a4 = new List<Point> { new(0, 0), new(3, 0), new(1.5f, 3) };
            var b4 = new List<Point> { new(1, 1), new(4, 1), new(4, 4), new(1, 4) };
            Debug.Log($"비대칭 겹침: {new GJKPractice().GJK_Flow(a4, b4)}");  // true
        }

        public bool GJK_Flow(List<Point> shapeA, List<Point> shapeB)
        {
            //첫 두 점을 구한다.
            Vector2 dir = Vector2.right;
            Point a = Support(shapeA, shapeB, dir);
            Point b = Support(shapeA, shapeB, -dir);

            //첫 방향을 구한다.
            dir = DirectionABlineToOrigin(a, b);

            //충분한 반복
            int iter = 128;
            for(int i = 0; i < iter; i++)
            {
                Point p = Support(shapeA, shapeB,dir);

                //충돌 여부
                float crashDot = p.x * dir.x + p.y * dir.y;
                if (crashDot < 0)//충돌 안함
                {
                    return false;
                }

                ///원점이 삼각형 내에 있는지 검사
                if (IsInTriangle(a, p, b))
                {
                    //EPA로
                    List<Point> triangleList = new List<Point> { a,p,b};
                    var result = EPA_Flow(shapeA, shapeB, triangleList);
                    //Debug.Log(result.depth + " " + result.direction);
                    return true;
                }
                else
                {
                    //다음 방향을 찾아야함
                    //원점이 어느변에 가까이 있냐에 따라 a,b중 먼 점을 제거한다.
                    //(PAxPB)xPB
                    Vector2 po = new Vector2(-p.x,-p.y);
                    Vector2 pb = new Vector2(b.x-p.x,b.y-p.y);
                    Vector2 pa = new Vector2(a.x-p.x,a.y-p.y);
                    float z = Cross_Vector2(pa, pb);
                    Vector2 judgeVec = new Vector2(-z*pb.y,z*pb.x);

                    //원점 방향에 투영
                    float nearDot = judgeVec.x * po.x + judgeVec.y * po.y;

                    if (nearDot>0)//a제거
                    {
                        a = p;
                        dir = DirectionABlineToOrigin(a,b);
                    }
                    else//b제거
                    {
                        b = p;
                        dir = DirectionABlineToOrigin(a,b);
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
            //CCW 정렬
            SortCCW(triangleList);

            int iter = 128;
            float EPCILLON = 0.01f;
            for(int i = 0; i <iter; i++)
            {
                //원점에서 가장 가까운 변을 구하기
                //변을 구하는거니 만둘어둔 구조체 활용
                Edge side = ClosetSideInOrigin(triangleList);
               
                //그 방향으로 부터 가장 멀리 있는 점 구하기
                //원점에서 가장 먼 변을 찾기
                //이미 normal벡터가 있어 여기서 재계산할 필요없음
                Point p = Support(shapeA, shapeB, side.normal);

                //그 점이 가장 바깥점인가?
                float dist = p.x*side.normal.x+p.y*side.normal.y;
                //맞으면 종료, 아니면 리스트에 추가
                if (Mathf.Abs(side.distance-dist)<EPCILLON)
                {
                    //edge 정보 자체는 위에서 이미 반환된 채로 나오므로 조건만 되면 바료 반환만 하고 종료한다.
                    return (side.distance, side.normal);
                }
                else
                {
                    triangleList.Insert(side.index+1, p);
                }
            }
            //마지막 방향
            Edge lastEdge = ClosetSideInOrigin(triangleList);
            return (lastEdge.distance, lastEdge.normal);
        }

        /// <summary>
        /// 해당방향으로부터 가장 멀리 있는 점
        /// </summary>
        Point Support(List<Point> shapeA, List<Point> shapeB, Vector2 dir)
        {
            Point A = FindFartestPoint(shapeA,dir);
            Point B = FindFartestPoint(shapeB,-dir);
            //민코브스키 차, A-B
            Point mincovDiffPoint = new Point(A.x-B.x,A.y-B.y);
            return mincovDiffPoint;
        }
        /// <summary>
        /// 가장 멀리 있는 점 찾기
        /// </summary>
        /// <returns></returns>
        Point FindFartestPoint(List<Point> shapeList, Vector2 dir)
        {
            Point target = new Point();
            float dist = -float.MaxValue;

            foreach (Point p in shapeList)
            {
                float curDist = p.x*dir.x+ p.y*dir.y;
                if (curDist > dist)
                {
                    dist= curDist;
                    target = p;
                }
            }

            return target;
        }

        /// <summary>
        /// 두 벡터의 외적
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns></returns>
        float Cross_Vector2(Vector2 A, Vector2 B)
        {
            return A.x * B.y - A.y * B.x;
        }

        /// <summary>
        /// 선분 AB에서 원점을 향하는 방향
        /// </summary>
        /// <returns></returns>
        Vector2 DirectionABlineToOrigin(Point a, Point b)
        {
            Vector2 AB = new Vector2(b.x-a.x,b.y-a.y);
            Vector2 AO = new Vector2(-a.x,-a.y);
            float z = Cross_Vector2(AB, AO);
            Vector2 cross_OriginAO = new Vector2(-z*AB.y,z*AB.x);
            return cross_OriginAO;
        }

        /// <summary>
        /// 원점이 삼각형 내에 있는가?
        /// </summary>
        /// <returns></returns>
        bool IsInTriangle(Point a, Point p,Point b)
        {
            Vector2 oa = new Vector2(a.x,a.y);
            Vector2 op = new Vector2(p.x, p.y);
            Vector2 ob = new Vector2(b.x, b.y);

            if(Cross_Vector2(oa,op)>=0 && Cross_Vector2(op,ob)>=0 && Cross_Vector2(ob, oa) >= 0)
            {
                return true;
            }
            if (Cross_Vector2(oa, op) <= 0 && Cross_Vector2(op, ob) <= 0 && Cross_Vector2(ob, oa) <= 0)
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// CCW 정렬
        /// </summary>
        void SortCCW(List<Point> triangleList)
        {
            //ap X ab 으로 비교
            Vector2 ap = new Vector2(triangleList[1].x - triangleList[0].x, triangleList[1].y - triangleList[0].y);
            Vector2 ab = new Vector2(triangleList[2].x - triangleList[0].x, triangleList[2].y - triangleList[0].y);

            float ccw = Cross_Vector2(ap,ab);
            if (ccw > 0)//반시계 
            {

            }
            else//시계
            {
                (triangleList[1], triangleList[2]) = (triangleList[2], triangleList[1]);
            }
        }

        /// <summary>
        /// 원점에서 가장 가까운 변 구히기
        /// </summary>
        /// <returns></returns>
        Edge ClosetSideInOrigin(List<Point> triangleList)
        {
            int count = triangleList.Count;
            float dist = float.MaxValue;
            Edge closeestSide = new Edge();

            for (int i = 0; i < count; i++)
            {
                Point p1 = triangleList[i];
                Point p2 = triangleList[(i + 1) % count];

                //변의 노멀벡터를 구함
                Vector2 side = new Vector2(p2.x-p1.x,p2.y-p1.y);
                Vector2 normal = new Vector2(side.y, -side.x).normalized;

                //해당 방향으로 거리를 구함
                float curDist = normal.x* p1.x+normal.y*p1.y;
                //더 가까운 거리
                if(dist > curDist)
                {
                    dist = curDist;
                    closeestSide.index = i;
                    closeestSide.normal = normal;
                    closeestSide.distance = dist;
                }
            }
            return closeestSide;
        }
    }
}

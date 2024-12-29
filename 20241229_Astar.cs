using System.Collections.Generic;
using UnityEngine;

public class AStar
{
    private class Node
    {
        public Vector2Int position;
        public Node parent;
        public float g; // 시작점으로부터의 비용
        public float h; // 목표점까지의 추정 비용
        public float f => g + h; // 총 비용

        public Node(Vector2Int pos, Node parent = null)
        {
            position = pos;
            this.parent = parent;
        }
    }

    private static readonly Vector2Int[] directions = {
        new Vector2Int(0, 1),  // 상
        new Vector2Int(1, 0),  // 우
        new Vector2Int(0, -1), // 하
        new Vector2Int(-1, 0), // 좌
        new Vector2Int(1, 1),  // 우상
        new Vector2Int(1, -1), // 우하
        new Vector2Int(-1, 1), // 좌상
        new Vector2Int(-1, -1) // 좌하
    };

    public static List<Vector2Int> FindPath(bool[,] grid, Vector2Int start, Vector2Int goal)
    {
        int width = grid.GetLength(0);
        int height = grid.GetLength(1);

        var openSet = new List<Node>();
        var closedSet = new HashSet<Vector2Int>();
        var startNode = new Node(start);

        openSet.Add(startNode);

        while (openSet.Count > 0)
        {
            var current = GetLowestFCostNode(openSet);
            
            if (current.position == goal)
            {
                return RetracePath(current);
            }

            openSet.Remove(current);
            closedSet.Add(current.position);

            foreach (var direction in directions)
            {
                var neighborPos = current.position + direction;

                if (!IsValidPosition(neighborPos, width, height) || 
                    !grid[neighborPos.x, neighborPos.y] || 
                    closedSet.Contains(neighborPos))
                {
                    continue;
                }

                float newG = current.g + GetDistance(current.position, neighborPos);
                var neighbor = new Node(neighborPos, current);

                if (!openSet.Exists(n => n.position == neighborPos) || newG < neighbor.g)
                {
                    neighbor.g = newG;
                    neighbor.h = GetDistance(neighborPos, goal);

                    if (!openSet.Exists(n => n.position == neighborPos))
                    {
                        openSet.Add(neighbor);
                    }
                }
            }
        }

        return null; // 경로를 찾지 못함
    }

    private static Node GetLowestFCostNode(List<Node> nodes)
    {
        var lowest = nodes[0];
        for (int i = 1; i < nodes.Count; i++)
        {
            if (nodes[i].f < lowest.f)
            {
                lowest = nodes[i];
            }
        }
        return lowest;
    }

    private static float GetDistance(Vector2Int a, Vector2Int b)
    {
        var diff = a - b;
        return Mathf.Sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    private static bool IsValidPosition(Vector2Int pos, int width, int height)
    {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }

    private static List<Vector2Int> RetracePath(Node endNode)
    {
        var path = new List<Vector2Int>();
        var current = endNode;

        while (current != null)
        {
            path.Add(current.position);
            current = current.parent;
        }

        path.Reverse();
        return path;
    }
}

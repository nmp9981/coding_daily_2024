public static class SAT
{
    public static bool Overlaps(Vector2[] a, Vector2[] b)
    {
        // 두 도형의 모든 변에서 후보 축 수집
        if (!CheckAxes(a, a, b)) return false;
        if (!CheckAxes(b, a, b)) return false;
        return true; // 모든 축에서 겹침 → 충돌
    }

    private static bool CheckAxes(Vector2[] poly, Vector2[] a, Vector2[] b)
    {
        for (int i = 0; i < poly.Length; i++)
        {
            Vector2 edge = poly[(i + 1) % poly.Length] - poly[i];
            Vector2 axis = new Vector2(-edge.y, edge.x).normalized; // 법선

            Project(a, axis, out float minA, out float maxA);
            Project(b, axis, out float minB, out float maxB);

            // 구간이 분리되면 충돌 아님
            if (maxA < minB || maxB < minA) return false;
        }
        return true;
    }

    private static void Project(Vector2[] poly, Vector2 axis, out float min, out float max)
    {
        min = max = Vector2.Dot(poly[0], axis);
        for (int i = 1; i < poly.Length; i++)
        {
            float p = Vector2.Dot(poly[i], axis);
            if (p < min) min = p;
            else if (p > max) max = p;
        }
    }
}

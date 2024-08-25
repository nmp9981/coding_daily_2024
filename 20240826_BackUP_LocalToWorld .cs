using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using Newtonsoft.Json;
using Model;
using ScreenShot;
using PathFinding;
using Cysharp.Threading.Tasks;
using Metacle;
using System.Linq;
using RTG;
using static UnityEngine.GraphicsBuffer;
using System;

[System.Serializable]
public class ResultData
{
    public int areaNum;
    public int[,] pos;
}
[System.Serializable]
public class ResultDataSet
{
    public ResultData[] AreaData;
}

public class LocalToWorld : SingletonMonoBehaviour<LocalToWorld>
{
    public static ResultDataSet resultDataSet;
    GridManager gridManager;
    [SerializeField] GameObject cube;
    public List<GameObject> pipePoint = new List<GameObject>();
    public List<GameObject> pipeSidePoint = new List<GameObject>();

    Vector3 fromTarget;
    public Vector3 toTarget;
    Vector3 targetSize;
    Vector3 fromLocalPos;
    Vector3 firstPos;
    float rate = 256f / 1920f;
    
    string fromEquipment;
    string toEquipment;
    string fromNozzle;
    string toNozzle;

    Nozzle startNozzle;
    int len;
    public List<Vector3> usePos = new List<Vector3>();

    void Awake()
    {
        LoadData();
        gridManager = GameObject.Find("Manager").GetComponent<GridManager>();
    }
    
    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.F9))
        {
            PipeOff();
        }
    }

    public async UniTask JsonSearch()
    {
        await UniTask.WaitUntil(() => ScreenShotRuntime.Instance.ScreenshotCount == ScreenShotRuntime.Instance.MaxScreenshotCount);
        Debug.Log(ScreenShotRuntime.resultAIJsonData.Count + " json데이터 개수");

        foreach(var jsondata in ScreenShotRuntime.resultAIJsonData)
        {
            foreach(var d in jsondata.Item1)
            {
                Debug.Log(d);
            }
            Debug.Log(jsondata.Item1.Length+" 메세지 길이 ");
            Debug.Log(jsondata.Item2+" 찐 경로");
            len = jsondata.Item1.Length;
        }
    }
    public void FindTarget(Nozzle fromNozzle, Nozzle toNozzle)
    {
        startNozzle = fromNozzle;
        //만약 replace노즐이면 바운드를 다르게 잡기
        if (startNozzle.gameObject.name.Contains("Replace"))
        {
            Bounds startBound = startNozzle.gameObject.GetComponent<MeshRenderer>().bounds;
            fromTarget = startBound.center;
            targetSize = startBound.size;
        }
        else
        {
            Bounds startBound = startNozzle.MyGeometryObject.MyMeshRender.bounds;
            fromTarget = startBound.center;
            targetSize = startBound.size;
        }

        if (toNozzle.gameObject.name.Contains("Replace"))
        {
            Bounds endBounds = toNozzle.gameObject.GetComponent<MeshRenderer>().bounds;
            toTarget = endBounds.center;
            targetSize = endBounds.size;
        }
        else
        {
            Bounds endBounds = toNozzle.MyGeometryObject.MyMeshRender.bounds;
            toTarget = endBounds.center;
            targetSize = endBounds.size;
        }
            
    }
    public void PipePointSetting(string json)
    {
        int len = json.Length;

        if (len < 42)
        {
            Debug.Log("Json 데이터가 비었다.");
            SetPipeSide(fromTarget,toTarget,0);//여기
            return;
        }

        var resultDataSet = JsonConvert.DeserializeObject<ResultDataSet>(json);
        List<Vector3> usePos = new();

        foreach (var areaData in resultDataSet.AreaData)
        {
            int len1 = areaData.pos.GetLength(0);
            int len2 = areaData.pos.GetLength(1);

            for (int j = 0; j < areaData.pos.GetLength(0); j++)//좌표 수
            {
                Vector3 pos = new Vector3(areaData.pos[j, 0], areaData.pos[j, 1], areaData.pos[j, 1]);

                if (usePos.Count == 0)
                {
                    usePos.Add(pos);
                    firstPos = pos;
                    continue;
                }

                Vector3 last = usePos[^1];

                if (Vector3.Distance(pos, last) < 4) continue;
                usePos.Add(pos);
            }
        }
        if (usePos.Count <= 1)//경로 생성 불가
        {
            usePos.Clear();
            SetPipeSide(fromTarget, toTarget,0);//여기
            return;
        }
        PipeSetAIgorithm(usePos, fromTarget, toTarget);//여기
    }
    public void PipeSetAIgorithm(List<Vector3> usePos,Vector3 fromPos, Vector3 toPos)
    {
        //지점 설치
        pipePoint.Clear();
        for (int j = 0; j < usePos.Count; j++)//좌표 수
        {
            GameObject gm = Instantiate(cube);
            gm.transform.parent = this.transform;
            Vector3 localPos = usePos[j];
            gm.transform.position = localPos;

            gm.transform.position = LocalToWorldPosition(localPos);//여기에서 좌표 변환
            gm.transform.localScale = Vector3.one * rate;
            pipePoint.Add(gm);
        }

        //최소힙
        List<(float,Vector3)> fromDistList = new List<(float,  Vector3)>();
        List<(float, Vector3)> toDistList = new List<(float,  Vector3)>();
        fromDistList.Add((0,fromPos));
        toDistList.Add((0, toPos));
        for (int j = 0; j < pipePoint.Count; j++)
        {
            float fromDist = Mathf.Abs((fromPos - pipePoint[j].transform.position).magnitude);
            float toDist = Mathf.Abs((toPos - pipePoint[j].transform.position).magnitude);

            //둘중 더 가까운 쪽
            if(fromDist <= toDist) fromDistList.Add((fromDist, pipePoint[j].transform.position));
            else toDistList.Add((toDist, pipePoint[j].transform.position));
        }

        fromDistList.Sort();
        toDistList.Sort();

        //from
        while (fromDistList.Count > 1)
        {
            SetPipeSide(fromDistList[0].Item2, fromDistList[1].Item2, 1);//그리기
            (float, Vector3) standard = fromDistList[1];//기준
            fromDistList.RemoveAt(0);//삭제

            if (fromDistList.Count == 1) break;//그리기 종료

            List<(float, Vector3)> fromDistSubList = new List<(float, Vector3)>();
            for (int j = 0; j < fromDistList.Count; j++)//기준과의 거리
            {
                float fromDist = Mathf.Abs((standard.Item2 - fromDistList[j].Item2).magnitude);
                fromDistSubList.Add((fromDist, fromDistList[j].Item2));
            }
            fromDistSubList.Sort();

            fromDistList.Clear();
            fromDistList.Add(standard);
            foreach (var pi in fromDistSubList) fromDistList.Add(pi);
        }

        //to
        while (toDistList.Count > 1)
        {
            SetPipeSide(toDistList[0].Item2, toDistList[1].Item2, 1);//그리기
            (float, Vector3) standard = toDistList[1];//기준
            toDistList.RemoveAt(0);//삭제

            if (toDistList.Count == 1) break;//그리기 종료

            List<(float, Vector3)> toDistSubList = new List<(float, Vector3)>();
            for (int j = 0; j < toDistList.Count; j++)//기준과의 거리
            {
                float toDist = Mathf.Abs((standard.Item2 - toDistList[j].Item2).magnitude);
                toDistSubList.Add((toDist, toDistList[j].Item2));
            }
            toDistSubList.Sort();

            toDistList.Clear();
            toDistList.Add(standard);
            foreach (var pi in toDistSubList) toDistList.Add(pi);
        }
        SetPipeSide(fromDistList[0].Item2, toDistList[1].Item2, 1);//나머지 그리기
    }
    void SetPipeSide(Vector3 prevPos,Vector3 nextPos,int idx)
    {
        string objName = string.Empty;
        if (idx==0) objName = "AnswerPipeFromTO";
        else objName = "AnswerPipeFromTO "+idx.ToString();

        GameObject obj = new GameObject(objName);
        obj.transform.parent = this.transform;
        if (obj.GetComponent<Pipeline>() == null) obj.AddComponent<Pipeline>();

        float middleX = (nextPos.x + prevPos.x) / 2f;
        float middleY = (nextPos.y + prevPos.y) / 2f;
        float middleZ = (nextPos.z +prevPos.z) / 2f;
        Vector3 middle = new Vector3(middleX, middleY, middleZ);

        bool sameX = (nextPos.x == prevPos.x) ? true : false;
        bool sameY = (nextPos.y == prevPos.y) ? true : false;
        bool sameZ = (nextPos.z == prevPos.z) ? true : false;

        float diffX = nextPos.x - prevPos.x;
        float diffY = nextPos.y - prevPos.y;
        float diffZ = nextPos.z - prevPos.z;

        GameObject gm = Instantiate(cube);
        gm.transform.parent = obj.transform;

        //각 케이스별로
        if (!sameX && sameY && sameZ)
        {
            gm.transform.position = middle;
            gm.transform.localScale = new Vector3(diffX, rate, rate);
            pipeSidePoint.Add(gm);
        }
        else if (sameX && !sameY && sameZ)
        {
            gm.transform.position = middle;
            gm.transform.localScale = new Vector3(rate, diffY , rate);
            pipeSidePoint.Add(gm);
        }
        else if (sameX && sameY && !sameZ)
        {
            gm.transform.position = middle;
            gm.transform.localScale = new Vector3(rate, rate, diffZ );
            pipeSidePoint.Add(gm);
        }
        else if (!sameX && !sameY && sameZ)
        {
            gm.transform.position = new Vector3(middleX, prevPos.y, prevPos.z);
            gm.transform.localScale = new Vector3(diffX, rate, rate);

            GameObject gm2 = Instantiate(cube);
            gm2.transform.parent = obj.transform;
            gm2.transform.position = new Vector3(nextPos.x, middleY, nextPos.z);
            gm2.transform.localScale = new Vector3(rate, diffY, rate);

            pipeSidePoint.Add(gm);
            pipeSidePoint.Add(gm2);
        }
        else if (!sameX && sameY && !sameZ)
        {
            gm.transform.position = new Vector3(middleX, prevPos.y, prevPos.z);
            gm.transform.localScale = new Vector3(diffX, rate, rate);

            GameObject gm2 = Instantiate(cube);
            gm2.transform.parent = obj.transform;
            gm2.transform.position = new Vector3(nextPos.x, prevPos.y, middleZ);
            gm2.transform.localScale = new Vector3(rate, rate, diffZ);

            pipeSidePoint.Add(gm);
            pipeSidePoint.Add(gm2);
        }
        else if (sameX && !sameY && !sameZ)
        {
            gm.transform.position = new Vector3(prevPos.x, middleY, prevPos.z);
            gm.transform.localScale = new Vector3(rate, diffY, rate);

            GameObject gm2 = Instantiate(cube);
            gm2.transform.parent = obj.transform;
            gm2.transform.position = new Vector3(nextPos.x, prevPos.y, middleZ);
            gm2.transform.localScale = new Vector3(rate, rate, diffZ);

            pipeSidePoint.Add(gm);
            pipeSidePoint.Add(gm2);
        }
        else if (!sameX && !sameY && !sameZ)
        {
            gm.transform.position = new Vector3(middleX, prevPos.y, prevPos.z);
            gm.transform.localScale = new Vector3(diffX , rate, rate);

            GameObject gm2 = Instantiate(cube);
            gm2.transform.parent = obj.transform;
            gm2.transform.position = new Vector3(nextPos.x, middleY, prevPos.z);
            gm2.transform.localScale = new Vector3(rate, diffY ,rate);

            GameObject gm3 = Instantiate(cube);
            gm3.transform.parent = obj.transform;
            gm3.transform.position = new Vector3(nextPos.x, nextPos.y, middleZ);
            gm3.transform.localScale = new Vector3(rate, rate, diffZ);

            pipeSidePoint.Add(gm);
            pipeSidePoint.Add(gm2);
            pipeSidePoint.Add(gm3);
        }
    }
    void PipeOff()
    {
        foreach(var pi in pipePoint)
        {
            Destroy(pi);
        }
        foreach (var pi in pipeSidePoint)
        {
            Destroy(pi);
        }
        pipePoint.Clear();
        pipeSidePoint.Clear();
    }
    public void LoadData()
    {
        //string path = "Assets/Resources/Table/PipeData/PipeArea.json";
        string path = "Assets/Resources/Table/PipeData/processed_3d_points.json";
        string data = File.ReadAllText(path);
        //resultDataSet = JsonUtility.FromJson<ResultDataSet>(data);
        resultDataSet = JsonConvert.DeserializeObject<ResultDataSet>(data);
    }
    
    Vector3 LocalToWorldPosition(Vector3 pos)
    {
        Vector3 O = fromTarget;
        Vector3 MaxPos = toTarget;
        Vector3 diff = new Vector3(pos.x - firstPos.x, pos.z - firstPos.z, pos.y - firstPos.y);
        //Vector3 diff = new Vector3(pos.x- resultDataSet.AreaData[0].pos[0, 0], pos.z- resultDataSet.AreaData[0].pos[0, 2], pos.y- resultDataSet.AreaData[0].pos[0, 1]);
        Vector3 worldPos = O+diff*rate;
        return worldPos;
    }
    #region 안쓸거
    void PipeConnectedMainBus()
    {
        Vector3 lastPointPos = pipePoint[pipePoint.Count - 1].transform.position;
        //SetPipeSide(lastPointPos,toTarget);
    }
    public void PipeSet()
    {
        string filePath = "C:/Users/tybna/OneDrive/바탕 화면/오피스/vector.txt";
        StreamWriter sw;
        sw = new StreamWriter(filePath);

        //지점 설치
        pipePoint.Clear();
        for (int i = 0; i < 1; i++)//각 파이프 별로(총 몇줄인지)
        {
            for (int j = 0; j < len; j++)//좌표 수
            {
                GameObject gm = Instantiate(cube);
                gm.transform.parent = this.transform;
                Vector3 localPos = new Vector3(resultDataSet.AreaData[i].pos[j, 0], resultDataSet.AreaData[i].pos[j, 1], resultDataSet.AreaData[i].pos[j, 2]);
                gm.transform.position = localPos;

                gm.transform.position = LocalToWorldPosition(localPos);//여기에서 좌표 변환

                string posTxt = $"{gm.transform.position.x} {gm.transform.position.y} {gm.transform.position.z}";
                sw.WriteLine(posTxt);
                gm.transform.localScale = Vector3.one * rate;
                pipePoint.Add(gm);

            }
        }
        sw.Close();

        //사이에 설치
        for (int i = 0; i < 1; i++)//각 파이프 별로(총 몇줄인지)
        {
            string objName = "AnswerPipe" + i;
            GameObject obj = new GameObject(objName);
            obj.transform.parent = this.transform;
            if (obj.GetComponent<Pipeline>() == null) obj.AddComponent<Pipeline>();

            for (int j = 0; j < len; j++)
            {
                float middleX = (resultDataSet.AreaData[i].pos[j + 1, 0] + resultDataSet.AreaData[i].pos[j, 0]) / 2f;
                float middleY = (resultDataSet.AreaData[i].pos[j + 1, 1] + resultDataSet.AreaData[i].pos[j, 1]) / 2f;
                float middleZ = (resultDataSet.AreaData[i].pos[j + 1, 2] + resultDataSet.AreaData[i].pos[j, 2]) / 2f;
                Vector3 middle = new Vector3(middleX, middleY, middleZ);

                bool sameX = (resultDataSet.AreaData[i].pos[j + 1, 0] == resultDataSet.AreaData[i].pos[j, 0]) ? true : false;
                bool sameY = (resultDataSet.AreaData[i].pos[j + 1, 1] == resultDataSet.AreaData[i].pos[j, 1]) ? true : false;
                bool sameZ = (resultDataSet.AreaData[i].pos[j + 1, 2] == resultDataSet.AreaData[i].pos[j, 2]) ? true : false;

                float diffX = resultDataSet.AreaData[i].pos[j + 1, 0] - resultDataSet.AreaData[i].pos[j, 0];
                float diffY = resultDataSet.AreaData[i].pos[j + 1, 1] - resultDataSet.AreaData[i].pos[j, 1];
                float diffZ = resultDataSet.AreaData[i].pos[j + 1, 2] - resultDataSet.AreaData[i].pos[j, 2];

                GameObject gm = Instantiate(cube);
                gm.transform.parent = obj.transform;

                //각 케이스별로
                if (!sameX && sameY && sameZ)
                {
                    gm.transform.position = middle;
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);
                    pipePoint.Add(gm);
                    /*
                    if(gm.GetComponent<Pipe>()==null) gm.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    */
                }
                else if (sameX && !sameY && sameZ)
                {
                    gm.transform.position = middle;
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(rate, rate, diffY * rate);
                    pipePoint.Add(gm);
                    /*
                    if (gm.GetComponent<Pipe>() == null) gm.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    */
                }
                else if (sameX && sameY && !sameZ)
                {
                    gm.transform.position = middle;
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(rate, diffZ * rate, rate);
                    pipePoint.Add(gm);
                    /*
                    if (gm.GetComponent<Pipe>() == null) gm.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    */
                }
                else if (!sameX && !sameY && sameZ)
                {
                    gm.transform.position = new Vector3(middleX, resultDataSet.AreaData[i].pos[j, 1], resultDataSet.AreaData[i].pos[j, 2]);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(resultDataSet.AreaData[i].pos[j + 1, 0], middleY, resultDataSet.AreaData[i].pos[j, 2]);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, rate, diffY * rate);

                    pipePoint.Add(gm);
                    pipePoint.Add(gm2);
                    /*
                    if (gm.GetComponent<Pipe>() == null) gm.AddComponent<Pipe>();
                    if (gm2.GetComponent<Pipe>() == null) gm2.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    obj.GetComponent<Pipeline>().AddPipe(gm2.GetComponent<Pipe>());
                    */
                }
                else if (!sameX && sameY && !sameZ)
                {
                    gm.transform.position = new Vector3(middleX, resultDataSet.AreaData[i].pos[j, 1], resultDataSet.AreaData[i].pos[j, 2]);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(resultDataSet.AreaData[i].pos[j + 1, 0], resultDataSet.AreaData[i].pos[j, 1], middleZ);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, diffZ * rate, rate);

                    pipePoint.Add(gm);
                    pipePoint.Add(gm2);
                    /*
                    if (gm.GetComponent<Pipe>() == null) gm.AddComponent<Pipe>();
                    if (gm2.GetComponent<Pipe>() == null) gm2.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    obj.GetComponent<Pipeline>().AddPipe(gm2.GetComponent<Pipe>());
                    */
                }
                else if (sameX && !sameY && !sameZ)
                {
                    gm.transform.position = new Vector3(resultDataSet.AreaData[i].pos[j, 0], middleY, resultDataSet.AreaData[i].pos[j, 2]);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(rate, rate, diffY * rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(resultDataSet.AreaData[i].pos[j + 1, 0], resultDataSet.AreaData[i].pos[j, 1], middleZ);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, diffZ * rate, rate);

                    pipePoint.Add(gm);
                    pipePoint.Add(gm2);
                    /*
                    if (gm.GetComponent<Pipe>() == null) gm.AddComponent<Pipe>();
                    if (gm2.GetComponent<Pipe>() == null) gm2.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    obj.GetComponent<Pipeline>().AddPipe(gm2.GetComponent<Pipe>());
                    */
                }
                else if (!sameX && !sameY && !sameZ)
                {
                    gm.transform.position = new Vector3(middleX, resultDataSet.AreaData[i].pos[j, 1], resultDataSet.AreaData[i].pos[j, 2]);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(resultDataSet.AreaData[i].pos[j + 1, 0], middleY, resultDataSet.AreaData[i].pos[j, 2]);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, rate, diffY * rate);

                    GameObject gm3 = Instantiate(cube);
                    gm3.transform.parent = obj.transform;
                    gm3.transform.position = new Vector3(resultDataSet.AreaData[i].pos[j + 1, 0], resultDataSet.AreaData[i].pos[j + 1, 1], middleZ);
                    gm3.transform.position = LocalToWorldPosition(gm3.transform.position);
                    gm3.transform.localScale = new Vector3(rate, diffZ * rate, rate);

                    pipePoint.Add(gm);
                    pipePoint.Add(gm2);
                    pipePoint.Add(gm3);
                    /*
                    if (gm.GetComponent<Pipe>() == null) gm.AddComponent<Pipe>();
                    if (gm2.GetComponent<Pipe>() == null) gm2.AddComponent<Pipe>();
                    if (gm3.GetComponent<Pipe>() == null) gm3.AddComponent<Pipe>();
                    obj.GetComponent<Pipeline>().AddPipe(gm.GetComponent<Pipe>());
                    obj.GetComponent<Pipeline>().AddPipe(gm2.GetComponent<Pipe>());
                    obj.GetComponent<Pipeline>().AddPipe(gm3.GetComponent<Pipe>());
                    */
                }
            }
            //ObjectManager.Instance.PipelineList.Add(obj.GetComponent<Pipeline>());
        }
    }
    void PastPipeSetting()
    {
        pipeSidePoint.Clear();
        //사이에 설치
        for (int i = 0; i < 1; i++)//각 파이프 별로(총 몇줄인지)
        {
            string objName = "AnswerPipe" + i;
            GameObject obj = new GameObject(objName);
            obj.transform.parent = this.transform;
            if (obj.GetComponent<Pipeline>() == null) obj.AddComponent<Pipeline>();

            //LinkedList<JPSNode> nodes = new LinkedList<JPSNode>();
            for (int j = 0; j < usePos.Count - 1; j++)
            {
                float middleX = (usePos[j + 1].x + usePos[j].x) / 2f;
                float middleY = (usePos[j + 1].y + usePos[j].y) / 2f;
                float middleZ = (usePos[j + 1].z + usePos[j].z) / 2f;
                Vector3 middle = new Vector3(middleX, middleY, middleZ);

                bool sameX = (usePos[j + 1].x == usePos[j].x) ? true : false;
                bool sameY = (usePos[j + 1].y == usePos[j].y) ? true : false;
                bool sameZ = (usePos[j + 1].z == usePos[j].z) ? true : false;

                float diffX = usePos[j + 1].x - usePos[j].x;
                float diffY = usePos[j + 1].y - usePos[j].y;
                float diffZ = usePos[j + 1].z - usePos[j].z;
                /*
                Vector3 Dir = LocalToWorldPosition(usePos[j + 1]) - LocalToWorldPosition(usePos[j]);
                if (Dir.magnitude > 0.7f)
                {
                    //수직 방향 
                    if (Dir.z != 0 && (Dir.x == 0 && Dir.y == 0))
                    {
                        Debug.Log("수직");
                        LinkedList<JPSNode> path = VerticalGrid(Dir, usePos[j], usePos[j+1]);
                        if (j != 0)
                        {
                            path.RemoveFirst();
                        }
                        nodes.AddRange(path);
                    }
                    //수평방향
                    if (Dir.z == 0 && (Dir.x != 0 || Dir.y != 0))
                    {
                        Debug.Log("수평");
                        LinkedList<JPSNode> path = HorizonGrid(Dir, usePos[j], usePos[j+1]);
                        if (j != 0)
                        {
                            path.RemoveFirst();
                        }
                        nodes.AddRange(path);
                    }
                    //대각선
                    if (Dir.z != 0 && (Dir.x != 0 || Dir.y != 0))
                    {
                        Debug.Log("대각선");
                        Vector3 localMiddlePos = new Vector3(usePos[j].x, usePos[j].y, usePos[j + 1].z);
                        Vector3 worldMiddlePos = LocalToWorldPosition(localMiddlePos);
                        LinkedList<JPSNode> path = VerticalGrid((usePos[j+1] - worldMiddlePos).normalized, usePos[j], worldMiddlePos);
                        if (j != 0)
                        {
                            path.RemoveFirst();
                        }
                        nodes.AddRange(path);
                        path = HorizonGrid((worldMiddlePos - usePos[j]).normalized, worldMiddlePos, usePos[j+1]);
                        nodes.AddRange(path);
                    }
                    continue;
                }
                */
                GameObject gm = Instantiate(cube);
                gm.transform.parent = obj.transform;

                //각 케이스별로
                if (!sameX && sameY && sameZ)
                {
                    gm.transform.position = middle;
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);
                    pipeSidePoint.Add(gm);
                }
                else if (sameX && !sameY && sameZ)
                {
                    gm.transform.position = middle;
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(rate, rate, diffY * rate);
                    pipeSidePoint.Add(gm);
                }
                else if (sameX && sameY && !sameZ)
                {
                    gm.transform.position = middle;
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(rate, diffZ * rate, rate);
                    pipeSidePoint.Add(gm);
                }
                else if (!sameX && !sameY && sameZ)
                {
                    gm.transform.position = new Vector3(middleX, usePos[j].y, usePos[j].z);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(usePos[j + 1].x, middleY, usePos[j + 1].z);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, rate, diffY * rate);

                    pipeSidePoint.Add(gm);
                    pipeSidePoint.Add(gm2);
                }
                else if (!sameX && sameY && !sameZ)
                {
                    gm.transform.position = new Vector3(middleX, usePos[j].y, usePos[j].z);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(usePos[j + 1].x, usePos[j].y, middleZ);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, diffZ * rate, rate);

                    pipeSidePoint.Add(gm);
                    pipeSidePoint.Add(gm2);
                }
                else if (sameX && !sameY && !sameZ)
                {
                    gm.transform.position = new Vector3(usePos[j].x, middleY, usePos[j].z);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(rate, rate, diffY * rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(usePos[j + 1].x, usePos[j].y, middleZ);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, diffZ * rate, rate);

                    pipeSidePoint.Add(gm);
                    pipeSidePoint.Add(gm2);
                }
                else if (!sameX && !sameY && !sameZ)
                {
                    gm.transform.position = new Vector3(middleX, usePos[j].y, usePos[j].z);
                    gm.transform.position = LocalToWorldPosition(gm.transform.position);
                    gm.transform.localScale = new Vector3(diffX * rate, rate, rate);

                    GameObject gm2 = Instantiate(cube);
                    gm2.transform.parent = obj.transform;
                    gm2.transform.position = new Vector3(usePos[j + 1].x, middleY, usePos[j].z);
                    gm2.transform.position = LocalToWorldPosition(gm2.transform.position);
                    gm2.transform.localScale = new Vector3(rate, rate, diffY * rate);

                    GameObject gm3 = Instantiate(cube);
                    gm3.transform.parent = obj.transform;
                    gm3.transform.position = new Vector3(usePos[j + 1].x, usePos[j + 1].y, middleZ);
                    gm3.transform.position = LocalToWorldPosition(gm3.transform.position);
                    gm3.transform.localScale = new Vector3(rate, diffZ * rate, rate);

                    pipeSidePoint.Add(gm);
                    pipeSidePoint.Add(gm2);
                    pipeSidePoint.Add(gm3);
                }
            }
            //var paths = NozzleSelectedSystem.GetPathForPipeGenerating(startNozzle, nodes);
            //var pipeline = NozzleSelectedSystem.GeneratePipeRun(startNozzle, paths);
        }

    }
    #endregion
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColorPicker : MonoBehaviour
{
    [SerializeField]
    Image circlePalette;
    [SerializeField]
    Image picker;
    [SerializeField]
    Camera mainCam;
    public Color selectedColor;
    public GameObject linkedObject;

    private Vector2 sizeOfPalette;
    private CircleCollider2D paletteCollider;

    public Slider redSlider;
    public Slider greenSlider;
    public Slider blueSlider;
    public Slider alphaSlider;

    private static ColorPicker instance = null;
    public static ColorPicker Instance
    {
        get
        {
            if (null == instance) instance = FindObjectOfType<ColorPicker>();
            return instance;
        }
    }

    private void Awake()
    {
        if (null == instance) instance = this;

        if (redSlider == null || greenSlider == null ||
            blueSlider == null || alphaSlider == null) return;

        // 슬라이더 초기값 설정 (0~1 사이)
        InitializeSlider(redSlider);
        InitializeSlider(greenSlider);
        InitializeSlider(blueSlider);
        InitializeSlider(alphaSlider);

        // 이벤트 리스너 등록
        redSlider.onValueChanged.AddListener(UpdateBackgroundColor);
        greenSlider.onValueChanged.AddListener(UpdateBackgroundColor);
        blueSlider.onValueChanged.AddListener(UpdateBackgroundColor);
        alphaSlider.onValueChanged.AddListener(UpdateBackgroundColor);
    }

    void Start()
    {
        
        paletteCollider = circlePalette.GetComponent<CircleCollider2D>();

        sizeOfPalette = new Vector2(
            circlePalette.GetComponent<RectTransform>().rect.width,
            circlePalette.GetComponent<RectTransform>().rect.height);

    }

    public void mousePointerDown()
    {
        selectColor();
    }

    public void mouseDrag()
    {
        selectColor();
    }
    private void InitializeSlider(Slider slider)
    {
        slider.minValue = 0f;
        slider.maxValue = 255f;

        // 각 슬라이더의 초기값 설정
        if (slider == redSlider)
            slider.value = mainCam.backgroundColor.r * 255f;
        else if (slider == greenSlider)
            slider.value = mainCam.backgroundColor.g * 255f;
        else if (slider == blueSlider)
            slider.value = mainCam.backgroundColor.b * 255f;
        else if (slider == alphaSlider)
            slider.value = mainCam.backgroundColor.a * 255f;
    }

    private void UpdateBackgroundColor(float value)
    {
        // 각 슬라이더 값을 0-1 범위로 정규화하여 Color 생성
        Color newColor = new Color(
            redSlider.value / 255f,
        greenSlider.value / 255f,
            blueSlider.value / 255f,
            alphaSlider.value / 255f
        );
        mainCam.backgroundColor = newColor;
    }
    public Color getColor()
    {
        Vector2 circlePalettePosition = circlePalette.transform.position;
        Vector2 pickerPosition = picker.transform.position;

        Vector2 position = pickerPosition - circlePalettePosition + sizeOfPalette * 0.5f;
        Vector2 normalized = new Vector2(
            (position.x / (circlePalette.GetComponent<RectTransform>().rect.width)),
            (position.y / (circlePalette.GetComponent<RectTransform>().rect.height)));

        Texture2D texture = circlePalette.mainTexture as Texture2D;
        Color circularSelectedColor = texture.GetPixelBilinear(normalized.x, normalized.y);

        return circularSelectedColor;
    }

    private void selectColor()
    {
        Vector3 offset = Input.mousePosition - transform.position;
        Vector3 diff = Vector3.ClampMagnitude(offset, paletteCollider.radius);

        picker.transform.position = transform.position + diff;

        selectedColor = getColor();

        mainCam.backgroundColor = selectedColor;
        SettingColorSlider(selectedColor);
        //linkedObject.GetComponent<MeshRenderer>().materials[0].color = selectedColor;
    }
    /// <summary>
    /// 기능 : 슬라이더 바인딩
    /// </summary>
    /// <param name="selectedColor"></param>
    void SettingColorSlider(Color selectedColor)
    {
        redSlider.value = selectedColor.r * 255f;
        greenSlider.value = selectedColor.g * 255f;
        blueSlider.value = selectedColor.b * 255f;
        alphaSlider.value = selectedColor.a * 255f;
    }
}
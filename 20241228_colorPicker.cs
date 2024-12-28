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
    public Slider[] colorSlider = new Slider[3];


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
        colorSlider[0].value = selectedColor.r * 255f;
        colorSlider[1].value = selectedColor.g * 255f;
        colorSlider[2].value = selectedColor.b * 255f;
    }
}

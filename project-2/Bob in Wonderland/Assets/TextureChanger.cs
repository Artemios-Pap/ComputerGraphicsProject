using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TextureChanger : MonoBehaviour
{
    void OnTriggerEnter(Collider other)
    {
      
        Material material = other.GetComponent<Renderer>()?.material;

        
        if (material != null)
        {
            GetComponent<Renderer>().material = material;
        }
    }
}

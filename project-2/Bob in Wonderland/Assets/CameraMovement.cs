using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    public float moveSpeed = 15f;
    public float ySpeed = 10f;

    // Update is called once per frame
    void Update()
    {
        float horizontal = 0f;
        float vertical = 0f;

        if (Input.GetKey(KeyCode.UpArrow))
        {
            vertical = 1f;
        }
        else if (Input.GetKey(KeyCode.DownArrow))
        {
            vertical = -1f;
        }

        if (Input.GetKey(KeyCode.LeftArrow))
        {
            horizontal = -1f;
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            horizontal = 1f;
        }

        float yInput = 0f;
        if (Input.GetKey(KeyCode.Equals))
        {
            yInput = 1f;
        }
        else if (Input.GetKey(KeyCode.Minus))
        {
                yInput = -1f;   
        }

        Vector3 moveDirection = new Vector3(horizontal, yInput, vertical).normalized;
        transform.Translate(moveDirection * moveSpeed * Time.deltaTime);
        float newY = Mathf.Clamp(transform.position.y + yInput * ySpeed * Time.deltaTime, 0f, Mathf.Infinity);
        transform.position = new Vector3(transform.position.x, newY, transform.position.z);
    }
}
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BobMovement : MonoBehaviour
{
    public float movementSpeed = 50f;  // Movement speed
    public float rotationSpeed = 50f;  // Rotation speed
    public float jumpHeight = 15f;     // Jump Height
    public int speedMode = 3;
    private bool isFKeyPressed = false;
    private bool isRKeyPressed = false;


    // Update is called once per frame
    void Update()
    {

        //change the movement speed of Bob
        if(Input.GetKey("f") && speedMode<5 && !isFKeyPressed)
        {
            movementSpeed = movementSpeed + 20f;
            rotationSpeed = rotationSpeed + 20f;
            jumpHeight = jumpHeight + 3.33f;
            speedMode++;
            isFKeyPressed = true;
}
        if (Input.GetKey("r") && speedMode > 1 && !isRKeyPressed)
        {
            movementSpeed = movementSpeed - 20f;
            rotationSpeed = rotationSpeed - 20f;
            jumpHeight = jumpHeight - 3.33f;
            speedMode--;
            isRKeyPressed = true;
        }
        //making sure it changes once per key press
        if (!Input.GetKey("f"))
        {
            isFKeyPressed = false;
        }
        if (!Input.GetKey("r"))
        {
            isRKeyPressed = false;
        }
        //Move on x axis
        if (Input.GetKey("a"))
        {
            Vector3 move = new Vector3(1f, 0f, 0f);
            transform.Translate(move * movementSpeed * Time.deltaTime);
        }
        if (Input.GetKey("d"))
        {
            Vector3 move = new Vector3(-1f, 0f, 0f);
            transform.Translate(move * movementSpeed * Time.deltaTime);
        }
        //Move on z axis
        if (Input.GetKey("s"))
        {
            Vector3 rotate = new Vector3(0f, 0f, 1f);
            transform.Translate(rotate * rotationSpeed * Time.deltaTime);
        }
        if (Input.GetKey("x"))
        {
            Vector3 rotate = new Vector3(0f, 0f, -1f);
            transform.Translate(rotate * rotationSpeed * Time.deltaTime);
        }
        //Move on y axis 
        if (Input.GetKey("w"))
        {
            Vector3 jump = new Vector3(0f, 1f, 0f);
            transform.Translate(jump * jumpHeight * Time.deltaTime);
        }
        if (Input.GetKey("e"))
        {
            Vector3 jump = new Vector3(0f, -1f, 0f);
            transform.Translate(jump * jumpHeight * Time.deltaTime);
        }
        float halfFloorSizeXZ = 38.5f;
        float minY = 19.5f;
        float maxY = 30f;

        Vector3 clampedPosition = transform.position;
        clampedPosition.x = Mathf.Clamp(clampedPosition.x, -halfFloorSizeXZ, halfFloorSizeXZ);
        clampedPosition.z = Mathf.Clamp(clampedPosition.z, -halfFloorSizeXZ, halfFloorSizeXZ);
        clampedPosition.y = Mathf.Clamp(clampedPosition.y, minY, maxY);

        transform.position = clampedPosition;
    }
}

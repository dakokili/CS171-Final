#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "defines.h"
#include <vector>

class Camera{
  public:
    Camera::Camera(){
      Pos=glm::vec3(0.0f, 0.1f, 1.0f);
      Front=glm::vec3(0.0f, 0.0f, -1.0f);
      Right=glm::vec3(1.0f, 0.0f, 0.0f);
      Up=glm::vec3(0.0f, 1.0f, 0.0f);
      WorldUp=glm::vec3(0.0f,1.0f,0.0f);
      yaw=-90.0f;
      pitch=0.0f;
      fov=45.0f;
      sensitivity = 0.05f;
      cameraSpeed = 0.05f;
    };
    glm::vec3 Pos;
    glm::vec3 Front,Right,Up,WorldUp;
    float yaw,pitch,fov,sensitivity,cameraSpeed;
    void ProcessMouseMovement(float xoffset, float yoffset){
      xoffset *= sensitivity;
      yoffset *= sensitivity;
      yaw += xoffset;
      pitch += yoffset;
      if(pitch > 89.0f)
        pitch =  89.0f;
      if(pitch < -89.0f)
        pitch = -89.0f;
      updateCameraVectors();
    };
    void updateCameraVectors(){
      glm::vec3 front;
      front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
      front.y = sin(glm::radians(pitch));
      front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
      Front = glm::normalize(front);
      Right = glm::normalize(glm::cross(Front, WorldUp));
      Up = glm::normalize(glm::cross(Right, Front));
    };
    void ProcessMouseScroll(float yoffset){
      if(fov >= 2.0f && fov <= 90.0f)
        fov -= yoffset;
      if(fov <= 2.0f)
        fov = 2.0f;
      if(fov >= 90.0f)
        fov = 90.0f;
    };
    void ProcessKeyboard(int direction)
    {
        if (direction == 0)
            Pos += cameraSpeed * glm::normalize(vec3(Front.x,0,Front.z));
        if (direction == 1)
            Pos -= cameraSpeed * glm::normalize(vec3(Front.x,0,Front.z));
        if (direction == 2)
            Pos -= glm::normalize(vec3(Right.x,0,Right.z)) * cameraSpeed;
        if (direction == 3)
            Pos += glm::normalize(vec3(Right.x,0,Right.z)) * cameraSpeed;
        if (direction == 4)
            Pos += cameraSpeed * WorldUp;
        if (direction == 5)
            Pos -= cameraSpeed * WorldUp;
            if(Pos.y<0.1f) Pos.y=0.1f;
    };
    glm::mat4 getView(){
      return glm::lookAt(Pos, Pos + Front, Up);
    };
};

#endif
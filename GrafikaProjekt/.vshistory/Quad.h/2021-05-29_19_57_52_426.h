float verts[] = {
  1.0f,-1.0f,0.0f,1.0f, //A
 -1.0f, 1.0f,0.0f,1.0f, //B
 -1.0f,-1.0f,0.0f,1.0f, //C

  1.0f,-1.0f,0.0f,1.0f, //A
  1.0f, 1.0f,0.0f,1.0f, //D
 -1.0f, 1.0f,0.0f,1.0f }; //B
//Tablica wsp�rz�dnych teksturowania
float texCoords[] = {
  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //ABC
  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //ADB
};
//Tablica wektor�w normalnych
float normals[] = {
  0.0f,0.0f,1.0f,0.0f, //A
  0.0f,0.0f,1.0f,0.0f, //B
  0.0f,0.0f,1.0f,0.0f, //C

  0.0f,0.0f,1.0f,0.0f, //A
  0.0f,0.0f,1.0f,0.0f, //D
  0.0f,0.0f,1.0f,0.0f, //B
};
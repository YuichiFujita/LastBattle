xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 42;
 -0.06972;3.52230;-0.03984;,
 -0.06972;-0.59676;7.24062;,
 6.34230;-1.62996;-0.03984;,
 -0.06972;-5.16552;11.57124;,
 4.76172;-6.50604;6.97770;,
 -0.06972;-29.73504;6.80952;,
 6.14862;-23.21268;0.31212;,
 -0.06972;-17.53812;8.74728;,
 -0.06972;-10.90374;-9.09876;,
 8.09910;-13.62258;-0.03984;,
 -0.06972;-19.09332;-7.05186;,
 -0.06972;-1.62996;-7.55838;,
 6.34230;-1.62996;-0.03984;,
 -0.06972;3.52230;-0.03984;,
 -6.48168;-1.62996;-0.03984;,
 -4.90116;-6.50604;6.97770;,
 -8.23854;-13.62258;-0.03984;,
 -6.28800;-23.21268;0.31212;,
 8.09910;-13.62258;-0.03984;,
 -6.28800;-23.21268;0.31212;,
 -8.23854;-13.62258;-0.03984;,
 -6.48168;-1.62996;-0.03984;,
 -4.47600;-44.08506;0.46968;,
 -0.06972;-43.22646;4.28220;,
 4.33662;-44.14542;0.46968;,
 4.33662;-44.14542;0.46968;,
 -0.06972;-42.85158;-3.12798;,
 6.14862;-23.21268;0.31212;,
 2.65968;-48.55806;0.54900;,
 -0.07662;-51.75024;2.86296;,
 2.65968;-48.55806;0.54900;,
 -0.07662;-51.62196;-1.83972;,
 -4.47600;-44.08506;0.46968;,
 2.14410;-51.02658;0.45864;,
 2.14410;-51.02658;0.45864;,
 -2.56338;-50.94732;0.98352;,
 -2.99286;-48.76194;0.81144;,
 -2.99286;-48.76194;0.81144;,
 -2.56338;-50.94732;0.98352;,
 -0.07662;-55.06518;0.78312;,
 -0.07662;-55.06518;0.78312;,
 -0.07662;-55.06518;0.78312;;
 
 50;
 3;0,1,2;,
 3;1,3,4;,
 3;5,6,7;,
 3;8,9,10;,
 3;11,12,8;,
 3;11,13,12;,
 3;1,0,14;,
 3;7,15,16;,
 3;5,7,17;,
 3;6,18,7;,
 3;4,2,1;,
 3;19,20,10;,
 3;11,8,21;,
 3;13,11,21;,
 3;17,22,5;,
 3;23,24,5;,
 3;25,26,27;,
 3;10,26,19;,
 3;28,23,29;,
 3;28,24,23;,
 3;30,26,25;,
 3;30,31,26;,
 3;9,27,10;,
 3;12,9,8;,
 3;15,14,16;,
 3;7,16,17;,
 3;20,8,10;,
 3;8,20,21;,
 3;22,23,5;,
 3;24,6,5;,
 3;26,10,27;,
 3;26,32,19;,
 3;18,2,4;,
 3;18,4,7;,
 3;1,14,15;,
 3;3,7,4;,
 3;15,3,1;,
 3;15,7,3;,
 3;29,33,28;,
 3;30,34,31;,
 3;31,35,36;,
 3;37,38,29;,
 3;38,39,29;,
 3;31,40,35;,
 3;36,26,31;,
 3;36,32,26;,
 3;37,23,22;,
 3;37,29,23;,
 3;41,33,29;,
 3;40,31,34;;
 
 MeshMaterialList {
  1;
  50;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\MODEL\\TEXTURE\\dragonBoss000.png";
   }
  }
 }
 MeshNormals {
  54;
  0.625929;0.778972;-0.037617;,
  0.640274;0.589745;0.492189;,
  0.737177;-0.159515;0.656602;,
  0.718971;-0.101599;0.687575;,
  0.663164;-0.127034;-0.737615;,
  0.712831;-0.150825;-0.684926;,
  0.745664;0.015408;-0.666144;,
  0.690735;0.419372;-0.589078;,
  0.779660;0.587858;0.215762;,
  0.728196;-0.077373;-0.680987;,
  0.757786;-0.096865;0.645273;,
  0.667761;-0.141350;0.730832;,
  -0.757813;-0.096927;0.645233;,
  -0.848506;0.528496;0.027013;,
  -0.842605;-0.078484;0.532782;,
  0.794741;0.167102;0.583493;,
  -0.689247;-0.180322;-0.701728;,
  -0.673535;-0.116227;0.729960;,
  0.654581;-0.159412;0.738993;,
  -0.794742;0.167104;0.583491;,
  0.703631;0.186130;0.685754;,
  -0.749206;-0.308180;-0.586272;,
  0.738951;-0.219502;0.637001;,
  0.695129;-0.171950;0.698018;,
  0.725353;-0.223283;-0.651159;,
  0.746409;-0.270750;0.607920;,
  -0.630266;-0.412621;0.657654;,
  0.755434;0.108835;-0.646122;,
  -0.640276;0.589745;0.492187;,
  -0.625932;0.778970;-0.037617;,
  -0.737178;-0.159516;0.656600;,
  -0.719351;-0.101594;0.687178;,
  0.842604;-0.078484;0.532784;,
  -0.686594;-0.113764;-0.718085;,
  -0.728197;-0.077373;-0.680986;,
  -0.712832;-0.150826;-0.684924;,
  -0.690738;0.419370;-0.589075;,
  -0.745665;0.015409;-0.666143;,
  -0.756498;0.107161;-0.645157;,
  0.599479;-0.146412;-0.786885;,
  0.686455;-0.113695;-0.718229;,
  -0.664589;-0.128424;-0.736090;,
  0.676793;-0.154744;-0.719726;,
  -0.638577;-0.137067;0.757253;,
  -0.599742;-0.150072;-0.785995;,
  -0.703631;0.186130;0.685754;,
  0.725307;-0.283377;-0.627398;,
  -0.744332;-0.239955;-0.623211;,
  -0.633202;-0.109120;0.766256;,
  -0.633453;-0.239948;0.735637;,
  -0.640962;-0.201115;0.740757;,
  -0.729811;-0.414264;-0.543839;,
  0.736966;-0.359235;0.572565;,
  0.702020;-0.431539;-0.566517;;
  50;
  3;0,1,8;,
  3;1,20,15;,
  3;3,10,2;,
  3;6,9,5;,
  3;7,27,6;,
  3;7,0,8;,
  3;28,29,13;,
  3;30,19,14;,
  3;31,30,12;,
  3;10,32,2;,
  3;15,8,1;,
  3;33,34,35;,
  3;36,37,38;,
  3;29,36,13;,
  3;12,17,31;,
  3;18,11,3;,
  3;39,4,40;,
  3;35,41,33;,
  3;23,18,22;,
  3;23,11,18;,
  3;42,4,39;,
  3;42,24,4;,
  3;9,40,5;,
  3;27,9,6;,
  3;19,13,14;,
  3;30,14,12;,
  3;34,37,35;,
  3;37,34,13;,
  3;17,43,31;,
  3;11,10,3;,
  3;4,5,40;,
  3;41,44,33;,
  3;32,8,15;,
  3;32,15,2;,
  3;28,13,19;,
  3;20,2,15;,
  3;19,45,28;,
  3;19,30,45;,
  3;22,25,23;,
  3;42,46,24;,
  3;47,21,16;,
  3;48,49,50;,
  3;49,26,50;,
  3;47,51,21;,
  3;16,41,47;,
  3;16,44,41;,
  3;48,43,17;,
  3;48,50,43;,
  3;52,25,22;,
  3;53,24,46;;
 }
 MeshTextureCoords {
  42;
  0.071710;0.159280;,
  0.090130;0.159280;,
  0.094760;0.100950;,
  0.110570;0.159280;,
  0.116560;0.115330;,
  0.220440;0.159280;,
  0.191270;0.102720;,
  0.165900;0.159280;,
  0.105390;0.511530;,
  0.118060;0.434110;,
  0.143550;0.511530;,
  0.062180;0.511530;,
  0.062180;0.450760;,
  0.038170;0.511530;,
  0.094760;0.217600;,
  0.116560;0.203230;,
  0.148390;0.233580;,
  0.191270;0.215840;,
  0.148390;0.084970;,
  0.162750;0.570470;,
  0.118060;0.588960;,
  0.062180;0.572310;,
  0.284610;0.199360;,
  0.280770;0.159280;,
  0.284880;0.119200;,
  0.260300;0.469770;,
  0.254270;0.511530;,
  0.162750;0.452590;,
  0.297480;0.121850;,
  0.321610;0.158940;,
  0.273430;0.472530;,
  0.281800;0.511180;,
  0.260020;0.553300;,
  0.310890;0.115690;,
  0.287400;0.466120;,
  0.286850;0.557180;,
  0.274840;0.552370;,
  0.298840;0.198470;,
  0.310370;0.203090;,
  0.339990;0.203090;,
  0.303840;0.511810;,
  0.340080;0.115690;;
 }
}

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
 22;
 7.22202;-6.30054;27.24750;,
 -0.00192;-9.75450;25.58706;,
 -0.00192;-7.76604;4.58382;,
 7.22202;-6.30054;27.24750;,
 10.49958;-0.78588;29.89932;,
 7.22202;6.08202;29.20194;,
 8.47302;-0.78438;5.13072;,
 8.47302;-0.78438;5.13072;,
 0.00000;-0.78420;-6.30270;,
 -0.00192;7.69314;2.52414;,
 -0.00192;0.95166;29.79852;,
 -0.00192;-5.43768;27.14718;,
 -0.00192;6.94488;29.10162;,
 -0.00192;15.00072;32.85408;,
 -0.00192;-5.43768;27.14718;,
 0.00000;-0.78420;-6.30270;,
 -7.22202;-6.30054;27.24750;,
 -7.22202;6.08202;29.20194;,
 -10.49958;-0.78588;29.89932;,
 -7.22202;-6.30054;27.24750;,
 -8.47302;-0.78438;5.13072;,
 -8.47302;-0.78438;5.13072;;
 
 28;
 3;0,1,2;,
 3;3,4,5;,
 3;0,2,6;,
 3;7,8,9;,
 3;5,10,3;,
 3;3,7,4;,
 3;5,4,7;,
 3;10,11,3;,
 3;10,5,12;,
 3;5,9,13;,
 3;5,7,9;,
 3;14,1,0;,
 3;5,13,12;,
 3;15,6,2;,
 3;2,1,16;,
 3;17,18,19;,
 3;20,2,16;,
 3;9,8,21;,
 3;19,10,17;,
 3;18,21,19;,
 3;21,18,17;,
 3;19,11,10;,
 3;12,17,10;,
 3;13,9,17;,
 3;9,21,17;,
 3;16,1,14;,
 3;12,13,17;,
 3;2,20,15;;
 
 MeshMaterialList {
  1;
  28;
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
  32;
  0.717700;-0.658528;-0.226379;,
  -0.473010;-0.137364;0.870283;,
  -0.091299;-0.156881;0.983388;,
  0.671246;-0.733582;-0.106234;,
  0.566809;-0.791408;-0.228911;,
  0.724750;0.630553;-0.277742;,
  0.734336;0.659902;-0.158994;,
  -0.000106;-0.995548;-0.094252;,
  -0.000011;-0.144117;0.989561;,
  0.000015;-0.361676;0.932304;,
  0.000009;-0.159120;0.987259;,
  -0.717797;-0.658416;-0.226395;,
  0.682803;0.527015;-0.506000;,
  0.473010;-0.137364;0.870283;,
  0.091283;-0.156881;0.983390;,
  -0.671355;-0.733486;-0.106213;,
  -0.102715;-0.262449;0.959463;,
  0.791326;0.557087;-0.251908;,
  0.996093;-0.034014;-0.081502;,
  0.795071;0.592280;-0.130635;,
  0.000014;-0.339889;0.940466;,
  0.000017;-0.422245;0.906482;,
  0.588727;-0.680471;-0.436302;,
  -0.566982;-0.791279;-0.228928;,
  0.102706;-0.262450;0.959464;,
  -0.724911;0.630368;-0.277741;,
  -0.682914;0.526792;-0.506082;,
  -0.791415;0.556956;-0.251917;,
  -0.996093;-0.034014;-0.081502;,
  -0.795171;0.592154;-0.130600;,
  -0.734516;0.659713;-0.158949;,
  -0.588857;-0.680297;-0.436398;;
  28;
  3;3,7,4;,
  3;16,1,2;,
  3;3,4,0;,
  3;17,12,5;,
  3;2,8,16;,
  3;3,0,18;,
  3;19,18,17;,
  3;8,9,16;,
  3;8,2,10;,
  3;19,5,6;,
  3;19,17,5;,
  3;9,20,16;,
  3;2,21,10;,
  3;22,0,4;,
  3;23,7,15;,
  3;14,13,24;,
  3;11,23,15;,
  3;25,26,27;,
  3;24,8,14;,
  3;28,11,15;,
  3;27,28,29;,
  3;24,9,8;,
  3;10,14,8;,
  3;30,25,29;,
  3;25,27,29;,
  3;24,20,9;,
  3;10,21,14;,
  3;23,11,31;;
 }
 MeshTextureCoords {
  22;
  0.316840;0.066420;,
  0.277890;0.156090;,
  0.084190;0.156090;,
  0.236250;0.602520;,
  0.268880;0.643860;,
  0.288850;0.602520;,
  0.135260;0.050890;,
  0.141000;0.618300;,
  0.088090;0.603620;,
  0.156630;0.511390;,
  0.274320;0.511390;,
  0.238700;0.511390;,
  0.291290;0.511390;,
  0.338330;0.511390;,
  0.321500;0.156090;,
  0.034390;0.065330;,
  0.316840;0.066420;,
  0.288850;0.602520;,
  0.268880;0.643860;,
  0.236250;0.602520;,
  0.135260;0.050890;,
  0.141000;0.618300;;
 }
}

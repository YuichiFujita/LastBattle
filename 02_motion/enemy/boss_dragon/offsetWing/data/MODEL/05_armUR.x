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
 34;
 -4.45014;-0.05766;0.02538;,
 -2.58381;3.49764;0.02538;,
 -2.58381;-0.05766;-4.19907;,
 1.76025;4.96596;0.02538;,
 1.76025;-0.05766;-5.35653;,
 5.60421;3.26454;0.02538;,
 5.60421;-0.05766;-3.51039;,
 8.01207;3.61854;0.02538;,
 5.60421;-0.05766;-3.51039;,
 8.01207;-0.23085;-4.07151;,
 5.60421;-3.03021;0.02538;,
 1.76025;-4.29462;0.02538;,
 -2.58381;-3.57423;0.02538;,
 -2.58381;-0.05766;-4.19907;,
 -4.45014;-0.05766;0.02538;,
 -2.58381;-0.05766;4.24983;,
 1.76025;-0.05766;5.40726;,
 5.60421;-0.05766;3.56115;,
 8.01207;-3.67503;0.02538;,
 8.01207;-0.23085;4.12215;,
 8.01207;-0.23085;4.12215;,
 5.60421;-0.05766;3.56115;,
 -2.58381;-0.05766;4.24983;,
 16.24035;-0.22809;2.92836;,
 16.24035;2.49957;0.02538;,
 16.24035;-0.22809;-2.87769;,
 16.24035;-0.22809;-2.87769;,
 16.24035;-2.66868;0.02538;,
 16.24035;-0.22809;2.92836;,
 18.16380;-0.22809;0.02538;,
 18.16380;-0.22809;0.02538;,
 8.01207;-0.23085;-4.07151;,
 1.76025;-0.05766;-5.35653;,
 1.76025;-0.05766;5.40726;;
 
 40;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,3;,
 3;5,7,6;,
 3;8,9,10;,
 3;10,11,8;,
 3;12,13,11;,
 3;12,14,13;,
 3;1,0,15;,
 3;3,1,16;,
 3;5,3,17;,
 3;18,19,10;,
 3;20,7,17;,
 3;10,21,11;,
 3;12,11,22;,
 3;14,12,22;,
 3;20,23,7;,
 3;24,25,7;,
 3;9,26,18;,
 3;27,28,18;,
 3;25,24,29;,
 3;26,30,27;,
 3;24,23,29;,
 3;30,28,27;,
 3;4,2,1;,
 3;6,4,3;,
 3;7,31,6;,
 3;9,18,10;,
 3;11,32,8;,
 3;13,32,11;,
 3;1,15,16;,
 3;3,16,17;,
 3;19,21,10;,
 3;7,5,17;,
 3;21,33,11;,
 3;11,33,22;,
 3;23,24,7;,
 3;25,31,7;,
 3;26,27,18;,
 3;28,19,18;;
 
 MeshMaterialList {
  1;
  40;
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
  45;
  -0.999997;-0.002344;0.000000;,
  -0.611048;0.691609;-0.385094;,
  0.130638;0.724019;-0.677296;,
  0.102469;0.724938;-0.681150;,
  0.418755;-0.695090;0.584375;,
  -0.053692;-0.764337;-0.642578;,
  0.061291;-0.774960;-0.629031;,
  -0.710616;-0.655945;-0.254482;,
  -0.914708;-0.002144;-0.404111;,
  -0.033553;0.742379;-0.669140;,
  0.103623;0.725398;-0.680485;,
  -0.003979;0.728772;-0.684745;,
  0.322784;0.689770;-0.648095;,
  -0.103807;-0.761303;0.640033;,
  -0.103817;-0.761308;-0.640025;,
  -0.004843;-0.765428;0.643503;,
  -0.914708;-0.002144;0.404111;,
  -0.033553;0.742377;0.669141;,
  0.103629;0.725395;0.680488;,
  0.817244;0.576292;0.000007;,
  0.322781;0.689761;0.648106;,
  0.435259;0.656126;-0.616481;,
  0.112130;-0.767345;-0.631355;,
  -0.144564;-0.769030;-0.622651;,
  0.130636;0.724018;0.677297;,
  -0.204682;0.732591;0.649166;,
  0.102469;0.724938;0.681150;,
  -0.053681;-0.764334;0.642582;,
  -0.003967;0.728762;0.684756;,
  -0.003972;0.728764;0.684753;,
  0.112138;-0.767343;0.631355;,
  0.061291;-0.774960;0.629032;,
  -0.126430;-0.762394;0.634642;,
  -0.144562;-0.769030;0.622652;,
  0.309984;-0.727741;-0.611803;,
  -0.004843;-0.765438;-0.643492;,
  0.309982;-0.727732;0.611815;,
  0.785404;-0.618984;0.000006;,
  0.418758;-0.695098;-0.584364;,
  0.435255;0.656118;0.616493;,
  -0.169097;0.754083;-0.634637;,
  -0.003982;0.728772;-0.684745;,
  0.062674;-0.784183;-0.617356;,
  -0.169093;0.754084;0.634638;,
  0.062674;-0.784182;0.617358;;
  40;
  3;0,1,8;,
  3;2,9,1;,
  3;3,10,2;,
  3;3,11,10;,
  3;22,14,5;,
  3;5,6,22;,
  3;7,23,6;,
  3;7,0,8;,
  3;1,0,16;,
  3;24,25,17;,
  3;26,24,18;,
  3;15,13,27;,
  3;28,29,18;,
  3;27,30,31;,
  3;32,31,33;,
  3;0,7,16;,
  3;28,20,29;,
  3;21,12,11;,
  3;14,34,35;,
  3;4,36,15;,
  3;12,21,19;,
  3;34,37,38;,
  3;39,20,19;,
  3;37,36,4;,
  3;9,40,1;,
  3;10,9,2;,
  3;11,41,10;,
  3;14,35,5;,
  3;6,42,22;,
  3;23,42,6;,
  3;25,43,17;,
  3;24,17,18;,
  3;13,30,27;,
  3;29,26,18;,
  3;30,44,31;,
  3;31,44,33;,
  3;20,39,29;,
  3;12,41,11;,
  3;34,38,35;,
  3;36,13,15;;
 }
 MeshTextureCoords {
  34;
  0.030950;0.508530;,
  0.056130;0.508530;,
  0.056130;0.560980;,
  0.114730;0.508530;,
  0.114730;0.575350;,
  0.166580;0.508530;,
  0.166580;0.552430;,
  0.199060;0.508530;,
  0.167530;0.114460;,
  0.197860;0.107960;,
  0.167530;0.155460;,
  0.119110;0.155460;,
  0.064380;0.155460;,
  0.064380;0.106480;,
  0.040870;0.155460;,
  0.056130;0.456070;,
  0.114730;0.441700;,
  0.166580;0.464620;,
  0.197860;0.155460;,
  0.197860;0.202970;,
  0.199060;0.457660;,
  0.167530;0.196460;,
  0.064380;0.204450;,
  0.310060;0.472480;,
  0.310060;0.508530;,
  0.310060;0.544570;,
  0.301510;0.121800;,
  0.301510;0.155460;,
  0.301510;0.189120;,
  0.336010;0.508530;,
  0.325740;0.155460;,
  0.199060;0.559400;,
  0.119110;0.093060;,
  0.119110;0.217870;;
 }
}

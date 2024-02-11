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
 39;
 5.99220;-55.02360;124.94244;,
 3.05136;-57.50472;124.58520;,
 6.22848;-69.83604;135.55849;,
 2.78544;-57.90456;122.95921;,
 -5.56476;13.02120;76.02048;,
 -5.44596;0.65892;6.79944;,
 -0.74580;7.44912;69.69264;,
 4.81860;-30.59988;0.08856;,
 5.07288;3.27408;0.10524;,
 1.92948;-31.28112;9.17820;,
 0.95952;-45.39288;0.58596;,
 1.60164;-27.42456;-5.39328;,
 0.00000;0.00000;-8.36844;,
 -0.12828;8.62944;-0.05916;,
 -0.52044;22.13988;64.70232;,
 5.07288;3.27408;0.10524;,
 -5.56476;13.02120;76.02048;,
 -0.16044;-56.60088;128.85012;,
 -0.57960;10.47624;71.40780;,
 -0.74580;7.44912;69.69264;,
 -0.38508;-4.02108;13.93104;,
 -5.44596;0.65892;6.79944;,
 -1.33392;-32.17716;3.99624;,
 4.90908;15.70608;69.36840;,
 0.46344;-3.64812;13.12296;,
 4.90908;15.70608;69.36840;,
 0.01008;7.99500;69.38148;,
 0.00000;0.00000;-8.36844;,
 0.01008;7.99500;69.38148;,
 -0.38508;-4.02108;13.93104;,
 0.46344;-3.64812;13.12296;,
 -0.57960;10.47624;71.40780;,
 1.57188;-30.22344;58.06728;,
 -0.38508;-4.02108;13.93104;,
 1.92948;-31.28112;9.17820;,
 0.01008;7.99500;69.38148;,
 2.78544;-57.90456;122.95921;,
 0.46344;-3.64812;13.12296;,
 -0.74580;7.44912;69.69264;;
 
 42;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;7,8,9;,
 3;7,9,10;,
 3;7,10,11;,
 3;12,8,11;,
 3;13,14,15;,
 3;16,17,18;,
 3;1,17,2;,
 3;2,17,3;,
 3;3,17,19;,
 3;20,21,9;,
 3;9,22,10;,
 3;10,22,11;,
 3;22,21,11;,
 3;13,5,14;,
 3;0,3,23;,
 3;8,24,9;,
 3;8,7,11;,
 3;14,25,15;,
 3;17,16,19;,
 3;23,3,26;,
 3;21,22,9;,
 3;21,27,11;,
 3;5,4,14;,
 3;25,28,15;,
 3;5,29,6;,
 3;28,30,15;,
 3;23,31,0;,
 3;17,1,18;,
 3;31,1,0;,
 3;13,15,12;,
 3;5,13,27;,
 3;14,16,18;,
 3;25,14,31;,
 3;32,33,34;,
 3;35,36,32;,
 3;32,37,35;,
 3;37,32,34;,
 3;32,38,33;,
 3;32,36,38;;
 
 MeshMaterialList {
  2;
  42;
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
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\MODEL\\TEXTURE\\dragonBoss000.png";
   }
  }
  Material {
   0.558400;0.555200;0.624000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  42;
  0.766290;0.292819;-0.571889;,
  0.672999;0.642365;-0.366662;,
  0.802579;0.582902;0.126855;,
  -0.510050;0.504220;0.696858;,
  -0.505280;0.497363;0.705211;,
  0.830567;-0.324437;-0.452657;,
  -0.919131;-0.385149;-0.082815;,
  -0.954324;-0.248690;0.165588;,
  0.971074;-0.047381;0.234032;,
  0.951910;-0.254083;-0.171198;,
  0.838446;-0.078691;-0.539274;,
  -0.997627;-0.068819;-0.001847;,
  0.792153;-0.491089;-0.362388;,
  -0.436861;0.536679;0.721892;,
  0.984391;0.115086;-0.133155;,
  0.981026;-0.130842;-0.143071;,
  -0.958658;-0.258041;-0.119959;,
  0.735154;0.479651;0.479044;,
  -0.996341;-0.084173;0.014791;,
  -0.978986;-0.166404;0.117884;,
  0.955464;-0.283184;-0.083037;,
  0.990107;-0.084289;0.112175;,
  0.565620;0.469738;0.677806;,
  0.717262;-0.411056;-0.562644;,
  0.714431;-0.400039;-0.574070;,
  0.724207;0.398233;0.562970;,
  0.742527;0.491552;0.455006;,
  0.715265;0.558321;0.420326;,
  -0.799825;-0.529436;-0.282804;,
  -0.850054;-0.405567;-0.336041;,
  -0.920410;-0.320480;-0.223913;,
  -0.895473;-0.173482;0.409918;,
  -0.972527;-0.199659;0.119699;,
  -0.938481;-0.085549;-0.334567;,
  -0.888491;0.401400;-0.222398;,
  -0.888999;0.448802;-0.090875;,
  0.796252;0.595774;-0.105052;,
  -0.933856;0.110645;-0.340104;,
  -0.217448;0.544188;0.810294;,
  -0.401291;0.555176;0.728522;,
  -0.151686;0.493220;0.856578;,
  0.998997;0.044741;0.001542;;
  42;
  3;13,3,4;,
  3;23,24,5;,
  3;16,18,6;,
  3;15,14,8;,
  3;15,8,9;,
  3;15,9,10;,
  3;0,14,10;,
  3;1,2,14;,
  3;25,17,22;,
  3;26,17,27;,
  3;28,29,30;,
  3;30,29,6;,
  3;7,18,31;,
  3;31,19,32;,
  3;32,19,33;,
  3;19,18,33;,
  3;34,18,35;,
  3;23,5,12;,
  3;14,21,8;,
  3;14,15,10;,
  3;2,36,14;,
  3;29,16,6;,
  3;12,5,20;,
  3;18,19,31;,
  3;18,37,33;,
  3;18,16,35;,
  3;12,20,14;,
  3;18,7,6;,
  3;20,21,14;,
  3;38,39,13;,
  3;17,26,22;,
  3;39,3,13;,
  3;1,14,0;,
  3;18,34,37;,
  3;2,25,22;,
  3;38,40,22;,
  3;11,7,31;,
  3;20,5,41;,
  3;41,21,20;,
  3;21,41,8;,
  3;11,6,7;,
  3;11,30,6;;
 }
 MeshTextureCoords {
  39;
  0.297510;0.513220;,
  0.303870;0.494350;,
  0.345140;0.510740;,
  0.297870;0.528400;,
  0.351170;0.512220;,
  0.004440;0.498760;,
  0.330380;0.584670;,
  0.239680;0.511820;,
  0.076740;0.513090;,
  0.238850;0.447140;,
  0.371240;0.512180;,
  0.211710;0.564890;,
  0.085950;0.592200;,
  0.021830;0.434650;,
  0.311280;0.431510;,
  0.004440;0.498760;,
  0.094800;0.511730;,
  0.297510;0.513220;,
  0.112190;0.475950;,
  0.110490;0.539910;,
  0.123750;0.410410;,
  0.076740;0.513090;,
  0.239680;0.511820;,
  0.094800;0.511730;,
  0.119350;0.420020;,
  0.351170;0.512220;,
  0.108980;0.537200;,
  0.086580;0.597570;,
  0.332380;0.577720;,
  0.063240;0.588230;,
  0.057740;0.579860;,
  0.110300;0.479840;,
  0.495990;0.125540;,
  0.429240;0.121490;,
  0.429410;0.187990;,
  0.502370;0.024940;,
  0.590830;0.133800;,
  0.427780;0.122060;,
  0.503180;0.025660;;
 }
}

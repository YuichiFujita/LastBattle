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
 -9.39900;-0.05952;-0.07908;,
 -5.66640;7.05108;-0.07908;,
 -5.66640;-0.05952;-7.04658;,
 -5.60520;13.11984;-0.07908;,
 -4.99884;16.96476;-0.07908;,
 -3.31188;13.06398;-2.48862;,
 14.69472;6.34722;-0.07908;,
 9.96942;0.26952;-6.83610;,
 3.02172;9.98772;-0.07908;,
 1.05204;-8.53344;-0.07908;,
 3.02172;-0.05952;-8.95566;,
 6.98514;-6.61884;-0.07908;,
 -5.66640;-7.09266;-0.07908;,
 -5.66640;-0.05952;-7.04658;,
 -9.39900;-0.05952;-0.07908;,
 -5.66640;-0.05952;6.88830;,
 -1.44228;6.94848;4.35930;,
 3.02172;-0.05952;8.79744;,
 9.96942;0.26952;6.67800;,
 3.02172;-0.05952;-8.95566;,
 -1.44228;6.94848;-4.51734;,
 9.96942;0.26952;6.67800;,
 3.02172;-0.05952;8.79744;,
 -5.66640;-0.05952;6.88830;,
 24.30210;0.13986;4.70892;,
 24.30210;5.59536;-0.07908;,
 24.30210;0.13986;-4.86702;,
 24.30210;0.13986;-4.86702;,
 24.30210;-4.74114;-0.07908;,
 9.96942;0.26952;-6.83610;,
 28.14912;0.13986;-0.07908;,
 28.14912;0.13986;-0.07908;,
 24.30210;0.13986;4.70892;,
 -0.88848;14.71398;-0.07908;,
 -3.31188;13.06398;2.33034;,
 3.02172;9.98772;-0.07908;,
 -1.44228;6.94848;-4.51734;,
 -5.66640;7.05108;-0.07908;,
 -1.44228;6.94848;4.35930;;
 
 46;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,9;,
 3;12,14,13;,
 3;1,0,15;,
 3;8,16,17;,
 3;6,8,18;,
 3;7,19,8;,
 3;20,2,1;,
 3;21,22,11;,
 3;12,9,23;,
 3;14,12,23;,
 3;18,24,6;,
 3;25,26,6;,
 3;27,28,29;,
 3;11,28,21;,
 3;26,25,30;,
 3;27,31,28;,
 3;25,24,30;,
 3;31,32,28;,
 3;10,29,11;,
 3;13,10,9;,
 3;16,15,17;,
 3;8,17,18;,
 3;22,9,11;,
 3;9,22,23;,
 3;24,25,6;,
 3;26,7,6;,
 3;28,11,29;,
 3;28,32,21;,
 3;33,34,35;,
 3;19,2,20;,
 3;19,20,8;,
 3;36,5,35;,
 3;1,15,16;,
 3;3,5,37;,
 3;38,34,37;,
 3;4,33,5;,
 3;34,4,3;,
 3;34,33,4;,
 3;34,38,35;,
 3;5,33,35;,
 3;5,36,37;,
 3;34,3,37;;
 
 MeshMaterialList {
  1;
  46;
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
  51;
  -0.881477;-0.002095;-0.472221;,
  -0.943347;0.331809;0.000002;,
  0.337542;0.558855;-0.757461;,
  0.119705;0.673037;-0.729857;,
  0.290880;-0.675940;-0.677122;,
  0.181660;-0.726897;-0.662285;,
  -0.023040;-0.719719;-0.693883;,
  -0.511767;-0.604638;-0.610334;,
  -0.687544;0.286952;-0.667040;,
  0.105825;-0.731443;-0.673641;,
  0.166979;0.669293;-0.723993;,
  0.294126;0.647074;-0.703409;,
  0.166976;0.669294;0.723992;,
  -0.687546;0.286941;0.667042;,
  0.097015;0.590171;0.801428;,
  -0.173779;0.483118;-0.858136;,
  0.683133;0.481721;-0.548885;,
  0.294127;0.647078;0.703405;,
  0.391085;0.607090;-0.691733;,
  -0.173783;0.483120;0.858134;,
  -0.723207;0.042688;-0.689310;,
  -0.722339;0.060383;-0.688898;,
  0.411407;0.504361;0.759187;,
  0.438696;0.472182;0.764585;,
  -0.718383;0.113292;-0.686360;,
  -0.151519;-0.696678;-0.701201;,
  -0.881474;-0.002095;0.472228;,
  0.337539;0.558856;0.757461;,
  0.119705;0.673041;0.729854;,
  0.097014;0.590179;-0.801422;,
  0.133581;-0.716387;0.684796;,
  0.105820;-0.731442;0.673643;,
  0.181656;-0.726898;0.662285;,
  -0.511764;-0.604634;0.610340;,
  -0.023042;-0.719716;0.693887;,
  -0.151522;-0.696673;0.701205;,
  0.397646;-0.642524;-0.655012;,
  0.133585;-0.716384;-0.684799;,
  0.290881;-0.675944;0.677118;,
  0.664194;-0.523493;-0.533668;,
  0.391087;0.607093;0.691729;,
  0.683136;0.481722;0.548880;,
  0.664197;-0.523495;0.533663;,
  0.397647;-0.642527;0.655008;,
  0.438706;0.472183;-0.764578;,
  -0.723199;0.042694;0.689319;,
  0.325717;0.594822;-0.734912;,
  0.411419;0.504375;-0.759171;,
  -0.718367;0.113289;0.686378;,
  -0.722322;0.060382;0.688916;,
  0.325708;0.594806;0.734928;;
  46;
  3;0,1,8;,
  3;21,24,20;,
  3;3,10,2;,
  3;6,9,5;,
  3;7,25,6;,
  3;7,0,8;,
  3;1,26,13;,
  3;27,19,14;,
  3;28,27,12;,
  3;10,29,2;,
  3;15,8,1;,
  3;30,31,32;,
  3;33,34,35;,
  3;26,33,13;,
  3;12,17,28;,
  3;18,11,3;,
  3;36,4,37;,
  3;32,38,30;,
  3;11,18,16;,
  3;36,39,4;,
  3;40,17,41;,
  3;42,43,38;,
  3;9,37,5;,
  3;25,9,6;,
  3;19,13,14;,
  3;27,14,12;,
  3;31,34,32;,
  3;34,31,35;,
  3;17,40,28;,
  3;11,10,3;,
  3;4,5,37;,
  3;38,43,30;,
  3;22,23,27;,
  3;29,8,15;,
  3;29,15,2;,
  3;15,44,2;,
  3;1,13,19;,
  3;21,20,1;,
  3;19,45,1;,
  3;46,47,44;,
  3;45,48,49;,
  3;23,22,50;,
  3;23,19,27;,
  3;44,47,2;,
  3;20,15,1;,
  3;45,49,1;;
 }
 MeshTextureCoords {
  39;
  0.012150;0.511770;,
  0.048400;0.511770;,
  0.048400;0.574060;,
  0.030910;0.775110;,
  0.038140;0.713190;,
  0.058280;0.776010;,
  0.246140;0.511770;,
  0.200250;0.572180;,
  0.132780;0.511770;,
  0.115160;0.155400;,
  0.130610;0.091280;,
  0.161720;0.155400;,
  0.062440;0.155400;,
  0.062440;0.105070;,
  0.033150;0.155400;,
  0.048400;0.449490;,
  0.089430;0.472100;,
  0.132780;0.432420;,
  0.200250;0.451370;,
  0.132780;0.591130;,
  0.089430;0.551450;,
  0.185140;0.204210;,
  0.130610;0.219520;,
  0.062440;0.205730;,
  0.339450;0.468970;,
  0.339450;0.511770;,
  0.339450;0.554570;,
  0.297610;0.120810;,
  0.297610;0.155400;,
  0.185140;0.106590;,
  0.376810;0.511770;,
  0.327790;0.155400;,
  0.297610;0.189990;,
  0.087210;0.749440;,
  0.058280;0.776010;,
  0.133900;0.825550;,
  0.080600;0.874510;,
  0.030170;0.872850;,
  0.080600;0.874510;;
 }
}

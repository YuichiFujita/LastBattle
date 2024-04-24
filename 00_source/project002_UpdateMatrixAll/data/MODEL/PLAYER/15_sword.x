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
 274;
 -1.03158;0.01449;8.06550;,
 -0.70752;0.79680;8.06550;,
 -0.70752;0.79680;-6.49503;,
 -1.03158;0.01449;-6.49503;,
 -0.70752;-0.76782;-6.49503;,
 -0.70752;-0.76782;8.06550;,
 0.07479;1.12086;8.06550;,
 0.07479;1.12086;-6.49503;,
 -0.70752;0.79680;-6.49503;,
 0.07479;-1.09188;-6.49503;,
 0.07479;-1.09188;8.06550;,
 -0.70752;-0.76782;-6.49503;,
 0.85710;0.79680;8.06550;,
 0.85710;0.79680;-6.49503;,
 0.07479;1.12086;-6.49503;,
 0.85710;-0.76782;-6.49503;,
 0.85710;-0.76782;8.06550;,
 0.07479;-1.09188;-6.49503;,
 1.18116;0.01449;8.06550;,
 1.18116;0.01449;-6.49503;,
 0.85710;0.79680;-6.49503;,
 0.85710;-0.76782;-6.49503;,
 1.72296;1.12590;-6.70389;,
 2.40561;0.01449;-6.70389;,
 5.48052;0.01449;-7.10550;,
 3.89721;1.45260;-7.10550;,
 -1.57338;1.12590;-6.70389;,
 0.07479;1.58628;-6.70389;,
 0.07479;2.04828;-7.10550;,
 -3.74760;1.45260;-7.10550;,
 3.89721;1.45260;-7.10550;,
 0.07479;2.04828;-7.10550;,
 -2.25603;0.01449;-6.70389;,
 -3.74760;1.45260;-7.10550;,
 -5.33094;0.01449;-7.10550;,
 1.39566;1.33539;10.00572;,
 1.94280;0.01449;10.00572;,
 1.94280;0.01449;8.39859;,
 1.39566;1.33539;8.39859;,
 -1.24608;1.33539;10.00572;,
 0.07479;1.88247;10.00572;,
 0.07479;1.88247;8.39859;,
 -1.24608;1.33539;8.39859;,
 1.39566;1.33539;8.39859;,
 0.07479;1.88247;8.39859;,
 -1.79319;0.01449;10.00572;,
 -1.24608;1.33539;8.39859;,
 -1.79319;0.01449;8.39859;,
 -0.99117;1.08045;8.06550;,
 0.07479;1.52199;8.06550;,
 -0.70752;0.79680;8.06550;,
 1.14075;1.08045;8.06550;,
 0.07479;1.12086;8.06550;,
 -1.43268;0.01449;8.06550;,
 -1.03158;0.01449;8.06550;,
 1.58229;0.01449;8.06550;,
 0.85710;0.79680;8.06550;,
 0.07479;1.47150;11.91495;,
 1.10508;1.04475;11.91495;,
 1.10508;1.04475;10.33875;,
 0.07479;1.47150;10.33875;,
 -1.38222;0.01449;11.91495;,
 -0.95547;1.04475;11.91495;,
 -0.95547;1.04475;10.33875;,
 -1.38222;0.01449;10.33875;,
 1.53183;0.01449;11.91495;,
 1.53183;0.01449;10.33875;,
 1.10508;1.04475;10.33875;,
 0.07479;1.47150;10.33875;,
 -0.95547;1.04475;10.33875;,
 0.07479;0.01449;11.91495;,
 0.07479;1.47150;11.91495;,
 -1.38222;0.01449;11.91495;,
 1.10508;1.04475;11.91495;,
 -0.95547;1.04475;11.91495;,
 -1.89552;0.01449;-6.37080;,
 -1.31847;0.95400;-6.37080;,
 -1.31847;0.95400;-6.37080;,
 0.07479;1.34316;-6.37080;,
 0.07479;1.34316;-6.37080;,
 1.46805;0.95400;-6.37080;,
 1.46805;0.95400;-6.37080;,
 2.04510;0.01449;-6.37080;,
 -1.79319;0.01449;8.39859;,
 1.94280;0.01449;8.39859;,
 -1.38222;0.01449;10.33875;,
 -1.24608;1.33539;10.00572;,
 0.07479;1.88247;10.00572;,
 1.39566;1.33539;10.00572;,
 1.94280;0.01449;10.00572;,
 -2.69073;1.05498;-8.28396;,
 -3.83625;0.01449;-8.28396;,
 -5.33094;0.01449;-7.10550;,
 0.07479;1.48593;-8.28396;,
 3.98589;0.01449;-8.28396;,
 2.84031;1.05498;-8.28396;,
 5.48052;0.01449;-7.10550;,
 0.07479;0.88641;-8.28396;,
 -1.56387;0.63105;-8.28396;,
 0.07479;1.48593;-8.28396;,
 2.39223;0.01449;-8.28396;,
 1.71351;0.63105;-8.28396;,
 3.98589;0.01449;-8.28396;,
 -2.24268;0.01449;-8.28396;,
 -2.69073;1.05498;-8.28396;,
 2.84031;1.05498;-8.28396;,
 0.07479;0.01449;-8.28396;,
 0.07479;0.88641;-8.28396;,
 2.39223;0.01449;-8.28396;,
 -1.56387;0.63105;-8.28396;,
 1.71351;0.63105;-8.28396;,
 1.72296;-1.09692;-6.70389;,
 3.89721;-1.42359;-7.10550;,
 0.07479;-2.01927;-7.10550;,
 0.07479;-1.55727;-6.70389;,
 -1.57338;-1.09692;-6.70389;,
 -3.74760;-1.42362;-7.10550;,
 3.89721;-1.42359;-7.10550;,
 0.07479;-2.01927;-7.10550;,
 -3.74760;-1.42362;-7.10550;,
 1.39566;-1.30641;10.00572;,
 1.39566;-1.30641;8.39859;,
 0.07479;-1.85349;8.39859;,
 0.07479;-1.85349;10.00572;,
 -1.24608;-1.30641;10.00572;,
 -1.24608;-1.30641;8.39859;,
 1.39566;-1.30641;8.39859;,
 0.07479;-1.85349;8.39859;,
 -1.24608;-1.30641;8.39859;,
 0.07479;-1.49298;8.06550;,
 -0.99117;-1.05147;8.06550;,
 -0.70752;-0.76782;8.06550;,
 1.14075;-1.05147;8.06550;,
 0.07479;-1.09188;8.06550;,
 0.85710;-0.76782;8.06550;,
 1.10508;-1.01580;10.33875;,
 1.10508;-1.01580;11.91495;,
 0.07479;-1.44252;11.91495;,
 0.07479;-1.44252;10.33875;,
 -0.95547;-1.01580;10.33875;,
 -0.95547;-1.01580;11.91495;,
 1.10508;-1.01580;10.33875;,
 0.07479;-1.44252;10.33875;,
 -0.95547;-1.01580;10.33875;,
 0.07479;-1.44252;11.91495;,
 1.10508;-1.01580;11.91495;,
 -0.95547;-1.01580;11.91495;,
 -1.31847;-0.92502;-6.37080;,
 -1.31847;-0.92502;-6.37080;,
 0.07479;-1.31418;-6.37080;,
 0.07479;-1.31418;-6.37080;,
 1.46805;-0.92502;-6.37080;,
 1.46805;-0.92502;-6.37080;,
 -1.24608;-1.30641;10.00572;,
 0.07479;-1.85349;10.00572;,
 1.39566;-1.30641;10.00572;,
 -2.69073;-1.02597;-8.28396;,
 0.07479;-1.45698;-8.28396;,
 2.84031;-1.02597;-8.28396;,
 -1.56387;-0.60201;-8.28396;,
 0.07479;-0.85737;-8.28396;,
 0.07479;-1.45698;-8.28396;,
 1.71351;-0.60201;-8.28396;,
 -2.69073;-1.02597;-8.28396;,
 2.84031;-1.02597;-8.28396;,
 0.07479;-0.85737;-8.28396;,
 -1.56387;-0.60201;-8.28396;,
 1.71351;-0.60201;-8.28396;,
 0.07479;0.01449;-6.37080;,
 0.07479;0.01449;-6.37080;,
 2.04510;0.01449;-6.37080;,
 0.88911;0.66360;-62.29374;,
 -0.75423;0.66360;-62.32410;,
 -1.55607;0.66360;-61.32333;,
 1.80543;0.66360;-61.29609;,
 -0.87012;0.66360;-49.77228;,
 1.12071;0.66360;-49.77228;,
 1.80543;0.66360;-61.29609;,
 -1.55607;0.66360;-61.32333;,
 1.44261;0.66360;-63.43548;,
 -1.29303;0.66360;-63.43548;,
 0.88911;0.66360;-62.29374;,
 0.07479;0.66360;-66.42213;,
 -1.29303;0.66360;-63.43548;,
 -0.87012;0.66360;-23.65983;,
 1.12071;0.66360;-23.65983;,
 -0.87012;0.66360;-49.77228;,
 -1.63278;0.66360;-17.27946;,
 1.88343;0.66360;-17.27946;,
 -0.87012;0.66360;-23.65983;,
 -0.51804;0.66360;-15.42477;,
 0.76866;0.66360;-15.42477;,
 -1.63278;0.66360;-17.27946;,
 -2.02452;0.66360;-14.06004;,
 2.27514;0.66360;-14.06004;,
 -0.51804;0.66360;-15.42477;,
 -0.71892;0.66360;-11.85015;,
 0.96957;0.66360;-11.85015;,
 -2.02452;0.66360;-14.06004;,
 -0.71892;0.37665;-8.08329;,
 0.96957;0.37665;-8.08329;,
 -0.71892;0.66360;-11.85015;,
 1.82052;0.01449;-49.76343;,
 2.51772;0.01449;-61.55505;,
 1.12071;0.66360;-49.77228;,
 -2.26704;0.01449;-61.55505;,
 -1.56987;0.01449;-49.76343;,
 1.72632;0.01449;-62.41668;,
 2.21616;0.01449;-63.42702;,
 0.07479;0.01449;-68.10264;,
 1.44261;0.66360;-63.43548;,
 -1.57674;0.01449;-62.41668;,
 -0.75423;0.66360;-62.32410;,
 -2.06658;0.01449;-63.42702;,
 0.07479;0.66360;-66.42213;,
 -1.56987;0.01449;-23.74350;,
 1.82052;0.01449;-23.74350;,
 1.12071;0.66360;-23.65983;,
 -2.42145;0.01449;-17.23329;,
 2.67210;0.01449;-17.23329;,
 1.88343;0.66360;-17.27946;,
 -1.41408;0.01449;-15.55728;,
 1.66470;0.01449;-15.55728;,
 0.76866;0.66360;-15.42477;,
 -2.91726;0.01449;-14.19546;,
 3.16791;0.01449;-14.19546;,
 2.27514;0.66360;-14.06004;,
 -1.41867;0.01449;-11.65890;,
 1.66932;0.01449;-11.65890;,
 0.96957;0.66360;-11.85015;,
 -1.41867;0.01449;-7.38354;,
 1.66932;0.01449;-7.38354;,
 0.96957;0.37665;-8.08329;,
 -1.55607;-0.63462;-61.32333;,
 -0.75423;-0.63462;-62.32410;,
 0.88911;-0.63462;-62.29374;,
 1.80543;-0.63462;-61.29609;,
 1.80543;-0.63462;-61.29609;,
 1.12071;-0.63462;-49.77228;,
 -0.87012;-0.63462;-49.77228;,
 -1.55607;-0.63462;-61.32333;,
 -1.29303;-0.63462;-63.43548;,
 1.44261;-0.63462;-63.43548;,
 0.88911;-0.63462;-62.29374;,
 0.07479;-0.63462;-66.42213;,
 -1.29303;-0.63462;-63.43548;,
 1.12071;-0.63462;-23.65983;,
 -0.87012;-0.63462;-23.65983;,
 -0.87012;-0.63462;-49.77228;,
 1.88343;-0.63462;-17.27946;,
 -1.63278;-0.63462;-17.27946;,
 -0.87012;-0.63462;-23.65983;,
 0.76866;-0.63462;-15.42477;,
 -0.51804;-0.63462;-15.42477;,
 -1.63278;-0.63462;-17.27946;,
 2.27514;-0.63462;-14.06004;,
 -2.02452;-0.63462;-14.06004;,
 -0.51804;-0.63462;-15.42477;,
 0.96957;-0.63462;-11.85015;,
 -0.71892;-0.63462;-11.85015;,
 -2.02452;-0.63462;-14.06004;,
 0.96957;-0.34767;-8.08329;,
 -0.71892;-0.34767;-8.08329;,
 -0.71892;-0.63462;-11.85015;,
 1.12071;-0.63462;-49.77228;,
 1.44261;-0.63462;-63.43548;,
 -0.75423;-0.63462;-62.32410;,
 0.07479;-0.63462;-66.42213;,
 1.12071;-0.63462;-23.65983;,
 1.88343;-0.63462;-17.27946;,
 0.76866;-0.63462;-15.42477;,
 2.27514;-0.63462;-14.06004;,
 0.96957;-0.63462;-11.85015;,
 0.96957;-0.34767;-8.08329;;
 
 164;
 4;0,1,2,3;,
 4;4,5,0,3;,
 4;1,6,7,8;,
 4;9,10,5,11;,
 4;6,12,13,14;,
 4;15,16,10,17;,
 4;12,18,19,20;,
 4;19,18,16,21;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;27,22,30,31;,
 4;32,26,33,34;,
 4;35,36,37,38;,
 4;39,40,41,42;,
 4;40,35,43,44;,
 4;45,39,46,47;,
 4;48,49,6,50;,
 4;49,51,12,52;,
 4;53,48,1,54;,
 4;51,55,18,56;,
 4;57,58,59,60;,
 4;61,62,63,64;,
 4;58,65,66,67;,
 4;62,57,68,69;,
 3;70,58,71;,
 3;70,62,72;,
 3;70,65,73;,
 3;70,57,74;,
 4;26,32,75,76;,
 4;27,26,77,78;,
 4;22,27,79,80;,
 4;23,22,81,82;,
 4;48,53,83,42;,
 4;49,48,46,44;,
 4;51,49,41,38;,
 4;55,51,43,84;,
 4;45,85,63,86;,
 4;39,63,68,87;,
 4;40,68,59,88;,
 4;35,59,66,89;,
 4;90,91,92,29;,
 4;93,90,33,31;,
 4;94,95,30,96;,
 4;95,93,28,25;,
 4;97,98,90,99;,
 4;100,101,95,102;,
 4;98,103,91,104;,
 4;101,97,93,105;,
 3;106,98,107;,
 3;106,101,108;,
 3;106,103,109;,
 3;106,97,110;,
 4;24,23,111,112;,
 4;113,114,115,116;,
 4;117,111,114,118;,
 4;119,115,32,34;,
 4;37,36,120,121;,
 4;122,123,124,125;,
 4;126,120,123,127;,
 4;128,124,45,47;,
 4;10,129,130,131;,
 4;16,132,129,133;,
 4;5,130,53,54;,
 4;18,55,132,134;,
 4;135,136,137,138;,
 4;139,140,61,64;,
 4;66,65,136,141;,
 4;142,137,140,143;,
 3;136,70,144;,
 3;140,70,72;,
 3;65,70,145;,
 3;137,70,146;,
 4;75,32,115,147;,
 4;148,115,114,149;,
 4;150,114,111,151;,
 4;152,111,23,82;,
 4;83,53,130,125;,
 4;128,130,129,127;,
 4;122,129,132,121;,
 4;126,132,55,84;,
 4;139,85,45,153;,
 4;142,139,124,154;,
 4;135,142,123,155;,
 4;66,135,120,89;,
 4;92,91,156,116;,
 4;119,156,157,118;,
 4;117,158,94,96;,
 4;113,157,158,112;,
 4;156,159,160,161;,
 4;158,162,100,102;,
 4;91,103,159,163;,
 4;157,160,162,164;,
 3;159,106,165;,
 3;162,106,108;,
 3;103,106,166;,
 3;160,106,167;,
 3;77,168,78;,
 3;168,81,78;,
 3;148,150,169;,
 3;150,152,169;,
 3;152,170,169;,
 3;170,81,169;,
 3;77,75,169;,
 3;75,148,169;,
 4;171,172,173,174;,
 4;175,176,177,178;,
 4;179,180,172,181;,
 3;179,182,183;,
 4;184,185,176,186;,
 4;187,188,185,189;,
 4;190,191,188,192;,
 4;193,194,191,195;,
 4;196,197,194,198;,
 4;199,200,197,201;,
 4;202,203,177,204;,
 4;205,206,175,178;,
 4;207,208,179,181;,
 4;208,209,182,210;,
 4;211,205,173,212;,
 4;203,207,171,174;,
 4;209,213,180,214;,
 4;213,211,172,183;,
 4;206,215,184,186;,
 4;216,202,176,217;,
 4;215,218,187,189;,
 4;219,216,185,220;,
 4;218,221,190,192;,
 4;222,219,188,223;,
 4;221,224,193,195;,
 4;225,222,191,226;,
 4;224,227,196,198;,
 4;228,225,194,229;,
 4;227,230,199,201;,
 4;231,228,197,232;,
 4;233,234,235,236;,
 4;237,238,239,240;,
 4;234,241,242,243;,
 3;244,242,245;,
 4;238,246,247,248;,
 4;246,249,250,251;,
 4;249,252,253,254;,
 4;252,255,256,257;,
 4;255,258,259,260;,
 4;258,261,262,263;,
 4;237,203,202,264;,
 4;239,206,205,240;,
 4;242,208,207,243;,
 4;244,209,208,265;,
 4;233,205,211,266;,
 4;235,207,203,236;,
 4;241,213,209,267;,
 4;234,211,213,245;,
 4;247,215,206,248;,
 4;238,202,216,268;,
 4;250,218,215,251;,
 4;246,216,219,269;,
 4;253,221,218,254;,
 4;249,219,222,270;,
 4;256,224,221,257;,
 4;252,222,225,271;,
 4;259,227,224,260;,
 4;255,225,228,272;,
 4;262,230,227,263;,
 4;258,228,231,273;;
 
 MeshMaterialList {
  3;
  164;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.181600;0.150400;0.216800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.097600;0.166400;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.728000;0.728000;0.728000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  175;
  -1.000000;0.000000;0.000000;,
  -0.707107;0.707107;0.000000;,
  -0.707107;-0.707107;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.707107;0.707107;0.000000;,
  0.707107;-0.707107;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.300434;0.501561;0.811280;,
  0.422448;-0.000001;0.906387;,
  0.148195;-0.000001;0.988958;,
  0.143471;0.363632;0.920428;,
  -0.300435;0.501562;0.811280;,
  -0.000000;0.705571;0.708640;,
  -0.000000;0.588086;0.808798;,
  -0.143471;0.363633;0.920428;,
  -0.422449;0.000000;0.906387;,
  -0.148196;0.000000;0.988958;,
  0.634872;0.634887;0.440291;,
  0.897859;0.000000;0.440284;,
  0.912494;0.000000;-0.409090;,
  0.645223;0.645239;-0.409090;,
  -0.634874;0.634879;0.440298;,
  -0.000000;0.897855;0.440292;,
  0.000000;0.912497;-0.409084;,
  -0.645229;0.645232;-0.409091;,
  -0.897856;0.000000;0.440290;,
  -0.912495;0.000000;-0.409087;,
  -0.252735;0.252734;-0.933944;,
  0.000000;0.357425;-0.933942;,
  0.252733;0.252738;-0.933943;,
  -0.357424;0.000000;-0.933942;,
  0.357421;0.000000;-0.933943;,
  -0.000005;1.000000;0.000000;,
  0.707103;0.707110;0.000000;,
  0.634876;0.634883;0.440290;,
  -0.000003;0.897853;0.440294;,
  -1.000000;0.000005;0.000000;,
  -0.634877;0.634878;0.440296;,
  -0.897855;0.000003;0.440291;,
  1.000000;0.000005;0.000000;,
  0.897859;0.000003;0.440283;,
  0.000000;0.000000;1.000000;,
  -0.349736;0.000000;0.936848;,
  -0.237773;0.329399;0.913762;,
  0.000000;0.447392;0.894338;,
  0.237773;0.329399;0.913762;,
  0.349735;0.000000;0.936849;,
  -0.187857;0.420519;-0.887622;,
  -0.296333;-0.000001;-0.955085;,
  -0.000000;0.902514;-0.430661;,
  0.296334;-0.000003;-0.955084;,
  0.187857;0.420522;-0.887621;,
  0.000000;0.000000;-1.000000;,
  0.300434;-0.501570;0.811275;,
  0.143469;-0.363638;0.920426;,
  0.000001;-0.588093;0.808793;,
  0.000001;-0.705585;0.708626;,
  -0.300433;-0.501570;0.811275;,
  -0.143468;-0.363636;0.920427;,
  0.634872;-0.634890;0.440286;,
  0.645221;-0.645238;-0.409094;,
  0.000000;-0.912493;-0.409092;,
  -0.000000;-0.897859;0.440284;,
  -0.634875;-0.634883;0.440293;,
  -0.645227;-0.645231;-0.409096;,
  0.000000;-0.357419;-0.933944;,
  -0.252731;-0.252731;-0.933945;,
  0.252730;-0.252735;-0.933945;,
  0.634873;-0.634889;0.440287;,
  0.707098;-0.707116;-0.000000;,
  -0.000005;-1.000000;0.000000;,
  -0.000003;-0.897859;0.440284;,
  -0.634875;-0.634883;0.440292;,
  -0.707102;-0.707112;0.000000;,
  -0.237774;-0.329406;0.913759;,
  0.000000;-0.447405;0.894331;,
  0.237773;-0.329405;0.913759;,
  -0.187858;-0.420522;-0.887621;,
  0.000002;-0.902520;-0.430648;,
  0.187859;-0.420527;-0.887618;,
  0.305371;0.951178;-0.044826;,
  -0.312894;0.949136;-0.035196;,
  -0.328055;0.938883;-0.104295;,
  0.320492;0.940378;-0.113906;,
  -0.365349;0.930808;0.010824;,
  0.365359;0.930804;0.010828;,
  0.334483;0.942395;0.003672;,
  -0.334482;0.942395;0.003673;,
  0.000000;0.974632;-0.223813;,
  -0.357599;0.933618;-0.021926;,
  0.357585;0.933623;-0.021926;,
  -0.333781;0.939864;0.072422;,
  0.333776;0.939866;0.072424;,
  -0.286990;0.956994;-0.042422;,
  0.286989;0.956994;-0.042419;,
  -0.287642;0.956744;-0.043630;,
  0.287639;0.956745;-0.043632;,
  -0.307229;0.944076;0.119710;,
  0.307227;0.944077;0.119708;,
  -0.298775;0.952755;0.054696;,
  0.298775;0.952755;0.054696;,
  0.680213;0.732737;0.020160;,
  0.612506;0.759900;-0.217690;,
  -0.624329;0.755524;-0.198485;,
  -0.680198;0.732751;0.020152;,
  0.602188;0.793446;-0.088395;,
  0.642751;0.766043;0.007057;,
  0.618315;0.733142;-0.283177;,
  -0.613235;0.786883;-0.068980;,
  -0.642749;0.766044;0.007058;,
  -0.667996;0.743037;-0.040958;,
  0.667972;0.743058;-0.040958;,
  -0.634193;0.760831;0.137604;,
  0.634185;0.760837;0.137608;,
  -0.576241;0.812829;-0.085178;,
  0.576239;0.812831;-0.085173;,
  -0.577093;0.811974;-0.087535;,
  0.577087;0.811978;-0.087538;,
  -0.585865;0.787876;0.189773;,
  0.585862;0.787879;0.189770;,
  -0.570156;0.821045;0.028419;,
  0.570156;0.821045;0.028419;,
  -0.328056;-0.938883;-0.104295;,
  -0.312895;-0.949136;-0.035196;,
  0.305371;-0.951178;-0.044826;,
  0.320492;-0.940378;-0.113906;,
  0.365359;-0.930804;0.010828;,
  -0.365349;-0.930808;0.010824;,
  -0.334482;-0.942395;0.003673;,
  0.334483;-0.942395;0.003672;,
  0.000000;-0.974632;-0.223813;,
  0.357585;-0.933623;-0.021926;,
  -0.357599;-0.933618;-0.021926;,
  0.333776;-0.939866;0.072424;,
  -0.333781;-0.939864;0.072422;,
  0.286989;-0.956994;-0.042419;,
  -0.286990;-0.956994;-0.042422;,
  0.287639;-0.956745;-0.043632;,
  -0.287642;-0.956744;-0.043630;,
  0.307227;-0.944077;0.119708;,
  -0.307229;-0.944076;0.119710;,
  0.298775;-0.952755;0.054696;,
  -0.298775;-0.952755;0.054696;,
  -0.511593;0.563246;-0.648865;,
  -0.338245;0.757163;-0.558833;,
  0.338245;0.757167;-0.558828;,
  0.511593;0.563256;-0.648857;,
  -0.511589;-0.563250;-0.648866;,
  -0.338247;-0.757167;-0.558826;,
  0.338247;-0.757173;-0.558818;,
  0.511589;-0.563264;-0.648853;,
  -0.618315;0.733142;-0.283177;,
  0.612506;-0.759900;-0.217690;,
  0.680213;-0.732737;0.020160;,
  -0.680198;-0.732751;0.020152;,
  -0.624330;-0.755524;-0.198486;,
  0.642751;-0.766043;0.007057;,
  0.602188;-0.793446;-0.088395;,
  0.618315;-0.733142;-0.283177;,
  -0.613236;-0.786882;-0.068980;,
  -0.642749;-0.766044;0.007058;,
  -0.618315;-0.733142;-0.283177;,
  -0.667996;-0.743037;-0.040958;,
  0.667972;-0.743058;-0.040958;,
  -0.634193;-0.760831;0.137604;,
  0.634185;-0.760837;0.137608;,
  -0.576241;-0.812829;-0.085178;,
  0.576239;-0.812831;-0.085173;,
  -0.577093;-0.811974;-0.087535;,
  0.577086;-0.811978;-0.087539;,
  -0.585865;-0.787876;0.189773;,
  0.585862;-0.787879;0.189770;,
  -0.570156;-0.821045;0.028419;,
  0.570156;-0.821045;0.028419;;
  164;
  4;0,1,1,0;,
  4;2,2,0,0;,
  4;1,3,3,1;,
  4;4,4,2,2;,
  4;3,5,5,3;,
  4;6,6,4,4;,
  4;5,7,7,5;,
  4;7,7,6,6;,
  4;8,9,10,11;,
  4;12,13,14,15;,
  4;13,8,11,14;,
  4;16,12,15,17;,
  4;18,19,20,21;,
  4;22,23,24,25;,
  4;23,18,21,24;,
  4;26,22,25,27;,
  4;28,29,53,53;,
  4;29,30,53,53;,
  4;31,28,53,53;,
  4;30,32,53,53;,
  4;33,34,35,36;,
  4;37,1,38,39;,
  4;34,40,41,35;,
  4;1,33,36,38;,
  3;42,42,42;,
  3;42,42,42;,
  3;42,42,42;,
  3;42,42,42;,
  4;12,16,43,44;,
  4;13,12,44,45;,
  4;8,13,45,46;,
  4;9,8,46,47;,
  4;28,31,27,25;,
  4;29,28,25,24;,
  4;30,29,24,21;,
  4;32,30,21,20;,
  4;26,39,38,22;,
  4;22,38,36,23;,
  4;23,36,35,18;,
  4;18,35,41,19;,
  4;48,49,144,145;,
  4;50,48,145,50;,
  4;51,52,146,147;,
  4;52,50,50,146;,
  4;53,53,48,53;,
  4;53,53,52,51;,
  4;53,53,49,48;,
  4;53,53,53,52;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  4;10,9,54,55;,
  4;56,57,58,59;,
  4;55,54,57,56;,
  4;59,58,16,17;,
  4;20,19,60,61;,
  4;62,63,64,65;,
  4;61,60,63,62;,
  4;65,64,26,27;,
  4;53,66,67,53;,
  4;53,68,66,53;,
  4;53,67,31,53;,
  4;53,32,68,53;,
  4;69,70,71,72;,
  4;73,74,37,39;,
  4;41,40,70,69;,
  4;72,71,74,73;,
  3;42,42,42;,
  3;42,42,42;,
  3;42,42,42;,
  3;42,42,42;,
  4;43,16,58,75;,
  4;75,58,57,76;,
  4;76,57,54,77;,
  4;77,54,9,47;,
  4;27,31,67,65;,
  4;65,67,66,62;,
  4;62,66,68,61;,
  4;61,68,32,20;,
  4;73,39,26,64;,
  4;72,73,64,63;,
  4;69,72,63,60;,
  4;41,69,60,19;,
  4;148,49,78,149;,
  4;149,78,79,79;,
  4;150,80,51,151;,
  4;79,79,80,150;,
  4;78,53,53,53;,
  4;80,53,53,51;,
  4;49,53,53,78;,
  4;53,53,53,80;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;44,42,45;,
  3;42,46,45;,
  3;75,76,42;,
  3;76,77,42;,
  3;77,47,42;,
  3;47,46,42;,
  3;44,43,42;,
  3;43,75,42;,
  4;81,82,83,84;,
  4;85,86,84,83;,
  4;87,88,82,81;,
  3;87,89,88;,
  4;90,91,86,85;,
  4;92,93,91,90;,
  4;94,95,93,92;,
  4;96,97,95,94;,
  4;98,99,97,96;,
  4;100,101,99,98;,
  4;102,103,84,86;,
  4;104,105,85,83;,
  4;106,107,87,81;,
  4;107,108,89,87;,
  4;109,104,83,82;,
  4;103,106,81,84;,
  4;152,110,88,89;,
  4;110,109,82,88;,
  4;105,111,90,85;,
  4;112,102,86,91;,
  4;111,113,92,90;,
  4;114,112,91,93;,
  4;113,115,94,92;,
  4;116,114,93,95;,
  4;115,117,96,94;,
  4;118,116,95,97;,
  4;117,119,98,96;,
  4;120,118,97,99;,
  4;119,121,100,98;,
  4;122,120,99,101;,
  4;123,124,125,126;,
  4;126,127,128,123;,
  4;124,129,130,125;,
  3;131,130,129;,
  4;127,132,133,128;,
  4;132,134,135,133;,
  4;134,136,137,135;,
  4;136,138,139,137;,
  4;138,140,141,139;,
  4;140,142,143,141;,
  4;126,153,154,127;,
  4;128,155,156,123;,
  4;130,157,158,125;,
  4;131,159,157,130;,
  4;123,156,160,124;,
  4;125,158,153,126;,
  4;129,161,162,131;,
  4;124,160,161,129;,
  4;133,163,155,128;,
  4;127,154,164,132;,
  4;135,165,163,133;,
  4;132,164,166,134;,
  4;137,167,165,135;,
  4;134,166,168,136;,
  4;139,169,167,137;,
  4;136,168,170,138;,
  4;141,171,169,139;,
  4;138,170,172,140;,
  4;143,173,171,141;,
  4;140,172,174,142;;
 }
 MeshTextureCoords {
  274;
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  13.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  13.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  13.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  13.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  13.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  13.000000;1.000000;,
  13.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  12.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;,
  7.000000;1.000000;;
 }
}
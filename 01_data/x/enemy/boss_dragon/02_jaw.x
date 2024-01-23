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
 184;
 -11.65200;13.13640;-22.60308;,
 -8.61108;9.42216;-25.40316;,
 -11.91288;13.75416;-32.68308;,
 -10.65312;9.91068;-33.94692;,
 -9.50328;14.86296;-19.94904;,
 -7.66440;8.03400;-42.78192;,
 -8.94876;7.72536;-39.36132;,
 -5.39940;5.55504;-40.69560;,
 -5.91732;6.29856;-43.97064;,
 -0.00504;2.66052;-49.09320;,
 -3.12912;4.54824;-48.69876;,
 -9.50328;17.61840;-18.73236;,
 -0.00504;18.25884;-17.91324;,
 -0.00504;15.94548;-29.40852;,
 -7.02828;13.45848;-30.42516;,
 -0.00504;10.35816;-28.33128;,
 -0.00504;8.54304;-23.60904;,
 -0.00504;4.48356;-37.93848;,
 -0.00504;10.50192;-41.91948;,
 -4.78152;9.62856;-41.09376;,
 -6.47928;10.89996;-29.43540;,
 -0.00504;8.06916;-40.52580;,
 -4.48116;8.52564;-40.67904;,
 -0.00504;6.25296;-46.51584;,
 11.91288;13.75416;-32.68308;,
 8.61108;9.42216;-25.40316;,
 11.65200;13.13640;-22.60308;,
 10.65312;9.91068;-33.94692;,
 9.50328;14.86296;-19.94904;,
 5.39940;5.55504;-40.69560;,
 8.94876;7.72536;-39.36132;,
 7.66440;8.03400;-42.78192;,
 5.91732;6.29856;-43.97064;,
 3.12912;4.54824;-48.69876;,
 9.50328;17.61840;-18.73236;,
 7.02828;13.45848;-30.42516;,
 4.78152;9.62856;-41.09376;,
 6.47928;10.89996;-29.43540;,
 4.48116;8.52564;-40.67904;,
 -0.00504;7.78272;12.06600;,
 -10.04652;7.23564;10.28040;,
 -0.00504;3.98352;-0.33756;,
 -0.00504;7.97844;33.27948;,
 -16.65420;2.57196;0.14172;,
 10.04652;7.23564;10.28040;,
 16.65420;2.57196;0.14172;,
 -0.00504;16.14120;-18.63228;,
 -7.27872;19.86888;-14.91768;,
 7.27872;19.86888;-14.91768;,
 11.00892;23.45292;-18.13224;,
 9.50328;21.38904;-19.93560;,
 -0.00504;22.46172;-18.53928;,
 -9.50328;21.38904;-19.93560;,
 -11.00892;23.45292;-18.13224;,
 -0.00504;26.35164;-13.56684;,
 -0.00504;26.48436;0.00864;,
 14.56260;23.59704;-11.20308;,
 -14.56260;23.59704;-11.20308;,
 12.43332;25.87512;-15.26784;,
 -12.43332;25.87512;-15.26784;,
 -0.00504;7.32276;-20.85468;,
 -0.00504;2.63832;-24.03468;,
 -10.04652;4.71552;-17.00604;,
 -16.65420;2.57196;0.14172;,
 -10.04652;4.71552;-17.00604;,
 -0.00504;3.98352;-0.33756;,
 -15.25572;10.38096;0.22488;,
 -9.29652;8.85192;-10.31460;,
 -16.65420;2.57196;0.14172;,
 -15.25572;10.38096;0.22488;,
 -16.65420;2.57196;0.14172;,
 -10.04652;7.23564;10.28040;,
 -0.00504;6.14772;-15.22020;,
 -0.00504;7.97844;33.27948;,
 -0.00504;16.47996;18.78480;,
 -0.00504;2.63832;-24.03468;,
 -11.58552;16.98912;-5.48916;,
 -14.56260;23.59704;-11.20308;,
 -7.27872;19.86888;-14.91768;,
 -11.58552;16.98912;-5.48916;,
 -15.25572;10.38096;0.22488;,
 10.04652;4.71552;-17.00604;,
 10.04652;4.71552;-17.00604;,
 16.65420;2.57196;0.14172;,
 9.29652;8.85192;-10.31460;,
 15.25572;10.38096;0.22488;,
 16.65420;2.57196;0.14172;,
 10.04652;7.23564;10.28040;,
 16.65420;2.57196;0.14172;,
 15.25572;10.38096;0.22488;,
 11.58552;16.98912;-5.48916;,
 11.58552;16.98912;-5.48916;,
 7.27872;19.86888;-14.91768;,
 14.56260;23.59704;-11.20308;,
 15.25572;10.38096;0.22488;,
 -0.00504;-7.23672;-50.52336;,
 -5.31204;-0.15744;-45.91404;,
 -0.00504;0.35748;-49.77096;,
 -4.06872;2.18772;-47.57892;,
 -5.91732;6.29856;-43.97064;,
 -6.72144;3.44640;-43.32456;,
 -8.30376;0.92952;-40.31628;,
 -0.00504;1.71780;-50.87220;,
 -3.12912;4.54824;-48.69876;,
 -0.00504;4.89168;-51.37128;,
 -8.81784;5.04744;-40.48860;,
 -7.66440;8.03400;-42.78192;,
 -13.49784;-1.29132;-35.28852;,
 -11.96292;2.98308;-33.92400;,
 -8.94876;7.72536;-39.36132;,
 -10.53852;5.27616;-35.71800;,
 -10.65312;9.91068;-33.94692;,
 -12.02412;7.33512;-30.94800;,
 -14.72508;4.64376;-25.77132;,
 -13.46736;8.31300;-20.36688;,
 -20.27928;3.35664;-14.54664;,
 -11.91288;13.75416;-32.68308;,
 -11.65200;13.13640;-22.60308;,
 -12.43332;12.10728;-17.70396;,
 -12.43332;9.13332;-16.77192;,
 -12.43332;12.10728;-17.70396;,
 -12.43332;13.38900;-10.55892;,
 -11.00892;15.72300;-15.36720;,
 -12.43332;12.10728;-17.70396;,
 -12.43332;20.04444;-10.05432;,
 -11.00892;19.15356;-14.85000;,
 -11.00892;15.72300;-15.36720;,
 -11.00892;23.45292;-18.13224;,
 -11.00892;19.15356;-14.85000;,
 -12.43332;9.13332;-16.77192;,
 -0.00504;4.89168;-51.37128;,
 -0.00504;13.86576;-8.85756;,
 -0.00504;20.52096;-8.35308;,
 -0.00504;9.60996;-11.10372;,
 -0.00504;9.60996;-11.10372;,
 -12.43332;9.13332;-16.77192;,
 -0.00504;7.89564;-18.56448;,
 -20.27928;3.35664;-14.54664;,
 -14.72508;4.64376;-25.77132;,
 -0.00504;1.74012;-37.94220;,
 -11.96292;2.98308;-33.92400;,
 -13.49784;-1.29132;-35.28852;,
 -8.30376;0.92952;-40.31628;,
 -0.00504;-0.19008;-46.33560;,
 -5.31204;-0.15744;-45.91404;,
 -0.00504;-7.23672;-50.52336;,
 5.31204;-0.15744;-45.91404;,
 4.06872;2.18772;-47.57892;,
 6.72144;3.44640;-43.32456;,
 5.91732;6.29856;-43.97064;,
 8.30376;0.92952;-40.31628;,
 3.12912;4.54824;-48.69876;,
 7.66440;8.03400;-42.78192;,
 8.81784;5.04744;-40.48860;,
 13.49784;-1.29132;-35.28852;,
 11.96292;2.98308;-33.92400;,
 10.53852;5.27616;-35.71800;,
 8.94876;7.72536;-39.36132;,
 12.02412;7.33512;-30.94800;,
 10.65312;9.91068;-33.94692;,
 14.72508;4.64376;-25.77132;,
 20.27928;3.35664;-14.54664;,
 13.46736;8.31300;-20.36688;,
 11.91288;13.75416;-32.68308;,
 12.43332;12.10728;-17.70396;,
 11.65200;13.13640;-22.60308;,
 12.43332;9.13332;-16.77192;,
 11.00892;15.72300;-15.36720;,
 12.43332;13.38900;-10.55892;,
 12.43332;12.10728;-17.70396;,
 12.43332;12.10728;-17.70396;,
 11.00892;19.15356;-14.85000;,
 12.43332;20.04444;-10.05432;,
 11.00892;15.72300;-15.36720;,
 11.00892;23.45292;-18.13224;,
 11.00892;19.15356;-14.85000;,
 12.43332;9.13332;-16.77192;,
 12.43332;9.13332;-16.77192;,
 20.27928;3.35664;-14.54664;,
 14.72508;4.64376;-25.77132;,
 11.96292;2.98308;-33.92400;,
 13.49784;-1.29132;-35.28852;,
 8.30376;0.92952;-40.31628;,
 5.31204;-0.15744;-45.91404;;
 
 227;
 3;0,1,2;,
 3;3,2,1;,
 3;0,4,1;,
 3;5,6,7;,
 3;8,5,7;,
 3;3,7,6;,
 3;8,9,10;,
 3;4,11,12;,
 3;4,13,14;,
 3;1,15,16;,
 3;17,1,16;,
 3;7,1,17;,
 3;9,7,17;,
 3;14,18,19;,
 3;20,21,15;,
 3;1,14,20;,
 3;22,19,23;,
 3;19,18,23;,
 3;20,19,22;,
 3;23,21,22;,
 3;3,1,7;,
 3;8,7,9;,
 3;4,12,13;,
 3;1,20,15;,
 3;14,13,18;,
 3;20,22,21;,
 3;1,4,14;,
 3;20,14,19;,
 3;24,25,26;,
 3;25,24,27;,
 3;25,28,26;,
 3;29,30,31;,
 3;29,31,32;,
 3;30,29,27;,
 3;33,9,32;,
 3;12,34,28;,
 3;35,13,28;,
 3;16,15,25;,
 3;16,25,17;,
 3;17,25,29;,
 3;17,29,9;,
 3;36,18,35;,
 3;15,21,37;,
 3;37,35,25;,
 3;23,36,38;,
 3;23,18,36;,
 3;38,36,37;,
 3;38,21,23;,
 3;29,25,27;,
 3;9,29,32;,
 3;13,12,28;,
 3;15,37,25;,
 3;18,13,35;,
 3;21,38,37;,
 3;35,28,25;,
 3;36,35,37;,
 3;39,40,41;,
 3;39,42,40;,
 3;43,41,40;,
 3;41,44,39;,
 3;44,42,39;,
 3;44,41,45;,
 3;46,47,48;,
 3;49,50,51;,
 3;51,52,53;,
 3;49,51,54;,
 3;53,54,51;,
 3;55,56,57;,
 4;56,48,47,57;,
 3;54,58,49;,
 3;53,59,54;,
 3;60,61,62;,
 3;63,64,65;,
 3;66,67,62;,
 3;68,66,62;,
 3;69,70,71;,
 3;64,72,65;,
 3;73,74,71;,
 3;74,69,71;,
 3;64,75,72;,
 3;67,46,60;,
 3;69,74,55;,
 3;62,67,60;,
 3;66,47,67;,
 3;67,47,46;,
 3;69,55,76;,
 3;55,57,76;,
 3;77,78,79;,
 3;78,80,79;,
 3;81,61,60;,
 3;65,82,83;,
 3;81,84,85;,
 3;81,85,86;,
 3;87,88,89;,
 3;65,72,82;,
 3;87,74,73;,
 3;87,89,74;,
 3;72,75,82;,
 3;60,46,84;,
 3;55,74,89;,
 3;60,84,81;,
 3;84,48,85;,
 3;46,48,84;,
 3;90,55,89;,
 3;90,56,55;,
 3;91,92,93;,
 3;91,94,92;,
 3;95,96,97;,
 3;98,97,96;,
 3;99,98,100;,
 3;100,96,101;,
 3;98,102,97;,
 3;102,103,104;,
 3;99,105,106;,
 3;105,101,107;,
 3;108,105,107;,
 3;109,105,110;,
 3;111,110,112;,
 3;112,108,113;,
 3;114,113,115;,
 3;116,112,114;,
 3;117,114,118;,
 3;119,114,115;,
 3;120,121,122;,
 3;123,4,0;,
 3;122,124,125;,
 3;126,11,4;,
 3;125,59,127;,
 3;128,52,11;,
 3;99,103,98;,
 3;100,98,96;,
 3;98,103,102;,
 3;99,100,105;,
 3;105,100,101;,
 3;108,110,105;,
 3;109,106,105;,
 3;111,109,110;,
 3;112,110,108;,
 3;114,112,113;,
 3;116,111,112;,
 3;117,116,114;,
 3;119,118,114;,
 3;120,129,121;,
 3;123,126,4;,
 3;122,121,124;,
 3;126,128,11;,
 3;125,124,59;,
 3;128,53,52;,
 3;10,9,130;,
 3;12,52,51;,
 3;11,52,12;,
 3;124,54,59;,
 3;124,131,132;,
 3;121,133,131;,
 3;134,135,136;,
 3;135,137,136;,
 3;137,138,136;,
 3;136,138,139;,
 3;138,140,139;,
 3;140,141,139;,
 3;141,142,139;,
 3;139,142,143;,
 3;142,144,143;,
 3;144,145,143;,
 3;124,132,54;,
 3;124,121,131;,
 3;121,129,133;,
 3;97,146,95;,
 3;146,97,147;,
 3;148,147,149;,
 3;150,146,148;,
 3;97,102,147;,
 3;104,151,102;,
 3;152,153,149;,
 3;154,150,153;,
 3;154,153,155;,
 3;156,153,157;,
 3;158,156,159;,
 3;160,155,158;,
 3;161,160,162;,
 3;162,158,163;,
 3;164,162,165;,
 3;161,162,166;,
 3;167,168,169;,
 3;26,28,170;,
 3;171,172,167;,
 3;28,34,173;,
 3;174,58,171;,
 3;34,50,175;,
 3;147,151,149;,
 3;146,147,148;,
 3;102,151,147;,
 3;153,148,149;,
 3;150,148,153;,
 3;153,156,155;,
 3;153,152,157;,
 3;156,157,159;,
 3;155,156,158;,
 3;160,158,162;,
 3;158,159,163;,
 3;162,163,165;,
 3;162,164,166;,
 3;168,176,169;,
 3;28,173,170;,
 3;172,168,167;,
 3;34,175,173;,
 3;58,172,171;,
 3;50,49,175;,
 3;130,9,33;,
 3;51,50,12;,
 3;12,50,34;,
 3;58,54,172;,
 3;132,131,172;,
 3;131,133,168;,
 3;136,177,134;,
 3;136,178,177;,
 3;136,179,178;,
 3;139,179,136;,
 3;139,180,179;,
 3;139,181,180;,
 3;139,182,181;,
 3;143,182,139;,
 3;143,183,182;,
 3;143,145,183;,
 3;54,132,172;,
 3;131,168,172;,
 3;133,176,168;;
 
 MeshMaterialList {
  4;
  227;
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
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  3,
  2,
  3,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  3,
  2,
  3,
  2,
  3,
  0,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  3,
  2,
  3,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  3,
  2,
  3,
  2,
  3,
  0,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.241600;0.241600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.696800;0.620800;0.395200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.257600;0.147200;0.150400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
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
  207;
  0.572303;0.738034;-0.357457;,
  0.537159;0.805000;-0.251862;,
  0.859757;0.502549;-0.090895;,
  0.674957;0.717408;-0.172508;,
  -0.337181;0.831285;-0.441897;,
  0.654252;0.737761;0.166322;,
  0.532201;0.845664;0.040182;,
  0.506483;0.861696;-0.030895;,
  0.608710;0.792311;0.041424;,
  0.000537;0.986259;0.165205;,
  0.519000;0.791457;0.322853;,
  0.082181;0.053989;-0.995154;,
  0.000007;0.282863;-0.959160;,
  -0.000176;0.963076;-0.269230;,
  -0.264002;0.910211;-0.319090;,
  -0.000039;-0.983221;-0.182419;,
  -0.000012;-0.933420;-0.358785;,
  0.000185;0.978149;-0.207904;,
  -0.000225;0.870108;-0.492861;,
  -0.239929;0.818171;-0.522522;,
  -0.081825;-0.996627;0.006268;,
  -0.000108;-0.975200;0.221324;,
  -0.890818;-0.351060;-0.288446;,
  -0.797708;-0.390148;-0.459833;,
  -0.859757;0.502549;-0.090895;,
  -0.537066;0.805053;-0.251894;,
  -0.572303;0.738034;-0.357457;,
  -0.674957;0.717408;-0.172508;,
  0.337066;0.831336;-0.441891;,
  -0.506280;0.861816;-0.030890;,
  -0.532201;0.845664;0.040182;,
  -0.654252;0.737761;0.166322;,
  -0.608123;0.792776;0.041130;,
  -0.517824;0.792329;0.322603;,
  -0.082094;0.053988;-0.995161;,
  0.263607;0.910335;-0.319063;,
  0.239450;0.818297;-0.522546;,
  0.081675;-0.996639;0.006289;,
  0.890616;-0.351182;-0.288919;,
  0.000013;-0.988268;0.152727;,
  0.047911;-0.972892;0.226244;,
  0.000034;-0.993935;0.109973;,
  0.000026;-0.999957;0.009226;,
  0.087672;-0.986059;0.141423;,
  -0.047875;-0.972891;0.226255;,
  -0.087619;-0.986063;0.141430;,
  -0.485837;0.243647;-0.839404;,
  0.000000;0.705840;-0.708371;,
  -0.045896;0.746857;-0.663399;,
  -0.109597;0.083014;-0.990503;,
  0.000045;0.490406;-0.871494;,
  0.109711;0.083022;-0.990490;,
  0.045936;0.746863;-0.663389;,
  0.000022;0.782448;-0.622715;,
  -0.572045;0.753335;0.324424;,
  0.000000;0.868001;-0.496563;,
  -0.056187;0.775982;-0.628247;,
  0.056230;0.775991;-0.628233;,
  -0.172507;0.662543;-0.728888;,
  -0.751217;0.526275;0.398382;,
  -0.000191;0.561655;-0.827371;,
  0.000059;-0.992953;0.118506;,
  -0.907065;0.420813;0.012261;,
  -0.605429;0.600090;-0.522827;,
  -0.692866;0.327533;-0.642385;,
  -0.850792;0.457830;0.257965;,
  0.906995;0.420962;0.012333;,
  0.605277;0.600168;-0.522913;,
  0.692650;0.327557;-0.642606;,
  0.850631;0.458011;0.258173;,
  -0.637756;-0.119646;-0.760889;,
  -0.628027;-0.253649;-0.735692;,
  -0.591256;-0.243455;-0.768860;,
  -0.591077;0.079526;-0.802686;,
  -0.000055;-0.804593;0.593826;,
  -0.000050;-0.210204;0.977657;,
  -0.000057;0.321330;0.946967;,
  0.000018;-0.984217;0.176968;,
  0.000044;-0.952760;0.303723;,
  0.000032;-0.887840;0.460152;,
  -0.738957;-0.035582;-0.672813;,
  -0.802555;-0.083537;-0.590700;,
  -0.820897;0.155230;-0.549574;,
  -0.892163;0.294946;-0.342129;,
  -0.835348;0.478752;-0.270166;,
  -0.660612;0.736375;-0.146093;,
  -0.741230;-0.111447;-0.661935;,
  -0.836399;0.023210;-0.547630;,
  -0.860484;0.063381;-0.505520;,
  -0.921394;0.171200;-0.348888;,
  -0.932062;0.167455;-0.321278;,
  -0.908003;0.415300;-0.055293;,
  -0.929050;0.368252;-0.035437;,
  -0.941192;0.325497;0.090603;,
  -0.933731;0.206701;-0.292269;,
  -0.982134;0.145926;-0.118818;,
  -0.938861;-0.122971;-0.321587;,
  -0.961206;-0.050586;-0.271154;,
  0.738653;-0.035755;-0.673137;,
  0.802555;-0.083537;-0.590700;,
  0.820897;0.155230;-0.549574;,
  0.892163;0.294946;-0.342129;,
  0.835348;0.478752;-0.270166;,
  0.660612;0.736375;-0.146093;,
  0.740857;-0.111779;-0.662297;,
  0.836399;0.023210;-0.547630;,
  0.860484;0.063381;-0.505520;,
  0.921394;0.171200;-0.348888;,
  0.932062;0.167455;-0.321278;,
  0.908003;0.415300;-0.055293;,
  0.929050;0.368252;-0.035437;,
  0.941192;0.325497;0.090603;,
  0.933731;0.206701;-0.292269;,
  0.982134;0.145926;-0.118818;,
  0.938861;-0.122971;-0.321587;,
  0.961206;-0.050586;-0.271154;,
  -0.296897;-0.834131;-0.464841;,
  0.000089;0.962136;-0.272570;,
  -0.932668;-0.187007;-0.308480;,
  -0.913452;-0.314346;-0.258440;,
  -0.919500;-0.318562;-0.230299;,
  -0.000249;0.734312;-0.678812;,
  -0.000119;-0.956978;0.290159;,
  -0.106603;-0.965333;0.238261;,
  -0.967480;0.081420;-0.239484;,
  -0.951030;0.114576;-0.287078;,
  0.296880;-0.834138;-0.464839;,
  0.913452;-0.314346;-0.258440;,
  0.932668;-0.187007;-0.308480;,
  0.797126;-0.390315;-0.460699;,
  0.919391;-0.318650;-0.230615;,
  0.106366;-0.965352;0.238288;,
  0.951030;0.114576;-0.287078;,
  0.967480;0.081420;-0.239484;,
  0.107193;-0.993168;0.046127;,
  -0.980907;0.174758;0.085329;,
  -0.816897;0.360192;0.450490;,
  -0.774049;0.546120;0.320314;,
  0.000033;-0.929071;0.369901;,
  -0.790256;-0.204530;-0.577636;,
  -0.615394;0.338340;0.711910;,
  -0.580011;-0.666416;-0.468484;,
  -0.107098;-0.993178;0.046119;,
  0.980907;0.174758;0.085329;,
  0.816718;0.360491;0.450574;,
  0.750923;0.526618;0.398482;,
  0.773737;0.546449;0.320507;,
  0.726947;0.167812;-0.665873;,
  0.649762;-0.037206;-0.759227;,
  0.679166;0.562877;0.471066;,
  0.790162;-0.204596;-0.577741;,
  0.615114;0.338577;0.712039;,
  0.580011;-0.666416;-0.468484;,
  -0.799246;0.007201;-0.600961;,
  -0.716239;-0.049381;-0.696105;,
  -0.847513;-0.076047;-0.525298;,
  -0.934895;0.100731;-0.340331;,
  -0.918814;0.031050;-0.393467;,
  -0.975471;0.081733;-0.204393;,
  -0.870413;0.492299;0.004685;,
  -0.936868;0.033979;-0.348028;,
  -0.940724;-0.215629;-0.261808;,
  -0.905972;-0.256885;-0.336490;,
  -0.911650;-0.201203;-0.358346;,
  0.000659;0.714426;0.699711;,
  -0.140535;0.318133;0.937572;,
  -0.000051;0.666562;0.745450;,
  -0.126558;0.661193;0.739464;,
  -0.162427;-0.479169;0.862563;,
  -0.145510;-0.906947;0.395316;,
  0.156689;0.162460;0.974195;,
  0.000057;0.164567;0.986366;,
  0.217676;-0.976012;-0.004208;,
  0.081303;-0.983140;0.163783;,
  0.126078;-0.777761;0.615786;,
  0.259568;-0.377033;0.889084;,
  0.164953;-0.953850;-0.250921;,
  0.070364;-0.995850;0.057721;,
  0.040788;-0.815968;0.576656;,
  0.000061;-0.510883;0.859650;,
  0.590414;-0.243838;-0.769386;,
  0.590347;0.079579;-0.803217;,
  0.799246;0.007201;-0.600961;,
  0.626972;-0.254084;-0.736442;,
  0.636537;-0.119803;-0.761885;,
  0.715590;-0.049675;-0.696752;,
  0.847513;-0.076047;-0.525298;,
  0.934895;0.100731;-0.340331;,
  0.918814;0.031050;-0.393467;,
  0.975471;0.081733;-0.204393;,
  0.870413;0.492299;0.004685;,
  0.936868;0.033979;-0.348028;,
  0.905972;-0.256885;-0.336490;,
  0.940724;-0.215629;-0.261808;,
  0.911650;-0.201203;-0.358346;,
  0.126457;0.661202;0.739473;,
  0.140424;0.318138;0.937587;,
  0.162300;-0.479182;0.862579;,
  0.145396;-0.906961;0.395326;,
  -0.156576;0.162611;0.974188;,
  -0.081262;-0.983146;0.163766;,
  -0.217562;-0.976037;-0.004267;,
  -0.125991;-0.777735;0.615836;,
  -0.259379;-0.376989;0.889158;,
  -0.070292;-0.995858;0.057665;,
  -0.164796;-0.953836;-0.251077;,
  -0.040711;-0.815972;0.576656;;
  227;
  3;0,1,2;,
  3;3,2,1;,
  3;0,4,1;,
  3;5,6,7;,
  3;8,5,7;,
  3;3,7,6;,
  3;8,9,10;,
  3;4,11,12;,
  3;4,13,14;,
  3;116,15,16;,
  3;17,1,117;,
  3;7,1,17;,
  3;9,7,17;,
  3;14,18,19;,
  3;20,21,15;,
  3;116,118,119;,
  3;22,120,23;,
  3;19,18,121;,
  3;119,120,22;,
  3;122,21,123;,
  3;3,1,7;,
  3;8,7,9;,
  3;4,12,13;,
  3;116,20,15;,
  3;14,13,18;,
  3;20,123,21;,
  3;124,125,118;,
  3;119,118,120;,
  3;24,25,26;,
  3;25,24,27;,
  3;25,28,26;,
  3;29,30,31;,
  3;29,31,32;,
  3;30,29,27;,
  3;33,9,32;,
  3;12,34,28;,
  3;35,13,28;,
  3;16,15,126;,
  3;117,25,17;,
  3;17,25,29;,
  3;17,29,9;,
  3;36,18,35;,
  3;15,21,37;,
  3;127,128,126;,
  3;129,130,38;,
  3;121,18,36;,
  3;38,130,127;,
  3;131,21,122;,
  3;29,25,27;,
  3;9,29,32;,
  3;13,12,28;,
  3;15,37,126;,
  3;18,13,35;,
  3;21,131,37;,
  3;128,132,133;,
  3;130,128,127;,
  3;39,40,41;,
  3;39,42,40;,
  3;43,41,40;,
  3;41,44,39;,
  3;44,42,39;,
  3;44,41,45;,
  3;46,47,47;,
  3;48,49,50;,
  3;50,51,52;,
  3;48,50,53;,
  3;52,53,50;,
  3;54,55,55;,
  4;55,47,47,55;,
  3;53,56,48;,
  3;52,57,53;,
  3;58,60,63;,
  3;43,134,41;,
  3;62,64,63;,
  3;135,62,63;,
  3;62,135,136;,
  3;134,61,41;,
  3;137,59,136;,
  3;59,62,136;,
  3;134,138,61;,
  3;64,46,58;,
  3;62,59,54;,
  3;63,64,58;,
  3;62,139,64;,
  3;64,139,46;,
  3;62,54,65;,
  3;54,140,65;,
  3;141,139,141;,
  3;139,62,65;,
  3;67,60,58;,
  3;41,142,45;,
  3;67,68,66;,
  3;67,66,143;,
  3;144,143,66;,
  3;41,61,142;,
  3;144,145,146;,
  3;144,66,145;,
  3;61,138,142;,
  3;147,148,68;,
  3;149,145,66;,
  3;58,68,67;,
  3;68,150,66;,
  3;148,150,68;,
  3;69,149,66;,
  3;69,151,149;,
  3;152,150,152;,
  3;69,66,150;,
  3;73,80,72;,
  3;86,72,80;,
  3;153,86,87;,
  3;87,80,81;,
  3;86,71,72;,
  3;71,154,70;,
  3;153,88,155;,
  3;88,81,82;,
  3;83,88,82;,
  3;156,88,89;,
  3;157,89,90;,
  3;90,83,84;,
  3;91,84,85;,
  3;158,90,91;,
  3;159,91,92;,
  3;93,91,85;,
  3;92,95,94;,
  3;92,4,159;,
  3;94,97,96;,
  3;94,160,125;,
  3;96,161,162;,
  3;96,163,160;,
  3;153,154,86;,
  3;87,86,80;,
  3;86,154,71;,
  3;153,87,88;,
  3;88,87,81;,
  3;83,89,88;,
  3;156,155,88;,
  3;157,156,89;,
  3;90,89,83;,
  3;91,90,84;,
  3;158,157,90;,
  3;159,158,91;,
  3;93,92,91;,
  3;92,93,95;,
  3;92,94,4;,
  3;94,95,97;,
  3;94,96,160;,
  3;96,97,161;,
  3;96,162,163;,
  3;10,9,164;,
  3;12,51,50;,
  3;11,51,12;,
  3;165,166,167;,
  3;165,75,76;,
  3;168,74,75;,
  3;74,169,77;,
  3;170,170,171;,
  3;172,173,77;,
  3;77,173,78;,
  3;173,174,78;,
  3;174,175,78;,
  3;176,177,78;,
  3;78,177,79;,
  3;177,178,79;,
  3;178,179,79;,
  3;165,76,166;,
  3;165,168,75;,
  3;168,169,74;,
  3;180,98,181;,
  3;98,180,104;,
  3;105,104,182;,
  3;99,98,105;,
  3;180,183,104;,
  3;184,185,183;,
  3;186,106,182;,
  3;100,99,106;,
  3;100,106,101;,
  3;107,106,187;,
  3;108,107,188;,
  3;102,101,108;,
  3;103,102,109;,
  3;109,108,189;,
  3;110,109,190;,
  3;103,109,111;,
  3;112,113,110;,
  3;190,28,110;,
  3;114,115,112;,
  3;132,191,112;,
  3;192,193,114;,
  3;191,194,114;,
  3;104,185,182;,
  3;98,104,105;,
  3;183,185,104;,
  3;106,105,182;,
  3;99,105,106;,
  3;106,107,101;,
  3;106,186,187;,
  3;107,187,188;,
  3;101,107,108;,
  3;102,108,109;,
  3;108,188,189;,
  3;109,189,190;,
  3;109,110,111;,
  3;113,111,110;,
  3;28,112,110;,
  3;115,113,112;,
  3;191,114,112;,
  3;193,115,114;,
  3;194,192,114;,
  3;164,9,33;,
  3;50,49,12;,
  3;12,49,34;,
  3;195,166,196;,
  3;76,75,196;,
  3;75,74,197;,
  3;77,198,74;,
  3;171,199,199;,
  3;77,200,201;,
  3;78,200,77;,
  3;78,202,200;,
  3;78,203,202;,
  3;78,204,205;,
  3;79,204,78;,
  3;79,206,204;,
  3;79,179,206;,
  3;166,76,196;,
  3;75,197,196;,
  3;74,198,197;;
 }
 MeshTextureCoords {
  184;
  0.917470;0.566860;,
  0.919820;0.589810;,
  0.950120;0.594030;,
  0.951430;0.603830;,
  0.904380;0.570000;,
  0.973110;0.643560;,
  0.964990;0.627550;,
  0.961580;0.648870;,
  0.973130;0.655560;,
  0.977950;0.696360;,
  0.982170;0.682700;,
  0.900510;0.566570;,
  0.878680;0.608330;,
  0.915280;0.640790;,
  0.932400;0.611820;,
  0.911850;0.637750;,
  0.896820;0.624420;,
  0.942440;0.664870;,
  0.955110;0.676110;,
  0.961520;0.653050;,
  0.928060;0.611740;,
  0.950670;0.672170;,
  0.959550;0.653370;,
  0.969740;0.689090;,
  0.950120;0.594030;,
  0.919820;0.589810;,
  0.917470;0.566860;,
  0.951430;0.603830;,
  0.904380;0.570000;,
  0.961580;0.648870;,
  0.964990;0.627550;,
  0.973110;0.643560;,
  0.973130;0.655560;,
  0.982170;0.682700;,
  0.900510;0.566570;,
  0.932400;0.611820;,
  0.961520;0.653050;,
  0.928060;0.611740;,
  0.959550;0.653370;,
  0.343550;0.913630;,
  0.380680;0.907040;,
  0.343550;0.867810;,
  0.343550;0.992010;,
  0.405100;0.869570;,
  0.380680;0.907040;,
  0.405100;0.869570;,
  0.192630;0.161600;,
  0.187680;0.137860;,
  0.187680;0.137860;,
  0.901850;0.557420;,
  0.904340;0.569960;,
  0.880680;0.610100;,
  0.904340;0.569960;,
  0.901850;0.557420;,
  0.407890;0.235020;,
  0.184170;0.513020;,
  0.155970;0.582840;,
  0.155970;0.582840;,
  0.400330;0.237400;,
  0.400330;0.237400;,
  0.223160;0.159880;,
  0.322990;0.159540;,
  0.259960;0.100220;,
  0.293080;0.958080;,
  0.260780;0.874230;,
  0.211630;0.955740;,
  0.215330;0.046390;,
  0.219250;0.103130;,
  0.269480;0.017540;,
  0.206010;0.602180;,
  0.217290;0.653140;,
  0.260150;0.602780;,
  0.211630;0.882960;,
  0.338660;0.513020;,
  0.250940;0.513020;,
  0.211630;0.839870;,
  0.180990;0.592510;,
  0.418390;0.256130;,
  0.396110;0.274800;,
  0.444980;0.281070;,
  0.471570;0.306010;,
  0.259960;0.100220;,
  0.260780;0.874230;,
  0.293080;0.958080;,
  0.219250;0.103130;,
  0.215330;0.046390;,
  0.269480;0.017540;,
  0.260150;0.602780;,
  0.217290;0.653140;,
  0.206010;0.602180;,
  0.180990;0.592510;,
  0.444980;0.281070;,
  0.396110;0.274800;,
  0.418390;0.256130;,
  0.471570;0.306010;,
  0.202310;0.065770;,
  0.190390;0.037480;,
  0.202370;0.035030;,
  0.190800;0.026840;,
  0.155180;0.019780;,
  0.156880;0.027920;,
  0.153180;0.039850;,
  0.202350;0.027170;,
  0.191350;0.019010;,
  0.200800;0.012330;,
  0.121930;0.028690;,
  0.120470;0.015970;,
  0.121790;0.057020;,
  0.100340;0.036880;,
  0.108450;0.015970;,
  0.102360;0.028110;,
  0.057090;0.011710;,
  0.058900;0.024570;,
  0.058770;0.044820;,
  0.040910;0.043190;,
  0.040770;0.069980;,
  0.039130;0.014330;,
  0.031790;0.019270;,
  0.030430;0.037140;,
  0.032820;0.056500;,
  0.389510;0.306330;,
  0.421250;0.299920;,
  0.399890;0.288230;,
  0.903560;0.549170;,
  0.423490;0.266590;,
  0.402190;0.271050;,
  0.893040;0.549620;,
  0.387610;0.249530;,
  0.891400;0.548160;,
  0.411270;0.321220;,
  0.985200;0.702790;,
  0.428800;0.297530;,
  0.431040;0.264210;,
  0.418830;0.318840;,
  0.315540;0.939110;,
  0.311220;0.940470;,
  0.296610;0.950310;,
  0.324210;0.958250;,
  0.278330;0.953310;,
  0.247450;0.961610;,
  0.257640;0.958060;,
  0.254180;0.970280;,
  0.241420;0.963930;,
  0.226150;0.967130;,
  0.227220;0.967040;,
  0.215530;0.987280;,
  0.190390;0.037480;,
  0.190800;0.026840;,
  0.156880;0.027920;,
  0.155180;0.019780;,
  0.153180;0.039850;,
  0.191350;0.019010;,
  0.120470;0.015970;,
  0.121930;0.028690;,
  0.121790;0.057020;,
  0.100340;0.036880;,
  0.102360;0.028110;,
  0.108450;0.015970;,
  0.058900;0.024570;,
  0.057090;0.011710;,
  0.058770;0.044820;,
  0.040770;0.069980;,
  0.040910;0.043190;,
  0.039130;0.014330;,
  0.030430;0.037140;,
  0.031790;0.019270;,
  0.032820;0.056500;,
  0.399890;0.288230;,
  0.421250;0.299920;,
  0.389510;0.306330;,
  0.903560;0.549170;,
  0.402190;0.271050;,
  0.423490;0.266590;,
  0.893040;0.549620;,
  0.387610;0.249530;,
  0.891400;0.548160;,
  0.411270;0.321220;,
  0.311220;0.940470;,
  0.324210;0.958250;,
  0.278330;0.953310;,
  0.257640;0.958060;,
  0.254180;0.970280;,
  0.241420;0.963930;,
  0.227220;0.967040;;
 }
}
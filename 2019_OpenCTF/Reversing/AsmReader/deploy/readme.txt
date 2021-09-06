[Info]
Title:AsmReader
Description:Can you understand assembly?
Attach:AsmReader.zip

[Deploy]
 Not necessary

[Sovling Strategy]
 1. decompile(ida -> specifiy shell code as code & set code as function)
  v34 = "Reading bytecode is not easy";
  v2 = 16;
  v3 = 19;
  v4 = 89;
  v5 = 116;
  v6 = 98;
  v7 = 92;
  v8 = 109;
  v9 = 14;
  v10 = 7;
  v11 = 50;
  v12 = 53;
  v13 = 116;
  v14 = 24;
  v15 = 63;
  v16 = 108;
  v17 = 98;
  v18 = 121;
  v19 = 72;
  v20 = 12;
  v21 = 68;
  v22 = 119;
  v23 = 77;
  v24 = 9;
  v25 = 13;
  v26 = 67;
  v27 = 4;
  v28 = 81;
  v29 = 105;
  v33 = 31337;
  v32 = 79;
  v31 = 127;
  for ( i = 0; i <= 27; ++i )
  {
    v30 = (v33 * *(char *)(i + a1) + v32) % v31;
    if ( v34[i] != v30 )
      return 0LL;
    *(_BYTE *)(a1 + i) = *(_BYTE *)(i + a1) ^ *(&v2 + i);
  }

 2. make invert function
 3. get the flag!

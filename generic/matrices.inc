int aa2int[26] = {0,1,2,3,4,5,6,7,8,-1,9,10,11,12,-1,13,14,15,16,17,-1,18,19,20,21,22};

int int2aa[23] = {'A','B','C','D','E','F','G','H','I','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z'};

int **load_matrix(char *matName);
void free_matrix(int **mat);
int **load_matrix(char *matName)
{
 int i, **blosum62;

  blosum62 = (int **)ckalloc(23*sizeof(int *));
for (i=0; i<23; i++) {
     blosum62[i] = (int *)ckalloc(23*sizeof(int));
}

  blosum62[0][0]=8; blosum62[0][1]=-4; blosum62[0][2]=0; blosum62[0][3]=-4; blosum62[0][4]=-2; blosum62[0][5]=-4; blosum62[0][6]=0; blosum62[0][7]=-4; blosum62[0][8]=-2; blosum62[0][9]=-2; blosum62[0][10]=-2; blosum62[0][11]=-2; blosum62[0][12]=-4; blosum62[0][13]=-2; blosum62[0][14]=-2; blosum62[0][15]=-2; blosum62[0][16]=2; blosum62[0][17]=0; blosum62[0][18]=0; blosum62[0][19]=-6; blosum62[0][20]=0; blosum62[0][21]=-4; blosum62[0][22]=-2;

  blosum62[1][0]=-4; blosum62[1][1]=8; blosum62[1][2]=-6; blosum62[1][3]=8; blosum62[1][4]=2; blosum62[1][5]=-6; blosum62[1][6]=-2; blosum62[1][7]=0; blosum62[1][8]=-6; blosum62[1][9]=0; blosum62[1][10]=-8; blosum62[1][11]=-6; blosum62[1][12]=6; blosum62[1][13]=-4; blosum62[1][14]=0; blosum62[1][15]=-2; blosum62[1][16]=0; blosum62[1][17]=-2; blosum62[1][18]=-6; blosum62[1][19]=-8; blosum62[1][20]=-2; blosum62[1][21]=-6; blosum62[1][22]=2;

  blosum62[2][0]=0; blosum62[2][1]=-6; blosum62[2][2]=18; blosum62[2][3]=-6; blosum62[2][4]=-8; blosum62[2][5]=-4; blosum62[2][6]=-6; blosum62[2][7]=-6; blosum62[2][8]=-2; blosum62[2][9]=-6; blosum62[2][10]=-2; blosum62[2][11]=-2; blosum62[2][12]=-6; blosum62[2][13]=-6; blosum62[2][14]=-6; blosum62[2][15]=-6; blosum62[2][16]=-2; blosum62[2][17]=-2; blosum62[2][18]=-2; blosum62[2][19]=-4; blosum62[2][20]=-4; blosum62[2][21]=-4; blosum62[2][22]=-6;

  blosum62[3][0]=-4; blosum62[3][1]=8; blosum62[3][2]=-6; blosum62[3][3]=12; blosum62[3][4]=4; blosum62[3][5]=-6; blosum62[3][6]=-2; blosum62[3][7]=-2; blosum62[3][8]=-6; blosum62[3][9]=-2; blosum62[3][10]=-8; blosum62[3][11]=-6; blosum62[3][12]=2; blosum62[3][13]=-2; blosum62[3][14]=0; blosum62[3][15]=-4; blosum62[3][16]=0; blosum62[3][17]=-2; blosum62[3][18]=-6; blosum62[3][19]=-8; blosum62[3][20]=-2; blosum62[3][21]=-6; blosum62[3][22]=2;

  blosum62[4][0]=-2; blosum62[4][1]=2; blosum62[4][2]=-8; blosum62[4][3]=4; blosum62[4][4]=10; blosum62[4][5]=-6; blosum62[4][6]=-4; blosum62[4][7]=0; blosum62[4][8]=-6; blosum62[4][9]=2; blosum62[4][10]=-6; blosum62[4][11]=-4; blosum62[4][12]=0; blosum62[4][13]=-2; blosum62[4][14]=4; blosum62[4][15]=0; blosum62[4][16]=0; blosum62[4][17]=-2; blosum62[4][18]=-4; blosum62[4][19]=-6; blosum62[4][20]=-2; blosum62[4][21]=-4; blosum62[4][22]=8;

  blosum62[5][0]=-4; blosum62[5][1]=-6; blosum62[5][2]=-4; blosum62[5][3]=-6; blosum62[5][4]=-6; blosum62[5][5]=12; blosum62[5][6]=-6; blosum62[5][7]=-2; blosum62[5][8]=0; blosum62[5][9]=-6; blosum62[5][10]=0; blosum62[5][11]=0; blosum62[5][12]=-6; blosum62[5][13]=-8; blosum62[5][14]=-6; blosum62[5][15]=-6; blosum62[5][16]=-4; blosum62[5][17]=-4; blosum62[5][18]=-2; blosum62[5][19]=2; blosum62[5][20]=-2; blosum62[5][21]=6; blosum62[5][22]=-6;

  blosum62[6][0]=0; blosum62[6][1]=-2; blosum62[6][2]=-6; blosum62[6][3]=-2; blosum62[6][4]=-4; blosum62[6][5]=-6; blosum62[6][6]=12; blosum62[6][7]=-4; blosum62[6][8]=-8; blosum62[6][9]=-4; blosum62[6][10]=-8; blosum62[6][11]=-6; blosum62[6][12]=0; blosum62[6][13]=-4; blosum62[6][14]=-4; blosum62[6][15]=-4; blosum62[6][16]=0; blosum62[6][17]=-4; blosum62[6][18]=-6; blosum62[6][19]=-4; blosum62[6][20]=-2; blosum62[6][21]=-6; blosum62[6][22]=-4;

  blosum62[7][0]=-4; blosum62[7][1]=0; blosum62[7][2]=-6; blosum62[7][3]=-2; blosum62[7][4]=0; blosum62[7][5]=-2; blosum62[7][6]=-4; blosum62[7][7]=16; blosum62[7][8]=-6; blosum62[7][9]=-2; blosum62[7][10]=-6; blosum62[7][11]=-4; blosum62[7][12]=2; blosum62[7][13]=-4; blosum62[7][14]=0; blosum62[7][15]=0; blosum62[7][16]=-2; blosum62[7][17]=-4; blosum62[7][18]=-6; blosum62[7][19]=-4; blosum62[7][20]=-2; blosum62[7][21]=4; blosum62[7][22]=0;

  blosum62[8][0]=-2; blosum62[8][1]=-6; blosum62[8][2]=-2; blosum62[8][3]=-6; blosum62[8][4]=-6; blosum62[8][5]=0; blosum62[8][6]=-8; blosum62[8][7]=-6; blosum62[8][8]=8; blosum62[8][9]=-6; blosum62[8][10]=4; blosum62[8][11]=2; blosum62[8][12]=-6; blosum62[8][13]=-6; blosum62[8][14]=-6; blosum62[8][15]=-6; blosum62[8][16]=-4; blosum62[8][17]=-2; blosum62[8][18]=6; blosum62[8][19]=-6; blosum62[8][20]=-2; blosum62[8][21]=-2; blosum62[8][22]=-6;

  blosum62[9][0]=-2; blosum62[9][1]=0; blosum62[9][2]=-6; blosum62[9][3]=-2; blosum62[9][4]=2; blosum62[9][5]=-6; blosum62[9][6]=-4; blosum62[9][7]=-2; blosum62[9][8]=-6; blosum62[9][9]=10; blosum62[9][10]=-4; blosum62[9][11]=-2; blosum62[9][12]=0; blosum62[9][13]=-2; blosum62[9][14]=2; blosum62[9][15]=4; blosum62[9][16]=0; blosum62[9][17]=-2; blosum62[9][18]=-4; blosum62[9][19]=-6; blosum62[9][20]=-2; blosum62[9][21]=-4; blosum62[9][22]=2;

  blosum62[10][0]=-2; blosum62[10][1]=-8; blosum62[10][2]=-2; blosum62[10][3]=-8; blosum62[10][4]=-6; blosum62[10][5]=0; blosum62[10][6]=-8; blosum62[10][7]=-6; blosum62[10][8]=4; blosum62[10][9]=-4; blosum62[10][10]=8; blosum62[10][11]=4; blosum62[10][12]=-6; blosum62[10][13]=-6; blosum62[10][14]=-4; blosum62[10][15]=-4; blosum62[10][16]=-4; blosum62[10][17]=-2; blosum62[10][18]=2; blosum62[10][19]=-4; blosum62[10][20]=-2; blosum62[10][21]=-2; blosum62[10][22]=-6;

  blosum62[11][0]=-2; blosum62[11][1]=-6; blosum62[11][2]=-2; blosum62[11][3]=-6; blosum62[11][4]=-4; blosum62[11][5]=0; blosum62[11][6]=-6; blosum62[11][7]=-4; blosum62[11][8]=2; blosum62[11][9]=-2; blosum62[11][10]=4; blosum62[11][11]=10; blosum62[11][12]=-4; blosum62[11][13]=-4; blosum62[11][14]=0; blosum62[11][15]=-2; blosum62[11][16]=-2; blosum62[11][17]=-2; blosum62[11][18]=2; blosum62[11][19]=-2; blosum62[11][20]=-2; blosum62[11][21]=-2; blosum62[11][22]=-2;

  blosum62[12][0]=-4; blosum62[12][1]=6; blosum62[12][2]=-6; blosum62[12][3]=2; blosum62[12][4]=0; blosum62[12][5]=-6; blosum62[12][6]=0; blosum62[12][7]=2; blosum62[12][8]=-6; blosum62[12][9]=0; blosum62[12][10]=-6; blosum62[12][11]=-4; blosum62[12][12]=12; blosum62[12][13]=-4; blosum62[12][14]=0; blosum62[12][15]=0; blosum62[12][16]=2; blosum62[12][17]=0; blosum62[12][18]=-6; blosum62[12][19]=-8; blosum62[12][20]=-2; blosum62[12][21]=-4; blosum62[12][22]=0;

  blosum62[13][0]=-2; blosum62[13][1]=-4; blosum62[13][2]=-6; blosum62[13][3]=-2; blosum62[13][4]=-2; blosum62[13][5]=-8; blosum62[13][6]=-4; blosum62[13][7]=-4; blosum62[13][8]=-6; blosum62[13][9]=-2; blosum62[13][10]=-6; blosum62[13][11]=-4; blosum62[13][12]=-4; blosum62[13][13]=14; blosum62[13][14]=-2; blosum62[13][15]=-4; blosum62[13][16]=-2; blosum62[13][17]=-2; blosum62[13][18]=-4; blosum62[13][19]=-8; blosum62[13][20]=-4; blosum62[13][21]=-6; blosum62[13][22]=-2;

  blosum62[14][0]=-2; blosum62[14][1]=0; blosum62[14][2]=-6; blosum62[14][3]=0; blosum62[14][4]=4; blosum62[14][5]=-6; blosum62[14][6]=-4; blosum62[14][7]=0; blosum62[14][8]=-6; blosum62[14][9]=2; blosum62[14][10]=-4; blosum62[14][11]=0; blosum62[14][12]=0; blosum62[14][13]=-2; blosum62[14][14]=10; blosum62[14][15]=2; blosum62[14][16]=0; blosum62[14][17]=-2; blosum62[14][18]=-4; blosum62[14][19]=-4; blosum62[14][20]=-2; blosum62[14][21]=-2; blosum62[14][22]=6;

  blosum62[15][0]=-2; blosum62[15][1]=-2; blosum62[15][2]=-6; blosum62[15][3]=-4; blosum62[15][4]=0; blosum62[15][5]=-6; blosum62[15][6]=-4; blosum62[15][7]=0; blosum62[15][8]=-6; blosum62[15][9]=4; blosum62[15][10]=-4; blosum62[15][11]=-2; blosum62[15][12]=0; blosum62[15][13]=-4; blosum62[15][14]=2; blosum62[15][15]=10; blosum62[15][16]=-2; blosum62[15][17]=-2; blosum62[15][18]=-6; blosum62[15][19]=-6; blosum62[15][20]=-2; blosum62[15][21]=-4; blosum62[15][22]=0;

  blosum62[16][0]=2; blosum62[16][1]=0; blosum62[16][2]=-2; blosum62[16][3]=0; blosum62[16][4]=0; blosum62[16][5]=-4; blosum62[16][6]=0; blosum62[16][7]=-2; blosum62[16][8]=-4; blosum62[16][9]=0; blosum62[16][10]=-4; blosum62[16][11]=-2; blosum62[16][12]=2; blosum62[16][13]=-2; blosum62[16][14]=0; blosum62[16][15]=-2; blosum62[16][16]=8; blosum62[16][17]=2; blosum62[16][18]=-4; blosum62[16][19]=-6; blosum62[16][20]=0; blosum62[16][21]=-4; blosum62[16][22]=0;

  blosum62[17][0]=0; blosum62[17][1]=-2; blosum62[17][2]=-2; blosum62[17][3]=-2; blosum62[17][4]=-2; blosum62[17][5]=-4; blosum62[17][6]=-4; blosum62[17][7]=-4; blosum62[17][8]=-2; blosum62[17][9]=-2; blosum62[17][10]=-2; blosum62[17][11]=-2; blosum62[17][12]=0; blosum62[17][13]=-2; blosum62[17][14]=-2; blosum62[17][15]=-2; blosum62[17][16]=2; blosum62[17][17]=10; blosum62[17][18]=0; blosum62[17][19]=-4; blosum62[17][20]=0; blosum62[17][21]=-4; blosum62[17][22]=-2;

  blosum62[18][0]=0; blosum62[18][1]=-6; blosum62[18][2]=-2; blosum62[18][3]=-6; blosum62[18][4]=-4; blosum62[18][5]=-2; blosum62[18][6]=-6; blosum62[18][7]=-6; blosum62[18][8]=6; blosum62[18][9]=-4; blosum62[18][10]=2; blosum62[18][11]=2; blosum62[18][12]=-6; blosum62[18][13]=-4; blosum62[18][14]=-4; blosum62[18][15]=-6; blosum62[18][16]=-4; blosum62[18][17]=0; blosum62[18][18]=8; blosum62[18][19]=-6; blosum62[18][20]=-2; blosum62[18][21]=-2; blosum62[18][22]=-4;

  blosum62[19][0]=-6; blosum62[19][1]=-8; blosum62[19][2]=-4; blosum62[19][3]=-8; blosum62[19][4]=-6; blosum62[19][5]=2; blosum62[19][6]=-4; blosum62[19][7]=-4; blosum62[19][8]=-6; blosum62[19][9]=-6; blosum62[19][10]=-4; blosum62[19][11]=-2; blosum62[19][12]=-8; blosum62[19][13]=-8; blosum62[19][14]=-4; blosum62[19][15]=-6; blosum62[19][16]=-6; blosum62[19][17]=-4; blosum62[19][18]=-6; blosum62[19][19]=22; blosum62[19][20]=-4; blosum62[19][21]=4; blosum62[19][22]=-6;

  blosum62[20][0]=0; blosum62[20][1]=-2; blosum62[20][2]=-4; blosum62[20][3]=-2; blosum62[20][4]=-2; blosum62[20][5]=-2; blosum62[20][6]=-2; blosum62[20][7]=-2; blosum62[20][8]=-2; blosum62[20][9]=-2; blosum62[20][10]=-2; blosum62[20][11]=-2; blosum62[20][12]=-2; blosum62[20][13]=-4; blosum62[20][14]=-2; blosum62[20][15]=-2; blosum62[20][16]=0; blosum62[20][17]=0; blosum62[20][18]=-2; blosum62[20][19]=-4; blosum62[20][20]=-2; blosum62[20][21]=-2; blosum62[20][22]=-2;

  blosum62[21][0]=-4; blosum62[21][1]=-6; blosum62[21][2]=-4; blosum62[21][3]=-6; blosum62[21][4]=-4; blosum62[21][5]=6; blosum62[21][6]=-6; blosum62[21][7]=4; blosum62[21][8]=-2; blosum62[21][9]=-4; blosum62[21][10]=-2; blosum62[21][11]=-2; blosum62[21][12]=-4; blosum62[21][13]=-6; blosum62[21][14]=-2; blosum62[21][15]=-4; blosum62[21][16]=-4; blosum62[21][17]=-4; blosum62[21][18]=-2; blosum62[21][19]=4; blosum62[21][20]=-2; blosum62[21][21]=14; blosum62[21][22]=-4;

  blosum62[22][0]=-2; blosum62[22][1]=2; blosum62[22][2]=-6; blosum62[22][3]=2; blosum62[22][4]=8; blosum62[22][5]=-6; blosum62[22][6]=-4; blosum62[22][7]=0; blosum62[22][8]=-6; blosum62[22][9]=2; blosum62[22][10]=-6; blosum62[22][11]=-2; blosum62[22][12]=0; blosum62[22][13]=-2; blosum62[22][14]=6; blosum62[22][15]=0; blosum62[22][16]=0; blosum62[22][17]=-2; blosum62[22][18]=-4; blosum62[22][19]=-6; blosum62[22][20]=-2; blosum62[22][21]=-4; blosum62[22][22]=8;

  if (strcmp(matName,"blosum62") == 0) {
    return blosum62;
  };

  /* unknown matrix name: free and return NULL (the caller reports the
   * error) instead of falling off the end of a non-void function */
  free_matrix(blosum62);
  return NULL;
}

void free_matrix(int **mat)
{
  int i;

  if (mat == NULL) return;
  for (i=0; i<23; i++) {
    ckfree((char *)mat[i]);
  }
  ckfree((char *)mat);
}


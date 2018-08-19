#include <stdio.h>

int getSetbitStr(int S,int* str){
	int i,count;

	for(i=0,count=0;S!=0;i++,S>>=1){
		if(S&1){
			str[count++] = i; 
		}
	}
	return count;
}
int setBitNumber(int n)
{
	if (n == 0)
		return 0;

	int msb = 0;
	while (n != 0) {
		n = n / 2;
		msb++;
	}

	return msb-1;
}

int moduloReduction(int F,int S,int *remainder){
	int A = setBitNumber(F);
	int B = setBitNumber(S);
	int Stemp;
	int quotient = 0;
	while(A>=B && F!=0){
		Stemp = S<<(A-B);
		quotient^=(1<<A-B);
		F^=Stemp;
		A = setBitNumber(F);
	}
	*remainder = F;
	return quotient;
}

int multiplyPolyMod2(int F, int S){
	int i,count;
	int str[100];
	count = getSetbitStr(S,str);
	
	int Result = 0;

	for(i=0;i<count;i++){
		int num = str[i];
		Result^= F<<num;
	}

	return Result;
}

int inverse(int a, int p){
	int t = 0;
	int newt = 1;
	int r = p;
	int newr = a;
	int quotient,rem;
	int r1,r2;
	while(newr){
		quotient = moduloReduction(r,newr,&rem);
		
		r1 = newr;
		r2 = r ^ multiplyPolyMod2(quotient,newr);
		r = r1;
		newr = r2;
		r1 = newt;
		r2 = t ^ multiplyPolyMod2(quotient,newt);
		t = r1;
		newt = r2;
		
	}
	return t;
}

void ascii2binINT(int c,int res[]){
	int i = 0, temp[8];
	int n = c;

	if(n==0){
		for(int l=0;l<8;l++){
			res[l] = 0;
		}
		return;
	}

	while(n){
		temp[i] = n%2;
		n = n>>1;
		if(n)i++;
	}

	while(i<8){
		temp[++i] = 0;
	}

	int k = 7;
	i = 0;
	while(k>=0){
		res[k--] = temp[i++];
	}

	
}

int bin2dec(int A[], int n){
	int pos = n-1;
	int m = 0;
	int num = 0;
	while(pos>=0){
		num+=(A[pos]<<m);
		pos--;
		m++;
	}
	return num;
}

void reverse(int A[],int n){
	int i = 0,j = n-1;
	int temp;
	for(;i<j;i++,j--){
		temp = A[i];
		A[i] = A[j];
		A[j] = temp;
	}
}

int subBytes(int a){
	int z;

	if(a!=0){
		z = inverse(a,283);
	}
	else{
		z = 0;
	}
	int c = 99;

	int A[8],C[8];
	ascii2binINT(z,A);
	ascii2binINT(c,C);

	reverse(A,8);
	reverse(C,8);

	int B[8];

	for(int i=0;i<8;i++){
		B[i] = (A[i]+A[(i+4)%8]+A[(i+5)%8]+A[(i+6)%8]+A[(i+7)%8]+C[i])%2;
	}

	reverse(B,8);

	int b = bin2dec(B,8);
	return b;
}

void string2intArr(char S[],int A[]){
	int count = 0;
	while(S[count++]!='\0');
	count--;
	
	if(count!=16){
		printf("Error!!!, entered string is not 16 chars long!\n");
		return;
	}
	for(int i=0;i<16;i++){
		ascii2binINT((int)(S[i]),A+i*8);
	}
}

void addroundkey(int State[],int K[],int n){

	for(int i=0;i<n;i++){
		State[i]^=K[i];
	}
}

void shiftRow(int (*State)[4],int m,int n){
	
	int row,col;

	int Stemp[4][4];

	for(row=0;row<m;row++){
		for(col=0;col<n;col++){
		
			Stemp[row][col] = State[row][col];
		}
	}


	for(row=0;row<m;row++){
		for(col=0;col<n;col++){

			State[row][col] = Stemp[row][(col+row)%4];
		
		}
	}
}

void mixColumn(int (*State)[4],int C){
	
	int T[4];
	for(int i=0;i<4;i++){
		T[i] = State[i][C];
	}

	int quotient;

	int term[4],rem;
	

	for(int i=0;i<4;i++){
		term[0] = multiplyPolyMod2(2,T[i]);
		quotient = moduloReduction(term[0],283,&rem);
		term[0] = rem;

		term[1] = multiplyPolyMod2(3,T[(i+1)%4]);
		quotient = moduloReduction(term[1],283,&rem);
		term[1] = rem;

		term[2] = T[(i+2)%4];

		term[3] = T[(i+3)%4];

		State[i][C] = (term[0] ^ term[1]) ^ (term[2] ^ term[3]);
	}


}

void RotWord(int temp[]){

	int ytemp[4];

	for(int i=0;i<4;i++)
		ytemp[i] = temp[i];

	for(int i=0;i<4;i++){
		temp[i] = ytemp[(i+1)%4];
	}
}

void SubWord(int temp[]){
	for(int i=0;i<4;i++){
		temp[i] = subBytes(temp[i]);
	}
}

void getRoundKey1(int K0[],int K1[]){

	int W[8][4];

	int RCon[4];

	RCon[0] = 1;
	RCon[1] = 0;
	RCon[2] = 0;
	RCon[3] = 0;


	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			W[i][j] = bin2dec(K0+8*j+32*i,8);
		}
	}
	int temp[4];
	for(int i=4;i<8;i++){
		for(int j=0;j<4;j++){
			temp[j] = W[i-1][j];
		}
		if((i%4)==0){

			RotWord(temp);
			SubWord(temp);

			for(int j=0;j<4;j++){
				temp[j] = temp[j]^RCon[j];
			}
		}
		for(int j=0;j<4;j++){
			W[i][j] = W[i-4][j] ^ temp[j];
		}
	}

	
	for(int i=4;i<8;i++){
		for(int j=0;j<4;j++){
			ascii2binINT(W[i][j], K1+(i-4)*32+j*8); 
		}
	}
}

char getHEX(int a,int b,int c,int d){
	int Val;
	char res;

	Val = d + (c<<1) + (b<<2) + (a<<3);

	if(Val<=9){
		res = (char)(Val + '0');
		return res;
	}
	else{
		res = (char)(Val-10+'A');
		return res;
	}
}

void hex2binchar(char c, int res[]){
	int Val;
	if(c>='0' && c<='9'){
		Val = (int)(c - '0');
	}
	else{
		Val = (int)(c - 'A')+10;
	}

	int r[8];
	ascii2binINT(Val,r);

	for(int i = 4; i<8; i++){
		res[i-4] = r[i];
		
	}
	
}

void hex2bin(char Key[],int K0[]){
	
	int count = 0;
	while(Key[count++]!='\0');
	count--;
	if(count!=32){
		printf("Error!!!, entered Key is not 32 chars long!\n");
		return;
	}

	for(int i=0;i<32;i++){
		hex2binchar(Key[i],K0+4*i);
	}
}

void encrypt(char Plain[],char Key[], char Cipher[]){

	int P[128];
	int K0[128];

	string2intArr(Plain,P);
	hex2bin(Key,K0);

	addroundkey(P,K0,128);

	int Sta[16];
	
	for(int i=0;i<16;i++){
		Sta[i] = bin2dec(P+8*i,8);
		Sta[i] = subBytes(Sta[i]);
	}
	int State[4][4];

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			State[i][j] = Sta[i+4*j];
		}
	}

	shiftRow(State,4,4);

	for(int c=0;c<4;c++)
		mixColumn(State,c);


	int K1[128];
	int C[128];


	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			ascii2binINT(State[i][j],C+8*i+32*j) ;
		}
	}


	getRoundKey1(K0,K1);

	addroundkey(C,K1,128);

	for(int i=0;i<32;i++){
		Cipher[i] = getHEX(C[4*i],C[4*i+1],C[4*i+2],C[4*i+3]);
	}
	Cipher[32] = '\0';
}

int main(){
	char Plain[] = "Let's go to IMAX"; //Plain is of 16 chars, i.e. 128 bits

	char Key[] =   "4559706564315C79204B755E66204675"; //Key is in hex of 32 length, i.e. 128 bits

	char Cipher[33];					//Cipher will be in hexadecimal, i.e. 32*4 = 128 bits
	encrypt(Plain,Key,Cipher);

	printf("%s\n",Cipher);


	return 0;

}
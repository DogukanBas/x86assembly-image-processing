#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short* resimdizi_org = (short*)malloc(N * M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i * N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf("%d", &filter_size);

	switch (menu) {
	case 1:
		Dilation(N * M, filter_size, resimdizi_org);
		resimYaz(dil_resim, resimdizi_org, N, M, Q);
		break;
	case 2:
		Erosion(N * M, filter_size, resimdizi_org);
		resimYaz(ero_resim, resimdizi_org, N, M, Q);
		break;
	case 3:
		system("EXIT");
		break;
	default:
		system("EXIT");
		break;
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* resim_org) {
	/*for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			resim_org[i][j] = 255;*/
	__asm {
		MOV ECX, n
		MOV ESI, resim_org
		XOR EBX, EBX
		L1 :
			MOV EDI, 512
			MOV EAX, EBX
			XOR EDX, EDX
			DIV EDI
			; EAX[i][j] eax i   edx j
			MOV EDI, filter_size
			SHR EDI, 1
			ADD EAX, EDI
			CMP EAX, 512
			JAE SONUC
			SUB EAX, EDI
			CMP EDI, EAX
			JAE SONUC
			ADD EDX, EDI
			CMP EDX, 512
			JAE SONUC
			SUB EDX, EDI
			CMP EDI, EDX
			JAE SONUC
			JMP DEVAM
			TEMP : JMP L1
		DEVAM :
			XOR EDX, EDX
			XOR EAX, EAX
			PUSH ECX
			XOR ECX, ECX
			MOV CL, 9
			SHL EDI, CL
			PUSH EBX
			SUB EBX, EDI
			SHR EDI, CL
			MOV ECX, filter_size
			SUTUN :
				PUSH ECX
				MOV ECX, filter_size
				PUSH EBX
				SUB EBX, EDI
			SATIR :
					SHL EBX, 1
					MOV AX, WORD PTR[EBX + ESI]
					SHR EBX, 1
					CMP DX, AX
					JAE DEGISME
					MOV DX, AX
					DEGISME :
					ADD EBX, 1
				LOOP SATIR
				POP EBX
				POP ECX
				ADD EBX, 512
			LOOP SUTUN
			POP EBX
			POP ECX
			PUSH DX


			jmp EXIT

		SONUC :
			SHL EBX, 1
			MOV AX, WORD PTR[EBX + ESI]
			PUSH AX
			SHR EBX, 1
		
		EXIT :
			ADD EBX, 1
			LOOP TEMP

		MOV ECX, n
		XOR EAX, EAX
		MOV EBX, resim_org
		ADD EBX, n
		ADD EBX, n

		L2 :
			sub EBX, 2
			pop(AX)
			MOV WORD PTR[EBX], ax
		loop l2

	}
	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {
	__asm {
		//KODUNUZU BURAYA YAZINIZ, ASAGIDAKI 5 ORNEK ERISIM ICIN VERILMISTIR SIZ YAZMAYA BASLAMADAN SILINIZ
		MOV ECX, n
		MOV ESI, resim_org
		XOR EBX, EBX
		L1 :
			MOV EDI, 512
			MOV EAX, EBX
			XOR EDX, EDX
			DIV EDI
			; EAX[i][j] eax i   edx j
			MOV EDI, filter_size
			SHR EDI, 1
			ADD EAX, EDI
			CMP EAX, 512
			JAE SONUC
			SUB EAX, EDI
			CMP EDI, EAX
			JAE SONUC
			ADD EDX, EDI
			CMP EDX, 512
			JAE SONUC
			SUB EDX, EDI
			CMP EDI, EDX
			JAE SONUC
			JMP DEVAM
		TEMP : JMP L1
		DEVAM :
			MOV EDX,255 
			XOR EAX, EAX
			PUSH ECX
			XOR ECX, ECX
			MOV CL, 9
			SHL EDI, CL
			PUSH EBX
			SUB EBX, EDI
			SHR EDI, CL
			MOV ECX, filter_size
		SUTUN :
				PUSH ECX
				MOV ECX, filter_size
				PUSH EBX
				SUB EBX, EDI
				SATIR :
					SHL EBX, 1
					MOV AX, WORD PTR[EBX + ESI]
					SHR EBX, 1
					CMP AX, DX 
					JAE DEGISME
					MOV DX, AX
					DEGISME :
					ADD EBX, 1
				LOOP SATIR
				POP EBX
				POP ECX
				ADD EBX, 512
			LOOP SUTUN
			POP EBX
			POP ECX
			PUSH DX
			jmp EXIT
	SONUC : SHL EBX, 1
			MOV AX, WORD PTR[EBX + ESI]
			PUSH AX
			SHR EBX, 1
		
	EXIT :
			ADD EBX, 1
			LOOP TEMP


		MOV ECX, n
		XOR EAX, EAX
		MOV EBX, resim_org
		ADD EBX, n
		ADD EBX, n


	L2 :
		sub EBX, 2
		pop(AX)
		MOV WORD PTR[EBX], ax
		loop l2

	}
	
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}

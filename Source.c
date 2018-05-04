#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
/*
ibrahim kaya 02.05.2018
variable name changes upcoming 
*/
int hash(int);
void update(struct index sabit, struct index sabitTemp, int satirNo);
void updateData(struct data sabitD, struct data sabitDtemp, int satirNo);
void ara(int siraNo, int key, struct index sabitTemp);
void showData(int bucketNo, int siraNo);

struct index {
	int satirNo;
	int b1;
	int b2;
	int b3;
	int b4;
	int b5; // keylerin de�erli textte i�lem yapmak i�in 
};
struct data {
	int satirNo;
	char b1Yn[30]; char b1Ki[30]; //birinci kitap yazar ismi ,birinci kitap kitap ismi 
	char b2Yn[30]; char b2Ki[30];
	char b3Yn[30]; char b3Ki[30];
	char b4Yn[30]; char b4Ki[30];
	char b5Yn[30]; char b5Ki[30];
};

void main() {

	FILE *indexPtr;
	struct index sabit = { 1, 9999, 9999, 9999, 9999,9999 }; // ilk de�er sat�r de�er di�erleri kitap key de�erleri 
															 //burada 9999 de�erini null olarak yani de�er girilmemi� olarak alg�l�yorum baz� derleyiciler int e null de�er verirken sorun yarat�yor
	struct index sabitTemp = { 1, 8888, 8888, 8888, 8888, 8888 };
	FILE *dataPtr;

	struct data sabitD = { 1,"NULL","NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" };
	struct data sabitDtemp = { 1,"NULL","NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" ,"NULL" };
	if ((indexPtr = fopen("index.dat", "rb+")) == NULL) {// bu kontrol dosya e�er yoksa dosyay� yazma modunda a��p yaz varsa yapma
		if ((indexPtr = fopen("index.dat", "wb+ ")) == NULL) {
			printf("File could not be opened.\n");
		}
		else {


			for (int x = 1; x <= 30; x++) { //30 sat�r NULL yazd�rmayak i�in 

				fwrite(&sabit, sizeof(struct index), 1, indexPtr);
				sabit.satirNo++;
			}
		}

	}

	if ((dataPtr = fopen("data.dat", "rb")) == NULL) {// bu kontrol dosya e�er yoksa dosyay� yazma modunda a��p yaz varsa yapma
		if ((dataPtr = fopen("data.dat", "wb+")) == NULL) {
			printf("File could not be opened.\n");
		}
		else {

			for (int x = 1; x <= 30; x++) { //30 sat�r NULL yazd�rmayak i�in 

				fwrite(&sabitD, sizeof(struct data), 1, dataPtr);
				sabitD.satirNo++;

			}
		}
	}
	fclose(indexPtr);
	fclose(dataPtr);


	struct kitapBilgi { //kullan�c�dan ald���m�z bilgiler i�in kullan�cam sonras�nda dosyaya aktarmak i�in 
		int kitapNo;
		int key;
		char kitapismi[30];
		char yazarismi[30];
	};
	struct kitapBilgi kitap = { 0,0,"","" };


	int menu = 1;

	int secim = 0;
	int key = 0, siraNo; char kitapismi[30], yazarismi[30];

	secim = 0;

	printf("1-List all the bookshelf \n2-Insert a book \n3-Search a book\n4-Exit ");
	scanf("%d", &secim);
	switch (secim) {
	case 1:
		printf("shelf number|book key 1     |book key 2     |book key 3      |book key 4     |book key 5 \n");
		indexPtr = fopen("index.dat", "rb+");

		for (int i = 0; i < 30; i++) {
			fread(&sabit, sizeof(struct index), 1, indexPtr);
			printf("%d               %d             %d              %d             %d           %d\n", sabit.satirNo, sabit.b1, sabit.b2, sabit.b3, sabit.b4, sabit.b5);
		}
		fclose(indexPtr);

		dataPtr = fopen("data.dat", "rb+");

		printf("shelf N. |Author 1 Book 1|Author 2	Book 2|Author 3	Book 4|Author 4	Book 4|Author 5	Book 5|\n");

		for (int i = 0; i < 30; i++) {
			fread(&sabitD, sizeof(struct data), 1, dataPtr);
			printf("%d          %s     %s     %s     %s     %s     %s     %s     %s     %s     %s     \n", sabitD.satirNo, sabitD.b1Yn, sabitD.b1Ki,
				sabitD.b2Yn, sabitD.b2Ki, sabitD.b3Yn, sabitD.b3Ki, sabitD.b4Yn, sabitD.b4Ki, sabitD.b5Yn, sabitD.b5Ki);
		}
		fclose(dataPtr);
		break;
	case 2:
		indexPtr = fopen("index.dat", "ab+");
		dataPtr = fopen("data.dat", "ab+");
		rewind(indexPtr);
		rewind(dataPtr);
		printf("insert a book no ,book name and author name, (eg for name:The_Lord_of_rings)\n");



		scanf("%d %s %s ", &key, kitapismi, yazarismi);



		siraNo = hash(key);//s�ra no yu keyden ge�irerek kitab�n s�ras�n� buluyorum
		kitap.key = key;


		strcpy(kitap.kitapismi, kitapismi);
		strcpy(kitap.yazarismi, yazarismi);



		//fseek ile poiteri do�ru yere al , o yerdeki bilgiyi sabit bilgiye al�yorum sonra yer bo� mu diye bak�yorum (5x kez ) bucket kontrol�
		//bo�sa sabit deki bilgiye yeninisi giriyorum
		// sonra ayn� yere sabiti yazd�r�yorum 

		fseek(indexPtr, (siraNo - 1) * sizeof(struct index), SEEK_SET);//do�ru yere dosya imlecini al�yorum
		fread(&sabitTemp, sizeof(struct index), 1, indexPtr); //de�i�tirece�im satirdaki bilgileri al�yorum

		fseek(dataPtr, (siraNo - 1) * sizeof(struct data), SEEK_SET);
		fread(&sabitDtemp, sizeof(struct data), 1, dataPtr);

		fclose(indexPtr);
		fclose(dataPtr);
		//key kontrol� 5x 

		if (sabitTemp.b1 == 9999) {//dosyadan okudu�u de�er  9999 yani null ise  yeni struct in ilgili yerine benim keyimi atas�n ve tekrar yazs�n 
			sabitTemp.b1 = kitap.key;
			//�rne�in birinci bucket bo� ise struct yap�s�n�n birinci bucket e yeni gelen kitap bilgilerini yaz�yorum
			strcpy(sabitDtemp.b1Ki, kitap.kitapismi); //girilen kitap bilgisini bo� bucket daki bilgiye kopyal�yorum
			strcpy(sabitDtemp.b1Yn, kitap.yazarismi);

			update(sabit, sabitTemp, siraNo);
			updateData(sabitD, sabitDtemp, siraNo);
		}
		else if (sabitTemp.b2 == 9999) {

			sabitTemp.b2 = kitap.key;
			strcpy(sabitDtemp.b2Ki, kitap.kitapismi);
			strcpy(sabitDtemp.b2Yn, kitap.yazarismi);

			update(sabit, sabitTemp, siraNo);
			updateData(sabitD, sabitDtemp, siraNo);
		}
		else if (sabitTemp.b3 == 9999) {
			sabitTemp.b3 = kitap.key;
			strcpy(sabitDtemp.b3Ki, kitap.kitapismi);
			strcpy(sabitDtemp.b3Yn, kitap.yazarismi);

			update(sabit, sabitTemp, siraNo);
			updateData(sabitD, sabitDtemp, siraNo);
		}
		else if (sabitTemp.b4 == 9999) {
			sabitTemp.b4 = kitap.key;
			strcpy(sabitDtemp.b4Ki, kitap.kitapismi);
			strcpy(sabitDtemp.b4Yn, kitap.yazarismi);

			update(sabit, sabitTemp, siraNo);
			updateData(sabitD, sabitDtemp, siraNo);

		}
		else if (sabitTemp.b5 == 9999) {
			sabitTemp.b5 = kitap.key;
			strcpy(sabitDtemp.b5Ki, kitap.kitapismi);
			strcpy(sabitDtemp.b5Yn, kitap.yazarismi);

			update(sabit, sabitTemp, siraNo);
			updateData(sabitD, sabitDtemp, siraNo);
		}
		else {
			printf("shelf full");
		}



		break;

	case 3:
		printf("book no are the looking for");
		scanf("%d", &key);
		siraNo = hash(key);
		//void ara(int siraNo, int key, struct index sabitTemp);

		ara(siraNo, key, sabitTemp);
		break;
	case 4:
		menu = 0;
		break;
	}




	printf("\n");
	system("PAUSE");

}



int hash(int x) {//hash fonksiyonu

	x = x % 30;
	return x;
}

void update(struct index sabit, struct index sabitTemp, int satirNo) {

	FILE *indexPtr;
	FILE *indexPtrNew;
	indexPtr = fopen("index.dat", "rb+");
	indexPtrNew = fopen("indexNew.dat", "wb+");


	for (int i = 0; i<30; i++) {//g�ncellenecek bilgi ise temp deki de�i�tirilmi� bilgiyi de�il ise eski dosyadan okudu�unu yeni dosyaya yaz
		if (satirNo == sabit.satirNo + 1) {//hangi satiri g�ncellemek istiyorsam o satira gelince gecici struct yap�s�ndaki bilgiyi yeni dosyaya yaz�yorum
			fread(&sabit, sizeof(struct index), 1, indexPtr);
			fwrite(&sabitTemp, sizeof(struct index), 1, indexPtrNew);



		}
		else {

			fread(&sabit, sizeof(struct index), 1, indexPtr);
			fwrite(&sabit, sizeof(struct index), 1, indexPtrNew);
		}


	}
	fclose(indexPtr);
	fclose(indexPtrNew);

	//eski dosyay� silme 
	int ret;
	ret = remove("index.dat");
	//yeni yaratt���m dosyan�n ismini de�i�tirme

	char oldname[] = "indexNew.dat";
	char newname[] = "index.dat";

	ret = rename(oldname, newname);
	if (ret == 0) {
		printf("insertion is successful");
	}
	else {
		printf("Error: unable to insert");
	}


}


void updateData(struct data sabitD, struct data sabitDtemp, int satirNo) {
	FILE* dataPtr;
	FILE* dataPtrNew;

	dataPtr = fopen("data.dat", "rb+");
	dataPtrNew = fopen("dataYeni.dat", "wb+");
	for (int i = 0; i<30; i++) {
		if (satirNo == sabitD.satirNo + 1) {
			fwrite(&sabitDtemp, sizeof(struct data), 1, dataPtrNew);
			fread(&sabitD, sizeof(struct data), 1, dataPtr);
		}
		else
		{
			fread(&sabitD, sizeof(struct data), 1, dataPtr);

			fwrite(&sabitD, sizeof(struct data), 1, dataPtrNew);

		}

	}
	fclose(dataPtr);
	fclose(dataPtrNew);


	//eski dosyay� silme 
	int ret;
	ret = remove("data.dat");
	//yeni yaratt���m dosyan�n ismini de�i�tirme

	char oldname[] = "dataYeni.dat";
	char newname[] = "data.dat";

	ret = rename(oldname, newname);

	if (ret == 0) {
		printf("insertion is successful");
	}
	else {
		printf("Error: unable to insert");
	}


}


void ara(int siraNo, int key, struct index sabitTemp) {
	FILE* indexPtr;
	indexPtr = fopen("index.dat", "rb+");
	fseek(indexPtr, (siraNo - 1) * sizeof(struct index), SEEK_SET);
	fread(&sabitTemp, sizeof(struct index), 1, indexPtr);
	int bucketNo = 0;
	if (sabitTemp.b1 == key) {
		bucketNo = 1;
		showData(bucketNo, siraNo);

	}
	else if (sabitTemp.b2 == key)
	{
		bucketNo = 2;
		showData(bucketNo, siraNo);
	}
	else if (sabitTemp.b3 == key)
	{
		bucketNo = 3;
		showData(bucketNo, siraNo);
	}
	else if (sabitTemp.b4 == key)
	{
		bucketNo = 4;
		showData(bucketNo, siraNo);
	}
	else if (sabitTemp.b5 == key)
	{
		bucketNo = 5;
		showData(bucketNo, siraNo);
	}
	else
	{
		printf("can't found book\n");
	}
	fclose(indexPtr);

}

void showData(int bucketNo, int siraNo) {
	FILE* dataPtr;
	dataPtr = fopen("data.dat", "rb+");
	struct data sabitDtemp;
	fseek(dataPtr, (siraNo - 1) * sizeof(struct data), SEEK_SET);
	fread(&sabitDtemp, sizeof(struct data), 1, dataPtr);

	switch (bucketNo)
	{
	case 1:
		printf(" shelf: %d \nrow: %d \nbook name: %s   author name: %s \n", siraNo, bucketNo, sabitDtemp.b1Ki, sabitDtemp.b1Yn);
		break;

	case 2:
		printf(" shelf: %d \nrow: %d \nbook name: %s   author name: %s\n", siraNo, bucketNo, sabitDtemp.b2Ki, sabitDtemp.b2Yn);
		break;
	case 3:
		printf(" shelf: %d \nrow: %d \nbook name: %s   author name: %s \n", siraNo, bucketNo, sabitDtemp.b3Ki, sabitDtemp.b3Yn);
		break;
	case 4:
		printf(" shelf: %d \nrow: %d \nbook name: %s   author name: %s \n", siraNo, bucketNo, sabitDtemp.b4Ki, sabitDtemp.b4Yn);
		break;
	case 5:
		printf(" shelf: %d \nrow: %d \nbook name: %s   author name: %s \n", siraNo, bucketNo, sabitDtemp.b5Ki, sabitDtemp.b5Yn);
		break;
	default:
		break;
	}
	fclose(dataPtr);


}



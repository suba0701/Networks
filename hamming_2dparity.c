#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdarg.h>

//=====================================================================
// OUTPUT FILE HANDLING
//=====================================================================

FILE *outFile;

void myPrintf(const char *fmt, ...)
{
    va_list argsScreen, argsFile;
    va_start(argsScreen, fmt);
    va_copy(argsFile, argsScreen);
    vprintf(fmt, argsScreen);
    if(outFile != NULL)
        vfprintf(outFile, fmt, argsFile);
    va_end(argsScreen);
    va_end(argsFile);
}
#define printf myPrintf

//---------------- URL TABLE ----------------
struct URL
{
    char url[30];
    char srcIP[20];
    char srcMAC[20];
};

struct URL table[5]=
{
    {"google.com", "192.168.1.10", "AA:BB:CC:11:22:33"},
    {"gmail.com", "192.168.1.20", "11:22:33:44:55:66"},
    {"yahoo.com", "10.0.0.1", "12:34:56:78:90:AB"},
    {"youtube.com", "142.250.72.14", "A4:5E:60:D3:2B:19"},
    {"facebook.com", "157.240.22.35", "F0:2F:74:6B:88:11"}
};

//---------------- LINEAR SEARCH FUNCTION ----------------
int findURLIndex(char url[])
{
    int i;
    for(i = 0; i < 5; i++)
    {
        if(strcmp(table[i].url, url) == 0)
        {
            return i;
        }
    }
    return -1;
}

//---------------- DECIMAL TO BINARY ----------------
void decimalToBinary(int num, char bin[], int size)
{
    int i;
    bin[size]='\0';
    for(i=size-1; i>=0; i--)
    {
        bin[i]=(num%2)+'0';
        num=num/2;
    }
}

//---------------- BINARY TO DECIMAL ----------------
int binaryToDecimal(char bin[])
{
    int i, value=0;
    int len=strlen(bin);
    for(i=0;i<len;i++)
    {
        value = value*2 + (bin[i]-'0');
    }
    return value;
}

//---------------- ASCII TO BINARY ----------------
void asciiToBinary(char ch, char bin[])
{
    decimalToBinary((int)ch, bin, 8);
}

//---------------- IP TO BINARY ----------------
void ipToBinary(char ip[], char result[])
{
    int a,b,c,d;
    char temp[9];
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    decimalToBinary(a, temp, 8);
    strcpy(result, temp);
    decimalToBinary(b, temp, 8);
    strcat(result, temp);
    decimalToBinary(c, temp, 8);
    strcat(result, temp);
    decimalToBinary(d, temp, 8);
    strcat(result, temp);
}

//---------------- MAC HEX TO BINARY ----------------
void macToBinary(char mac[], char result[])
{
    int i;
    result[0]='\0';
    for(i=0; i<strlen(mac); i++)
    {
        if(mac[i]!=':')
        {
            int value;
            if(mac[i]>='0' && mac[i]<='9')
                value=mac[i]-'0';
            else if(mac[i]>='A' && mac[i]<='F')
                value=mac[i]-'A'+10;
            else if(mac[i]>='a' && mac[i]<='f')
                value=mac[i]-'a'+10;
            else
                continue;
            char temp[5];
            decimalToBinary(value, temp, 4);
            strcat(result, temp);
        }
    }
}

//---------------- FILE READ ----------------
void readFile(char filename[], char data[])
{
    FILE *fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("File not found\n");
        exit(0);
    }
    int i=0;
    int ch;
    while((ch=fgetc(fp))!=EOF && i < 499)
    {
        data[i]=(char)ch;
        i++;
    }
    data[i]='\0';
    fclose(fp);
}

//---------------- ASCII FILE TO BINARY ----------------
void fileAsciiToBinary(char data[], char binary[])
{
    char temp[9];
    int i;
    binary[0]='\0';
    for(i=0; i<strlen(data); i++)
    {
        asciiToBinary(data[i], temp);
        strcat(binary, temp);
    }
}

//---------------- PORT GENERATION ----------------
void generatePort(int *src, int *dst)
{
    srand((unsigned)time(NULL));
    *src=1000+rand()%50000;
    *dst=1000+rand()%50000;
}

//---------------- PACKET CREATION WITH PADDING ----------------
int createPackets(char data[], char srcIP[], char dstIP[])
{
    int count=0;
    int i, j;
    char srcIPBinary[50];
    char dstIPBinary[50];
    ipToBinary(srcIP, srcIPBinary);
    ipToBinary(dstIP, dstIPBinary);
    printf("\nNETWORK LAYER");
    printf("\n--------------\n");
    for(i=0; i<strlen(data); i=i+16)
    {
        count++;
        printf("\nPacket %d:\n", count);
        printf("Data : ");
        for(j=0; j<16; j++)
        {
            if((i + j) < strlen(data))
                printf("%c", data[i + j]);
            else
                printf("0");
        }
        printf("\n");
        printf("Src IP : %s\n", srcIPBinary);
        printf("Dst IP : %s\n", dstIPBinary);
    }
    return count;
}

//---------------- TRAILER GENERATION ----------------
void generateTrailer(char data[], char trailer[])
{
    int i;
    for(i=0; i<8; i++)
    {
        if(data[i]=='0')
            trailer[i]='1';
        else
            trailer[i]='0';
    }
    trailer[8]='\0';
}

//---------------- FRAME CREATION WITH PADDING & TRAILER ----------------
int createFrames(char data[], char srcIP[], char dstIP[], char srcMAC[], char dstMAC[])
{
    int frameCount=0;
    int i, j;
    char srcIPBinary[50];
    char dstIPBinary[50];
    char srcMACBinary[100];
    char dstMACBinary[100];
    char currentFrameData[9];
    char trailer[9];
    ipToBinary(srcIP, srcIPBinary);
    ipToBinary(dstIP, dstIPBinary);
    macToBinary(srcMAC, srcMACBinary);
    macToBinary(dstMAC, dstMACBinary);
    printf("\nDATA LINK LAYER");
    printf("\n----------------\n");
    for(i=0; i<strlen(data); i=i+8)
    {
        frameCount++;
        printf("\nFrame %d:\n", frameCount);
        for(j=0; j<8; j++)
        {
            if((i + j) < strlen(data))
                currentFrameData[j] = data[i + j];
            else
                currentFrameData[j] = '0';
        }
        currentFrameData[8] = '\0';
        generateTrailer(currentFrameData, trailer);
        printf("Data : %s\n", currentFrameData);
        printf("Src IP : %s\n", srcIPBinary);
        printf("Dst IP : %s\n", dstIPBinary);
        printf("Src MAC : %s\n", srcMACBinary);
        printf("Dst MAC : %s\n", dstMACBinary);
        printf("Trailer : %s\n", trailer);
    }
    return frameCount;
}

void displayTable()
{
    int i;
    printf("\nURL TABLE\n");
    printf("-----------------------------------------------\n");
    printf("URL\t\tSource IP\t\tSource MAC\n");
    printf("-----------------------------------------------\n");
    for(i=0; i<5; i++)
    {
        printf("%s\t%s\t%s\n", table[i].url, table[i].srcIP, table[i].srcMAC);
    }
    printf("-----------------------------------------------\n");
}

//=====================================================================
// BIT STUFFING / DESTUFFING
//=====================================================================

void bitStuffing(char data[], char stuffed[])
{
    int i, ones = 0, k = 0;
    int len = strlen(data);

    for(i = 0; i < len; i++)
    {
        stuffed[k++] = data[i];
        if(data[i] == '1')
        {
            ones++;
            if(ones == 5)
            {
                stuffed[k++] = '0';
                ones = 0;
            }
        }
        else
        {
            ones = 0;
        }
    }
    stuffed[k] = '\0';
}

void bitDestuffing(char stuffed[], char destuffed[])
{
    int i, ones = 0, k = 0;
    int len = strlen(stuffed);

    for(i = 0; i < len; i++)
    {
        destuffed[k++] = stuffed[i];
        if(stuffed[i] == '1')
        {
            ones++;
            if(ones == 5)
            {
                i++;
                ones = 0;
            }
        }
        else
        {
            ones = 0;
        }
    }
    destuffed[k] = '\0';
}

//=====================================================================
// PPP BYTE STUFFING / DESTUFFING
//=====================================================================

#define FLAG_BYTE 01111110
#define ESC_BYTE 01111101

void byteStuffingPPP(char data[], char stuffed[])
{
    int len=strlen(data);
    int nbytes=len/8;
    int b, k=0;
    char byteStr[9];
    char escStr[9];
    decimalToBinary(01111101, escStr, 8);

    for(b=0; b<nbytes; b++)
    {
        strncpy(byteStr, data+(b*8), 8);
        byteStr[8]='\0';
        int value=binaryToDecimal(byteStr);
        if(value==01111110 || value==01111101)
        {
            strcpy(stuffed+k, escStr);
            k+=8;
        }
        strcpy(stuffed+k, byteStr);
        k+=8;
    }
    stuffed[k]='\0';
}

void byteDestuffingPPP(char stuffed[], char destuffed[])
{
    int len=strlen(stuffed);
    int nbytes=len/8;
    int b, k=0;
    char byteStr[9];
    for(b=0; b<nbytes; b++)
    {
        strncpy(byteStr, stuffed+(b*8), 8);
        byteStr[8]='\0';
        int value=binaryToDecimal(byteStr);
        if(value==01111101)
        {
            b++;
            strncpy(byteStr, stuffed+(b*8), 8);
            byteStr[8]='\0';
        }
        strcpy(destuffed+k, byteStr);
        k+=8;
    }
    destuffed[k]='\0';
}

//=====================================================================
// CHECKSUM LOGIC
//=====================================================================

void addBinary8(char sum[9], char word[9])
{
    int i, carry=0;
    char result[9];
    result[8]='\0';

    for(i=7; i>=0; i--)
    {
        int a = sum[i]-'0';
        int b = word[i]-'0';
        int total = a+b+carry;
        result[i] = (total%2)+'0';
        carry = total/2;
    }

    if(carry==1)
    {
        int c=1, i2;
        for(i2=7; i2>=0 && c>0; i2--)
        {
            int a = result[i2]-'0';
            int total = a+c;
            result[i2] = (total%2)+'0';
            c = total/2;
        }
    }
    strcpy(sum, result);
}

void computeChecksum(char data[], char checksum[9])
{
    int len = strlen(data);
    int nwords = len/8;
    int rem = len%8;
    char sum[9] = "00000000";
    char word[9];
    int w, i;
    int blockNum = 0;

    printf("\n========================================================\n");
    printf("          CHECKSUM GENERATION (SENDER)\n");
    printf("========================================================\n");

    for(w=0; w<nwords; w++)
    {
        strncpy(word, data+(w*8), 8);
        word[8]='\0';
        blockNum++;
        printf("Block %2d : %s\n", blockNum, word);
        printf("Previous Sum : %s\n", sum);
        printf("Add Block    : %s\n", word);
        addBinary8(sum, word);
        printf("New Sum      : %s\n\n", sum);
    }

    if(rem!=0)
    {
        for(i=0; i<8; i++)
        {
            if((nwords*8+i) < len)
                word[i] = data[nwords*8+i];
            else
                word[i] = '0';
        }
        word[8]='\0';
        blockNum++;
        printf("Block %2d : %s\n", blockNum, word);
        printf("Previous Sum : %s\n", sum);
        printf("Add Block    : %s\n", word);
        addBinary8(sum, word);
        printf("New Sum      : %s\n\n", sum);
    }

    printf("----------------------------------------\n");
    printf("Final Sum Before Complement : %s\n", sum);
    for(i=0; i<8; i++)
        checksum[i] = (sum[i]=='0') ? '1' : '0';
    checksum[8]='\0';
    printf("1's Complement              : %s\n", checksum);
    printf("----------------------------------------\n\n");
    printf("Generated Checksum\n");
    printf("------------------\n");
    printf("%s\n", checksum);
}

int verifyChecksum(char data[], char checksum[])
{
    int len = strlen(data);
    int nwords = len/8;
    int rem = len%8;
    char sum[9] = "00000000";
    char word[9];
    int w, i;
    int blockNum = 0;
    int result;

    printf("\n========================================================\n");
    printf("         CHECKSUM VERIFICATION (RECEIVER)\n");
    printf("========================================================\n");

    for(w=0; w<nwords; w++)
    {
        strncpy(word, data+(w*8), 8);
        word[8]='\0';
        blockNum++;
        printf("Block %2d : %s\n", blockNum, word);
        printf("Previous Sum : %s\n", sum);
        printf("Add Block    : %s\n", word);
        addBinary8(sum, word);
        printf("New Sum      : %s\n\n", sum);
    }

    if(rem!=0)
    {
        for(i=0; i<8; i++)
        {
            if((nwords*8+i) < len)
                word[i] = data[nwords*8+i];
            else
                word[i] = '0';
        }
        word[8]='\0';
        blockNum++;
        printf("Block %2d : %s\n", blockNum, word);
        printf("Previous Sum : %s\n", sum);
        printf("Add Block    : %s\n", word);
        addBinary8(sum, word);
        printf("New Sum      : %s\n\n", sum);
    }

    printf("========================================================\n");
    printf("ADD RECEIVED CHECKSUM\n");
    printf("========================================================\n");
    printf("Current Sum       : %s\n", sum);
    printf("Received Checksum : %s\n", checksum);
    addBinary8(sum, checksum);
    printf("Final Sum         : %s\n", sum);
    printf("========================================================\n");

    result = (strcmp(sum, "11111111")==0);
    printf("Checksum Result : %s\n", result ? "CORRECT" : "INCORRECT");
    return result;
}

//=====================================================================
// FRAME FORMAT DISPLAY HELPERS
//=====================================================================

void printPPFrame(char title[], char dataLabel[], char dataContent[],
                  char srcMAC[], char dstMAC[], char trailerVal[])
{
    printf("\n========================================================\n");
    printf("%s\n", title);
    printf("========================================================\n");
    printf("\nFlag\n-----\n01111110\n");
    printf("\nAddress\n-------\n");
    printf("Source MAC : %s\n", srcMAC);
    printf("Destination MAC : %s\n", dstMAC);
    printf("\n%s\n", dataLabel);
    printf("--------------------\n");
    printf("%s\n", dataContent);
    printf("\nTrailer (Checksum)\n-------------------\n%s\n", trailerVal);
    printf("\nFlag\n-----\n01111110\n");
}

void printPPPFrame(char title[], char dataContent[], char fcsVal[])
{
    printf("\n========================================================\n");
    printf("%s\n", title);
    printf("========================================================\n");
    printf("\nFlag\n-----\n01111110\n");
    printf("\nAddress\n-------\n11111111\n");
    printf("\nControl\n-------\n00000011\n");
    printf("\nProtocol\n--------\n11000010 00100001\n");
    printf("\nInformation (Data)\n-------------------\n%s\n", dataContent);
    printf("\nFCS (Checksum)\n---------------\n%s\n", fcsVal);
    printf("\nFlag\n-----\n01111110\n");
}

//=====================================================================
// HAMMING CODE FUNCTIONS
//=====================================================================

int isPowerOfTwo(int x)
{
    return (x != 0 && (x & (x - 1)) == 0);
}

int calculateParityBits(int m)
{
    int r = 0;
    while((1 << r) < (m + r + 1))
        r++;
    return r;
}

void generateHammingCode(char data[], char hamming[])
{
    int m = strlen(data);
    int r = calculateParityBits(m);
    int total = m + r;
    int arr[200];
    int i, j, k = 0;

    for(i=1; i<=total; i++)
        arr[i] = 0;

    for(i=1; i<=total; i++)
    {
        if(isPowerOfTwo(i))
            arr[i] = 0;
        else
            arr[i] = data[k++] - '0';
    }

    for(i=0; i<r; i++)
    {
        int p = 1 << i;
        int count = 0;
        for(j=1; j<=total; j++)
        {
            if(j & p)
            {
                if(arr[j] == 1)
                    count++;
            }
        }
        arr[p] = (count % 2 == 0) ? 0 : 1;
    }

    for(i=1; i<=total; i++)
        hamming[i-1] = arr[i] + '0';
    hamming[total] = '\0';
}

void printHammingSender(char input[], char hamming[])
{
    int m = strlen(input);
    int r = calculateParityBits(m);
    int total = m + r;
    int arr[200];
    int i, j, k = 0;

    for(i=1; i<=total; i++)
        arr[i] = 0;

    for(i=1; i<=total; i++)
    {
        if(isPowerOfTwo(i))
            arr[i] = 0;
        else
            arr[i] = input[k++] - '0';
    }

    printf("\n========================================\n");
    printf("          HAMMING CODE - SENDER\n");
    printf("========================================\n\n");
    printf("STEP 1 : Input Data\n\n");
    printf("Data Word = %s\n\n", input);

    printf("STEP 2 : Calculate Number of Parity Bits\n\n");
    printf("Formula : 2^r >= m+r+1\n\n");

    int p = 0;
    while((1<<p) < (m + p + 1))
    {
        printf("%d >= %d\n", 1<<p, m + p + 1);
        printf("False\n\n");
        p++;
    }
    printf("%d >= %d\n", 1<<p, m + p + 1);
    printf("True\n\n");
    printf("Number of Parity Bits = %d\n\n", r);

    printf("STEP 3 : Generate Hamming Code\n\n");
    printf("Bit Placement:\n\n");
    printf("Position : 1-%d\n", total);
    printf("Bit      : ");
    for(i=1; i<=total; i++) printf("%d ", arr[i]);
    printf("\n\nParity Bit Calculation\n----------------------\n");

    for(i=0; i<r; i++)
    {
        int pos = 1 << i;
        int count = 0;
        printf("\nChecking P%d\n\n", pos);
        printf("Positions Covered : %d-%d\n", pos, total);

        printf("\nValues : ");
        for(j=1; j<=total; j++)
        {
            if(j & pos)
            {
                printf("%d ", arr[j]);
                if(arr[j] == 1) count++;
            }
        }

        printf("\n\nNumber of 1's = %d\n", count);
        if(count % 2 == 0)
            printf("Even Count\n");
        else
            printf("Odd Count\n");

        arr[pos] = (count % 2 == 0) ? 0 : 1;
        printf("P%d = %d\n", pos, arr[pos]);
    }

    for(i=1; i<=total; i++)
        hamming[i-1] = arr[i] + '0';
    hamming[total] = '\0';

    printf("\n\nFinal Hamming Code\n------------------\n\n");
    printf("Position : 1-%d\n", total);
    printf("Bit      : ");
    for(i=1; i<=total; i++) printf("%d ", arr[i]);
    printf("\n\nGenerated Hamming Code = %s\n\n", hamming);
    printf("Data Sent Successfully\n");
}

void printHammingReceiver(char received[])
{
    int total = strlen(received);
    int arr[200];
    int i, j, choice, errorPos = 0;

    for(i=1; i<=total; i++)
        arr[i] = received[i-1] - '0';

    printf("\n\n========================================\n");
    printf("          HAMMING CODE - RECEIVER\n");
    printf("========================================\n\n");
    printf("Receiving Data From Channel...\n\n");
    printf("Received Code = %s\n\n", received);

    printf("Do you want to introduce error? (1.Yes / 0.No): ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        int pos;
        printf("\nEnter Error Position (1-%d): ", total);
        scanf("%d", &pos);
        if(pos >= 1 && pos <= total)
        {
            arr[pos] = 1 - arr[pos];
            printf("Error introduced at position %d\n", pos);
        }
    }
    else
    {
        printf("\nNo Error Introduced\n");
    }

    printf("\n\nSTEP 1 : Error Detection\n-------------------------\n");

    for(i=0; (1<<i) <= total; i++)
    {
        int p = 1 << i;
        int count = 0;
        printf("\nChecking P%d\n\n", p);
        printf("Positions Covered : %d-%d\n", p, total);

        printf("\nValues : ");
        for(j=1; j<=total; j++)
        {
            if(j & p)
            {
                printf("%d ", arr[j]);
                if(arr[j] == 1) count++;
            }
        }

        printf("\n\nNumber of 1's = %d\n", count);
        if(count % 2 == 0)
            printf("P%d : OK\n", p);
        else
        {
            printf("P%d : ERROR\n", p);
            errorPos += p;
        }
    }

    printf("\n\nError Position Calculation\n--------------------------\n");
    if(errorPos == 0)
        printf("No Error Detected\n");
    else
        printf("Error detected at position %d\n", errorPos);

    printf("\n\nSTEP 3 : Hamming Distance\n-------------------------\n");
    int hd = 0;
    printf("Different Positions : ");
    for(i=1; i<=total; i++)
    {
        if(arr[i] != (received[i-1]-'0'))
        {
            printf("%d ", i);
            hd++;
        }
    }
    if(hd == 0)
        printf("None");
    printf("\nHamming Distance = %d\n", hd);

    printf("\nTransmission Completed\n");
}

//=====================================================================
// 2-D PARITY FUNCTIONS
//=====================================================================

void twoDParitySender(char msg[])
{
    int len = strlen(msg);
    int i, j;
    int matrix[50][8];
    int rowParity[50];
    int colParity[8];
    char bin[9];

    printf("\n=====================================\n");
    printf("      2-D PARITY SENDER\n");
    printf("=====================================\n");
    printf("Enter the message : %s\n\n", msg);
    printf("Original Message : %s\n\n", msg);

    printf("ASCII Values\n----------------------\n");
    for(i=0; i<len; i++)
        printf("%c = %d\n", msg[i], (int)msg[i]);

    printf("\nBinary Equivalent\n----------------------\n");
    for(i=0; i<len; i++)
    {
        asciiToBinary(msg[i], bin);
        printf("%s   (%c)\n", bin, msg[i]);
        for(j=0; j<8; j++)
            matrix[i][j] = bin[j]-'0';
    }

    for(i=0; i<len; i++)
    {
        int count = 0;
        for(j=0; j<8; j++)
            if(matrix[i][j] == 1) count++;
        rowParity[i] = (count % 2 == 0) ? 1 : 0;
    }

    for(j=0; j<8; j++)
    {
        int count = 0;
        for(i=0; i<len; i++)
            if(matrix[i][j] == 1) count++;
        colParity[j] = (count % 2 == 0) ? 1 : 0;
    }

    printf("\n\n2-D PARITY MATRIX\n");
    printf("-----------------------------------------------------------\n");
    printf("Char    B1 B2 B3 B4 B5 B6 B7 B8 | Row Parity\n");
    printf("-----------------------------------------------------------\n");
    for(i=0; i<len; i++)
    {
        printf("%c       ", msg[i]);
        for(j=0; j<8; j++)
            printf("%d  ", matrix[i][j]);
        printf("|     %d\n", rowParity[i]);
    }
    printf("-----------------------------------------------------------\n");
    printf("CP      ");
    for(j=0; j<8; j++)
        printf("%d  ", colParity[j]);
    printf("|\n");

    printf("\nData successfully written into output.txt\n\n");

    printf("Row Parity Bits\n----------------------\n");
    for(i=0; i<len; i++)
        printf("Row %d : %d\n", i+1, rowParity[i]);

    printf("\nColumn Parity Bits\n----------------------\n");
    for(j=0; j<8; j++)
        printf("Column %d : %d\n", j+1, colParity[j]);

    printf("\nSender completed successfully.\n");
}

void twoDParityReceiver(char msg[])
{
    int len = strlen(msg);
    int i, j, choice, r, c;
    int matrix[50][8];
    int rowParity[50];
    int colParity[8];
    int calcRowParity[50];
    int calcColParity[8];
    char bin[9];

    printf("\n\n========================================\n");
    printf("         2-D PARITY RECEIVER\n");
    printf("========================================\n\n");
    printf("Received Message : %s\n\n", msg);

    for(i=0; i<len; i++)
    {
        asciiToBinary(msg[i], bin);
        for(j=0; j<8; j++)
            matrix[i][j] = bin[j]-'0';
    }

    for(i=0; i<len; i++)
    {
        int count = 0;
        for(j=0; j<8; j++)
            if(matrix[i][j] == 1) count++;
        rowParity[i] = (count % 2 == 0) ? 1 : 0;
    }

    for(j=0; j<8; j++)
    {
        int count = 0;
        for(i=0; i<len; i++)
            if(matrix[i][j] == 1) count++;
        colParity[j] = (count % 2 == 0) ? 1 : 0;
    }

    printf("Received Data\n");
    printf("-----------------------------------------------------------\n");
    for(i=0; i<len; i++)
    {
        printf("%c - ", msg[i]);
        for(j=0; j<8; j++)
            printf("%d", matrix[i][j]);
        printf(" | RP=%d\n", rowParity[i]);
    }
    printf("-----------------------------------------------------------\n");
    printf("CP - ");
    for(j=0; j<8; j++)
        printf("%d ", colParity[j]);
    printf("\n\n");

    printf("How many bits do you want to modify (0-4)? : ");
    scanf("%d", &choice);

    if(choice > 0)
    {
        for(i=0; i<choice; i++)
        {
            printf("\nModification %d\n", i+1);
            printf("Enter Row Number (1-%d): ", len);
            scanf("%d", &r);
            printf("Enter Column Number (1-8): ");
            scanf("%d", &c);
            if(r>=1 && r<=len && c>=1 && c<=8)
            {
                matrix[r-1][c-1] = 1 - matrix[r-1][c-1];
                printf("Bit Modified Successfully.\n");
            }
        }
    }

    for(i=0; i<len; i++)
    {
        int count = 0;
        for(j=0; j<8; j++)
            if(matrix[i][j] == 1) count++;
        calcRowParity[i] = (count % 2 == 0) ? 1 : 0;
    }

    for(j=0; j<8; j++)
    {
        int count = 0;
        for(i=0; i<len; i++)
            if(matrix[i][j] == 1) count++;
        calcColParity[j] = (count % 2 == 0) ? 1 : 0;
    }

    printf("\nModified Data\n");
    printf("-----------------------------------------------------------\n");
    for(i=0; i<len; i++)
    {
        printf("%c - ", msg[i]);
        for(j=0; j<8; j++)
            printf("%d", matrix[i][j]);
        printf(" | RP=%d\n", calcRowParity[i]);
    }
    printf("-----------------------------------------------------------\n");
    printf("CP - ");
    for(j=0; j<8; j++)
        printf("%d ", calcColParity[j]);
    printf("\n\n");

    printf("========================================\n");
    printf("ROW PARITY VERIFICATION\n");
    printf("========================================\n");
    for(i=0; i<len; i++)
    {
        printf("Row %d : Received = %d  Calculated = %d --> %s\n",
               i+1, rowParity[i], calcRowParity[i],
               (rowParity[i] == calcRowParity[i]) ? "CORRECT" : "ERROR");
    }

    printf("\n========================================\n");
    printf("COLUMN PARITY VERIFICATION\n");
    printf("========================================\n");
    for(j=0; j<8; j++)
    {
        printf("Column %d : Received = %d  Calculated = %d --> %s\n",
               j+1, colParity[j], calcColParity[j],
               (colParity[j] == calcColParity[j]) ? "CORRECT" : "ERROR");
    }

    printf("\n========================================\n");
    if(choice == 0)
        printf("MESSAGE RECEIVED WITHOUT ERROR\n");
    else
        printf("MESSAGE RECEIVED WITH ERROR\n");
    printf("========================================\n");

    printf("\nResult successfully written into output.txt\n");
}

//=====================================================================
// MAIN FUNCTION
//=====================================================================
int main()
{
    char srcURL[30];
    char dstURL[30];
    char filename[50];
    char fileData[500];
    char fileBinary[5000];
    char binaryData[5000];
    char srcIPBinary[50];
    char dstIPBinary[50];
    char srcMACBinary[100];
    char dstMACBinary[100];
    char srcPortBinary[17];
    char dstPortBinary[17];
    int srcIndex, dstIndex;
    int srcPort, dstPort;
    int totalBits;
    int totalPackets;
    int totalFrames;
    char stuffedBits[6000];
    char destuffedBits[6000];
    char pppStuffed[10500];
    char pppDestuffed[10500];
    char pppChecksum[9];
    int pppValid;

    char hammingInput[5000];
    char hammingCode[6000];

    outFile = fopen("output.txt", "w");
    if(outFile == NULL)
    {
        printf("Could not create output.txt (continuing with screen output only)\n");
    }

    displayTable();
    printf("Enter Source URL : ");
    scanf("%s", srcURL);
    printf("Enter Destination URL : ");
    scanf("%s", dstURL);

    srcIndex = findURLIndex(srcURL);
    dstIndex = findURLIndex(dstURL);

    if(srcIndex == -1 || dstIndex == -1)
    {
        printf("URL NOT FOUND\n");
        return 0;
    }

    printf("\nURL TABLE DETAILS");
    printf("\n-----------------\n");
    printf("URL : %s\n", table[srcIndex].url);
    printf("Source IP : %s\n", table[srcIndex].srcIP);
    printf("Source MAC : %s\n", table[srcIndex].srcMAC);

    ipToBinary(table[srcIndex].srcIP, srcIPBinary);
    ipToBinary(table[dstIndex].srcIP, dstIPBinary);
    macToBinary(table[srcIndex].srcMAC, srcMACBinary);
    macToBinary(table[dstIndex].srcMAC, dstMACBinary);

    generatePort(&srcPort, &dstPort);
    printf("\nTRANSPORT LAYER");
    printf("\n----------------\n");
    printf("Source Port : %d\n", srcPort);
    printf("Destination Port : %d\n", dstPort);
    decimalToBinary(srcPort, srcPortBinary, 16);
    decimalToBinary(dstPort, dstPortBinary, 16);
    printf("Source Port Binary : %s\n", srcPortBinary);
    printf("Destination Port Binary : %s\n", dstPortBinary);

    printf("\nEnter File Name : ");
    scanf("%s", filename);
    readFile(filename, fileData);
    printf("\nFILE CONTENT : %s\n", fileData);

    fileAsciiToBinary(fileData, fileBinary);
    printf("\nASCII BINARY : %s\n", fileBinary);

    strcpy(binaryData, fileBinary);
    strcat(binaryData, srcPortBinary);
    strcat(binaryData, dstPortBinary);

    printf("\nTOTAL BIT STREAM");
    printf("\n----------------\n");
    printf("%s\n", binaryData);
    totalBits = strlen(binaryData);
    printf("\nTotal Bits : %d\n", totalBits);

    totalPackets = createPackets(binaryData, table[srcIndex].srcIP, table[dstIndex].srcIP);
    totalFrames = createFrames(binaryData, table[srcIndex].srcIP, table[dstIndex].srcIP,
                               table[srcIndex].srcMAC, table[dstIndex].srcMAC);

    printf("\n====================");
    printf("\nTotal Bits : %d", totalBits);
    printf("\nTotal Packets : %d", totalPackets);
    printf("\nTotal Frames : %d", totalFrames);
    printf("\n====================\n");

    bitStuffing(binaryData, stuffedBits);
    bitDestuffing(stuffedBits, destuffedBits);

    printf("\n========================================================\n");
    printf(" BIT STUFFING\n");
    printf("========================================================\n");
    printf("\nSender Side\n\n");
    printf("Original Data :\n%s\n", binaryData);
    printf("\nAfter Bit Stuffing :\n%s\n", stuffedBits);
    printf("\nOriginal Length : %d\n", (int)strlen(binaryData));
    printf("Stuffed Length : %d\n", (int)strlen(stuffedBits));

    printf("\n========================================================\n");
    printf(" BIT DESTUFFING\n");
    printf("========================================================\n");
    printf("\nReceiver Side\n\n");
    printf("Received Stuffed Data :\n%s\n", stuffedBits);
    printf("\nAfter Bit Destuffing :\n%s\n", destuffedBits);
    printf("\nTransmission Status : %s\n",
           (strcmp(binaryData, destuffedBits)==0) ? "SUCCESS" : "FAILED");

    computeChecksum(stuffedBits, pppChecksum);
    pppValid = verifyChecksum(stuffedBits, pppChecksum);

    printf("\n========================================================\n");
    printf(" CHECKSUM (HDLC - on Bit Stuffed Data)\n");
    printf("========================================================\n");
    printf("\nSender Side\n\n");
    printf("Data used for checksum (After Bit Stuffing):\n%s\n", stuffedBits);
    printf("\nComputed Checksum (8-bit, 1's complement) : %s\n", pppChecksum);
    printf("\nReceiver Side\n\n");
    printf("Sum of all data words + checksum should be all 1's\n");
    printf("Verification Result : %s\n", pppValid ? "11111111 -> VALID" : "NOT 11111111 -> INVALID");
    printf("Status : %s\n", pppValid ? "VALID" : "INVALID");

    printPPFrame(" FRAME FORMAT",
                 "Data (Stuffed Data)", stuffedBits,
                 table[srcIndex].srcMAC, table[dstIndex].srcMAC, pppChecksum);

    printf("\n\n");
    printPPFrame(" DESTUFFED FRAME FORMAT",
                 "Data (Destuffed Data)", destuffedBits,
                 table[srcIndex].srcMAC, table[dstIndex].srcMAC, pppChecksum);

    byteStuffingPPP(binaryData, pppStuffed);
    byteDestuffingPPP(pppStuffed, pppDestuffed);

    printf("\n\n========================================================\n");
    printf(" PPP BYTE STUFFING\n");
    printf("========================================================\n");
    printf("\nOriginal Data :\n%s\n", binaryData);
    printf("\nAfter Byte Stuffing :\n%s\n", pppStuffed);
    printf("\nOriginal Length : %d\n", (int)strlen(binaryData));
    printf("Stuffed Length : %d\n", (int)strlen(pppStuffed));

    printf("\n========================================================\n");
    printf(" PPP BYTE DESTUFFING\n");
    printf("========================================================\n");
    printf("\nReceiver Side\n\n");
    printf("Received Stuffed Data :\n%s\n", pppStuffed);
    printf("\nAfter Byte Destuffing :\n%s\n", pppDestuffed);
    printf("\nOriginal File Content :\n%s\n", fileData);
    printf("\nTransmission Status : %s\n",
           (strcmp(binaryData, pppDestuffed)==0) ? "SUCCESS" : "FAILED");

    computeChecksum(pppStuffed, pppChecksum);
    pppValid = verifyChecksum(pppStuffed, pppChecksum);

    printf("\n========================================================\n");
    printf(" CHECKSUM (PPP - on Byte Stuffed Data)\n");
    printf("========================================================\n");
    printf("\nSender Side\n\n");
    printf("Data used for checksum (After Byte Stuffing):\n%s\n", pppStuffed);
    printf("\nComputed Checksum / FCS (8-bit, 1's complement) : %s\n", pppChecksum);
    printf("\nReceiver Side\n\n");
    printf("Verification Result : %s\n", pppValid ? "11111111 -> VALID" : "NOT 11111111 -> INVALID");
    printf("Status : %s\n", pppValid ? "VALID" : "INVALID");

    printPPPFrame(" PPP FRAME FORMAT (SENDER)", pppStuffed, pppChecksum);
    printf("\n");
    printPPPFrame(" PPP FRAME DE-FORMAT (RECEIVER)", pppStuffed, pppChecksum);

    printf("\n========================================================\n");
    printf(" PHYSICAL LAYER\n");
    printf("========================================================\n");
    printf("Source MAC : %s\n", table[srcIndex].srcMAC);
    printf("Destination MAC : %s\n", table[dstIndex].srcMAC);
    printf("Source IP : %s\n", table[srcIndex].srcIP);
    printf("Destination IP : %s\n", table[dstIndex].srcIP);
    printf("Total Frames : %d\n", totalFrames);
    printf("\nComplete Physical Binary Data\n");
    printf("------------------------------------\n");
    printf("%s\n", binaryData);
    printf("\nTotal Binary Length : %d\n", totalBits);

    strcpy(hammingInput, fileBinary);
    generateHammingCode(hammingInput, hammingCode);
    printHammingSender(hammingInput, hammingCode);
    printHammingReceiver(hammingCode);

    twoDParitySender(fileData);
    twoDParityReceiver(fileData);

    if(outFile != NULL)
    {
        fclose(outFile);
        outFile = NULL;
    }

    return 0;
}

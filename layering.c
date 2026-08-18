#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

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
    {"gmail.com",  "192.168.1.20", "11:22:33:44:55:66"},
    {"yahoo.com",  "10.0.0.1",     "12:34:56:78:90:AB"}
};

//---------------- LINEAR SEARCH FUNCTION ----------------
int findURLIndex(char url[])
{
    int i;
    for(i = 0; i < 3; i++)
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
    char ch;
    while((ch=fgetc(fp))!=EOF && i < 499)
    {
        data[i]=ch;
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
    srand(time(NULL));
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
        printf("Data      : ");

        for(j=0; j<16; j++)
        {
            if((i + j) < strlen(data))
                printf("%c", data[i + j]);
            else
                printf("0");
        }
        printf("\n");
        printf("Src IP    : %s\n", srcIPBinary);
        printf("Dst IP    : %s\n", dstIPBinary);
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

        printf("Data     : %s\n", currentFrameData);
        printf("Src IP   : %s\n", srcIPBinary);
        printf("Dst IP   : %s\n", dstIPBinary);
        printf("Src MAC  : %s\n", srcMACBinary);
        printf("Dst MAC  : %s\n", dstMACBinary);
        printf("Trailer  : %s\n", trailer);
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

    for(i=0; i<3; i++)
    {
        printf("%s\t%s\t%s\n", table[i].url, table[i].srcIP, table[i].srcMAC);
    }
    printf("-----------------------------------------------\n");
}

//---------------- MAIN FUNCTION ----------------

int main()
{
    char srcURL[30];
    char dstURL[30];
    char filename[50];
    char fileData[500];
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

    // Physical Layer integration variables
    int i, j, frameIdx;
    char currentFrameData[9];
    char trailer[9];

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

    // Converting IP and MAC addresses into global binary strings
    ipToBinary(table[srcIndex].srcIP, srcIPBinary);
    ipToBinary(table[dstIndex].srcIP, dstIPBinary);
    macToBinary(table[srcIndex].srcMAC, srcMACBinary);
    macToBinary(table[dstIndex].srcMAC, dstMACBinary);

    // TRANSPORT LAYER
    generatePort(&srcPort, &dstPort);
    printf("\nTRANSPORT LAYER");
    printf("\n----------------\n");
    printf("Source Port : %d\n", srcPort);
    printf("Destination Port : %d\n", dstPort);
    decimalToBinary(srcPort, srcPortBinary, 16);
    decimalToBinary(dstPort, dstPortBinary, 16);

    printf("Source Port Binary : %s\n", srcPortBinary);
    printf("Destination Port Binary : %s\n", dstPortBinary);

    // FILE INPUT
    printf("\nEnter File Name : ");
    scanf("%s", filename);
    readFile(filename, fileData);
    printf("\nFILE CONTENT : %s\n", fileData);
    fileAsciiToBinary(fileData, binaryData);
    printf("\nASCII BINARY : %s\n", binaryData);

    // MERGE DATA + PORT
    strcat(binaryData, srcPortBinary);
    strcat(binaryData, dstPortBinary);

    printf("\nTOTAL BIT STREAM");
    printf("\n----------------\n");
    printf("%s\n", binaryData);
    totalBits = strlen(binaryData);
    printf("\nTotal Bits : %d\n", totalBits);

    // PACKETS DISPLAY
    totalPackets = createPackets(binaryData, table[srcIndex].srcIP, table[dstIndex].srcIP);

    // FRAMES DISPLAY
    totalFrames = createFrames(binaryData, table[srcIndex].srcIP, table[dstIndex].srcIP, table[srcIndex].srcMAC, table[dstIndex].srcMAC);

    //---------------- PHYSICAL LAYER GENERATION ----------------
    printf("\nPHYSICAL LAYER BIT STREAM");
    printf("\n-------------------------\n");

    frameIdx = 0;
    for(i = 0; i < totalBits; i = i + 8)
    {
        frameIdx++;

        // 1. Preamble (7 Bytes) & SFD (1 Byte)
        printf("10101010 10111011 ");

        // 2. Src MAC & Dst MAC Address Bits
        printf("%s %s ", srcMACBinary, dstMACBinary);

        // 3. Src IP & Dst IP Address Bits
        printf("%s %s ", srcIPBinary, dstIPBinary);

        // 4. Extracting and padding Frame segment data
        for(j = 0; j < 8; j++)
        {
            if((i + j) < totalBits)
                currentFrameData[j] = binaryData[i + j];
            else
                currentFrameData[j] = '0';
        }
        currentFrameData[8] = '\0';
        printf("%s ", currentFrameData);

        // 5. Calculating frame inverted trailer values
        generateTrailer(currentFrameData, trailer);
        printf("%s\n", trailer);
    }

    printf("\n====================");
    printf("\nTotal Bits    : %d", totalBits);
    printf("\nTotal Packets : %d", totalPackets);
    printf("\nTotal Frames  : %d", totalFrames);
    printf("\n====================\n");
    return 0;
}

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdarg.h>

//======================================================================
//   OUTPUT FILE HANDLING
//   Every printf() in this program is redirected (via macro below) to
//   myPrintf(), which prints to the screen AND writes the same text
//   into "output.txt" at the same time. So no individual printf call
//   anywhere in the code had to be changed.
//======================================================================

FILE *outFile;   // global file pointer -> output.txt

void myPrintf(const char *fmt, ...)
{
    va_list argsScreen, argsFile;

    va_start(argsScreen, fmt);
    va_copy(argsFile, argsScreen);

    vprintf(fmt, argsScreen);          // print to console
    if(outFile != NULL)
        vfprintf(outFile, fmt, argsFile);  // save to output.txt

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

//======================================================================
//   NEW SECTION 1 : GENERIC (HDLC STYLE) BIT STUFFING / DESTUFFING
//======================================================================

// After 5 consecutive 1's, insert a 0
void bitStuffing(char data[], char stuffed[])
{
    int i, ones=0, k=0;
    int len=strlen(data);

    for(i=0; i<len; i++)
    {
        stuffed[k++]=data[i];

        if(data[i]=='1')
        {
            ones++;
            if(ones==5)
            {
                stuffed[k++]='0';
                ones=0;
            }
        }
        else
        {
            ones=0;
        }
    }
    stuffed[k]='\0';
}

// Remove the stuffed 0 that follows 5 consecutive 1's
void bitDestuffing(char stuffed[], char destuffed[])
{
    int i, ones=0, k=0;
    int len=strlen(stuffed);

    for(i=0; i<len; i++)
    {
        destuffed[k++]=stuffed[i];

        if(stuffed[i]=='1')
        {
            ones++;
            if(ones==5)
            {
                i++;        // skip the stuffed bit
                ones=0;
            }
        }
        else
        {
            ones=0;
        }
    }
    destuffed[k]='\0';
}

//======================================================================
//   NEW SECTION 2 : PPP STYLE BYTE STUFFING / DESTUFFING
//======================================================================

#define FLAG_BYTE 0x7E   // 01111110
#define ESC_BYTE  0x7D   // 01111101

// Scan byte by byte (8 bits). If a byte equals the FLAG or ESC byte,
// an extra ESC byte is inserted just before it.
void byteStuffingPPP(char data[], char stuffed[])
{
    int len=strlen(data);
    int nbytes=len/8;
    int b, k=0;
    char byteStr[9];
    char escStr[9];

    decimalToBinary(ESC_BYTE, escStr, 8);

    for(b=0; b<nbytes; b++)
    {
        strncpy(byteStr, data+(b*8), 8);
        byteStr[8]='\0';

        int value=binaryToDecimal(byteStr);

        if(value==FLAG_BYTE || value==ESC_BYTE)
        {
            strcpy(stuffed+k, escStr);
            k+=8;
        }

        strcpy(stuffed+k, byteStr);
        k+=8;
    }
    stuffed[k]='\0';
}

// When an ESC byte is found, drop it and copy the following byte as-is
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

        if(value==ESC_BYTE)
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

//======================================================================
//   NEW SECTION 3 : FRAME FORMAT DISPLAY HELPERS
//======================================================================

void printHDLCFrame(char title[], char dataLabel[], char dataContent[],
                     char srcMAC[], char dstMAC[])
{
    printf("\n========================================================\n");
    printf("%s\n", title);
    printf("========================================================\n");

    printf("\nFlag\n-----\n01111110\n");

    printf("\nAddress\n-------\n");
    printf("Source MAC      : %s\n", srcMAC);
    printf("Destination MAC : %s\n", dstMAC);

    printf("\n%s\n", dataLabel);
    printf("--------------------\n");
    printf("%s\n", dataContent);

    printf("\nTrailer\n-------\n0000000000000000\n");

    printf("\nFlag\n-----\n01111110\n");
}

void printPPPFrame(char title[], char dataContent[])
{
    printf("\n========================================================\n");
    printf("%s\n", title);
    printf("========================================================\n");

    printf("\nFlag\n-----\n01111110\n");

    printf("\nAddress\n-------\n11111111\n");

    printf("\nControl\n-------\n00000011\n");

    printf("\nProtocol\n--------\n11000010 00100001\n");

    printf("\nInformation (Data)\n-------------------\n%s\n", dataContent);

    printf("\nFCS\n---\n0000000000000000\n");

    printf("\nFlag\n-----\n01111110\n");
}

//---------------- MAIN FUNCTION ----------------

int main()
{
    char srcURL[30];
    char dstURL[30];
    char filename[50];
    char fileData[500];
    char fileBinary[5000];        // ascii-to-binary of the file content ONLY
    char binaryData[5000];        // fileBinary + ports  -> physical layer content
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

    // Stuffing related buffers
    char stuffedBits[6000];
    char destuffedBits[6000];
    char pppStuffed[8200];
    char pppDestuffed[8200];

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
    fileAsciiToBinary(fileData, fileBinary);
    printf("\nASCII BINARY : %s\n", fileBinary);

    // MERGE DATA + PORT  ->  this becomes the physical layer bit stream
    strcpy(binaryData, fileBinary);
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


    printf("\n====================");
    printf("\nTotal Bits    : %d", totalBits);
    printf("\nTotal Packets : %d", totalPackets);
    printf("\nTotal Frames  : %d", totalFrames);
    printf("\n====================\n");

    //======================================================================
    //  BIT STUFFING / DESTUFFING  (input = physical layer bit stream)
    //======================================================================

    bitStuffing(binaryData, stuffedBits);
    bitDestuffing(stuffedBits, destuffedBits);

    printf("\n========================================================\n");
    printf("                 BIT STUFFING\n");
    printf("========================================================\n");
    printf("\nSender Side\n\n");
    printf("Original Data :\n%s\n", binaryData);
    printf("\nAfter Bit Stuffing :\n%s\n", stuffedBits);
    printf("\nOriginal Length : %d\n", (int)strlen(binaryData));
    printf("Stuffed Length  : %d\n", (int)strlen(stuffedBits));

    printf("\n========================================================\n");
    printf("                 BIT DESTUFFING\n");
    printf("========================================================\n");
    printf("\nReceiver Side\n\n");
    printf("Received Stuffed Data :\n%s\n", stuffedBits);
    printf("\nAfter Bit Destuffing :\n%s\n", destuffedBits);
    printf("\nTransmission Status : %s\n",
           (strcmp(binaryData, destuffedBits)==0) ? "SUCCESS" : "FAILED");

    //======================================================================
    //  HDLC FRAME FORMAT  /  DESTUFFED FRAME FORMAT
    //======================================================================

    printHDLCFrame("                     FRAME FORMAT",
                    "Data (Stuffed Data)", stuffedBits,
                    table[srcIndex].srcMAC, table[dstIndex].srcMAC);

    printf("\n\n");

    printHDLCFrame("               DESTUFFED FRAME FORMAT",
                    "Data (Destuffed Data)", destuffedBits,
                    table[srcIndex].srcMAC, table[dstIndex].srcMAC);

    //======================================================================
    //  PPP BYTE STUFFING / DESTUFFING  (input = file content ascii binary)
    //======================================================================

    byteStuffingPPP(fileBinary, pppStuffed);
    byteDestuffingPPP(pppStuffed, pppDestuffed);

    printf("\n\n========================================================\n");
    printf("               PPP BYTE STUFFING\n");
    printf("========================================================\n");
    printf("\nOriginal Data :\n%s\n", fileBinary);
    printf("\nAfter Byte Stuffing :\n%s\n", pppStuffed);
    printf("\nOriginal Length : %d\n", (int)strlen(fileBinary));
    printf("Stuffed Length  : %d\n", (int)strlen(pppStuffed));

    printf("\n========================================================\n");
    printf("              PPP BYTE DESTUFFING\n");
    printf("========================================================\n");
    printf("\nReceiver Side\n\n");
    printf("Received Stuffed Data :\n%s\n", pppStuffed);
    printf("\nAfter Byte Destuffing :\n%s\n", pppDestuffed);
    printf("\nOriginal File Content :\n%s\n", fileData);
    printf("\nTransmission Status : %s\n",
           (strcmp(fileBinary, pppDestuffed)==0) ? "SUCCESS" : "FAILED");

    //======================================================================
    //  PPP FRAME FORMAT  (SENDER)  /  PPP FRAME DE-FORMAT (RECEIVER)
    //======================================================================

    printPPPFrame("                 PPP FRAME FORMAT (SENDER)", pppStuffed);
    printf("\n");
    printPPPFrame("              PPP FRAME DE-FORMAT (RECEIVER)", pppStuffed);

    //======================================================================
    //  PHYSICAL LAYER SUMMARY
    //======================================================================

    printf("\n========================================================\n");
    printf("                    PHYSICAL LAYER\n");
    printf("========================================================\n");
    printf("Source MAC          : %s\n", table[srcIndex].srcMAC);
    printf("Destination MAC     : %s\n", table[dstIndex].srcMAC);
    printf("Source IP           : %s\n", table[srcIndex].srcIP);
    printf("Destination IP      : %s\n", table[dstIndex].srcIP);
    printf("Total Frames        : %d\n", totalFrames);

    printf("\nComplete Physical Binary Data\n");
    printf("------------------------------------\n");
    printf("%s\n", binaryData);
    printf("\nTotal Binary Length : %d\n", totalBits);

    if(outFile != NULL)
    {
        fclose(outFile);
        outFile = NULL;                 // avoid writing to a closed file handle

    }

    return 0;
}

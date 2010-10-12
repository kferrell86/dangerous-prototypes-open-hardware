//Image sender for NOKIA

// reference link http://dangerousprototypes.com/forum/index.php?topic=1056.0
// wikimedia      http://dangerousprototypes.com/docs/Mathieu:_Another_LCD_backpack


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32

#include <conio.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>
#include <WinDef.h>

#else

//#include <curses.h>
#define usleep(x) Sleep(x);

#endif
#include "serial.h"
char *dumpfile;
HANDLE dumphandle;
int modem =FALSE;   // use by command line switch -m to set to true
#define FREE(x) if(x) free(x);

#define MAX_BUFFER 16384   //16kbytes

//structure of bmp header
#ifndef _WIN32
//taken from msdn so that it will compile under debian not yet done!!
typedef struct tagBITMAPFILEHEADER {
  unsigned short bfType;
  unsigned long bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned long bfOffBits;
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
  unsigned long biSize;
  long  biWidth;
  long biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long  biXPelsPerMeter;
  long  biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[1];
} BITMAPINFO;
#endif

#  define BF_TYPE 0x4D42                   // MB
int print_usage(char * appname)
{
		//print usage
		printf("\n");
		printf("\n");
	    printf(" Help Menu\n");
        printf(" Usage:              \n");
		printf("   %s  -p device -f sample.bmp -s speed -b bytes \n ",appname);
		printf("\n");
		printf("   Example Usage:   %s -d COM1 -s 115200 -f sample.bmp \n",appname);
		printf("\n");
		printf("           Where: -p device is port e.g.  COM1  \n");
		printf("                  -s Speed is port Speed  default is 115200 \n");
		printf("                  -f Filename of BMP file \n");
		printf("                  -b bytes to send - must be multiple of 3's. default is 6 \n");

		printf("\n");

        printf("-----------------------------------------------------------------------------\n");

		return 0;
}

int main(int argc, char** argv)
{
	   int opt;
	   char buffer[MAX_BUFFER]={0};
	   int fd;
	   int res,c,counter,i,in;
	   FILE *fp;
	   char *param_port = NULL;
	   char *param_speed = NULL;
	   char *param_imagefile=NULL;
       BITMAPFILEHEADER header;
       BITMAPINFO *headerinfo;
       int headersize,bitmapsize;
       uint8_t *i_bits=NULL,*o_bits=NULL;  // input bits array from original bitmap file, output bit as converted
       int chunksize=6;  //default chunk of bytes to send must be set to max the device can handle without loss of data
       uint8_t b[3] ={0};
       BOOL breakout=FALSE;
		printf("-----------------------------------------------------------------------------\n");
		printf("\n");
		printf(" BMP image sender for Nokia LCD Backpack V.0.1 \n");
		printf(" http://www.dangerousprototypes.com\n");
		printf("\n");
		printf("-----------------------------------------------------------------------------\n");

		if (argc <= 1)  {
			print_usage(argv[0]);
			exit(-1);
		}

		while ((opt = getopt(argc, argv, "b:s:p:f:")) != -1) {

			switch (opt) {
				case 'p':  // device   eg. com1 com12 etc
					if ( param_port != NULL){
						printf(" Device/PORT error!\n");
						exit(-1);
					}
					param_port = strdup(optarg);
					break;
				case 'f':
					if (param_imagefile != NULL) {
						printf(" Invalid Parameter after Option -f \n");
						exit(-1);
					}
					param_imagefile = strdup(optarg);
					break;
				case 's':
					if (param_speed != NULL) {
						printf(" Speed should be set: eg  115200 \n");
						exit(-1);
					}
					param_speed = strdup(optarg);
					break;
				case 'b':    //
				    if ((atol(optarg) % 3 ==0)&&(atol(optarg)!=0) )
				         chunksize=atol(optarg);
				    else {
				       printf(" Invalid send byte parameter: using default\n");

				    }

					break;
				default:
					printf(" Invalid argument %c", opt);
					print_usage(argv[0]);
					exit(-1);
					break;
			}
		}

		if (param_port==NULL){
			printf(" No serial port specified\n");
			print_usage(argv[0]);
			exit(-1);
		}

		if (param_speed==NULL)
           param_speed=strdup("115200");  //default is 115200kbps

		if (param_imagefile !=NULL) {   //checks is needed to make sure this is a valid bmp file
			//open the Imagefile  file
			   if ((fp = fopen(param_imagefile, "rb")) == NULL) {
				   printf(" Cannot open image file: %s\n",param_imagefile);
				   exit(-1);
			   }
			    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1){
                    printf(" Invalid Image file.. requires BMP file \n");
                    fclose(fp);
                    exit(-1);
               }
                if (header.bfType != BF_TYPE) {    //BM as signature
                     printf("File: %s is not a valid bitmap file! \n ",param_imagefile);
                     fclose(fp);
                     exit(-1);
               }
               headersize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

               if ((headerinfo = (BITMAPINFO *)malloc(headersize)) == NULL){
                    printf("Error allocating memory\n");
                    fclose(fp);
                    exit(-1);
               }

               if (fread(headerinfo, 1, headersize, fp) < headersize){
                    fclose(fp);
                    exit(-1);
               }
               printf(" Header size is %lu\n",headerinfo->bmiHeader.biSize);
               printf(" Image size is  %lupx x %lupx\n",headerinfo->bmiHeader.biWidth,headerinfo->bmiHeader.biHeight);
               printf(" Number of colour planes is %d\n",headerinfo->bmiHeader.biPlanes);
               printf(" Bits per pixel is %d\n",headerinfo->bmiHeader.biBitCount);
             //  printf(" Compression type is %lu\n",headerinfo->bmiHeader.biCompression);
             //  printf(" Image size is %lu\n",headerinfo->bmiHeader.biSizeImage);
             //  printf(" Number of colours is %lu\n",headerinfo->bmiHeader.biClrUsed);
              // printf(" Number of required colours is %lu\n",headerinfo->bmiHeader.biClrImportant);


               if ((bitmapsize = headerinfo->bmiHeader.biSizeImage) == 0){
                    bitmapsize = (headerinfo->bmiHeader.biWidth * headerinfo->bmiHeader.biBitCount + 7) / 8 * abs(headerinfo->bmiHeader.biHeight);
               }
               if ((i_bits = malloc(bitmapsize)) == NULL){  //allocate enough memory
					printf(" Error: Cannot allocate enough memory \n");
					fclose(fp);
					exit(-1);
               }
                if ((o_bits = malloc(bitmapsize/2+1)) == NULL){  //allocate enough memory
					printf(" Error: Cannot allocate enough memory \n");
					fclose(fp);
					exit(-1);
               }
               for (i=0;i < bitmapsize/2+1;i++)
                     o_bits[i]=0x00;    // make sure padding is there
/*
               if (fread(i_bits, sizeof(unsigned char), bitmapsize, fp) < bitmapsize) {
                free(i_bits);
                fclose(fp);
                 exit(-1);
               }
*/

		}
        else{
            printf("Error: Image file is required\n");
            exit(-1);
        }
        //close and reopen bmp file
        fclose(fp);
        fp = fopen(param_imagefile, "rb");


		printf(" Opening Port on %s at %sbps, using image file %s chunksize = %i \n", param_port, param_speed,param_imagefile,chunksize);
		fd = serial_open(param_port);
		if (fd < 0) {
			fprintf(stderr, " Error opening serial port\n");
			return -1;
		}

		//setup port and speed
		serial_setup(fd,(speed_t) param_speed);

        printf(" Offset to image data is %lu bytes\n",header.bfOffBits);

        // i Reset and initialize the LCD.
        //  buffer[0]='i';
	    //	serial_write( fd, buffer,1 );

        //send the command 	//Send 'p' or 'P' send page of image data.
		printf(" Setting image mode \n");
        buffer[0]='P';
		serial_write( fd, buffer,1 );

        fseek(fp,header.bfOffBits,SEEK_SET); //and disable this if above fgets is enabled

        if ((res=fread(i_bits,sizeof(unsigned char),bitmapsize,fp)) != bitmapsize) {
                 printf(" Header information error: image data size inconsistent. %i %i\n",res,bitmapsize);
                 fclose(fp);
                 exit(-1);

		}

        printf(" Ready to convert %d bit into 12 bit image data.. \n",headerinfo->bmiHeader.biBitCount);
        printf(" Press any key when ready...\n");
        getch();
	    switch (headerinfo->bmiHeader.biBitCount) {
                    case 1:
                        printf(" 1 bit (black and white) not yet done");
                        fclose(fp);
                        exit(-1);
                        break;
                    case 4:
                        printf(" 4-bit (16 colors) not yet done");
                          fclose(fp);
                        exit(-1);
                        break;
                    case 8:
                        printf(" 8 bit (256 colors) not yet done");
                        fclose(fp);
                        exit(-1);
                        break;
                    case 16:
                        printf(" 16-bit not yet done");
                        fclose(fp);
                        exit(-1);
                        break;
                    case 24:  //convert i_bits to 12bits into o_bits

                        counter=0; // counter for original bitmap data
                        in=0;   //counter for putting bytes in o_bits
                        breakout=FALSE;
                        while(1) {

                                //the LCD needs 12bit color, this is 24bit
                                //we need to discard the lower 4 bits of each byte
                                //and pack six half bytes into 3 bytes to send the LCD
                                //if we only have one pixel to process (3bytes) instead of 2, then the last should be 0
                                //need to do checks for end of data, make this more effecient
                                //BMP byte order of BGR...
                               //rrrrgggg	bbbbrrrr	ggggbbbb	...	ggggbbbb

                                //grab six bytes
                                for (i=0;i < 6;i++){

                                   if (counter > bitmapsize-1){
                                       buffer[i]=0x00; //pad remaining then exit the loop
                                       breakout=TRUE;
                                   } else {


                                   buffer[i]=i_bits[counter++];
                                   }
                                   printf(" %02x",(unsigned char) buffer[i]);

                                }

                                // convert to 4 bits each RGB
                                printf("--->");

                                //R G
                                b[0]=((buffer[2]&0xf0)|((buffer[1]>>4)&0x0f));
                                //B R2
                                b[1]=(buffer[0]&0xf0)|((buffer[5]>>4)&0x0f);
                                //G2B2
                                b[2]=(buffer[4]&0xf0)|((buffer[3]>>4)&0x0f);
                               // put in o_bits
                                o_bits[in++] =b[0];
                                o_bits[in++] =b[1];
                                o_bits[in++] =b[2];
                                printf("%02X %02X %02X \n",b[0],b[1],b[2]);
                                if (breakout==TRUE) {
                                    //try to clean
                                    if ((b[0]==0x00)&& (b[1] == 0x00)&& (b[2]==0x00)) {
                                       //remove excess padding
                                       in-=3;
                                       break;
                                    }
                                }
                            }

                        break;
                    case 32:
                         printf(" 32-bit not yet done");
                        fclose(fp);
                        exit(-1);
                        break;

                    default:
                        printf(" Unrecognize Bit count \n");
                        fclose(fp);
                        exit(-1);

	    }


		printf(" sending image data..\n ");

		for (i=0;i < in ;i+=chunksize){
			for (c=0; c < chunksize; c++) {
			  buffer[c]= o_bits[i+c];
			  printf(" %02X ",  (uint8_t) buffer[c]);
			}
			printf("\n");
			serial_write( fd, buffer,chunksize);
		}
		Sleep(1);
		res= serial_read(fd, buffer, sizeof(buffer));
		printf("\n Reply received: %c\n",buffer[0]);
		printf(" Done! :-)\n\n");

		//close lcd
		fclose(fp);
		FREE(param_port);
		FREE(param_speed);
		FREE(param_imagefile);
		return 0;
 }  //end main()

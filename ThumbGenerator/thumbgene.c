/************************
Purpose:Generate Thumbnails
*************************/

/*

To compile:
gcc -o thumbgene thumbgene.c -lgd -ljpeg

*/


/*Constants for phpwebgallery thumbnails*/
#define H_THUMB_X 128
#define H_THUMB_Y 96
#define V_THUMB_X 72
#define V_THUMB_Y 96





#include<fnmatch.h> /* For wildcard matching */
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<gd.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>




const char *prog_name;
int pwg = 0; /* Check if we have to generate thumbnails for phpwebgallery application*/
int SizeFactor = 4; /* Default scale factor for thumbnail size*/
int arg_x,arg_y;



void print_usage(FILE* stream,int exit_code)
{
  fprintf(stream,"Usage: %s [options] \n",prog_name);
  fprintf(stream,
	  " -h      --help             Display this usage information.\n"
	  " -p      --phpwebgallery    Create thumbnails for phpwebgallery application.\n"
	  " -f      --factor           Enter scale factor.By default, it's 4.\n"
	  " -s      --size             Enter the size of x and y. Choices are 1024768, 640480 or 160120\n"
	  " -v      --verbose          Print verbose messages.\n");
  exit(exit_code);
 
}




int main(int argc,char **argv)
{

DIR *dir;
struct dirent * ent;
int checkCreateThumbnail;
int NbFile = 0;
int NbFilePB = 0;//for Progress Bar
int iPB; //for Progress Bar
int next_option;
int verbose = 0;
 int size_option;
const char* const short_options = "hpf:vs:";

const struct option long_options[] = {
  { "help", 0, NULL,'h' },
  { "phpwebgallery", 0, NULL,'p'},
  { "factor", 1, NULL,'f'},
  { "verbose", 0, NULL,'v'},
  { "size",1,NULL,'s'},
  { NULL, 0, NULL, 0},
 };


prog_name = argv[0];


 do {
   next_option = getopt_long(argc,argv,short_options,long_options,NULL);
   switch(next_option)
     {
     case 'h':
       print_usage(stdout,0);
     case 'v':
       verbose = 1;
       break;
     case 'p':
       pwg = 1;
       break;
      case 's':
	size_option = atof(optarg);
	  switch(size_option)
	  {
	  case 1024768:
	    arg_x = 1024;
	    arg_y = 768;
	    break;
	 case 640480:
	    arg_x=640;
	    arg_y=480;
	    break;
	  case 160120:
	    arg_x=160;
	    arg_y=120;
	 case -1:
	    break;
	 default:
	   abort();
	    }
      case 'f':
		SizeFactor = atof(optarg);
		break; 
     case '?':
       print_usage(stderr,1);
     case -1:
       break;
     default:
       abort();
     }
 }
 while(next_option != -1);



if ( argv[optind] == 0) 
{
  fprintf(stderr,"Usage: %s Path_To_Directory\n",argv[0]);
  exit(EXIT_FAILURE);
}



/* Change de working directory */
 if((chdir(argv[optind])) != 0)
   {
      fprintf(stderr,"Can not change to directory %s - Check syntaxe\n",argv[optind]);
      print_usage(stdout,0);
   }


/* Open working directory */
 if ((dir=opendir(".")) == NULL)
{
  fprintf(stderr,"Can not open directory %s\n",argv[optind]);
  exit(EXIT_FAILURE);
}




/* Presentation */

printf("Thanks for using ThumbGenerator\n");
printf("This can take a while\n");
printf("Please wait...\n");

/* End presentation */




while ((ent = readdir(dir)))
{
	
	if (!(fnmatch("[^TN]*.jp?(e)g",ent->d_name,FNM_CASEFOLD | FNM_EXTMATCH )))
	{	
	NbFilePB++;	//Get the total number of files to display progress bar
	}		
}




rewinddir(dir); /*resets the position of the directory stream
       			dir to the beginning of the directory from man rewinddir */


/* Read the directory*/


while ((ent = readdir(dir)))  
{ 	
     if (!(fnmatch("[^TN]*.jp?(e)g",ent->d_name,FNM_CASEFOLD | FNM_EXTMATCH))) /* Filter, only convert the *.jpeg,*.jpg without converting the "TN" ones */
	{ 

	++NbFile; 
  
	if (CreateThumbnail(ent->d_name) != 0 )
	  {
	    fprintf(stderr,"Error while trying to create thumbnail for %s",ent->d_name);
	    fprintf(stderr,"%d thumbnails have been created\n",NbFile-1);
	    exit(EXIT_FAILURE);		
	  }
	  else
	  {  
		if (verbose == 1 )
		{
		  /* A cool progress bar */		  
		  printf("\r0%% ");
		  for (iPB=0;iPB<NbFile;++iPB) printf("#");
		  for (iPB=NbFile;iPB<NbFilePB;++iPB) printf(" ");	
		  printf(" 100%%");
		 fflush(stdout);
	    }
	  }
     //printf("%d thumbnails have been created\n",NbFile);
     }
 }	 
printf("\n%d thumbnails have been created\n",NbFile);
printf("Have a nice day\n");
	 
	 
return 0;


}



int CreateThumbnail(char *ImgName)
{

gdImagePtr img_thumb,img_src; /* Pointer to the final image (the thumbnail)*/
FILE *img_original;
FILE *img_dest; /* Image in jpeg */
int x,y;
char thumb_name[50]= "TN-";

strcat(thumb_name,ImgName);


/* Open file to transform */

 if(!(img_original=fopen(ImgName,"rb")))
     {
       fprintf(stderr,"Can not open file %s\n",ImgName);
       exit(1);
     }

 

/* Open file that will be the thumbnail */

 if(!(img_dest=fopen(thumb_name,"wb")))
     {
       fprintf(stderr,"Can not open file %s\n",thumb_name);
       exit(1);
     }

img_src = gdImageCreateFromJpeg(img_original);

fclose(img_original);
x=img_src->sx;
y=img_src->sy;






/* Code to generate thumbnails for phpwebgallery */
if (pwg == 1 )
{	 
  /* Check if height < width */
 if (y < x )
   {   
     img_thumb = gdImageCreateTrueColor(H_THUMB_X,H_THUMB_Y);
     gdImageCopyResampled(img_thumb,img_src,0,0,0,0,H_THUMB_X,H_THUMB_Y,x,y);
   }
 else
   {
     img_thumb = gdImageCreateTrueColor(V_THUMB_X,V_THUMB_Y);
     gdImageCopyResampled(img_thumb,img_src,0,0,0,0,V_THUMB_X,V_THUMB_Y,x,y);
   }
}

/* Using the size */
else if (arg_x)
{
  /* Check if height < width */
  if (y<x)
    {
      img_thumb = gdImageCreateTrueColor(arg_x,arg_y);
      gdImageCopyResampled(img_thumb,img_src,0,0,0,0,arg_x,arg_y,x,y);
    }
  else
    {
      img_thumb = gdImageCreateTrueColor(arg_y,arg_x);
      gdImageCopyResampled(img_thumb,img_src,0,0,0,0,arg_y,arg_x,x,y);
    }
}
/* Using a size factor - default factor = 4 */
else
{   
img_thumb = gdImageCreateTrueColor(x/SizeFactor,y/SizeFactor);
gdImageCopyResampled(img_thumb,img_src,0,0,0,0,x/SizeFactor,y/SizeFactor,x,y);
}





gdImageJpeg(img_thumb,img_dest,-1);
fclose(img_dest);
gdImageDestroy(img_src);
gdImageDestroy(img_thumb);


return 0;

}

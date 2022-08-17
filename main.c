//Note: this main.c file is used to learn C_language grammer.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//void main(){
//        printf("Hello World!\n");
//}
   typedef struct{
      char name;
      int age;
      int score;
     }message;

 typedef struct abc{
      char name_1[10];
      int age_1;
      int score_1;
     }abc_1;

 typedef struct abcc{
      char name_1[10];
      int age_1;
      int score_1;
     }abc_2;

   typedef struct{
      char name[10];
      int age;
      int score;
     }message_po_func;
typedef unsigned int Uint32;
typedef unsigned char Uint8;
typedef int Bool;
#define True 1
#define False 0


message_po_func* f(int a,int b){
   // message_po_func * data = new message_po_func;
    message_po_func pp_gou;
    message_po_func* point_pp_gou = &pp_gou;
    point_pp_gou->age = a;
   // data->b = b;
   printf("%d\n",  point_pp_gou->age);

    return point_pp_gou;
}

void* osa_malloc(Uint32 size)
{
    return (Uint8*)malloc(size);
}

// ipv4 ipv6 allacted
void pdu_data_set_ip_type(Uint8* ip_pdu_ptr, Uint8 ip_type)
{
	*ip_pdu_ptr = ip_type;
}


//
void pdu_data_set_src_addr(Uint8* ip_pdu_ptr, Uint8* ip_addr)
{
	Uint8 i = 0;
	//IPV4
//	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
//	{
	//	for(i=0;i<4;i++)
		{
			*(ip_pdu_ptr+12+i) = *(ip_addr+i);
		}
//	}
	//IPV6
//	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
//	{
//		for(i=0;i<16;i++)
//		{
//			*(ip_pdu_ptr+8+i) = *(ip_addr+i);
//		}
//	}
    printf("ip_addr: %p\n",ip_addr);	
    printf("ip_addr: %d\n",*ip_addr);	
}

void pdu_data_set_src_addr_2(Uint8* ip_pdu_ptr, Uint8* spi)
{
	Uint8 i = 0;
	//IPV4
//	if(4 == ((*ip_pdu_ptr & 0xF0) >> 4))
//	{
	//	for(i=0;i<4;i++)
		{
			*(ip_pdu_ptr+12+i) = spi[i];
                        *(ip_pdu_ptr+15+i) = spi[i+2];
                        
                      //  *(ip_pdu_ptr+16+i)    = 156;
		}
//	}
	//IPV6
//	else if(6 == ((*ip_pdu_ptr & 0xF0) >> 4))
//	{
//		for(i=0;i<16;i++)
//		{
//			*(ip_pdu_ptr+8+i) = *(ip_addr+i);
//		}
//	}
    printf("spi: %p\n",spi[i]);	
    printf("spi +2: %p\n",spi[i+2]);	
    printf("spi_inner_value: %d\n",*spi);	
}

int main(){
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    int sub;
    int *p1 = &a[2];
    int *p2 = &a[8];
    Uint8 addr[16];
    Uint8* buff_base_1;
    buff_base_1 = (Uint8*)malloc(sizeof(Uint8));
    //Source Address
	addr[0] = 180;
	addr[1] = 101;
	addr[2] = 49;
	addr[3] = 12;
   pdu_data_set_src_addr(buff_base_1,addr);     

    Uint8 spi[4];
    Uint8* buff_base_2;
    buff_base_2 = (Uint8*)malloc(sizeof(Uint8));
    //Source Address
	spi[0] = 0x0F;
	spi[1] = 0x1F;
	spi[2] = 0x2F;
	spi[3] = 0x3F;
   pdu_data_set_src_addr_2(buff_base_2,spi);  




    int *p33;
    printf("p33: %p\n",p33);
    p33 =& a[2];
    printf("p33_1: %p\n",p33);
    

    Uint8* buff_base;// define a empty pointer
    buff_base = (Uint8*)malloc(sizeof(Uint8));// after the 
   // abc_1* point1;
//    point1-> score_1=3;
    
    int* pp_a =&a[0];

    sub = p2-p1;                                                                            
    printf("%d\n",sub);// minus value
    printf("address of a[0]:%p\n",a);// address of a[0] or a
    printf("address of a[0]:%p\n",pp_a);// address of a[0] or a
    
    printf("%p\n",p1); // point p1's value, actually, p1 is the address of a[2]
    printf("%p\n",*p1); // output the a[2], which is pointed by point p1
    
    message ccc; 
    ccc.age=1; 
    printf("%d\n", ccc.age);

    abc_1 baga;
    baga.age_1 = 2;
    printf("%d\n",baga.age_1);

    abc_1 jiuga;// jiuga is a abc_1 type variable

    abc_1* point1=&jiuga;// define a pointer (name point1) variable
 
    //the above sentence can be replaced by the following two sentences
    //abc_1* point1=NULL;
    //point1 = &jiuga;


   // point1 = &jiuga; // let the point1 point to variable jiuga
    point1-> score_1=3;// then assign values 
    printf("%d\n",  point1-> score_1);



 //   abc_2 baga2;
 //   baga2 *point1;
 //   point1->age_1 = 3;
 //   printf("%d\n",point1->age_1);
    

    // investigate the void* 
    void *p11;
    #define p111 ((void*)0)

    int *p22=&a[2];
    p11 = p22;
    printf("p11: %p\n",p11);
    printf("p22: %p\n",p22);
    printf("p111: %p\n",p111);
    

    FILE *fp;
    fp = fopen( "file.txt" , "w" );

    char *str="hello,I am a test program!";  
    fwrite(str,1,strlen(str)-1,fp); 
    fwrite(str,1,strlen(str)-1,stdout); 


    // point function
    message_po_func*mydata =f(1,2);
    printf("%d\n",  mydata->age);


    // malloc study
    char* tft_debug_info_ptr = (char*)osa_malloc(sizeof(int));
    printf("tft_debug_info_ptr: %p\n",tft_debug_info_ptr);
    
    printf("tft_debug_info_ptr: %p\n",tft_debug_info_ptr+1);

    //
   // Uint8* buff_base1;
   //int ap=20;
   int cd = (4<<4)+5;
   printf("cd: %p\n",cd);
    pdu_data_set_ip_type(buff_base,(4 << 4) + 5);
    printf("point1: %p\n",*buff_base);
    printf("point1: %p\n",buff_base);

 printf ("size of Uint32: %d\n",sizeof(Uint32));
 printf ("size of Uint8: %d\n",sizeof(Uint8));
 printf ("size of message: %d\n",sizeof(message));
 
 abc_2* pp1 = (abc_2*)malloc(sizeof(abc_2));
 printf ("address of pp1: %p\n",pp1);
 printf ("address of pp1+1: %p\n",pp1+1);
 printf("value: %d\n",sizeof(abc_2));

 Uint8 bs = 0x1F;

 Uint8* ptr1 = & bs;
 Uint32 temp;
 temp = ((Uint32)*ptr1)<<24;
 printf("temp value: %p\n",temp);
 Uint32 temp2 = 0x000000FF;
 temp |=temp2;
printf("temp value updated: %p\n",temp);
printf("the size of bs: %d\n",bs);
 
  int sd[5]={0};
  int i1 = 0;

 printf("i1: %d\n",i1);
 printf("the size of i1: %d\n",i1);

  
  sd[i1++] = 1;

  printf("i1: %d\n",i1);
  printf("sd[0]: %d\n",sd[0]);
  printf("sd[1]: %d\n",sd[1]);
  
  int i2=4;
  do// NOte : at least conduct once
  {printf("i2 value: %d\n",i2);
          i2 = i2 +1;
  } while(i2<=3);       
  

  int i3=4;
  while (i3<=3)// if not satisfied with the condition, no conduction.
  {printf("i3 value: %d\n",i3);
          i3 = i3 +1;
  };  


  Bool bool1 = 0;
  Uint8 bool2 = 1;
  printf("bool2:%d\n",bool2);
  
  bool2 = bool1;
  printf("bool2:%d\n",bool2);

    return 0;


}

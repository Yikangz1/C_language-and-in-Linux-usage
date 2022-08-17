#NOte: the correct makefile writting method with "variable", if you wanna add new C file, just add the C file corresponding .o file into variable "objects":
all1:com88 com
.PHONY:all1

objects=main.o second.o third.o forth.o 
$(info "bf=098")
com88: $(objects)
	gcc -o com8 $(objects)
	./com8 
	$(warning "output contents")
	$(info "cf=09")
#	gcc main.c -o mn3;
#	./mn3
			
main.o: tft.c construct_data.h  
	gcc -o main.o -c tft.c
second.o: tft_func.c tft_type.h 
	gcc -o second.o -c tft_func.c
third.o: construct_data.c construct_data.h 
	gcc -o third.o -c construct_data.c
forth.o: osa.c osa.h
	gcc -o forth.o -c osa.c
	
#cond: $(objects)
#	gcc -o com8 $(objects)
#	./com8
com:
	gcc main.c -o mn3;
	./mn3

tiaoshi:
	gcc tiaoshi.c -o mn4;
	./mn4

#$(info "bfff=098")

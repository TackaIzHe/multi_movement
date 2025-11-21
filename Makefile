OBJ = $(shell cd src && ls -R | grep -E '.c$$' | sed 's/.c$$/.o/' )

SERVER_OBJ =

CLIENT_OBJ = 

FLG := `pkg-config --cflags --libs xft xtst` -lX11 -lXext

.PHONY: clean MKDIR all asm_client asm_server

all: MKDIR ${OBJ} asm_client asm_server
	@echo
	@echo !!!!!!!!!comlite!!!!!!!!!!!
	@echo

asm_server:
	cd bin && gcc ${OBJ} ${SERVER_OBJ} $ ${FLG} -o ../server

asm_client:
	cd bin && gcc ${OBJ} ${CLIENT_OBJ} ${FLG} -o ../client

%.o: src/%.c
	gcc $^ -c -o ./bin/$@

%.o: src/socket/%.c
	gcc $^ -c -o ./bin/$@

%.o: src/server/%.c
	gcc $^ -c -o ./bin/server/$@
	$(eval SERVER_OBJ += server/$@)
	$(eval OBJ := $(filter-out $@, $(OBJ)))	
	@echo ${SERVER_OBJ}
	

%.o: src/client/%.c
	gcc $^ -c -o ./bin/client/$@
	$(eval CLIENT_OBJ += client/$@)
	$(eval OBJ := $(filter-out $@, $(OBJ)))	
	@echo ${CLIENT_OBJ}

MKDIR:
	mkdir bin -p
	mkdir bin/server -p
	mkdir bin/client -p

clean:
	rm -r bin  
	rm server
	rm client
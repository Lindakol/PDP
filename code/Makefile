# Compiler and flags
CC = mpicc
CFLAGS = -c -std=c11  
LFLAGS = -lm

# Paths to the sub-directories
BUILD_DIR = ./build

ACTOR_SOURCE_DIR = ./actorPattern/src
ACTOR_INCLUDE_DIR = ./actorPattern/include

MODEL_SOURCE_DIR = ./squirrelModel/src
MODEL_INCLUDE_DIR = ./squirrelModel/include

# Executable
EXE = bio_model 

ACTOR_SRC = $(wildcard $(ACTOR_SOURCE_DIR)/*.c)
MODEL_SRC = $(wildcard $(MODEL_SOURCE_DIR)/*.c)
PROV_SRC = $(wildcard $(MODEL_SOURCE_DIR)/provided/*.c)

ACTOR_INC = $(wildcard $(ACTOR_INCLUDE_DIR)/*.h)
MODEL_INC = $(wildcard $(MODEL_INCLUDE_DIR)/*.h)


ACTOR_OBJ = $(patsubst $(ACTOR_SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(ACTOR_SRC))
MODEL_OBJ = $(patsubst $(MODEL_SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(MODEL_SRC))
PROV_OBJ = $(patsubst $(MODEL_SOURCE_DIR)/provided/%.c, $(BUILD_DIR)/%.o, $(PROV_SRC))

all: dir $(BUILD_DIR)/$(EXE) 

dir:
	mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/$(EXE): $(PROV_OBJ) $(ACTOR_OBJ) $(MODEL_OBJ)
	$(CC) $^ -o $@ $(LFLAGS) 


$(BUILD_DIR)/%.o : $(ACTOR_INCLUDE_DIR)/%.h $(MODEL_INCLUDE_DIR)/%.h


$(MODEL_OBJ): $(BUILD_DIR)/%.o: $(MODEL_SOURCE_DIR)/%.c    
	$(CC) $(CFLAGS) $< -o $@ -I$(MODEL_INCLUDE_DIR) -I$(ACTOR_INCLUDE_DIR) 

$(PROV_OBJ): $(BUILD_DIR)/%.o: $(MODEL_SOURCE_DIR)/provided/%.c    
	$(CC) $(CFLAGS) $< -o $@ -I$(MODEL_INCLUDE_DIR)  

$(ACTOR_OBJ): $(BUILD_DIR)/%.o: $(ACTOR_SOURCE_DIR)/%.c    
	$(CC) $(CFLAGS) $< -o $@ -I$(ACTOR_INCLUDE_DIR) -I$(MODEL_INCLUDE_DIR) 



run:
	mpirun -np 50  ./$(BUILD_DIR)/$(EXE)

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(EXE) 
	rm -r $(BUILD_DIR)

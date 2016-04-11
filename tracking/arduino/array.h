/*
A dynamic bool array
*/
typedef struct {
  bool *array;
  size_t used;
  size_t size;
} BoolArray;


void fillTrue(BoolArray *a){
    for(int i = 0;i<a->size;i++){
      a->array[i]=true;
    }
    a->used = a->size;
}

void initArray(BoolArray *a, size_t initialSize) {
  a->array = (bool *)malloc(initialSize * sizeof(bool));
  a->used = 0;
  a->size = initialSize;
  fillTrue(a);
}

void insertArray(BoolArray *a, bool element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (bool *)realloc(a->array, a->size * sizeof(bool));
  }
  a->array[a->used++] = element;
}

void insertFirstRoll(BoolArray *a, bool element){
    for(int i = a->used-1;i>0;i--){
      a->array[i]=a->array[i-1];
    }
    a->array[0]=element;
}

void freeArray(BoolArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

bool orArray(BoolArray *a){
    for(int i = 0;i<a->used;i++){
      if(a->array[i]==true){
        return true;
      }
    }
    return false;
}

void printArray(BoolArray *a){
    for(int i = 0;i<a->used;i++){
      Serial.print(a->array[i]);
    }
}

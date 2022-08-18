Давайте напишем простой аллокатор.

N.B. Для получения баллов дефрагментацию делать не требуется, но очень рекомендуется сделать ее для самотренировки. Для того, чтобы ее сделать, придется реализовывать умные указатели, инвариантные относительно позиции блока памяти!

N.B. ReAlloc должен сохранять данные в блоке. То есть это не просто пара Free + Alloc.

N.B. Следующий вариант и ему подобные считаются читерством:

```C++
class SmallAllocator {
public:
        void *Alloc(unsigned int Size) {
                return malloc(Size);
        };
        void *ReAlloc(void *Pointer, unsigned int Size) {
                return realloc(Pointer, Size);
        };
        void Free(void *Pointer) {
                return free(Pointer);
        };
};
```

UPD: Так как возникло много вопросов, почему система не принимает некоторые работы, выкладываю тест. Он всего один. Программа не должна кориться и не должна выводить текст.

```C++
SmallAllocator A1;
int * A1_P1 = (int *) A1.Alloc(sizeof(int));
A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
A1.Free(A1_P1);
SmallAllocator A2;
int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
A2.Free(A2_P1);
A2.Free(A2_P2);
```

[Ссылка на задание](https://stepik.org/lesson/12574/step/7?thread=solutions&unit=3002)
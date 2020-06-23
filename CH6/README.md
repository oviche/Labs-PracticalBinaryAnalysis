# CHAPTER.6

### confuse Objdump 
- compile code using following command & check main function of **.text** seciton in objdump. 

  ```
  gcc -masm=intel confObjdmp.c -o program
  objdump -M intel -d -j .text program 
  ```

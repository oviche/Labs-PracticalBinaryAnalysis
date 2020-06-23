# CHAPTER.6

### confuse Objdump 
- compile code using following command. Then, check obfuscated code in main function of **.text** secton in **objdump**. 

  ```
  gcc -masm=intel confObjdmp.c -o program
  objdump -M intel -d -j .text program 
  ```

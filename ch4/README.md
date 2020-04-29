# Static Binary Loader

## Usage
```
'./demo' binaryfile <options> SectionName  # print binary,symbols,section information

<options>:
 -S         show section content in hexadecimal
```

## Example
```
./demo /bin/ls

# OUTPUT
binary=> '/bin/ls' | format: elf64-x86-64 | architecture: i386:x86-64 | (64 bits) | Entry_Address@0x0000000000005b20

::: SYMBOL TABLE :::

        SYMBOL_NAME                                                                      SYMBOL_ADDRESS    SYMBOL_TYPE    SYMBOL_BIND
    0: __ctype_toupper_loc                                                              0000000000000000   TYPE_FUNCTION   UNKNOWN
    1: getenv                                                                           0000000000000000   TYPE_FUNCTION   UNKNOWN
    2: cap_to_text                                                                      0000000000000000   TYPE_FUNCTION   UNKNOWN
    3: sigprocmask                                                                      0000000000000000   TYPE_FUNCTION   UNKNOWN
    4: __snprintf_chk                                                                   0000000000000000   TYPE_FUNCTION   UNKNOWN
    5: raise                                                                            0000000000000000   TYPE_FUNCTION   UNKNOWN
    6: free                                                                             0000000000000000   TYPE_FUNCTION   UNKNOWN
    7: abort                                                                            0000000000000000   TYPE_FUNCTION   UNKNOWN
    8: __errno_location                                                                 0000000000000000   TYPE_FUNCTION   UNKNOWN
    9: strncmp                                                                          0000000000000000   TYPE_FUNCTION   UNKNOWN
   10: localtime_r                                                                      0000000000000000   TYPE_FUNCTION   UNKNOWN
   11: _exit                                                                            0000000000000000   TYPE_FUNCTION   UNKNOWN
   12: strcpy                                                                           0000000000000000   TYPE_FUNCTION   UNKNOWN
   13: __fpending                                                                       0000000000000000   TYPE_FUNCTION   UNKNOWN
   14: isatty                                                                           0000000000000000   TYPE_FUNCTION   UNKNOWN
   15: sigaction                                                                        0000000000000000   TYPE_FUNCTION   UNKNOWN
   16: iswcntrl                                                                         0000000000000000   TYPE_FUNCTION   UNKNOWN
   17: wcswidth                                                                         0000000000000000   TYPE_FUNCTION   UNKNOWN
   18: localeconv                                                                       0000000000000000   TYPE_FUNCTION   UNKNOWN
   19: mbstowcs                                                                         0000000000000000   TYPE_FUNCTION   UNKNOWN
   20: readlink                                                                         0000000000000000   TYPE_FUNCTION   UNKNOWN
   21: clock_gettime                                                                    0000000000000000   TYPE_FUNCTION   UNKNOWN
   22: setenv                                                                           0000000000000000   TYPE_FUNCTION   UNKNOWN
   23: textdomain                                                                       0000000000000000   TYPE_FUNCTION   UNKNOWN
   24: fclose                                                                           0000000000000000   TYPE_FUNCTION   UNKNOWN
   25: opendir                                                                          0000000000000000   TYPE_FUNCTION   UNKNOWN
   26: getpwuid                                                                         0000000000000000   TYPE_FUNCTION   UNKNOWN
'''

::: SECTION TABLE :::

        SECTION_NAME                                                                     SECTION_ADDRESS    SECTION_TYPE   SIZE
    0: .interp                                                                          00000000000002a8   SEC_TYPE_DATA   28
    1: .note.gnu.build-id                                                               00000000000002c4   SEC_TYPE_DATA   36
    2: .note.ABI-tag                                                                    00000000000002e8   SEC_TYPE_DATA   32
    3: .gnu.hash                                                                        0000000000000308   SEC_TYPE_DATA   176
    4: .dynsym                                                                          00000000000003b8   SEC_TYPE_DATA   3072
    5: .dynstr                                                                          0000000000000fb8   SEC_TYPE_DATA   1468
    6: .gnu.version                                                                     0000000000001574   SEC_TYPE_DATA   256
    7: .gnu.version_r                                                                   0000000000001678   SEC_TYPE_DATA   128
    8: .rela.dyn                                                                        00000000000016f8   SEC_TYPE_DATA   7680
    9: .rela.plt                                                                        00000000000034f8   SEC_TYPE_DATA   24
   10: .init                                                                            0000000000004000   SEC_TYPE_CODE   27
   11: .plt                                                                             0000000000004020   SEC_TYPE_CODE   32
   12: .text                                                                            0000000000004040   SEC_TYPE_CODE   78722
   13: .fini                                                                            00000000000173c4   SEC_TYPE_CODE   13
   14: .rodata                                                                          0000000000018000   SEC_TYPE_DATA   21281
   15: .eh_frame_hdr                                                                    000000000001d324   SEC_TYPE_DATA   2388
   16: .eh_frame                                                                        000000000001dc78   SEC_TYPE_DATA   12360
   17: .init_array                                                                      0000000000021fd0   SEC_TYPE_DATA   8
   18: .fini_array                                                                      0000000000021fd8   SEC_TYPE_DATA   8
   19: .data.rel.ro                                                                     0000000000021fe0   SEC_TYPE_DATA   2680
   20: .dynamic                                                                         0000000000022a58   SEC_TYPE_DATA   512
   21: .got                                                                             0000000000022c58   SEC_TYPE_DATA   920
   22: .data                                                                            0000000000023000   SEC_TYPE_DATA   616
   23: .bss                                                                             0000000000023280   SEC_TYPE_DATA   4824
   24: .comment                                                                         0000000000000000   SEC_TYPE_DATA   76
'''

```

```
./demo /bin/ls -S .plt     #dump hexdump of .plt section

::: SECTION_INFO :::
Binary_loaded: /bin/ls      SECTION_TYPE : SEC_CODE_TYPE       START_ADDRESS : 0000000000004020      SIZE: 32

::: .plt SECTION :::

0000000000004020: ff353aec  0100ff25  3cec0100  0f1f4000
0000000000004030: ff253aec  01006800  000000e9  e0ffffff

```



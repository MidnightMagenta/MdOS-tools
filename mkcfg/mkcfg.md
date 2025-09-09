## Configuration file format

The Midnight Configuration file is specified with a key that must match the following regex `[a-zA-Z_][a-zA-Z0-9_-]+`. The key must be followed by an equal sign (=). Data is specified using the following formula `TYPE{DATA}`. Type must be one of the following
 - INT8 - 8 bit signed integer
 - INT16 - 16 bit signed integer
 - INT32 - 32 bit signed integer
 - INT64 - 64 bit signed integer
 - UINT8 - 8 bit unsigned integer
 - UINT16 - 16 bit unsigned integer
 - UINT32 - 32 bit unsigned integer
 - UINT64 - 64 bit unsigned integer
 - UTF8 - a string literal to be encoded as UTF8
 - UTF16 - a string literal to be encoded as UTF16
 - GUID - a UUID specified using the 8-4-4-4-12 format
 - BYTES - an array of comma delimited 8 bit unsigned bytes (e.g. BYTES{0x12, 0x34, 0x56})

Example:

```
KEY = TYPE{DATA}
# comment
```

## Binary file format

The Midnight Binary Configuration file format contains a header at offset 0. The header contains the following fields

```
uint8 magic = "MBCF" (0x4D, 0x42, 0x43, 0x46)
uint32 crc32
uint8 version
uint8 pad[7]
uint64 dataOffset
```

Data starts at the offset specified by the dataOffset field, and is encoded as follows

```
[UTF8 encoded key]\0 [uint16 size][data]\0\n
```

The first part of an entry is the null terminated UTF8 encoded value of they key string. The key is immediatly followed by 2 bytes stored in little endian order, and interpreted as a 16 bit unsigned integer, that represent the number of bytes contained in the data. The data immediatly follows the size bytes. The data is alternatively terminated with the sequence `\0\n` (hex 0x00 0x0A).

## mkcfg tool

The `mkcfg` tools can be used to convert Midnight Configuration files into Midnight Binary Configuration files. The tool takes the following parameters

 - `-i` - specifies the path to the input Midnight Configuration file
 - `-o` - specifies the path to the output Midnight Binary Configuration file

Both parameters **must** be passed to the tool.
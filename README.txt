The main objectives of our solution are to create a file, open a file, read the file, and write the file. Files are created
using the createPageFile() method after the storage manager has been initialized. In this method, a file pointer object
is declared, and a block of memory is created to store one page of the file, and meta data. The content of this memory is
intialized to contain 8192 bytes of '0's. Then the first 50 bytes of this memory is set to contain the number of pages.

After that, the content of the first 50 bytes of memory is copied to the rest of the remaining space.

The files are then opened using the openPageFile() method. After the file is opened, 4096 bytes of memory is allocated
for file metadata to be read from the hard drive. Once the metadata is read, it is stored in a temporary string, and then
converted to an integer.

The filename, number of pages, and current page position are then stored in the corresponding filehandle attributes. The
object inside SM_mgmtInfo then points to the file pointer, which then points to the *mgmtInfo attribute of the fileHandle.

When reading a block using the readBlock() method, a file pointer object that points to the metadata is created. Then, the
page number that was passed to the method is checked for vailidity. If the page number is valid, the file position of the
stream and the pointer to the current position is set to the start of the specified page number. Then the content of that
page is read into memory.

The readFirstBlock(), readPreviousBlock(), readCurrentBlock(), and readLastBlock() methods all call the readBlock() method,
after applying the neccessary checks for page number validity.

The write block methods are quite similar to the read methods, the only difference being that fwrite() is used instead
of fread().

To append an empty block, a new page is created. The file stream position is set to 
the beginning of this new block, and the content of the page is all '0's. 

The ensureCapacity() method calls the appendEmptyBlock() method if the number of pages attribute of the fileHandle is less
than the actual number of pages in the file. A for loop iterates over this method until the number of file pages matches 
the amount in the numPages attribute of the fileHandle.